#ifndef UI_MAIN_WINDOW_VIEWS_TORRENTS_H__
#define UI_MAIN_WINDOW_VIEWS_TORRENTS_H__

#include <QWidget>

namespace Ui {
class Torrents;
}

namespace Views {
class Torrents : public QWidget {
  Q_OBJECT

 public:
  explicit Torrents(QWidget *parent = 0);
  ~Torrents();

 private:
  Ui::Torrents *ui;
};
}  // namespace Views

#endif  // UI_MAIN_WINDOW_VIEWS_TORRENTS_H__
