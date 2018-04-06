#ifndef NOW_PLAYING_LIST_MODEL_H
#define NOW_PLAYING_LIST_MODEL_H

#include <QAbstractListModel>

#include <Robot.h>

class NowPlayingListModel : public QAbstractListModel {
  Q_OBJECT

 public:
  NowPlayingListModel(QObject *parent = 0);

  int rowCount(const QModelIndex &parent) const;
  QVariant data(const QModelIndex &index, int role) const;

  void setProcesses(QList<Robot::Process> processes);
  Robot::Process process(const QModelIndex &index) const;

 private:
  QList<Robot::Process> m_processes;
};

#endif  // NOW_PLAYING_LIST_MODEL_H
