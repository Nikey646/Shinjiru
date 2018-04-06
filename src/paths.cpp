/*  Copyright 2015 Lukáš Mandák
    Copyright 2015 Kazakuri

    This file is part of Shinjiru.

    Shinjiru is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Shinjiru is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Shinjiru.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "./paths.h"

#include <QApplication>
#include <QDir>
#include <QStandardPaths>
#include <QtDebug>

Paths::Paths(QObject *parent) : QObject(parent) {}

QString Paths::configDir(const QString &dirName) {
  QString dir;

  dir = QStandardPaths::locate(QStandardPaths::AppConfigLocation, dirName,
                               QStandardPaths::LocateDirectory);
  if (dir.isEmpty()) {
    QString tmp = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    dir = tmp + "/" + dirName;
    QDir(dir).mkpath(dir);
    qDebug() << "Created new config direcrory:" << dir;
  }

  return dir;
}

QString Paths::configFile(const QString &fileName) {
  QString name = QStandardPaths::locate(QStandardPaths::AppConfigLocation, fileName);

  if (name.isEmpty()) {
    QString dir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir(dir).mkpath(dir);
    name = dir + "/" + fileName;
  }

  return name;
}

QString Paths::dataDir(const QString &dirName) {
#ifdef Q_OS_LINUX
  QString name = QStandardPaths::locate(QStandardPaths::AppDataLocation, dirName,
                                        QStandardPaths::LocateDirectory);
  return name;
#else
  return qApp->applicationDirPath() + "/data/" + dirName;
#endif
}

QString Paths::dataFile(const QString &fileName) {
#ifdef Q_OS_LINUX
  QString name = QStandardPaths::locate(QStandardPaths::AppDataLocation, fileName);
  if (name.isEmpty()) qCritical() << "Missing data file:" << fileName;
  return name;
#else
  return qApp->applicationDirPath() + "/data/" + fileName;
#endif
}

QString Paths::logFileName() {
#if defined Q_OS_LINUX || defined Q_OS_MAC
  return QDir::homePath() + "/.Shinjiru.log";
#else
  return qApp->applicationDirPath() + "/Shinjiru.log";
#endif
}
