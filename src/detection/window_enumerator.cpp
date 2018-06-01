#include "./window_enumerator.h"

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QCoreApplication>
#include <QString>
#include <QDebug>

#include <regex>

#include "../clients/anilist.h"
#include "../models/media_list.h"
#include "./media_store.h"

using namespace Robot;

WindowEnumerator::WindowEnumerator() : Singleton() {
  readMediaPlayersJson(":/res/mediaPlayers.json");

  QString externalPath = QCoreApplication::applicationDirPath();
  externalPath.append("/customMediaPlayers.json");
  readMediaPlayersJson(externalPath);

  for (auto &&playerName : m_mediaPlayers.keys()) {
    auto playerData = m_mediaPlayers.value(playerName).toObject();
    auto exeNames = playerData.value("exeNames").toArray();

    for (auto &&exeName : exeNames) {
      m_exeNames.insert(exeName.toString().toLower(), playerName);
    }
  }

  AniList &anilist = AniList::instance();

  windowTimer = new QTimer(this);
  windowTimer->setInterval(5000);
  windowTimer->setSingleShot(true);

  connect(windowTimer, &QTimer::timeout, this, &WindowEnumerator::enumerateWindows);
  connect(&anilist, &AniList::authenticated, this, [this]() { windowTimer->start(); });
}

void WindowEnumerator::enumerateWindows() {
  MediaStore &store = MediaStore::instance();
  WindowList wList = Window::GetList();
  MediaList &mediaList = MediaList::instance();

  if (!s.get(Setting::UseAnimeRecognition).toBool() || mediaList.loading()) {
    windowTimer->start();
    return;
  }

  bool mediaFound = false;

  store.removeInvalid();

  for (auto &&window : wList) {
    if (!window.IsValid()) {
      continue;
    }

    Process process = window.GetProcess();

    if (store.hasMediaPlayer(process.GetPID())) {
      mediaFound |= detectMedia(process);
      continue;
    }

    if (store.hasProcess(process.GetPID())) {
      continue;
    }

    store.addProcess(process);

    if (isMediaPlayer(process)) {
      mediaFound |= detectMedia(process);
      store.addMediaPlayer(process);
    }
  }

  if (!mediaFound) {
    store.setMediaPlaying(nullptr, 0);
  }

  windowTimer->start();
}

bool WindowEnumerator::isMediaPlayer(const Process &process) {
  auto name = QString::fromStdString(process.GetName());

  return this->m_exeNames.contains(name.toLower());
}

bool WindowEnumerator::detectMedia(const Process &process) {
  auto name = QString::fromStdString(process.GetName());
  auto playerName = this->m_exeNames.value(name.toLower());
  auto playerData = this->m_mediaPlayers.value(playerName).toObject();

  auto method = playerData.value("method").toString();

  if (method == "TITLE") {
    auto titleRegex = playerData.value("titleRegex").toString();
    auto regex = std::regex(titleRegex.toStdString());

    for (auto &&window : process.GetWindows()) {
      auto title = window.GetTitle();
      std::smatch base_match;
      if (std::regex_match(title, base_match, regex)) {
        if (base_match.size() == 2) {
          auto base_sub_match = base_match[1];
          auto title = base_sub_match.str();
          processTitle(title);
          return true;
        }
      }
    }
  }

  return false;
}

void WindowEnumerator::processTitle(const std::string &title) {
  std::wstring ws;
  ws.assign(title.begin(), title.end());
  m_anitomy.Parse(ws);

  const auto &elements = m_anitomy.elements();

  const auto &mediaTitle = elements.get(anitomy::kElementAnimeTitle);
  const auto &mediaEpisode = elements.get(anitomy::kElementEpisodeNumber);

  auto &mediaList = MediaList::instance();
  auto &store = MediaStore::instance();

  auto media = mediaList.getMediaByTitle(QString::fromStdWString(mediaTitle));
  auto episode = QString::fromStdWString(mediaEpisode).toInt();

  if (!media.empty() && !store.isBlackListed(mediaTitle, (*media.begin())->id())) {
    store.setMediaPlaying(*media.begin(), episode);
    store.setCurrentTitle(mediaTitle);
  }
}

void WindowEnumerator::readMediaPlayersJson(const QString &mediaPlayersJsonPath) {
  QFile mediaPlayerJson(mediaPlayersJsonPath);

  if (!mediaPlayerJson.exists()) {
    qDebug() << "File not found" << mediaPlayersJsonPath;
    return;
  }

  mediaPlayerJson.open(QFile::ReadOnly);

  auto mediaPlayers = QJsonDocument().fromJson(mediaPlayersJson.readAll()).object();

  for(auto &&playerName : mediaPlayers.keys()) {
    if (m_mediaPlayers.contains(playerName)) {
      qInfo() << "Media Player already existed," << playerName;
    }
    m_mediaPlayers[playerName] = mediaPlayers[playerName];
  }
}