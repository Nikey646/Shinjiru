#ifndef ANIME_PANEL_H
#define ANIME_PANEL_H

#include <QDialog>
#include <QJsonObject>

#include "../../src/models/media.h"

namespace Ui {
class AnimePanel;
}

class AnimePanel : public QDialog {
  Q_OBJECT

 public:
  explicit AnimePanel(Media *media, QWidget *parent = 0);
  ~AnimePanel();

 private:
  void createScoreEditor();

 private:
  Ui::AnimePanel *ui;
  Media *media;
  QJsonObject changes;
};

#endif  // ANIME_PANEL_H
