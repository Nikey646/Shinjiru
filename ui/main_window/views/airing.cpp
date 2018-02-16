#include "airing.h"
#include "ui_airing.h"

namespace Views {

Airing::Airing(QWidget *parent) : QWidget(parent), ui(new Ui::Airing) {
  ui->setupUi(this);
}

Airing::~Airing() { delete ui; }

}  // namespace Views
