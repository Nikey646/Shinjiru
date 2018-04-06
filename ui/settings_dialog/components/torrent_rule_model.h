#ifndef TORRENTRULEMODEL_H
#define TORRENTRULEMODEL_H

#include <QAbstractItemModel>
#include <QJsonObject>
#include <QList>

#include "./torrent_rule.h"

class TorrentRuleModel : public QAbstractItemModel {
  Q_OBJECT

 public:
  explicit TorrentRuleModel(QObject *parent = nullptr);

  QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &index) const override;

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  TorrentRule *rule(const QModelIndex &index) const;

  void addRule(const QString &key, TorrentRule *rule);
  void save();
  void beginEdit();
  void endEdit();

 private:
  void loadFromObject(QJsonObject obj);

 private:
  TorrentRule *rootData;
};

#endif  // TORRENTRULEMODEL_H
