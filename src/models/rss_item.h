#ifndef SRC_MODELS_RSS_ITEM_H__
#define SRC_MODELS_RSS_ITEM_H__

#include <QObject>

class RSSItem : public QObject {
  Q_OBJECT

 public:
  explicit RSSItem(QObject *parent = nullptr);

 public:
  QString title;
  int mediaId;
  QString episode;
  QString subGroup;
  QString quality;
  QString size;
  QString link;
  QString fileName;
};

#endif  // SRC_MODELS_RSS_ITEM_H__
