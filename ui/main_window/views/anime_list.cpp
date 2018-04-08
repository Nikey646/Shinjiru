#include "anime_list.h"
#include "ui_animelist.h"

#include <chrono>

#include "../../../src/clients/anilist.h"

namespace Views {

AnimeList::AnimeList(QWidget *parent)
    : QWidget(parent), ui(new Ui::AnimeList), refreshTimer(new QTimer) {
  ui->setupUi(this);

  mediaList = &MediaList::instance();

  connect(refreshTimer, &QTimer::timeout, []() { AniList::instance().requestReload(); });

  using namespace std::chrono_literals;
  refreshTimer->setInterval(15min);
  refreshTimer->start();

  connect(ui->lineEdit, &QLineEdit::textChanged, this, [this](auto text) {
    std::for_each(listTabs.begin(), listTabs.end(),
                  [&text](auto table) { table->setFilter(text); });
  });

  connect(mediaList, &MediaList::mediaListsChanged, this, [this]() {
    auto lists = mediaList->getMediaLists();

    for (auto &&list : lists) {
      auto table = this->listTabs.value(list, nullptr);

      if (table == nullptr) {
        auto mediaSet = mediaList->getMediaList(list);
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

        auto tabTitle = tr("%1 (%2)").arg(title).arg(mediaSet.size());

        table = new AnimeTable(this, mediaSet);
        this->listTabs.insert(list, table);
        ui->tabWidget->addTab(table, tabTitle);
      }
    }

    this->sortTabs();
  });

  connect(mediaList, &MediaList::mediaListChanged, this, [this](auto list) {
    auto mediaSet = mediaList->getMediaList(list);

    this->listTabs[list]->setList(mediaSet);
  });

  connect(mediaList, &MediaList::loadFinished, this, [this]() {
    std::for_each(listTabs.keyBegin(), listTabs.keyEnd(), [this](auto title) {
      auto list = listTabs.value(title);
      auto trtitle = tr(qPrintable(title));
      auto index = ui->tabWidget->indexOf(list);
      auto mediaSet = mediaList->getMediaList(title);

      ui->tabWidget->setTabText(index, tr("%1 (%2)").arg(trtitle).arg(mediaSet.size()));
    });

    this->sortTabs();

    std::for_each(listTabs.begin(), listTabs.end(), [](auto table) { table->refresh(); });
  });
}

AnimeList::~AnimeList() {
  delete ui;
}

void AnimeList::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Escape) {
    QLineEdit *focus = qobject_cast<QLineEdit *>(focusWidget());

    if (focus = ui->lineEdit) {
      focus->clear();
    }
  }
}

void AnimeList::sortTabs() {
  auto tabBar = ui->tabWidget->tabBar();
  auto selectedTab = tabBar->currentIndex();

  // Yes, I know this is bubble sort
  // No, I don't care
  for (int i = 0; i < tabBar->count(); ++i) {
    for (int j = 0; j <= tabBar->count() - i; ++j) {
      auto current = tabBar->tabText(j);
      auto next = tabBar->tabText(j + 1);
      auto comparison = QString::compare(current, next, Qt::CaseInsensitive);
      auto currentValue = getTabValue(current);
      auto nextValue = getTabValue(next);
      auto valueEqual = currentValue == nextValue;
      auto valueGreater = currentValue > nextValue;

      if (valueGreater || (valueEqual && comparison > 0)) {
        tabBar->moveTab(j, j + 1);
      }
    }
  }

  tabBar->setCurrentIndex(selectedTab);
}

int AnimeList::getTabValue(const QString &text) {
  if (text.startsWith(tr("PLANNING"))) {
    return 4;
  } else if (text.startsWith(tr("COMPLETED"))) {
    return 5;
  } else if (text.startsWith(tr("CURRENT"))) {
    return 1;
  } else if (text.startsWith(tr("DROPPED"))) {
    return 6;
  } else if (text.startsWith(tr("PAUSED"))) {
    return 3;
  } else if (text.startsWith(tr("REPEATING"))) {
    return 2;
  }

  return 0;
}

}  // namespace Views
