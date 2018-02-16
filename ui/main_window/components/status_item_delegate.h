#ifndef UI_MAIN_WINDOW_COMPONENTS_MEDIA_STATUS_ITEM_DELEGATE_H__
#define UI_MAIN_WINDOW_COMPONENTS_MEDIA_STATUS_ITEM_DELEGATE_H__

#include <QStyledItemDelegate>

class StatusItemDelegate : public QStyledItemDelegate {
  Q_OBJECT

 public:
  explicit StatusItemDelegate(QObject *parent = 0);
  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                        const QModelIndex &index) const;
  void setEditorData(QWidget *editor, const QModelIndex &index) const;
  void setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const;
  void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
};

#endif  // UI_MAIN_WINDOW_COMPONENTS_MEDIA_STATUS_ITEM_DELEGATE_H__
