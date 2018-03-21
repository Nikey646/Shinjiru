#include "./torrent_settings.h"
#include "ui_torrent_settings.h"

namespace Views {
TorrentSettings::TorrentSettings(QWidget *parent)
    : QWidget(parent), ui(new Ui::TorrentSettings) {
  ui->setupUi(this);
}

TorrentSettings::~TorrentSettings() { delete ui; }
}  // namespace Views
