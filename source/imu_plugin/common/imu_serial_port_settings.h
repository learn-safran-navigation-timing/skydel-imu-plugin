#pragma once

#include <QtSerialPort/QSerialPort>

#include "imu_serial_port_utils.h"

struct SerialPortSettings
{
  int baudRate {SerialPortUtils::DEFAULT_BAUD_RATE};
  int dataBits {SerialPortUtils::DEFAULT_DATA_BIT};
  QSerialPort::Parity parity {SerialPortUtils::DEFAULT_PARITY};
  int stopBits {SerialPortUtils::DEFAULT_STOP_BIT};
  QSerialPort::FlowControl flowControl {SerialPortUtils::DEFAULT_FLOW_CONTROL};
  QString portName {""};
};
