#ifndef UI_MAIN_WINDOW_VIEWS_NOW_PLAYING_H__
#define UI_MAIN_WINDOW_VIEWS_NOW_PLAYING_H__

#include <QStringListModel>
#include <QTimer>
#include <QWidget>

#include "../../../src/models/media.h"
#include "../../../src/settings.h"
#include "../components/now_playing_list_model.h"

namespace Ui {
class NowPlaying;
}

namespace Views {
class NowPlaying : public QWidget {
  Q_OBJECT

 public:
  explicit NowPlaying(QWidget *parent = 0);
  ~NowPlaying();

  void updateMedia();

 private:
  Ui::NowPlaying *ui;
  Media *media{nullptr};
  QString episode;
  QTimer *timer{nullptr};
  int timerTime{0};
  NowPlayingListModel *model;
  Settings s;
};
}  // namespace Views

#endif  // UI_MAIN_WINDOW_VIEWS_NOW_PLAYING_H__
