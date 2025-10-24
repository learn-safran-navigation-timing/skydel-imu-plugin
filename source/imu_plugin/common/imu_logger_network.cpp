#include "imu_logger_network.h"

#include "imu_logger_utils.h"

ImuNetworkLogger::ImuNetworkLogger(ImuDataFormat dataFormat, const QString& address, uint16_t port) :
  ImuLogger(),
  m_address(address),
  m_port(port),
  m_dataFormat(dataFormat)
{
}

void ImuNetworkLogger::log(const Iml::ImuData& data)
{
  if (m_socket.writeDatagram(getFormattedData(data, m_dataFormat, false), m_address, m_port) == -1)
  {
    throw std::runtime_error(m_socket.errorString().toStdString());
  }
}
