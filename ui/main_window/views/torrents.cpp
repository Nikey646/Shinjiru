#include "torrents.h"
#include "ui_torrents.h"

#include <QDesktopServices>
#include <QEventLoop>
#include <QFile>

#include "../../../src/clients/nekomimi.h"
#include "../../../src/paths.h"
#include "../../../src/utilities/file_downloader.h"

namespace Views {

Torrents::Torrents(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Torrents),
      model(new RSSTableModel(this)),
      timer(new QTimer(this)) {
  ui->setupUi(this);

  model->setList(items);
  timer->setInterval(1000);

  ui->torrentTable->setModel(model);
  ui->torrentTable->horizontalHeader()->setStretchLastSection(true);
  ui->torrentTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
  ui->torrentTable->verticalHeader()->setDefaultSectionSize(
      ui->torrentTable->fontMetrics().height() + 8);
  ui->torrentTable->setSelectionBehavior(QAbstractItemView::SelectRows);
  ui->torrentTable->setSelectionMode(QAbstractItemView::SingleSelection);

  connect(ui->refreshButton, &QPushButton::clicked, this, [this]() {
    refresh = 1;
    timerTick();
  });

  connect(timer, SIGNAL(timeout()), this, SLOT(timerTick()));

  timer->start();
}

Torrents::~Torrents() {
  delete ui;
}

void Torrents::timerTick() {
  --refresh;

  if (refresh <= 0) {
    fetchTorrents();
    refresh = 900;
  }

  ui->refreshButton->setText(tr("Refresh (%1)").arg(refresh));
}
void Torrents::fetchTorrents() {
  if (!refreshLock.tryLock()) {
    return;
  }

  ui->refreshButton->setEnabled(false);

  for (auto &&item : items) {
    delete item;
  }

  items = Nekomimi::instance().fetch();

  model->setList(items);
  ui->torrentTable->resizeColumnsToContents();

  ui->refreshButton->setEnabled(true);
  refreshLock.unlock();

  checkForMatches();
}

void Torrents::checkForMatches() {
  QFile rules(Paths::configFile("torrent_rules.json"));

  if (!rules.exists()) {
    rules.open(QFile::WriteOnly);
    rules.write("{}");
    rules.close();
  }

  rules.open(QFile::ReadOnly);
  QJsonDocument doc = QJsonDocument::fromJson(rules.readAll());
  QJsonObject obj = doc.object();

  for (auto it = obj.begin(); it != obj.end(); ++it) {
    auto key = it.key();
    auto data = it.value().toArray();

    for (auto &&rule : data) {
      auto ruleObject = rule.toObject();

      checkRule(ruleObject);
    }
  }
}

void Torrents::checkRule(QJsonObject rule) {
  auto mediaId = rule["id"].toInt();
  auto subGroup = rule["subGroup"].toString();
  auto quality = rule["quality"].toString();

  for (auto &&item : this->items) {
    if (mediaId == item->mediaId && subGroup == item->subGroup && quality == item->quality) {
      downloadOnce(item);
    }
  }
}

void Torrents::downloadOnce(RSSItem *item) {
  QString file = item->fileName + ".dl";
  QDir history_dir(Paths::configDir("torrent_history"));
  QFile f(history_dir.absoluteFilePath(file));

  if (!f.exists()) {
    qDebug() << "Downloading" << item->fileName << "from torrent rule";

    download(item);

    f.open(QFile::WriteOnly);
    f.write("0");
    f.close();
  }
}

void Torrents::download(RSSItem *item) {
  FileDownloader f(item->link);

  QEventLoop evt;
  connect(&f, &FileDownloader::downloaded, &evt, &QEventLoop::quit);
  evt.exec();

  QFile temp(QDir::tempPath() + "/" + item->fileName + ".torrent");
  temp.open(QFile::WriteOnly);
  temp.write(f.downloadedData());
  temp.close();

  QDesktopServices::openUrl(QUrl::fromLocalFile(temp.fileName()));
}
}  // namespace Views
