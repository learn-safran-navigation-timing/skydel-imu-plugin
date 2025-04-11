#pragma once

#include <QFile>

#include "imu_data_format.h"
#include "imu_logger.h"

class QString;

class ImuFileLogger : public ImuLogger
{
public:
  explicit ImuFileLogger(ImuDataFormat dataFormat, const QString& logFilePath);

  void log(const Iml::ImuData& data) override;

private:
  void writeInFile(const QString& string);

  const ImuDataFormat m_dataFormat;
  QFile m_file;
};
