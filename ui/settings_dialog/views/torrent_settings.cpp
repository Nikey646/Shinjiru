#include "./torrent_settings.h"
#include "ui_torrent_settings.h"

namespace Views {
TorrentSettings::TorrentSettings(QWidget *parent)
    : CommittableWidget(parent), ui(new Ui::TorrentSettings) {
  ui->setupUi(this);
}

TorrentSettings::~TorrentSettings() {
  delete ui;
}

void TorrentSettings::resetToDefault() {}
}  // namespace Views
