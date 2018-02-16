#include "./media_table_model.h"

MediaTableModel::MediaTableModel(QObject *parent)
    : QAbstractTableModel(parent) {}

void MediaTableModel::setList(const QSet<int> &list) { m_list = list; }

void MediaTableModel::refresh() { emit layoutChanged(); }

int MediaTableModel::rowCount(const QModelIndex &p) const {
  Q_UNUSED(p);
  return m_list.count();
}

int MediaTableModel::columnCount(const QModelIndex &p) const {
  Q_UNUSED(p);
  return static_cast<int>(ListRoles::_size());
}

QVariant MediaTableModel::headerData(int s, Qt::Orientation o, int r) const {
  if (r == Qt::DisplayRole && o == Qt::Horizontal) {
    const ListRoles role = ListRoles::_from_integral(s);
    return tr(role._to_string());
  }

  return QVariant();
}

bool MediaTableModel::defaultHidden(int section) const {
  switch (section) {
    case ListRoles::ID:
      return true;
      break;
  }

  return false;
}

bool MediaTableModel::setData(const QModelIndex &index, const QVariant &value,
                              int role) {
  if (role != Qt::EditRole) {
    return false;
  }

  Media *media = this->media(index);

  if (media == nullptr) {
    return false;
  }

  if (index.column() == ListRoles::Progress) {
    media->setProgress(value.toInt());
    emit mediaChanged(media);
    return true;
  } else if (index.column() == ListRoles::Status) {
    media->setListStatus(value.toString());
    emit mediaChanged(media);
    return true;
  } else if (index.column() == ListRoles::Score) {
    media->setScore(value.toInt());
    emit mediaChanged(media);
    return true;
  }

  return false;
}

QVariant MediaTableModel::data(const QModelIndex &index, int role) const {
  if (index.row() < 0 || index.row() >= m_list.count() ||
      role != Qt::DisplayRole) {
    return QVariant();
  }

  Media *media = this->media(index);

  switch (index.column()) {
    case ListRoles::ID:
      return QString::number(media->id());
      break;
    case ListRoles::Title:
      return media->title();
      break;
    case ListRoles::Progress:
      return QString::number(media->progress());
      break;
    case ListRoles::Episodes: {
      auto episodes = media->episodes();
      return episodes == 0 ? "-" : QString::number(episodes);
      break;
    }
    case ListRoles::Score: {
      auto scoreFormat = User::instance().scoreFormat();
      auto score = media->score();

      if (scoreFormat == "POINT_100") {
        return QString::number(score);
      } else if (scoreFormat == "POINT_10") {
        return QString::number(score / 10);
      } else if (scoreFormat == "POINT_5") {
        return QString::number(score / 20) + " ★";
      } else if (scoreFormat == "POINT_3") {
        return "0";
      } else if (scoreFormat == "POINT_10_DECIMAL") {
        return QString::number(score / 10.0);
      }
      break;
    }
    case ListRoles::Status:
      return tr(qPrintable(media->listStatus()));
      break;
    case ListRoles::AiringStatus:
      return tr(qPrintable(media->airingStatus()));
      break;
  }

  return "";
}

Media *MediaTableModel::media(const QModelIndex &index) const {
  if (index.row() < 0 || index.row() >= m_list.count()) {
    return nullptr;
  }

  int media_id = *(m_list.begin() + index.row());
  Media *media = MediaList::instance().getMediaById(media_id);

  return media;
}

Qt::ItemFlags MediaTableModel::flags(const QModelIndex &index) const {
  Qt::ItemFlags flags = QAbstractTableModel::flags(index);

  if (index.column() == ListRoles::Progress) {
    flags |= Qt::ItemIsEditable;
  }

  if (index.column() == ListRoles::Status) {
    flags |= Qt::ItemIsEditable;
  }

  if (index.column() == ListRoles::Score) {
    flags |= Qt::ItemIsEditable;
  }

  return flags;
}
