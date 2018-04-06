#ifndef RULE_IMPORT_DIALOG_H
#define RULE_IMPORT_DIALOG_H

#include <QDialog>

namespace Ui {
class RuleImportDialog;
}

class RuleImportDialog : public QDialog {
  Q_OBJECT

 public:
  explicit RuleImportDialog(QWidget *parent = 0);
  ~RuleImportDialog();

 public:
  bool useSeason{false};
  QString list;
  QString season;

 private:
  Ui::RuleImportDialog *ui;
};

#endif  // RULE_IMPORT_DIALOG_H
