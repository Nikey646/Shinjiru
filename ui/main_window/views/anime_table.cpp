#include "anime_table.h"

#include <QApplication>
#include <QDebug>
#include <QHeaderView>
#include <QJsonArray>
#include <QJsonObject>

#include "../../../src/models/user.h"
#include "../../anime_panel.h"
#include "../components/progress_item_delegate.h"
#include "../components/score_item_delegate.h"
#include "../components/status_item_delegate.h"

namespace Views {

AnimeTable::AnimeTable(QWidget *parent, QSet<int> list) : QTableView(parent) {
  model = new MediaTableModel(this);
  model->setList(list);

  proxy_model = new MediaTableProxyModel(this);
  proxy_model->sort(ListRoles::Title);
  proxy_model->setDynamicSortFilter(true);
  proxy_model->setFilterCaseSensitivity(Qt::CaseInsensitive);
  proxy_model->setSortCaseSensitivity(Qt::CaseInsensitive);
  proxy_model->setSourceModel(model);

  setModel(proxy_model);

  setSortingEnabled(true);
  sortByColumn(ListRoles::Title, Qt::AscendingOrder);
  setAlternatingRowColors(true);

  horizontalHeader()->setStretchLastSection(true);

  verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
  verticalHeader()->setDefaultSectionSize(this->fontMetrics().height() + 8);

  setSelectionBehavior(QAbstractItemView::SelectRows);
  setSelectionMode(QAbstractItemView::SingleSelection);

  setItemDelegateForColumn(ListRoles::Progress, new ProgressItemDelegate(this));
  setItemDelegateForColumn(ListRoles::Score, new ScoreItemDelegate(this));
  setItemDelegateForColumn(ListRoles::Status, new StatusItemDelegate(this));

  for (int i = 0; i < model->columnCount(); ++i) {
    this->setColumnHidden(i, model->defaultHidden(i));
  }

  connect(this, &AnimeTable::doubleClicked, this, [this](auto idx) {
    auto row = idx.row();
    auto index = proxy_model->index(row, ListRoles::ID);

    if (idx.column() == ListRoles::Progress) return;
    if (idx.column() == ListRoles::Score) return;
    if (idx.column() == ListRoles::Status) return;

    auto &mediaList = MediaList::instance();
    int media_id = proxy_model->data(index, Qt::DisplayRole).toInt();
    selectedMedia = mediaList.getMediaById(media_id);

    this->openAnimePanel();
  });
}

void AnimeTable::setList(const QSet<int> &list) {
  model->setList(list);
}

void AnimeTable::refresh() {
  model->refresh();

  for (int i = 0; i < model->columnCount(); ++i) {
    QApplication::processEvents();
    this->resizeColumnToContents(i);
  }
}

void AnimeTable::setFilter(const QString &text) {
  proxy_model->setFilter(text);
}

void AnimeTable::contextMenuEvent(QContextMenuEvent *event) {
  auto indexes = this->selectedIndexes();
  auto row = indexes[0].row();
  auto index = proxy_model->index(row, ListRoles::ID);

  auto &mediaList = MediaList::instance();
  int media_id = proxy_model->data(index, Qt::DisplayRole).toInt();
  selectedMedia = mediaList.getMediaById(media_id);

  auto contextMenu = new QMenu;

  auto animePanel = new QAction(tr("Open Anime Panel"), contextMenu);
  auto increment = new QAction(tr("Increment Progress by 1"), contextMenu);
  auto statusUpdate = new QMenu(tr("Status"), contextMenu);
  auto customLists = new QMenu(tr("Custom Lists"), contextMenu);
  auto deleteEntry = new QAction(tr("Delete Entry"), contextMenu);

  contextMenu->addAction(animePanel);
  contextMenu->addAction(increment);
  contextMenu->addMenu(statusUpdate);
  contextMenu->addMenu(customLists);
  contextMenu->addAction(deleteEntry);

  connect(animePanel, SIGNAL(triggered(bool)), SLOT(openAnimePanel()));
  connect(increment, SIGNAL(triggered(bool)), SLOT(incrementProgress()));
  connect(deleteEntry, SIGNAL(triggered(bool)), SLOT(deleteEntry()));

  auto current = new QAction(tr("CURRENT"), statusUpdate);
  auto paused = new QAction(tr("PAUSED"), statusUpdate);
  auto planning = new QAction(tr("PLANNING"), statusUpdate);
  auto completed = new QAction(tr("COMPLETED"), statusUpdate);
  auto dropped = new QAction(tr("DROPPED"), statusUpdate);

  current->setCheckable(true);
  paused->setCheckable(true);
  planning->setCheckable(true);
  completed->setCheckable(true);
  dropped->setCheckable(true);

  if (selectedMedia->listStatus() == "CURRENT") {
    current->setChecked(true);
  } else if (selectedMedia->listStatus() == "PAUSED") {
    paused->setChecked(true);
  } else if (selectedMedia->listStatus() == "PLANNING") {
    planning->setChecked(true);
  } else if (selectedMedia->listStatus() == "COMPLETED") {
    completed->setChecked(true);
  } else if (selectedMedia->listStatus() == "DROPPED") {
    dropped->setChecked(true);
  }

  statusUpdate->addAction(current);
  statusUpdate->addAction(paused);
  statusUpdate->addAction(planning);
  statusUpdate->addAction(completed);
  statusUpdate->addAction(dropped);

  connect(current, SIGNAL(triggered(bool)), SLOT(setStatusCurrent()));
  connect(paused, SIGNAL(triggered(bool)), SLOT(setStatusPaused()));
  connect(planning, SIGNAL(triggered(bool)), SLOT(setStatusPlanning()));
  connect(completed, SIGNAL(triggered(bool)), SLOT(setStatusCompleted()));
  connect(dropped, SIGNAL(triggered(bool)), SLOT(setStatusDropped()));

  auto hideDefault = new QAction(tr("Hide Default"), customLists);

  hideDefault->setCheckable(true);
  hideDefault->setChecked(selectedMedia->hiddenFromStatusLists());

  auto customListNames = User::instance().customListNames();
  auto mediaCustomLists = selectedMedia->customLists();

  for (int i = 0; i < customListNames.size(); ++i) {
    const auto list = customListNames[i];

    auto *action = new QAction(list, customLists);
    action->setCheckable(true);
    action->setChecked(mediaCustomLists[list]);

    customLists->addAction(action);

    connect(action, &QAction::triggered, this,
            [i, this](bool checked) { setCustomList(i, checked); });
  }

  customLists->addSeparator();
  customLists->addAction(hideDefault);

  connect(hideDefault, SIGNAL(triggered(bool)), SLOT(toggleHiddenDefault(bool)));

  contextMenu->exec(event->globalPos());
  contextMenu->deleteLater();
}

void AnimeTable::openAnimePanel() {
  auto table = new AnimePanel(selectedMedia);

  connect(table, &AnimePanel::finished, [table]() { table->deleteLater(); });

  table->show();
}

void AnimeTable::incrementProgress() {
  auto episodes = selectedMedia->episodes();
  auto progress = selectedMedia->progress();

  if (episodes == 0 || progress < episodes) {
    QJsonObject data;
    data["progress"] = progress + 1;
    data["status"] = "CURRENT";

    if (progress + 1 == episodes) {
      data["status"] = "COMPLETED";
    }

    MediaList::instance().updateMedia(selectedMedia, data);
  }
}

void AnimeTable::deleteEntry() {
  MediaList::instance().removeMedia(selectedMedia);
}

void AnimeTable::setStatusCurrent() {
  QJsonObject data;
  data["status"] = "CURRENT";

  auto &mediaList = MediaList::instance();

  mediaList.updateMedia(selectedMedia, data);
}

void AnimeTable::setStatusPaused() {
  QJsonObject data;
  data["status"] = "PAUSED";

  auto &mediaList = MediaList::instance();
  mediaList.updateMedia(selectedMedia, data);
}

void AnimeTable::setStatusPlanning() {
  QJsonObject data;
  data["status"] = "PLANNING";

  auto &mediaList = MediaList::instance();
  mediaList.updateMedia(selectedMedia, data);
}

void AnimeTable::setStatusCompleted() {
  QJsonObject data;
  data["status"] = "COMPLETED";

  auto &mediaList = MediaList::instance();
  mediaList.updateMedia(selectedMedia, data);
}

void AnimeTable::setStatusDropped() {
  QJsonObject data;
  data["status"] = "DROPPED";

  auto &mediaList = MediaList::instance();
  mediaList.updateMedia(selectedMedia, data);
}

void AnimeTable::toggleHiddenDefault(const bool checked) {
  QJsonObject data;
  data["hiddenFromStatusLists"] = checked;

  auto customLists = selectedMedia->customLists();

  auto empty = std::all_of(customLists.begin(), customLists.end(), [](auto v) { return !v; });

  if (empty) {
    return;
  }

  auto &mediaList = MediaList::instance();
  mediaList.updateMedia(selectedMedia, data);
}

void AnimeTable::setCustomList(const int list, const bool checked) {
  auto customListNames = User::instance().customListNames();
  auto customLists = selectedMedia->customLists();

  QStringList newCustomLists;

  for (int i = 0; i < customListNames.length(); ++i) {
    const auto name = customListNames[i];
    if (customLists[name]) {
      if (i != list || (i == list && checked)) {
        newCustomLists.append(name);
      }
    } else if (i == list) {
      newCustomLists.append(name);
    }
  }

  QJsonObject data;
  data["customLists"] = QJsonArray::fromStringList(newCustomLists);

  if (newCustomLists.empty()) {
    data["hiddenFromStatusLists"] = false;
  }

  auto &mediaList = MediaList::instance();
  mediaList.updateMedia(selectedMedia, data);
}

}  // namespace Views
