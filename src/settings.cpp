#include "settings.h"

void Settings::set(const Setting &s, const QVariant &v) {
  auto k = toString(s);

  this->m_settings.setValue(k, v);
}

QVariant Settings::get(const Setting &s) const {
  auto k = toString(s);
  auto d = getDefault(s);

  return this->m_settings.value(k, d);
}

QVariant Settings::getDefault(const Setting &s) {
  switch (s) {
    case Setting::StartOnBoot:
      return false;
    case Setting::StartMinimized:
      return false;
    case Setting::MinimizeToTray:
      return false;
    case Setting::UseMasato:
      return true;
  }

  return QVariant();
}

QString Settings::toString(const Setting &s) const {
  return s._to_string();
}
