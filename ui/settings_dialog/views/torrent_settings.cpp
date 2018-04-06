#include "./torrent_settings.h"
#include "ui_torrent_settings.h"

#include <QDebug>
#include <QHash>

#include "../../../src/models/media.h"
#include "../../../src/models/media_list.h"
#include "../../../src/settings.h"
#include "./rule_import_dialog.h"

namespace Views {
TorrentSettings::TorrentSettings(QWidget *parent)
    : CommittableWidget(parent), ui(new Ui::TorrentSettings), model(new TorrentRuleModel) {
  ui->setupUi(this);

  QList<Media *> allMedia = MediaList::instance().allMedia().values();

  std::sort(allMedia.begin(), allMedia.end(),
            [](Media *l, Media *r) { return l->title() < r->title(); });

  ui->anime->addItem("", 0);

  for (Media *media : allMedia) {
    ui->anime->addItem(media->title(), media->id());
  }

  Settings s;

  auto defaultSubgroup = s.get(Setting::DefaultSubgroup).toString();
  auto defaultQuality = s.get(Setting::DefaultQuality).toString();

  ui->defaultSubgroup->setText(defaultSubgroup);
  ui->defaultQuality->setCurrentText(defaultQuality);

  connect(ui->defaultSubgroup, &QLineEdit::textChanged, this,
          [this](const QString &text) { this->changed_settings[Setting::DefaultSubgroup] = text; });

  connect(ui->defaultQuality, &QComboBox::currentTextChanged, this,
          [this](const QString &text) { this->changed_settings[Setting::DefaultQuality] = text; });

  ui->torrentRules->header()->hide();
  ui->torrentRules->setModel(model);
  auto selectionModel = ui->torrentRules->selectionModel();

  connect(selectionModel, &QItemSelectionModel::selectionChanged, this, [this](QItemSelection cur) {
    if (cur.indexes().empty()) {
      this->currentRule = nullptr;
    } else {
      this->currentRule = model->rule(cur.indexes().first());
    }

    if (!currentRule) {
      ui->deleteCurrentRule->setEnabled(false);
      ui->selectedRule->setEnabled(false);
      ui->anime->setCurrentIndex(0);
      ui->currentSubgroup->setText("");
      ui->currentQuality->setCurrentText("");
      ui->currentTag->setText("");
      return;
    }

    ui->deleteCurrentRule->setEnabled(true);

    if (currentRule->isTag) {
      ui->selectedRule->setEnabled(false);
      ui->anime->setCurrentIndex(0);
      ui->currentSubgroup->setText("");
      ui->currentQuality->setCurrentText("");
      ui->currentTag->setText(currentRule->tag);
    } else {
      ui->selectedRule->setEnabled(true);

      if (currentRule->id == 0) {
        ui->anime->setCurrentIndex(0);
      } else {
        auto index = ui->anime->findData(currentRule->id);

        if (index != 0) {
          ui->anime->setCurrentIndex(index);
        }
      }

      ui->currentSubgroup->setText(currentRule->subGroup);
      ui->currentQuality->setCurrentText(currentRule->quality);

      if (currentRule->parentItem()) {
        ui->currentTag->setText(currentRule->parentItem()->tag);
      }
    }
  });

  connect(ui->importRules, &QPushButton::clicked, this, [this]() {
    RuleImportDialog *dialog = new RuleImportDialog;

    connect(dialog, &RuleImportDialog::finished, this, [this, dialog](int result) {
      if (result == QDialog::Accepted) {
        if (dialog->useSeason) {
          // TODO
        } else {
          QSet<int> list = MediaList::instance().getMediaList(dialog->list);

          for (auto id : list) {
            TorrentRule *rule = new TorrentRule;
            rule->id = id;
            rule->subGroup = ui->defaultSubgroup->text();
            rule->quality = ui->defaultQuality->currentText();

            this->model->addRule(dialog->list, rule);
          }
        }
      }

      dialog->deleteLater();
    });

    dialog->show();
  });

  connect(ui->addNewRule, &QPushButton::clicked, this, [this, selectionModel]() {
    TorrentRule *rule = new TorrentRule;
    rule->id = 0;
    rule->subGroup = ui->defaultSubgroup->text();
    rule->quality = ui->defaultQuality->currentText();

    this->model->addRule("", rule);

    if (selectionModel->hasSelection()) {
      auto selection = selectionModel->selection().indexes().first();
      selectionModel->select(selection, QItemSelectionModel::Deselect);
    }

    auto index = this->model->index(rule->parentItem()->row() + rule->row(), 0);
    selectionModel->select(index, QItemSelectionModel::Select);
  });

  connect(ui->deleteCurrentRule, &QPushButton::clicked, this, [this, selectionModel]() {
    auto rule = this->currentRule;
    this->model->beginEdit();

    if (rule->parentItem()) {
      rule->parentItem()->removeChild(rule);
      this->currentRule = nullptr;
    }

    if (selectionModel->hasSelection()) {
      auto selection = selectionModel->selection().indexes().first();
      selectionModel->select(selection, QItemSelectionModel::Deselect);
    }

    this->model->endEdit();
  });

  connect(ui->anime, &QComboBox::currentTextChanged, this, [this](const QString &text) {
    auto rule = this->currentRule;

    if (rule) {
      this->model->beginEdit();

      auto index = ui->anime->findText(ui->anime->currentText());

      if (index != -1) {
        rule->id = ui->anime->itemData(index).toInt();
      }

      this->model->endEdit();
    }
  });

  connect(ui->currentSubgroup, &QLineEdit::textChanged, this, [this](const QString &text) {
    if (this->currentRule) {
      this->currentRule->subGroup = text;
    }
  });

  connect(ui->currentQuality, &QComboBox::currentTextChanged, this, [this](const QString &text) {
    if (this->currentRule) {
      this->currentRule->quality = text;
    }
  });

  connect(ui->currentTag, &QLineEdit::textChanged, this, [this](const QString &text) {
    auto rule = this->currentRule;

    if (rule && !rule->isTag) {
      if (rule->parentItem()) {
        if (rule->parentItem()->tag == text) {
          return;
        }

        rule->parentItem()->removeChild(rule);
      }

      model->addRule(text, rule);
    }
  });
}

TorrentSettings::~TorrentSettings() {
  delete ui;
  delete model;
}

void TorrentSettings::resetToDefault() {
  Settings s;

  auto defaultSubgroup = s.getDefault(Setting::DefaultSubgroup).toString();
  auto defaultQuality = s.getDefault(Setting::DefaultQuality).toString();

  ui->defaultSubgroup->setText(defaultSubgroup);
  ui->defaultQuality->setCurrentText(defaultQuality);
}

void TorrentSettings::commit() {
  model->save();
}
}  // namespace Views
