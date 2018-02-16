#include "anime_list.h"
#include <QDebug>
#include "ui_animelist.h"

namespace Views {

AnimeList::AnimeList(QWidget *parent) : QWidget(parent), ui(new Ui::AnimeList) {
  ui->setupUi(this);

  mediaList = &MediaList::instance();

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
        auto title = tr(qPrintable(list));

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
