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

#ifndef SRC_PATHS_H_
#define SRC_PATHS_H_

#include <QObject>

class Paths : public QObject {
  Q_OBJECT

 public:
  explicit Paths(QObject *parent = 0);

  static QString configDir(const QString &dirName = "");
  static QString configFile(const QString &fileName);
  static QString dataDir(const QString &dirName = "");
  static QString dataFile(const QString &fileName);
  static QString logFileName();
};

#endif  // SRC_PATHS_H_
