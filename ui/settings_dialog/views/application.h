#ifndef UI_SETTINGS_DIALOG_VIEWS_APPLICATION_H__
#define UI_SETTINGS_DIALOG_VIEWS_APPLICATION_H__

#include "../components/committable_widget.h"

namespace Ui {
class Application;
}

namespace Views {
class Application : public CommittableWidget {
  Q_OBJECT

 public:
  explicit Application(QWidget *parent = 0);
  ~Application();

  void resetToDefault() override;
  void commit() override;

 private:
  Ui::Application *ui;
};
}  // namespace Views

#endif  // UI_SETTINGS_DIALOG_VIEWS_APPLICATION_H__
