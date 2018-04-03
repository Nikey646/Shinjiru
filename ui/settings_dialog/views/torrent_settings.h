#ifndef UI_SETTINGS_DIALOG_VIEWS_TORRENTS_H__
#define UI_SETTINGS_DIALOG_VIEWS_TORRENTS_H__

#include "../components/committable_widget.h"

namespace Ui {
class TorrentSettings;
}

namespace Views {

class TorrentSettings : public CommittableWidget {
  Q_OBJECT

 public:
  explicit TorrentSettings(QWidget *parent = 0);
  ~TorrentSettings();

  void resetToDefault() override;

 private:
  Ui::TorrentSettings *ui;
};
}  // namespace Views

#endif  // UI_SETTINGS_DIALOG_VIEWS_TORRENTS_H__
