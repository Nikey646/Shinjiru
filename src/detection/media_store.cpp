#include "media_store.h"

#include "../models/media_list.h"

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

void MediaStore::removeInvalid() {
  std::for_each(m_processes.keyBegin(), m_processes.keyEnd(), [this](auto key) {
    auto process = m_processes.value(key);

    if (!process.IsValid()) {
      m_processes.remove(key);
      emit processesChanged();
    }
  });

  std::for_each(m_mediaPlayers.keyBegin(), m_mediaPlayers.keyEnd(), [this](auto key) {
    auto process = this->m_mediaPlayers.value(key);

    if (!process.IsValid()) {
      this->m_mediaPlayers.remove(key);
      emit mediaPlayersChanged();
    }
  });
}
