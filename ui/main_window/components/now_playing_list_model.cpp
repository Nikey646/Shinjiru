#include "now_playing_list_model.h"

NowPlayingListModel::NowPlayingListModel(QObject *parent) : QAbstractListModel(parent) {}

int NowPlayingListModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent)
  return m_processes.count();
}

QVariant NowPlayingListModel::data(const QModelIndex &index, int role) const {
  if (index.row() < 0 || index.row() >= m_processes.count() || role != Qt::DisplayRole) {
    return QVariant();
  }

  Robot::Process process = this->process(index);

  return QString::fromStdString(process.GetName());
}

void NowPlayingListModel::setProcesses(QList<Robot::Process> processes) {
  emit layoutAboutToBeChanged();
  m_processes = processes;
  emit layoutChanged();
}

Robot::Process NowPlayingListModel::process(const QModelIndex &index) const {
  return *(m_processes.begin() + index.row());
}
