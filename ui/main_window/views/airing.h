#ifndef UI_MAIN_WINDOW_VIEWS_AIRING_H__
#define UI_MAIN_WINDOW_VIEWS_AIRING_H__

#include <QWidget>

namespace Ui {
class Airing;
}

namespace Views {
class Airing : public QWidget {
  Q_OBJECT

 public:
  explicit Airing(QWidget *parent = 0);
  ~Airing();

 private:
  Ui::Airing *ui;
};
}  // namespace Views

#endif  // UI_MAIN_WINDOW_VIEWS_AIRING_H__
