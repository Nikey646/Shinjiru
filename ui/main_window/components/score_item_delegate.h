#ifndef UI_MAIN_WINDOW_COMPONENTS_MEDIA_SCORE_ITEM_DELEGATE_H__
#define UI_MAIN_WINDOW_COMPONENTS_MEDIA_SCORE_ITEM_DELEGATE_H__

#include <QStyledItemDelegate>

class ScoreItemDelegate : public QStyledItemDelegate {
  Q_OBJECT

 public:
  explicit ScoreItemDelegate(QObject *parent = 0);
  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                        const QModelIndex &index) const;
  void setEditorData(QWidget *editor, const QModelIndex &index) const;
  void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
  void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
};

#endif  // UI_MAIN_WINDOW_COMPONENTS_MEDIA_SCORE_ITEM_DELEGATE_H__
