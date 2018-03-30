#ifndef UI_MAIN_WINDOW_VIEWS_AIRING_H__
#define UI_MAIN_WINDOW_VIEWS_AIRING_H__

#include <QList>
#include <QQmlContext>
#include <QWidget>

#include "../../src/models/media.h"

namespace Ui {
class Airing;
}

namespace Views {
class Airing : public QWidget {
  Q_OBJECT

 public:
  explicit Airing(QWidget *parent = 0);
  ~Airing();
  void resizeEvent(QResizeEvent *event);

  QList<QObject *> media() const;

  Q_INVOKABLE void showAnimePanel(int id);

 private:
  Ui::Airing *ui;
  QList<QObject *> m_media;
  QQmlContext *context;
  QWidget *container;
};
}  // namespace Views

#endif  // UI_MAIN_WINDOW_VIEWS_AIRING_H__
