#include "media_store.h"

QList<Robot::Process> MediaStore::mediaPlayers() const {
  return this->m_mediaPlayers.values();
}

QList<Robot::Process> MediaStore::processes() const {
  return this->m_processes.values();
}

bool MediaStore::hasMediaPlayer(int pid) {
  return this->m_mediaPlayers.contains(pid);
}

bool MediaStore::hasProcess(int pid) { return this->m_processes.contains(pid); }

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

void MediaStore::removeInvalid() {
  for (auto &&key : this->m_processes.keys()) {
    auto process = this->m_processes.value(key);

    if (!process.IsValid()) {
      this->m_processes.remove(key);
      emit processesChanged();
    }
  }

  for (auto &&key : this->m_mediaPlayers.keys()) {
    auto process = this->m_mediaPlayers.value(key);

    if (!process.IsValid()) {
      this->m_mediaPlayers.remove(key);
      emit mediaPlayersChanged();
    }
  }
}
