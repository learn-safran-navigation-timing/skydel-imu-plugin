#pragma once

#include <QSerialPort>

#include "imu_data_format.h"
#include "imu_logger.h"

struct SerialPortSettings;

class ImuSerialLogger : public ImuLogger
{
public:
  explicit ImuSerialLogger(ImuDataFormat dataFormat, const SerialPortSettings& serialPortSettings);

  void log(const Iml::ImuData& data) override;

private:
  const ImuDataFormat m_dataFormat;
  QSerialPort m_serialPort;
};
