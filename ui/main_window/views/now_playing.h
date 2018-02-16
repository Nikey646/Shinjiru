#ifndef UI_MAIN_WINDOW_VIEWS_NOW_PLAYING_H__
#define UI_MAIN_WINDOW_VIEWS_NOW_PLAYING_H__

#include <QWidget>

namespace Ui {
class NowPlaying;
}

namespace Views {
class NowPlaying : public QWidget {
  Q_OBJECT

 public:
  explicit NowPlaying(QWidget *parent = 0);
  ~NowPlaying();

 private:
  Ui::NowPlaying *ui;
};
}  // namespace Views

#endif  // UI_MAIN_WINDOW_VIEWS_NOW_PLAYING_H__
