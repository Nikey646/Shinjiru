#include "anime_table.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLayout>
#include <QTableView>

#include "../components/progress_item_delegate.h"
#include "../components/score_item_delegate.h"
#include "../components/status_item_delegate.h"

namespace Views {

AnimeTable::AnimeTable(QWidget *parent, QSet<int> list) : QWidget(parent) {
  QLayout *layout = new QHBoxLayout(this);
  layout->setMargin(0);

  table = new QTableView(this);
  layout->addWidget(table);

  model = new MediaTableModel(this);
  model->setList(list);

  proxy_model = new MediaTableProxyModel(this);
  proxy_model->sort(ListRoles::Title);
  proxy_model->setDynamicSortFilter(true);
  proxy_model->setFilterCaseSensitivity(Qt::CaseInsensitive);
  proxy_model->setSortCaseSensitivity(Qt::CaseInsensitive);
  proxy_model->setSourceModel(model);

  table->setModel(proxy_model);
  table->horizontalHeader()->setStretchLastSection(true);
  table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
  table->verticalHeader()->setDefaultSectionSize(table->fontMetrics().height() +
                                                 8);
  table->setSelectionBehavior(QAbstractItemView::SelectRows);
  table->setSelectionMode(QAbstractItemView::SingleSelection);
  table->setItemDelegateForColumn(ListRoles::Progress,
                                  new ProgressItemDelegate(this));
  table->setItemDelegateForColumn(ListRoles::Score,
                                  new ScoreItemDelegate(this));
  table->setItemDelegateForColumn(ListRoles::Status,
                                  new StatusItemDelegate(this));

  for (int i = 0; i < model->columnCount(); ++i) {
    table->setColumnHidden(i, model->defaultHidden(i));
  }

  this->setLayout(layout);
}

void AnimeTable::setList(const QSet<int> &list) { model->setList(list); }

void AnimeTable::refresh() {
  model->refresh();

  for (int i = 0; i < model->columnCount(); ++i) {
    QApplication::processEvents();
    table->resizeColumnToContents(i);
  }
}

void AnimeTable::setFilter(const QString &text) {
  proxy_model->setFilter(text);
}

}  // namespace Views
