#pragma once

#include <QSerialPort>
#include <QString>

namespace SerialPortUtils
{

const std::vector<int> ValidBaudRates = {374400, 460800, 921600, 1843200};
const std::vector<QSerialPort::Parity> ParityValues = {QSerialPort::NoParity,
                                                       QSerialPort::EvenParity,
                                                       QSerialPort::OddParity,
                                                       QSerialPort::SpaceParity,
                                                       QSerialPort::MarkParity};
const std::vector<QSerialPort::FlowControl> FlowControlValues = {QSerialPort::NoFlowControl,
                                                                 QSerialPort::HardwareControl,
                                                                 QSerialPort::SoftwareControl};
constexpr int MIN_DATA_BIT = 8;
constexpr int MAX_DATA_BIT = 8;

constexpr int DEFAULT_BAUD_RATE = 374400;
constexpr int DEFAULT_DATA_BIT = 8;
constexpr auto DEFAULT_PARITY = QSerialPort::NoParity;
constexpr int DEFAULT_STOP_BIT = 1;
constexpr auto DEFAULT_FLOW_CONTROL = QSerialPort::NoFlowControl;

inline QSerialPort::Parity parseParityString(const QString& string)
{
  if (string == "NoParity")
    return QSerialPort::NoParity;
  else if (string == "Even")
    return QSerialPort::EvenParity;
  else if (string == "Odd")
    return QSerialPort::OddParity;
  else if (string == "Mark")
    return QSerialPort::MarkParity;
  else
    return QSerialPort::SpaceParity;
}

inline QSerialPort::FlowControl parseFlowControlString(const QString& string)
{
  if (string == "NoFlowControl")
    return QSerialPort::NoFlowControl;
  else if (string == "Hardware")
    return QSerialPort::HardwareControl;
  else
    return QSerialPort::SoftwareControl;
}

inline QString parityString(QSerialPort::Parity portParity)
{
  switch (portParity)
  {
    case QSerialPort::NoParity:
      return "NoParity";
    case QSerialPort::EvenParity:
      return "Even";
    case QSerialPort::OddParity:
      return "Odd";
    case QSerialPort::MarkParity:
      return "Mark";
    case QSerialPort::SpaceParity:
      return "Space";
    default:
      return "";
  }
}

inline QString flowControlString(QSerialPort::FlowControl portFlowControl)
{
  switch (portFlowControl)
  {
    case QSerialPort::NoFlowControl:
      return "NoFlowControl";
    case QSerialPort ::HardwareControl:
      return "Hardware";
    case QSerialPort::SoftwareControl:
      return "Software";
    default:
      return "";
  }
}

} // namespace SerialPortUtils
