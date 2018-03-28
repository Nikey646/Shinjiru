#include "recognition.h"
#include "ui_recognition.h"

namespace Views {
Recognition::Recognition(QWidget *parent) : QWidget(parent), ui(new Ui::Recognition) {
  ui->setupUi(this);
}

Recognition::~Recognition() {
  delete ui;
}
}  // namespace Views
