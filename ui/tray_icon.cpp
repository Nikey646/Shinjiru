#include "tray_icon.h"

#include <QApplication>
#include <QFile>
#include <QIcon>
#include <QMenu>
#include <QObject>
#include <QPixmap>
#include <QTimer>

#include "../src/detection/media_store.h"
#include "../src/models/media_list.h"
#include "./main_window.h"

TrayIcon::TrayIcon(QObject *parent) : QSystemTrayIcon(parent) {
  {
    QPixmap window_icon;
    QFile iconFile(":/res/icon.svg");
    iconFile.open(QFile::ReadOnly);
    QByteArray icon_data = iconFile.readAll();
    window_icon.loadFromData(icon_data);
    this->setIcon(QIcon(window_icon));
  }

  this->buildMenu();

  const auto version = QString("%1.%2.%3").arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_PATCH);
  this->setToolTip("Shinjiru " + version);

  connect(this, &TrayIcon::activated, this, [this](QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::DoubleClick) {
      this->restore();
    }
  });

  auto &store = MediaStore::instance();

  connect(&store, &MediaStore::mediaPlayingChanged, this, [this]() {
    auto &store = MediaStore::instance();
    auto &mediaList = MediaList::instance();

    auto media = store.mediaPlaying();

    if (media != nullptr) {
      auto episodePlaying = store.episodePlaying();

      currentEpisode = episodePlaying;
      currentMedia = media;

      if (episodePlaying > media->progress()) {
        QTimer *updateTimer = new QTimer;
        mediaList.resetCancel();
        connect(updateTimer, &QTimer::timeout, this, [this, updateTimer, media, episodePlaying]() {
          auto &store = MediaStore::instance();
          auto &mediaList = MediaList::instance();

          auto correctEpisode = episodePlaying == store.episodePlaying();
          auto correctMedia = media == store.mediaPlaying();

          if (!mediaList.updateCancelled() && correctEpisode && correctMedia) {
            if (s.get(Setting::UpdateOnClose).toBool()) {
              updatePending = true;
            } else {
              updateMediaProgress(media, episodePlaying);
            }
          }
          updateTimer->deleteLater();
        });

        auto delay = s.get(Setting::UpdateDelay).toInt();
        updateTimer->setSingleShot(true);
        updateTimer->start(delay * 1000);

        if (s.get(Setting::NotifyDetected).toBool()) {
          QString time;
          if (delay < 60) {
            time = tr("%n second(s)", "", delay);
          } else {
            time = tr("%n minute(s)", "", delay / 60);
          }

          auto message = tr("Updating %1 to episode %2 in %3")
                             .arg(media->title())
                             .arg(episodePlaying)
                             .arg(time);
          this->showMessage("Shinjiru", message);
        }
      }
    } else {
      if (updatePending) {
        updateMediaProgress(currentMedia, currentEpisode);
        updatePending = false;
      }

      currentMedia = media;
    }

  });

  this->show();
}

TrayIcon::~TrayIcon() {
  delete this->contextMenu();
}

void TrayIcon::setShowFunction(std::function<void()> fn) {
  show_fn = fn;
}

void TrayIcon::restore() {
  MainWindow *mw = reinterpret_cast<MainWindow *>(this->parent());

  if (show_fn) {
    show_fn();
  } else {
    mw->showNormal();
  }

  qApp->setActiveWindow(mw);
}

void TrayIcon::buildMenu() {
  QMenu *menu = new QMenu;

  QAction *actionRestore = menu->addAction(tr("Restore"));
  menu->addSeparator();
  QAction *actionExit = menu->addAction(tr("Exit"));

  connect(actionRestore, &QAction::triggered, this, &TrayIcon::restore);
  connect(actionExit, &QAction::triggered, this, []() { qApp->exit(); });

  this->setContextMenu(menu);
}

void TrayIcon::updateMediaProgress(Media *media, int episodePlaying) {
  auto &mediaList = MediaList::instance();

  QJsonObject data;
  data["progress"] = episodePlaying;
  data["status"] = "CURRENT";

  if (episodePlaying == media->episodes()) {
    data["status"] = "COMPLETED";
  }

  mediaList.updateMedia(media, data);

  if (s.get(Setting::NotifyUpdated).toBool()) {
    QString message;

    if (data["status"] == "COMPLETED") {
      message = tr("%1 marked as completed").arg(media->title());
    } else {
      message = tr("%1 updated to episode %2").arg(media->title()).arg(episodePlaying);
    }
    this->showMessage("Shinjiru", message);
  }
}
