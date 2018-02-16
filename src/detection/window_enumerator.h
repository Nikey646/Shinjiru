#ifndef SRC_DETECTION_WINDOW_ENUMERATOR_H__
#define SRC_DETECTION_WINDOW_ENUMERATOR_H__

#include "../utilities/singleton.h"

class WindowEnumerator : public Singleton<WindowEnumerator> {
  Q_OBJECT

 public:
  void enumerateWindows();
};

#endif  // SRC_DETECTION_WINDOW_ENUMERATOR_H__
