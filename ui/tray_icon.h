#ifndef TRAY_ICON_H
#define TRAY_ICON_H

#include <QSystemTrayIcon>

#include <functional>

#include "../src/settings.h"

class TrayIcon : public QSystemTrayIcon {
  Q_OBJECT
 public:
  TrayIcon(QObject *parent = nullptr);
  ~TrayIcon();

  void setShowFunction(std::function<void()> fn);

 public slots:
  void restore();

 private:
  void buildMenu();

 private:
  std::function<void()> show_fn;
  Settings s;
};

#endif  // TRAY_ICON_H
