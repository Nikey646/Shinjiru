#include "torrent_rule_model.h"

#include <QApplication>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

TorrentRuleModel::TorrentRuleModel(QObject *parent) : QAbstractItemModel(parent) {
  QFile rules(qApp->applicationDirPath() + "/torrent_rules.json");

  if (!rules.exists()) {
    rules.open(QFile::WriteOnly);
    rules.write("{}");
    rules.close();
  }

  rules.open(QFile::ReadOnly);
  QJsonDocument doc = QJsonDocument::fromJson(rules.readAll());
  QJsonObject obj = doc.object();

  rootData = new TorrentRule;

  loadFromObject(obj);
}

QModelIndex TorrentRuleModel::index(int row, int column, const QModelIndex &parent) const {
  if (!hasIndex(row, column, parent)) return QModelIndex();

  TorrentRule *parentItem;

  if (!parent.isValid())
    parentItem = rootData;
  else
    parentItem = static_cast<TorrentRule *>(parent.internalPointer());

  TorrentRule *childItem = parentItem->child(row);
  if (childItem)
    return createIndex(row, column, childItem);
  else
    return QModelIndex();
}

QModelIndex TorrentRuleModel::parent(const QModelIndex &index) const {
  if (!index.isValid()) return QModelIndex();

  TorrentRule *childItem = static_cast<TorrentRule *>(index.internalPointer());
  TorrentRule *parentItem = childItem->parentItem();

  if (parentItem == rootData) return QModelIndex();
  if (parentItem == nullptr) return QModelIndex();

  return createIndex(parentItem->row(), 0, parentItem);
}

int TorrentRuleModel::rowCount(const QModelIndex &parent) const {
  TorrentRule *parentItem;
  if (parent.column() > 0) return 0;

  if (!parent.isValid())
    parentItem = rootData;
  else
    parentItem = static_cast<TorrentRule *>(parent.internalPointer());

  return parentItem->childCount();
}

int TorrentRuleModel::columnCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return static_cast<TorrentRule *>(parent.internalPointer())->columnCount();
  else
    return rootData->columnCount();
}

QVariant TorrentRuleModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid()) return QVariant();

  if (role != Qt::DisplayRole) return QVariant();

  TorrentRule *item = static_cast<TorrentRule *>(index.internalPointer());

  return item->data(index.column());
}

TorrentRule *TorrentRuleModel::rule(const QModelIndex &index) const {
  if (!index.isValid()) return nullptr;

  TorrentRule *item = static_cast<TorrentRule *>(index.internalPointer());

  return item;
}

void TorrentRuleModel::addRule(const QString &key, TorrentRule *rule) {
  TorrentRule *parent = nullptr;

  if (key.size() == 0) {
    parent = rootData;
  } else {
    for (int i = 0; i < rootData->childCount(); ++i) {
      TorrentRule *tag = rootData->child(i);

      if (tag->isTag && tag->tag == key) {
        parent = tag;
      }
    }
  }

  if (parent == nullptr) {
    TorrentRule *tag = new TorrentRule(rootData);
    tag->isTag = true;
    tag->tag = key;
    rootData->appendChild(tag);
    parent = tag;
  }

  emit layoutAboutToBeChanged();
  parent->appendChild(rule);
  emit layoutChanged();
}

void TorrentRuleModel::save() {
  QFile rules(qApp->applicationDirPath() + "/torrent_rules.json");
  QJsonObject obj;

  rules.open(QFile::WriteOnly);

  for (int i = 0; i < rootData->childCount(); ++i) {
    TorrentRule *rule = rootData->child(i);

    if (rule->isTag) {
      QJsonArray arr;

      for (int j = 0; j < rule->childCount(); ++j) {
        TorrentRule *child = rule->child(j);

        QJsonObject data;

        data["id"] = child->id;
        data["subGroup"] = child->subGroup;
        data["quality"] = child->quality;

        arr.append(data);
      }

      obj[rule->tag] = arr;
    } else {
      if (!obj.contains("__rootObject")) {
        obj["__rootObject"] = QJsonArray();
      }

      QJsonObject data;

      data["id"] = rule->id;
      data["subGroup"] = rule->subGroup;
      data["quality"] = rule->quality;

      QJsonArray arr = obj["__rootObject"].toArray();
      arr.append(data);
      obj["__rootObject"] = arr;
    }
  }

  rules.write(QJsonDocument(obj).toJson());
}

void TorrentRuleModel::beginEdit() {
  emit layoutAboutToBeChanged();
}

void TorrentRuleModel::endEdit() {
  emit layoutChanged();
}

void TorrentRuleModel::loadFromObject(QJsonObject obj) {
  for (auto it = obj.begin(); it != obj.end(); ++it) {
    auto key = it.key();
    auto data = it.value().toArray();
    TorrentRule *parent = nullptr;

    if (key == "__rootObject") {
      parent = rootData;
    } else {
      for (int i = 0; i < rootData->childCount(); ++i) {
        TorrentRule *tag = rootData->child(i);

        if (tag->isTag && tag->tag == key) {
          parent = tag;
        }
      }
    }

    if (parent == nullptr) {
      TorrentRule *tag = new TorrentRule(rootData);
      tag->isTag = true;
      tag->tag = key;
      rootData->appendChild(tag);
      parent = tag;
    }

    for (auto datum : data) {
      auto ruleData = datum.toObject();

      TorrentRule *rule = new TorrentRule(parent);
      rule->id = ruleData["id"].toInt();
      rule->subGroup = ruleData["subGroup"].toString();
      rule->quality = ruleData["quality"].toString();
      parent->appendChild(rule);
    }
  }
}
