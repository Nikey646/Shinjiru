#include "committable_widget.h"

CommittableWidget::CommittableWidget(QWidget *parent) : QWidget(parent) {}

auto CommittableWidget::changes() const -> SettingMap {
  return changed_settings;
}

bool CommittableWidget::needsRestart() const {
  return restart_required;
}
