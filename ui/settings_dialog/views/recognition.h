#ifndef UI_SETTINGS_DIALOG_VIEWS_RECOGNITION_H__
#define UI_SETTINGS_DIALOG_VIEWS_RECOGNITION_H__

#include <QWidget>

namespace Ui {
class Recognition;
}

namespace Views {
class Recognition : public QWidget {
  Q_OBJECT

 public:
  explicit Recognition(QWidget *parent = 0);
  ~Recognition();

 private:
  Ui::Recognition *ui;
};
}  // namespace Views

#endif  // UI_SETTINGS_DIALOG_VIEWS_RECOGNITION_H__
