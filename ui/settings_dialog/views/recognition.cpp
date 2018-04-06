#include "recognition.h"
#include "ui_recognition.h"

#include "../../../src/settings.h"

namespace Views {
Recognition::Recognition(QWidget *parent) : CommittableWidget(parent), ui(new Ui::Recognition) {
  ui->setupUi(this);

  ui->updateDelay->addItem(tr("30 seconds"), 30);
  ui->updateDelay->addItem(tr("1 minute"), 60);
  ui->updateDelay->addItem(tr("2 minutes"), 120);
  ui->updateDelay->addItem(tr("5 minutes"), 300);

  Settings s;

  auto enableAnimeRecognition = s.get(Setting::UseAnimeRecognition).toBool();
  auto enableMasato = s.get(Setting::UseMasato).toBool();
  auto enableDiscord = s.get(Setting::UseDiscord).toBool();
  auto updateDelay = s.get(Setting::UpdateDelay).toInt();
  auto notifyDetected = s.get(Setting::NotifyDetected).toBool();
  auto notifyUpdated = s.get(Setting::NotifyUpdated).toBool();

  ui->enableAnimeRecognition->setChecked(enableAnimeRecognition);
  ui->enableMasato->setChecked(enableMasato);
  ui->enableDiscord->setChecked(enableDiscord);
  ui->updateDelay->setCurrentIndex(ui->updateDelay->findData(updateDelay));
  ui->notifyDetected->setChecked(notifyDetected);
  ui->notifyUpdated->setChecked(notifyUpdated);

  connect(ui->enableAnimeRecognition, &QCheckBox::clicked, this,
          [this](bool checked) { this->changed_settings[Setting::UseAnimeRecognition] = checked; });

  connect(ui->enableMasato, &QCheckBox::clicked, this,
          [this](bool checked) { this->changed_settings[Setting::UseMasato] = checked; });

  connect(ui->enableDiscord, &QCheckBox::clicked, this,
          [this](bool checked) { this->changed_settings[Setting::UseDiscord] = checked; });

  connect(ui->updateDelay, &QComboBox::currentTextChanged, this, [this]() {
    this->changed_settings[Setting::UpdateDelay] = ui->updateDelay->currentData();
  });

  connect(ui->notifyDetected, &QCheckBox::clicked, this,
          [this](bool checked) { this->changed_settings[Setting::NotifyDetected] = checked; });

  connect(ui->notifyUpdated, &QCheckBox::clicked, this,
          [this](bool checked) { this->changed_settings[Setting::NotifyUpdated] = checked; });
}

Recognition::~Recognition() {
  delete ui;
}

void Recognition::resetToDefault() {
  Settings s;

  auto enableAnimeRecognition = s.getDefault(Setting::UseAnimeRecognition).toBool();
  auto enableMasato = s.getDefault(Setting::UseMasato).toBool();
  auto enableDiscord = s.getDefault(Setting::UseDiscord).toBool();
  auto updateDelay = s.getDefault(Setting::UpdateDelay).toInt();
  auto notifyDetected = s.getDefault(Setting::NotifyDetected).toBool();
  auto notifyUpdated = s.getDefault(Setting::NotifyUpdated).toBool();

  ui->enableAnimeRecognition->setChecked(enableAnimeRecognition);
  ui->enableMasato->setChecked(enableMasato);
  ui->enableDiscord->setChecked(enableDiscord);
  ui->updateDelay->setCurrentIndex(ui->updateDelay->findData(updateDelay));
  ui->notifyDetected->setChecked(notifyDetected);
  ui->notifyUpdated->setChecked(notifyUpdated);
}
}  // namespace Views
