#include "media_store.h"

#include "../settings.h"

#include "../models/media_list.h"

MediaStore::MediaStore() {
  Settings s;
  this->m_blackList = s.get(Setting::BlackListedTitles).toMap();
}

QList<Robot::Process> MediaStore::mediaPlayers() const {
  return this->m_mediaPlayers.values();
}

QList<Robot::Process> MediaStore::processes() const {
  return this->m_processes.values();
}

Media *MediaStore::mediaPlaying() const {
  return m_mediaPlaying;
}

int MediaStore::episodePlaying() const {
  return m_episode;
}

bool MediaStore::hasMediaPlayer(int pid) const {
  return this->m_mediaPlayers.contains(pid);
}

bool MediaStore::hasProcess(int pid) const {
  return this->m_processes.contains(pid);
}

void MediaStore::addMediaPlayer(const Robot::Process &player) {
  if (this->m_mediaPlayers.contains(player.GetPID())) {
    return;
  }

  m_mediaPlayers.insert(player.GetPID(), player);
  emit mediaPlayersChanged();
}

void MediaStore::addProcess(const Robot::Process &process) {
  if (this->m_processes.contains(process.GetPID())) {
    return;
  }

  m_processes.insert(process.GetPID(), process);
  emit processesChanged();
}

void MediaStore::setMediaPlaying(Media *media, int episode) {
  if (media != nullptr) {
    const auto episodes = media->episodes();

    if (episodes != 0 && episodes < episode) {
      if (media->sequel() != 0) {
        auto sequel = MediaList::instance().getMediaById(media->sequel());
        this->setMediaPlaying(sequel, episode - episodes);

        return;
      }
    }
  }

  if (m_mediaPlaying != media) {
    m_mediaPlaying = media;
    m_episode = episode;
    emit mediaPlayingChanged();
  }
}

void MediaStore::setCurrentTitle(const std::wstring &title) {
  if (title != m_currentTitle) {
    m_currentTitle = title;
  }
}

void MediaStore::removeInvalid() {
  QMutableHashIterator<int, Robot::Process> iproc(m_processes);

  while (iproc.hasNext()) {
    auto process = iproc.next().value();

    if (process.HasExited()) {
      iproc.remove();
      emit processesChanged();
    }
  }

  QMutableHashIterator<int, Robot::Process> imedia(m_mediaPlayers);

  while (imedia.hasNext()) {
    auto process = imedia.next().value();

    if (process.HasExited()) {
      imedia.remove();
      emit mediaPlayersChanged();
    }
  }
}

void MediaStore::blackListCurrent() {
  auto title = QString::fromStdWString(this->m_currentTitle);
  auto id = this->m_mediaPlaying->id();

  this->m_blackList.insert(title, id);
  this->setMediaPlaying(nullptr, 0);

  Settings s;
  s.set(Setting::BlackListedTitles, this->m_blackList);
}

bool MediaStore::isBlackListed(std::wstring title, int id) {
  QString key = QString::fromStdWString(title);
  return this->m_blackList.contains(key) && this->m_blackList.value(key) == id;
}
