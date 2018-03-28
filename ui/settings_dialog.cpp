#include "settings_dialog.h"
#include "ui_settings_dialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::SettingsDialog),
      viewApplication(new Views::Application),
      viewRecognition(new Views::Recognition),
      viewTorrents(new Views::TorrentSettings) {
  ui->setupUi(this);

  ui->settingsPanel->addWidget(viewApplication);
  ui->settingsPanel->addWidget(viewRecognition);
  ui->settingsPanel->addWidget(viewTorrents);
  ui->settingsPanel->setCurrentIndex(0);
  ui->listSettings->setCurrentRow(0);

  connect(ui->listSettings, &QListWidget::currentRowChanged, this,
          [this](int row) { ui->settingsPanel->setCurrentIndex(row); });
}

SettingsDialog::~SettingsDialog() {
  delete ui;
}
