#ifndef COMMITTABLE_WIDGET_H
#define COMMITTABLE_WIDGET_H

#include <QMap>
#include <QString>
#include <QVariant>
#include <QWidget>

#include "../../../src/settings.h"

class CommittableWidget : public QWidget {
  Q_OBJECT

  using SettingMap = QMap<Setting, QVariant>;

 public:
  explicit CommittableWidget(QWidget *parent = nullptr);

  SettingMap changes() const;
  bool needsRestart() const;

  virtual void resetToDefault() = 0;

 protected:
  SettingMap changed_settings;
  bool restart_required{false};
};

#endif  // COMMITTABLE_WIDGET_H
