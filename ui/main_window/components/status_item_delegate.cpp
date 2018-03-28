#include "./status_item_delegate.h"

#include <QComboBox>

#include <limits>

#include "../../../src/models/media.h"
#include "../../../src/models/user.h"
#include "./media_table_model.h"

StatusItemDelegate::StatusItemDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

QWidget *StatusItemDelegate::createEditor(QWidget *t_parent, const QStyleOptionViewItem &t_option,
                                          const QModelIndex &t_index) const {
  Q_UNUSED(t_option);
  Q_UNUSED(t_index)

  QComboBox *editor = new QComboBox(t_parent);
  editor->setFrame(false);
  editor->addItems({tr("CURRENT"), tr("PLANNING"), tr("COMPLETED"), tr("DROPPED"), tr("PAUSED"),
                    tr("REPEATING")});

  return editor;
}

void StatusItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
  auto model = index.model();

  auto &mediaList = MediaList::instance();

  int media_id = model->data(model->index(index.row(), ListRoles::ID), Qt::DisplayRole).toInt();
  Media *media = mediaList.getMediaById(media_id);

  auto status = tr(qPrintable(media->listStatus()));

  QComboBox *comboBox = static_cast<QComboBox *>(editor);
  comboBox->setCurrentText(status);
}

void StatusItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                      const QModelIndex &index) const {
  QComboBox *comboBox = static_cast<QComboBox *>(editor);
  auto value = comboBox->currentText();

  if (value == tr("CURRENT")) {
    value = "CURRENT";
  } else if (value == tr("COMPLETED")) {
    value = "COMPLETED";
  } else if (value == tr("DROPPED")) {
    value = "DROPPED";
  } else if (value == tr("PAUSED")) {
    value = "PAUSED";
  } else if (value == tr("REPEATING")) {
    value = "REPEATING";
  }

  model->setData(index, value, Qt::EditRole);
}

void StatusItemDelegate::updateEditorGeometry(QWidget *t_editor, const QStyleOptionViewItem &t_opt,
                                              const QModelIndex &t_index) const {
  Q_UNUSED(t_index);
  t_editor->setGeometry(t_opt.rect);
}
