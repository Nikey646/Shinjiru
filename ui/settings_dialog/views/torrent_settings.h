#ifndef UI_SETTINGS_DIALOG_VIEWS_TORRENTS_H__
#define UI_SETTINGS_DIALOG_VIEWS_TORRENTS_H__

#include <QWidget>

namespace Ui {
class TorrentSettings;
}

namespace Views {

class TorrentSettings : public QWidget {
  Q_OBJECT

 public:
  explicit TorrentSettings(QWidget *parent = 0);
  ~TorrentSettings();

 private:
  Ui::TorrentSettings *ui;
};
}  // namespace Views

#endif  // UI_SETTINGS_DIALOG_VIEWS_TORRENTS_H__
