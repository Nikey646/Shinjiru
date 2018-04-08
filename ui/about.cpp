#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) : QDialog(parent), ui(new Ui::About) {
  ui->setupUi(this);

  const auto version = QString("%1.%2.%3").arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_PATCH);
  ui->appName->setText(tr("Shinjiru %1").arg(version));
  ui->builtWith->setText(tr("Built with Qt %1").arg(QT_VERSION_STR));
}

About::~About() {
  delete ui;
}
