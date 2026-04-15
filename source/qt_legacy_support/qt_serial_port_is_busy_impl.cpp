#include "qt_serial_port_is_busy_impl.h"

#include <QFileInfo>
#include <QSerialPortInfo>

#ifdef WIN64
#include <windows.h> // Needs to be included before the others
// ----
#include <errhandlingapi.h>
#include <fileapi.h>
#include <handleapi.h>
#else
#include <errno.h>
#include <signal.h>
#include <sys/types.h>

#include <QStandardPaths>
#endif

namespace QtLegacySupport
{

bool isBusy(const QSerialPortInfo& info)
{
  // The method QSerialPortInfo::isBusy was removed in Qt 6. To ease the migration from Qt 5 to Qt 6, the original
  // implementation was added to the codebase. See :
  // https://github.com/qt/qtserialport/blob/v5.15.18-lts-lgpl/src/serialport/qserialportinfo_win.cpp

#ifdef WIN64
  const HANDLE handle = ::CreateFile(reinterpret_cast<const wchar_t*>(info.systemLocation().utf16()),
                                     GENERIC_READ | GENERIC_WRITE,
                                     0,
                                     nullptr,
                                     OPEN_EXISTING,
                                     0,
                                     nullptr);

  if (handle == INVALID_HANDLE_VALUE)
  {
    if (::GetLastError() == ERROR_ACCESS_DENIED)
      return true;
  }
  else
  {
    ::CloseHandle(handle);
  }
  return false;
#else
  auto serialPortLockFilePath = [](const QString& portName) {
    static const QStringList lockDirectoryPaths = QStringList()
                                                  << QStringLiteral("/var/lock") << QStringLiteral("/etc/locks")
                                                  << QStringLiteral("/var/spool/locks")
                                                  << QStringLiteral("/var/spool/uucp") << QStringLiteral("/tmp")
                                                  << QStringLiteral("/var/tmp") << QStringLiteral("/var/lock/lockdev")
                                                  << QStringLiteral("/run/lock")
#ifdef Q_OS_ANDROID
                                                  << QStringLiteral("/data/local/tmp")
#endif
                                                  << QStandardPaths::writableLocation(QStandardPaths::TempLocation);

    QString fileName = portName;
    fileName.replace(QLatin1Char('/'), QLatin1Char('_'));
    fileName.prepend(QLatin1String("/LCK.."));

    QString lockFilePath;

    for (const QString& lockDirectoryPath : lockDirectoryPaths)
    {
      const QString filePath = lockDirectoryPath + fileName;

      QFileInfo lockDirectoryInfo(lockDirectoryPath);
      if (lockDirectoryInfo.isReadable())
      {
        if (QFile::exists(filePath) || lockDirectoryInfo.isWritable())
        {
          lockFilePath = filePath;
          break;
        }
      }
    }

    if (lockFilePath.isEmpty())
    {
      qWarning("The following directories are not readable or writable for detaling with lock files\n");
      for (const QString& lockDirectoryPath : lockDirectoryPaths)
        qWarning("\t%s\n", qPrintable(lockDirectoryPath));
      return QString();
    }

    return lockFilePath;
  };

  QString lockFilePath = serialPortLockFilePath(info.portName());

  if (lockFilePath.isEmpty())
    return false;

  QFile reader(lockFilePath);
  if (!reader.open(QIODevice::ReadOnly))
    return false;

  QByteArray pidLine = reader.readLine();
  pidLine.chop(1);
  if (pidLine.isEmpty())
    return false;

  qint64 pid = pidLine.toLongLong();

  if (pid && (::kill(pid, 0) == -1) && (errno == ESRCH))
    return false; // PID doesn't exist anymore

  return true;
#endif
}

} // namespace QtLegacySupport
