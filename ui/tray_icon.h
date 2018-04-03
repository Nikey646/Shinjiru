#ifndef TRAY_ICON_H
#define TRAY_ICON_H

#include <QSystemTrayIcon>

#include <functional>

class TrayIcon : public QSystemTrayIcon {
  Q_OBJECT
 public:
  TrayIcon(QObject *parent = nullptr);

  void setShowFunction(std::function<void()> fn);

 private:
  std::function<void()> show_fn;
};

#endif  // TRAY_ICON_H
