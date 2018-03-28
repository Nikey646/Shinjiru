#include "./score_item_delegate.h"

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QSpinBox>

#include <limits>

#include "../../../src/models/media.h"
#include "../../../src/models/user.h"
#include "./media_table_model.h"

ScoreItemDelegate::ScoreItemDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

QWidget *ScoreItemDelegate::createEditor(QWidget *t_parent, const QStyleOptionViewItem &t_option,
                                         const QModelIndex &t_index) const {
  Q_UNUSED(t_option);
  Q_UNUSED(t_index)

  auto &user = User::instance();
  auto scoreFormat = user.scoreFormat();

  if (scoreFormat == "POINT_10" || scoreFormat == "POINT_100") {
    QSpinBox *score_container = new QSpinBox(t_parent);

    score_container->setMaximum(scoreFormat == "POINT_10" ? 10 : 100);
    score_container->setMinimum(0);

    return score_container;
  } else if (scoreFormat == "POINT_5") {
    QComboBox *score_container = new QComboBox(t_parent);

    score_container->addItem("0 ★");
    score_container->addItem("1 ★");
    score_container->addItem("2 ★");
    score_container->addItem("3 ★");
    score_container->addItem("4 ★");
    score_container->addItem("5 ★");

    return score_container;
  } else if (scoreFormat == "POINT_3") {
    QComboBox *score_container = new QComboBox(t_parent);

    score_container->addItem("");
    score_container->addItem(":(");
    score_container->addItem(":|");
    score_container->addItem(":)");

    return score_container;
  } else if (scoreFormat == "POINT_10_DECIMAL") {
    QDoubleSpinBox *score_container = new QDoubleSpinBox(t_parent);

    score_container->setMaximum(10.0);
    score_container->setMinimum(0);
    score_container->setDecimals(1);
    score_container->setSingleStep(0.1);

    return score_container;
  }

  return new QWidget(t_parent);
}

void ScoreItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
  auto model = index.model();

  auto &user = User::instance();
  auto &mediaList = MediaList::instance();

  int media_id = model->data(model->index(index.row(), ListRoles::ID), Qt::DisplayRole).toInt();
  Media *media = mediaList.getMediaById(media_id);

  auto score = media->score();
  auto scoreFormat = user.scoreFormat();

  if (scoreFormat == "POINT_10" || scoreFormat == "POINT_100") {
    QSpinBox *score_container = static_cast<QSpinBox *>(editor);
    score_container->setValue((int)score);
  } else if (scoreFormat == "POINT_5") {
    QComboBox *score_container = static_cast<QComboBox *>(editor);

    score_container->setCurrentText(QString::number((int)score) + " ★");
  } else if (scoreFormat == "POINT_3") {
    QComboBox *score_container = static_cast<QComboBox *>(editor);

    switch ((int)score) {
      case 1:
        score_container->setCurrentText(":(");
        break;
      case 2:
        score_container->setCurrentText(":|");
        break;
      case 3:
        score_container->setCurrentText(":)");
        break;
      default:
        break;
    }
  } else if (scoreFormat == "POINT_10_DECIMAL") {
    QDoubleSpinBox *score_container = static_cast<QDoubleSpinBox *>(editor);

    score_container->setValue(score);
  }
}

void ScoreItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                     const QModelIndex &index) const {
  auto &user = User::instance();
  auto scoreFormat = user.scoreFormat();

  if (scoreFormat == "POINT_10" || scoreFormat == "POINT_100") {
    QSpinBox *score_container = static_cast<QSpinBox *>(editor);

    auto value = score_container->value();
    model->setData(index, value, Qt::EditRole);
  } else if (scoreFormat == "POINT_5") {
    QComboBox *score_container = static_cast<QComboBox *>(editor);

    auto score_str = score_container->currentText();
    auto value = score_str.leftRef(score_str.length() - 2).toInt();

    model->setData(index, value, Qt::EditRole);
  } else if (scoreFormat == "POINT_3") {
    QComboBox *score_container = static_cast<QComboBox *>(editor);

    auto score_str = score_container->currentText();
    auto value = score_str == ":)" ? 3 : score_str == ":|" ? 2 : score_str == ":(" ? 1 : 0;
    model->setData(index, value, Qt::EditRole);
  } else if (scoreFormat == "POINT_10_DECIMAL") {
    QDoubleSpinBox *score_container = static_cast<QDoubleSpinBox *>(editor);

    auto value = score_container->value();
    model->setData(index, value, Qt::EditRole);
  }
}

void ScoreItemDelegate::updateEditorGeometry(QWidget *t_editor, const QStyleOptionViewItem &t_opt,
                                             const QModelIndex &t_index) const {
  Q_UNUSED(t_index);
  t_editor->setGeometry(t_opt.rect);
}
