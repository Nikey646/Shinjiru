#include "settings_dialog.h"
#include "ui_settings_dialog.h"

#include <QSettings>

#include "../../src/settings.h"

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::SettingsDialog),
      viewApplication(new Views::Application),
      viewRecognition(new Views::Recognition),
      viewTorrents(new Views::TorrentSettings) {
  ui->setupUi(this);

  this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));

  ui->settingsPanel->addWidget(viewApplication);
  ui->settingsPanel->addWidget(viewRecognition);
  ui->settingsPanel->addWidget(viewTorrents);

  this->widgets.append(viewApplication);
  this->widgets.append(viewRecognition);
  this->widgets.append(viewTorrents);

  ui->settingsPanel->setCurrentIndex(0);
  ui->listSettings->setCurrentRow(0);

  connect(ui->listSettings, &QListWidget::currentRowChanged, this,
          [this](int row) { ui->settingsPanel->setCurrentIndex(row); });

  connect(ui->buttonCancel, &QPushButton::clicked, this, [this]() { this->reject(); });

  connect(ui->buttonOk, &QPushButton::clicked, this, [this]() {
    for (auto &&widget : this->widgets) {
      auto changes = widget->changes();
      Settings settings;

      std::for_each(changes.keyBegin(), changes.keyEnd(), [&changes, &settings](Setting key) {
        QVariant value = changes.value(key);
        settings.set(key, value);
      });

      widget->commit();
    }

    this->accept();
  });

  connect(ui->buttonDefault, &QPushButton::clicked, this, [this]() {
    // TODO
    bool accepted = true;

    if (accepted) {
      for (auto &&widget : this->widgets) {
        widget->resetToDefault();
      }
    }
  });
}

SettingsDialog::~SettingsDialog() {
  delete ui;
}
