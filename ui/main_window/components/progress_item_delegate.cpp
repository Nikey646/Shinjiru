#include "./progress_item_delegate.h"

#include <QSpinBox>

#include <limits>

#include "../../../src/models/media.h"
#include "./media_table_model.h"

ProgressItemDelegate::ProgressItemDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

QWidget *ProgressItemDelegate::createEditor(QWidget *t_parent, const QStyleOptionViewItem &t_option,
                                            const QModelIndex &t_index) const {
  Q_UNUSED(t_option);
  Q_UNUSED(t_index)

  QSpinBox *editor = new QSpinBox(t_parent);
  editor->setFrame(false);
  editor->setMinimum(0);
  editor->setMaximum(100);

  return editor;
}

void ProgressItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
  auto model = index.model();

  auto &mediaList = MediaList::instance();

  int media_id = model->data(model->index(index.row(), ListRoles::ID), Qt::DisplayRole).toInt();
  Media *media = mediaList.getMediaById(media_id);

  auto progress = media->progress();
  auto episodes = media->episodes();

  if (episodes == 0) {
    episodes = std::numeric_limits<decltype(episodes)>::max();
  }

  QSpinBox *spinBox = static_cast<QSpinBox *>(editor);
  spinBox->setMaximum(episodes);
  spinBox->setValue(progress);
}

void ProgressItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                        const QModelIndex &index) const {
  QSpinBox *spinBox = static_cast<QSpinBox *>(editor);
  spinBox->interpretText();
  int value = spinBox->value();

  model->setData(index, value, Qt::EditRole);
}

void ProgressItemDelegate::updateEditorGeometry(QWidget *t_editor,
                                                const QStyleOptionViewItem &t_opt,
                                                const QModelIndex &t_index) const {
  Q_UNUSED(t_index);
  t_editor->setGeometry(t_opt.rect);
}
