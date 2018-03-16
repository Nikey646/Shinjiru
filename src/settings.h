#ifndef SRC_SETTINGS_H__
#define SRC_SETTINGS_H__

#include <QSettings>
#include <QString>
#include <QVariant>

#include <better-enums/enum.h>

// clang-format off
BETTER_ENUM(
  Setting,
  int,
  StartOnBoot,
  StartMinimized,
  MinimizeToTray,
  UseMasato
)
// clang-format on

class Settings {
 public:
  Settings() = default;

  void set(const Setting &s, const QVariant &v);
  QVariant get(const Setting &s) const;
  static QVariant getDefault(const Setting &s);

 private:
  QString toString(const Setting &s) const;

 private:
  QSettings m_settings;
};

#endif  // SRC_SETTINGS_H__
