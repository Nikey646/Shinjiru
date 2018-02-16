#include "torrents.h"
#include "ui_torrents.h"

namespace Views {

Torrents::Torrents(QWidget *parent) : QWidget(parent), ui(new Ui::Torrents) {
  ui->setupUi(this);
}

Torrents::~Torrents() { delete ui; }

}  // namespace Views
