#include "now_playing.h"
#include "ui_now_playing.h"

#include "../../../src/clients/discord.h"
#include "../../../src/detection/media_store.h"
#include "../../../src/models/media_list.h"
#include "../../../src/utilities/file_downloader.h"
#include "../../process_details.h"

#include <chrono>

#include <QPainter>

namespace Views {

NowPlaying::NowPlaying(QWidget *parent)
    : QWidget(parent), ui(new Ui::NowPlaying), timer(new QTimer), model(new NowPlayingListModel) {
  ui->setupUi(this);
  ui->tabWidget->tabBar()->setVisible(false);

  timer->setInterval(1000);

  MediaStore &store = MediaStore::instance();

  QPixmap defaultCover(":/res/no_cover.jpg");
  defaultCover = defaultCover.scaledToWidth(ui->defaultImage->width(), Qt::SmoothTransformation);
  ui->defaultImage->setPixmap(defaultCover);
  ui->openProcesses->setModel(model);

  ui->tabWidget->setCurrentWidget(ui->tabNotPlaying);

  connect(&store, &MediaStore::mediaPlayingChanged, this, [&store, this]() {
    int currentEpisode = store.episodePlaying();

    this->media = store.mediaPlaying();
    this->episode = QString::number(currentEpisode);

    updateMedia();

    if (media != nullptr) {
      Discord::instance().updatePresence(media->title(), currentEpisode);
    } else {
      Discord::instance().clearPresence();
    }

    if (media != nullptr && currentEpisode > media->progress()) {
      timer->start();
      ui->cancelButton->setEnabled(true);
      timerTime = s.get(Setting::UpdateDelay).toInt();
      ui->updating->setText(tr("Updating in %1 seconds").arg(QString::number(timerTime)));
    }
  });

  connect(ui->openProcesses, &QListView::doubleClicked, this, [this](const QModelIndex &index) {
    Robot::Process process = model->process(index);

    ProcessDetails *details = new ProcessDetails(process);

    connect(details, &ProcessDetails::finished, this, [details]() { details->deleteLater(); });

    details->show();
  });

  connect(ui->cancelButton, &QPushButton::clicked, this, [this]() {
    MediaList::instance().cancelUpdate();
    timer->stop();
    ui->cancelButton->setEnabled(false);
    ui->updating->setText("");
  });

  connect(ui->falsePositiveButton, &QPushButton::clicked, this, [this]() {
    ui->cancelButton->click();
    MediaStore::instance().blackListCurrent();
  });

  connect(timer, &QTimer::timeout, this, [this]() {
    --timerTime;

    if (timerTime == 0) {
      timer->stop();
      ui->cancelButton->setEnabled(false);
      ui->updating->setText("");
    } else {
      auto timeRemaining = QString::number(timerTime);
      ui->updating->setText(tr("Updating in %1 seconds").arg(timeRemaining));
    }
  });

  connect(&store, &MediaStore::processesChanged, this, [&store, this]() {
    auto processes = store.processes();

    model->setProcesses(processes);
  });
}

NowPlaying::~NowPlaying() {
  delete ui;
  delete timer;
  delete model;
}

void NowPlaying::updateMedia() {
  if (media == nullptr) {
    ui->title->setText("");
    ui->nowPlaying->setText("");
    ui->description->setText("");
    ui->synonyms->setText("");
    ui->episodes->setText("");
    ui->status->setText("");

    ui->coverImage->setPixmap(QPixmap());

    ui->tabWidget->setCurrentWidget(ui->tabNotPlaying);
  } else {
    ui->title->setText(media->title());
    ui->nowPlaying->setText(tr("Now playing: Episode %1").arg(episode));
    ui->description->setText(media->description());
    ui->synonyms->setText(media->synonyms().join("\n"));
    ui->episodes->setText(QString::number(media->episodes()));
    ui->status->setText(tr(qPrintable(media->airingStatus())));

    FileDownloader *f = new FileDownloader(media->coverImage());

    connect(f, &FileDownloader::downloaded, this, [this, f]() {
      QPixmap image;
      image.loadFromData(f->downloadedData());
      image = image.scaledToWidth(ui->coverImage->width(), Qt::SmoothTransformation);
      ui->coverImage->setPixmap(image);
      f->deleteLater();
    });

    ui->tabWidget->setCurrentWidget(ui->tabPlaying);
  }
}
}  // namespace Views
