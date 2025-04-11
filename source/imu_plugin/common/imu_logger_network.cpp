#include "imu_logger_network.h"

#include "logger_utils.h"

ImuNetworkLogger::ImuNetworkLogger(ImuDataFormat dataFormat, const QString& address, uint16_t port) :
  ImuLogger(),
  m_address(address),
  m_port(port),
  m_dataFormat(dataFormat)
{
}

void ImuNetworkLogger::log(const Iml::ImuData& data)
{
  QByteArray byteArray;
  byteArray.append(getFormattedData(data, m_dataFormat).toUtf8());
  if (m_socket.writeDatagram(byteArray, m_address, m_port) == -1)
    throw std::runtime_error(m_socket.errorString().toStdString().c_str());
}
