#ifndef UI_SETTINGS_DIALOG_VIEWS_RECOGNITION_H__
#define UI_SETTINGS_DIALOG_VIEWS_RECOGNITION_H__

#include "../components/committable_widget.h"

#include <QMap>
#include <QString>
#include <QStringListModel>
#include <QVariant>

namespace Ui {
class Recognition;
}

namespace Views {
class Recognition : public CommittableWidget {
  Q_OBJECT

 public:
  explicit Recognition(QWidget *parent = 0);
  ~Recognition();

  void resetToDefault() override;

 private:
  Ui::Recognition *ui;
  QStringListModel *model;
  QString currentText;
  QMap<QString, QVariant> blackListedTitles;
};
}  // namespace Views

#endif  // UI_SETTINGS_DIALOG_VIEWS_RECOGNITION_H__
