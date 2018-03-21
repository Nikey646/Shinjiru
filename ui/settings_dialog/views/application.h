#ifndef UI_SETTINGS_DIALOG_VIEWS_APPLICATION_H__
#define UI_SETTINGS_DIALOG_VIEWS_APPLICATION_H__

#include <QWidget>

namespace Ui {
class Application;
}

namespace Views {
class Application : public QWidget {
  Q_OBJECT

 public:
  explicit Application(QWidget *parent = 0);
  ~Application();

 private:
  Ui::Application *ui;
};
}  // namespace Views

#endif  // UI_SETTINGS_DIALOG_VIEWS_APPLICATION_H__
