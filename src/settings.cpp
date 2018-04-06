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
    case Setting::Language:
      return "English";
    case Setting::StartOnBoot:
      return false;
    case Setting::CheckForUpdates:
      return false;
    case Setting::StartMinimized:
      return false;
    case Setting::MinimizeToTray:
      return false;
    case Setting::CloseToTray:
      return true;
    case Setting::UseAnimeRecognition:
      return true;
    case Setting::UseMasato:
      return true;
    case Setting::UseDiscord:
      return true;
    case Setting::UpdateDelay:
      return 120;
    case Setting::NotifyDetected:
      return true;
    case Setting::NotifyUpdated:
      return false;
    case Setting::DefaultSubgroup:
      return "HorribleSubs";
    case Setting::DefaultQuality:
      return "720p";
  }

  return QVariant();
}

QString Settings::toString(const Setting &s) const {
  return s._to_string();
}
