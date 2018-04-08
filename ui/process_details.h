#ifndef PROCESS_DETAILS_H
#define PROCESS_DETAILS_H

#include <QDialog>

#include <Robot.h>

namespace Ui {
class ProcessDetails;
}

class ProcessDetails : public QDialog {
  Q_OBJECT

 public:
  explicit ProcessDetails(const Robot::Process &process, QWidget *parent = 0);
  ~ProcessDetails();

 private:
  Ui::ProcessDetails *ui;
};

#endif  // PROCESS_DETAILS_H
