#include "process_details.h"
#include "ui_process_details.h"

ProcessDetails::ProcessDetails(const Robot::Process &process, QWidget *parent)
    : QDialog(parent), ui(new Ui::ProcessDetails) {
  ui->setupUi(this);
  ui->exeName->setText(QString::fromStdString(process.GetName()));

  auto windows = process.GetWindows();

  for (auto &&window : windows) {
    ui->windowTitle->setText(QString::fromStdString(window.GetTitle()));
  }
}

ProcessDetails::~ProcessDetails() {
  delete ui;
}
