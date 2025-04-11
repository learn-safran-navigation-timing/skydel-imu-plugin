#include "imu_logger_file.h"

#include <QTextStream>

#include "logger_utils.h"

namespace
{
QString getFileExtension(ImuDataFormat dataFormat)
{
  switch (dataFormat)
  {
    case ImuDataFormat::CSV:
      return csvFileExtension;
    case ImuDataFormat::JSON:
      return jsonFileExtension;
    case ImuDataFormat::STIM_IMU:
      return stimImuFileExtension;
  }
  return ".txt";
}

QString getHeader(ImuDataFormat dataFormat)
{
  switch (dataFormat)
  {
    case ImuDataFormat::CSV:
      return csvHeader;
    case ImuDataFormat::JSON:
      return jsonHeader;
    case ImuDataFormat::STIM_IMU:
      return stimImuHeader;
  }
  return "UNKNOWN";
}
} // namespace

ImuFileLogger::ImuFileLogger(ImuDataFormat dataFormat, const QString& logFilePath) :
  ImuLogger(),
  m_dataFormat(dataFormat),
  m_file(logFilePath + getFileExtension(m_dataFormat))
{
  if (!m_file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    throw std::runtime_error("ImuFileLogger - Can't open file.");

  writeInFile(getHeader(m_dataFormat));
}

void ImuFileLogger::writeInFile(const QString& string)
{
  QTextStream stream(&m_file);
  stream << string;

  if (stream.status() == QTextStream::WriteFailed)
    throw std::runtime_error("ImuFileLogger - Failed to write in file.");
}

void ImuFileLogger::log(const Iml::ImuData& data)
{
  writeInFile(getFormattedData(data, m_dataFormat));
}
