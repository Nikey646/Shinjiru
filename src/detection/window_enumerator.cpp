#include "./window_enumerator.h"

#include <QDebug>

#include <Robot.h>

#include "./media_store.h"

using namespace Robot;

void WindowEnumerator::enumerateWindows() {
  MediaStore &store = MediaStore::instance();
  WindowList wList = Window::GetList();

  store.removeInvalid();

  for (auto &&window : wList) {
    Process process = window.GetProcess();

    if (store.hasProcess(process.GetPID())) {
      continue;
    }

    if (store.hasMediaPlayer(process.GetPID())) {
      continue;
    }

    store.addProcess(process);

    if (false) {  // TODO: isMediaPlayer
      store.addMediaPlayer(process);
    }
  }
}
