#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H

#include <QDialog>

#include "settings_dialog/components/committable_widget.h"
#include "settings_dialog/views/application.h"
#include "settings_dialog/views/recognition.h"
#include "settings_dialog/views/torrent_settings.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog {
  Q_OBJECT

 public:
  explicit SettingsDialog(QWidget *parent = 0);
  ~SettingsDialog();

 private:
  Ui::SettingsDialog *ui;
  Views::Application *viewApplication;
  Views::Recognition *viewRecognition;
  Views::TorrentSettings *viewTorrents;
  QList<CommittableWidget *> widgets;
};

#endif  // SETTINGS_DIALOG_H
