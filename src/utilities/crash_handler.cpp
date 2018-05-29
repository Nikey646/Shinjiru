#include "crash_handler.h"
#include <QString>
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QProcess>

#ifdef Q_OS_WIN32
#include <breakpad/src/client/windows/handler/exception_handler.h>
#endif

namespace Breakpad {
/************************************************************************/
/* CrashHandlerPrivate                                                  */
/************************************************************************/
class CrashHandlerPrivate {
 public:
  CrashHandlerPrivate() {
    pHandler = NULL;
  }

  ~CrashHandlerPrivate() {
    delete pHandler;
  }

  void InitCrashHandler(const QString& dumpPath);
  static google_breakpad::ExceptionHandler* pHandler;
  static bool bReportCrashesToSystem;
};

google_breakpad::ExceptionHandler* CrashHandlerPrivate::pHandler = NULL;
bool CrashHandlerPrivate::bReportCrashesToSystem = false;

/************************************************************************/
/* DumpCallback                                                         */
/************************************************************************/
#ifdef Q_OS_WIN32
bool DumpCallback(const wchar_t* _dump_dir, const wchar_t* _minidump_id, void* context,
                  EXCEPTION_POINTERS* exinfo, MDRawAssertionInfo* assertion, bool success) {
  Q_UNUSED(context);
  Q_UNUSED(_dump_dir);
  Q_UNUSED(_minidump_id);
  Q_UNUSED(assertion);
  Q_UNUSED(exinfo);

  qDebug("BreakpadQt crash");

  /*
  NO STACK USE, NO HEAP USE THERE !!!
  Creating QString's, using qDebug, etc. - everything is crash-unfriendly.
  */
  return CrashHandlerPrivate::bReportCrashesToSystem ? success : true;
}
#endif

void CrashHandlerPrivate::InitCrashHandler(const QString& dumpPath) {
  if (pHandler != NULL) return;

#ifdef Q_OS_WIN32
  std::wstring pathAsStr = (const wchar_t*)dumpPath.utf16();
  pHandler = new google_breakpad::ExceptionHandler(pathAsStr,
                                                   /*FilterCallback*/ 0, DumpCallback,
                                                   /*context*/
                                                   0, true);
#endif
}

/************************************************************************/
/* CrashHandler                                                         */
/************************************************************************/
CrashHandler* CrashHandler::instance() {
  static CrashHandler globalHandler;
  return &globalHandler;
}

CrashHandler::CrashHandler() {
  d = new CrashHandlerPrivate();
}

CrashHandler::~CrashHandler() {
  delete d;
}

void CrashHandler::setReportCrashesToSystem(bool report) {
  d->bReportCrashesToSystem = report;
}

bool CrashHandler::writeMinidump() {
  bool res = d->pHandler->WriteMinidump();
  if (res) {
    qDebug("BreakpadQt: writeMinidump() success.");
  } else {
    qWarning("BreakpadQt: writeMinidump() failed.");
  }
  return res;
}

void CrashHandler::Init(const QString& reportPath) {
  d->InitCrashHandler(reportPath);
}
}  // namespace Breakpad
