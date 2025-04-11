#pragma once

#include <QUdpSocket>

#include "imu_data_format.h"
#include "imu_logger.h"

class QString;

class ImuNetworkLogger : public ImuLogger
{
public:
  explicit ImuNetworkLogger(ImuDataFormat dataFormat, const QString& address, uint16_t port);

  void log(const Iml::ImuData& data) override;

private:
  const QHostAddress m_address;
  const uint16_t m_port;
  QUdpSocket m_socket;
  const ImuDataFormat m_dataFormat;
};
