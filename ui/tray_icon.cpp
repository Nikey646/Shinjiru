#include "tray_icon.h"

#include <QApplication>
#include <QFile>
#include <QIcon>
#include <QMenu>
#include <QObject>
#include <QPixmap>

#include "./main_window.h"

TrayIcon::TrayIcon(QObject *parent) : QSystemTrayIcon(parent) {
  {
    QPixmap window_icon;
    QFile iconFile(":/res/icon.svg");
    iconFile.open(QFile::ReadOnly);
    QByteArray icon_data = iconFile.readAll();
    window_icon.loadFromData(icon_data);
    this->setIcon(QIcon(window_icon));
  }

  this->buildMenu();

  const auto version = QString("%1.%2.%3").arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_PATCH);
  this->setToolTip("Shinjiru " + version);

  connect(this, &TrayIcon::activated, this, [this](QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::DoubleClick) {
      this->restore();
    }
  });

  this->show();
}

TrayIcon::~TrayIcon() {
  delete this->contextMenu();
}

void TrayIcon::setShowFunction(std::function<void()> fn) {
  show_fn = fn;
}

void TrayIcon::restore() {
  MainWindow *mw = reinterpret_cast<MainWindow *>(this->parent());

  if (show_fn) {
    show_fn();
  } else {
    mw->showNormal();
  }

  qApp->setActiveWindow(mw);
}

void TrayIcon::buildMenu() {
  QMenu *menu = new QMenu;

  QAction *actionRestore = menu->addAction(tr("Restore"));
  menu->addSeparator();
  QAction *actionExit = menu->addAction(tr("Exit"));

  connect(actionRestore, &QAction::triggered, this, &TrayIcon::restore);
  connect(actionExit, &QAction::triggered, this, []() { qApp->exit(); });

  this->setContextMenu(menu);
}
