#include "rule_import_dialog.h"
#include "ui_rule_import_dialog.h"

#include <QDate>

#include "../../../src/models/media_list.h"

RuleImportDialog::RuleImportDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::RuleImportDialog) {
  ui->setupUi(this);

  auto &mediaList = MediaList::instance();
  auto mediaLists = mediaList.getMediaLists();
  QStringList lists;

  for (auto &&list : mediaLists) {
    auto title = list;

    if (list == "PLANNING") {
      title = tr("PLANNING");
    } else if (list == "COMPLETED") {
      title = tr("COMPLETED");
    } else if (list == "CURRENT") {
      title = tr("CURRENT");
    } else if (list == "DROPPED") {
      title = tr("DROPPED");
    } else if (list == "PAUSED") {
      title = tr("PAUSED");
    } else if (list == "REPEATING") {
      title = tr("REPEATING");
    }

    lists.append(title);
  }

  std::sort(lists.begin(), lists.end());

  ui->lists->addItems(lists);

  auto currentDate = QDate::currentDate();
  currentDate.addYears(-1);

  for (int i = 0; i < 3; ++i) {
    currentDate.addYears(1);
    auto year = QString::number(currentDate.year());
    ui->seasons->addItems({"Spring " + year, "Summer " + year, "Fall " + year, "Winter " + year});
  }

  connect(ui->groupBox, &QGroupBox::clicked, this, [this](bool checked) {
    ui->groupBox->setChecked(checked);
    ui->groupBox_2->setChecked(!checked);
  });

  connect(ui->groupBox_2, &QGroupBox::clicked, this, [this](bool checked) {
    ui->groupBox->setChecked(!checked);
    ui->groupBox_2->setChecked(checked);
  });

  connect(ui->lists, &QComboBox::currentTextChanged, this, [this](const QString &text) {
    auto title = text;

    if (list == tr("PLANNING")) {
      title = "PLANNING";
    } else if (list == tr("COMPLETED")) {
      title = "COMPLETED";
    } else if (list == tr("CURRENT")) {
      title = "CURRENT";
    } else if (list == tr("DROPPED")) {
      title = "DROPPED";
    } else if (list == tr("PAUSED")) {
      title = "PAUSED";
    } else if (list == tr("REPEATING")) {
      title = "REPEATING";
    }

    this->list = title;
  });

  connect(ui->seasons, &QComboBox::currentTextChanged, this,
          [this](const QString &text) { this->season = text; });

  connect(ui->cancelButton, &QPushButton::clicked, this, &RuleImportDialog::reject);

  connect(ui->importButton, &QPushButton::clicked, this, &RuleImportDialog::accept);
}

RuleImportDialog::~RuleImportDialog() {
  delete ui;
}
