#include "application.h"
#include "ui_application.h"

namespace Views {
Application::Application(QWidget *parent)
    : QWidget(parent), ui(new Ui::Application) {
  ui->setupUi(this);
}

Application::~Application() { delete ui; }
}  // namespace Views
