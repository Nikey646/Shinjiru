#include "./main_window.h"
#include "./ui_main_window.h"

#include <QApplication>
#include <QDesktopServices>

#include "../src/clients/anilist.h"
#include "../src/models/user.h"
#include "../src/utilities/file_downloader.h"
#include "./settings_dialog.h"

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

  connect(ui->actionExit, &QAction::triggered, []() { qApp->exit(); });

  connect(ui->actionRefreshList, &QAction::triggered,
          []() { AniList::instance().requestReload(); });

  connect(ui->actionOpenAnimeList, &QAction::triggered, []() {
    const auto id = QString::number(AniList::instance().userId());
    const QUrl url = "https://anilist.co/user/" + id + "/animelist";
    QDesktopServices::openUrl(url);
  });

  connect(ui->actionOpenUserPage, &QAction::triggered, []() {
    const auto id = QString::number(AniList::instance().userId());
    const QUrl url = "https://anilist.co/user/" + id;
    QDesktopServices::openUrl(url);
  });

  connect(ui->actionOpenHomePage, &QAction::triggered, []() {
    const QUrl url = "https://anilist.co";
    QDesktopServices::openUrl(url);
  });

  connect(ui->actionSettings, &QAction::triggered, [this]() {
    SettingsDialog *dialog = new SettingsDialog;

    connect(dialog, &SettingsDialog::finished,
            [dialog]() { dialog->deleteLater(); });

    dialog->show();
  });

  connect(ui->actionAbout, &QAction::triggered, []() {
    // TODO
  });

  connect(ui->actionCheckForUpdates, &QAction::triggered, []() {
    // TODO
  });
}

MainWindow::~MainWindow() {
  delete ui;
  delete viewAnimeList;
  delete viewAiring;
  delete viewNowPlaying;
  delete viewTorrents;
}

void MainWindow::downloadAvatar(const QString &url) {
  FileDownloader *f = new FileDownloader(url);

  connect(f, &FileDownloader::downloaded, [this, f]() {
    QPixmap image;
    image.loadFromData(f->downloadedData());
    ui->labelAvatar->setPixmap(image);
    f->deleteLater();
  });
}
