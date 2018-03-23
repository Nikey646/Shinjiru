#include "anime_list.h"
#include "ui_animelist.h"

#include <chrono>

#include "../../../src/clients/anilist.h"

namespace Views {

AnimeList::AnimeList(QWidget *parent)
    : QWidget(parent), ui(new Ui::AnimeList), refreshTimer(new QTimer) {
  ui->setupUi(this);

  mediaList = &MediaList::instance();

  connect(refreshTimer, &QTimer::timeout,
          []() { AniList::instance().requestReload(); });

  using namespace std::chrono_literals;
  refreshTimer->setInterval(15min);
  refreshTimer->start();

  connect(ui->lineEdit, &QLineEdit::textChanged, [this](const QString &text) {
    for (auto &&list : listTabs.values()) {
      list->setFilter(text);
    }
  });

  connect(mediaList, &MediaList::mediaListsChanged, [this]() {
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

        table = new AnimeTable(this, mediaSet);
        this->listTabs.insert(list, table);
        ui->tabWidget->addTab(table,
                              tr("%1 (%2)").arg(title).arg(mediaSet.size()));
      }
    }
  });

  connect(mediaList, &MediaList::mediaListChanged, [this](const QString &list) {
    auto mediaSet = mediaList->getMediaList(list);

    this->listTabs[list]->setList(mediaSet);
  });

  connect(mediaList, &MediaList::loadFinished, [this]() {
    for (auto &&title : listTabs.keys()) {
      auto list = listTabs.value(title);
      auto trtitle = tr(qPrintable(title));
      auto index = ui->tabWidget->indexOf(list);
      auto mediaSet = mediaList->getMediaList(title);

      ui->tabWidget->setTabText(
          index, tr("%1 (%2)").arg(trtitle).arg(mediaSet.size()));
    }

    for (auto &&list : listTabs.values()) {
      list->refresh();
    }
  });
}

AnimeList::~AnimeList() { delete ui; }

}  // namespace Views
