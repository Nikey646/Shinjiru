#include "./main_window.h"
#include "./ui_main_window.h"

#include "../src/models/user.h"
#include "../src/utilities/file_downloader.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      viewAnimeList(new Views::AnimeList),
      viewAiring(new Views::Airing),
      viewNowPlaying(new Views::NowPlaying),
      viewTorrents(new Views::Torrents) {
  ui->setupUi(this);
  ui->mainPanel->addWidget(viewAnimeList);
  ui->mainPanel->addWidget(viewAiring);
  ui->mainPanel->addWidget(viewNowPlaying);
  ui->mainPanel->addWidget(viewTorrents);
  ui->mainPanel->setCurrentWidget(viewAnimeList);

  User &user = User::instance();

  ui->labelDisplayName->setText(user.displayName());

  if (user.avatar().size() != 0) {
    downloadAvatar(user.avatar());
  }

  connect(&user, &User::displayNameChanged, [this, &user]() {
    ui->labelDisplayName->setText(user.displayName());
  });

  connect(&user, &User::avatarChanged, [this, &user]() {
    if (user.avatar().size() != 0) {
      downloadAvatar(user.avatar());
    }
  });

  connect(ui->buttonAnimeList, &QPushButton::clicked,
          [this]() { ui->mainPanel->setCurrentWidget(viewAnimeList); });

  connect(ui->buttonAiring, &QPushButton::clicked,
          [this]() { ui->mainPanel->setCurrentWidget(viewAiring); });

  connect(ui->buttonNowPlaying, &QPushButton::clicked,
          [this]() { ui->mainPanel->setCurrentWidget(viewNowPlaying); });

  connect(ui->buttonTorrents, &QPushButton::clicked,
          [this]() { ui->mainPanel->setCurrentWidget(viewTorrents); });
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::downloadAvatar(const QString &url) {
  FileDownloader *f = new FileDownloader(url);

  connect(f, &FileDownloader::downloaded, [this, f]() {
    QPixmap image;
    image.loadFromData(f->downloadedData());
    ui->labelAvatar->setPixmap(image);
    f->deleteLater();
  });
}
