INCLUDEPATH += "$$PWD/robot/Source"

SOURCES += \
  "$$PWD/robot/Source/Bounds.cc" \
  "$$PWD/robot/Source/Clipboard.cc" \
  "$$PWD/robot/Source/Color.cc" \
  "$$PWD/robot/Source/Hash.cc" \
  "$$PWD/robot/Source/Image.cc" \
  "$$PWD/robot/Source/Keyboard.cc" \
  "$$PWD/robot/Source/Memory.cc" \
  "$$PWD/robot/Source/Module.cc" \
  "$$PWD/robot/Source/Mouse.cc" \
  "$$PWD/robot/Source/Point.cc" \
  "$$PWD/robot/Source/Process.cc" \
  "$$PWD/robot/Source/Range.cc" \
  "$$PWD/robot/Source/Screen.cc" \
  "$$PWD/robot/Source/Size.cc" \
  "$$PWD/robot/Source/Timer.cc" \
  "$$PWD/robot/Source/Window.cc"

HEADERS += \
  "$$PWD/robot/Source/Bounds.h" \
  "$$PWD/robot/Source/Clipboard.h" \
  "$$PWD/robot/Source/Color.h" \
  "$$PWD/robot/Source/Enum.h" \
  "$$PWD/robot/Source/Global.h" \
  "$$PWD/robot/Source/Hash.h" \
  "$$PWD/robot/Source/Image.h" \
  "$$PWD/robot/Source/Keyboard.h" \
  "$$PWD/robot/Source/Memory.h" \
  "$$PWD/robot/Source/Module.h" \
  "$$PWD/robot/Source/Mouse.h" \
  "$$PWD/robot/Source/Point.h" \
  "$$PWD/robot/Source/Process.h" \
  "$$PWD/robot/Source/Range.h" \
  "$$PWD/robot/Source/Robot.h" \
  "$$PWD/robot/Source/Screen.h" \
  "$$PWD/robot/Source/Size.h" \
  "$$PWD/robot/Source/Timer.h" \
  "$$PWD/robot/Source/Types.h" \
  "$$PWD/robot/Source/Window.h"

win32:LIBS += -lUser32 -lKernel32 -lGdi32 -lAdvapi32 -lDwmapi
linux:LIBS += -lXtst -lXinerama
