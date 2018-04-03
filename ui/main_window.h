#ifndef UI_MAIN_WINDOW_H__
#define UI_MAIN_WINDOW_H__

#include <QMainWindow>

#include "./main_window/views/airing.h"
#include "./main_window/views/anime_list.h"
#include "./main_window/views/now_playing.h"
#include "./main_window/views/torrents.h"
#include "./tray_icon.h"

#include <QSimpleUpdater.h>

#include <functional>

#include "../src/settings.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  void changeEvent(QEvent *evt) override;
  void closeEvent(QCloseEvent *evt) override;

 private:
  void downloadAvatar(const QString &url);

 private:
  Ui::MainWindow *ui;
  Views::AnimeList *viewAnimeList;
  Views::Airing *viewAiring;
  Views::NowPlaying *viewNowPlaying;
  Views::Torrents *viewTorrents;
  QSimpleUpdater *updater;
  TrayIcon *tray;
  Settings settings;
};

#endif  // UI_MAIN_WINDOW_H__
