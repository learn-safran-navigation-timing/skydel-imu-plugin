#include "imu_logger_serial.h"

#include "imu_serial_port_settings.h"
#include "logger_utils.h"

ImuSerialLogger::ImuSerialLogger(ImuDataFormat dataFormat, const SerialPortSettings& serialPortSettings) :
  ImuLogger(),
  m_dataFormat(dataFormat)
{
  m_serialPort.setPortName(serialPortSettings.portName);
  m_serialPort.setBaudRate(serialPortSettings.baudRate);
  m_serialPort.setFlowControl(serialPortSettings.flowControl);
  m_serialPort.setDataBits(static_cast<QSerialPort::DataBits>(serialPortSettings.dataBits));
  m_serialPort.setStopBits(static_cast<QSerialPort::StopBits>(serialPortSettings.stopBits));
  m_serialPort.setParity(serialPortSettings.parity);
  m_serialPort.open(QSerialPort::WriteOnly);
}

void ImuSerialLogger::log(const Iml::ImuData& data)
{
  const auto imuDatagram = getFormattedDataByteArray(data, m_dataFormat);
  if (m_serialPort.write(imuDatagram) == -1)
  {
    throw std::runtime_error("Error while logging data on " + m_serialPort.portName().toStdString() + " port.");
  }
}
