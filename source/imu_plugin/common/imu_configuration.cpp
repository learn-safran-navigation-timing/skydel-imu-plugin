#include "imu_configuration.h"

#include <QJsonArray>

#include <optional>

#include "imu_configuration_keys.h"

namespace
{

void reportGetError(SkydelNotifierInterface* notifier, const QString& key)
{
  if (notifier)
  {
    notifier->notify("Failed to read the \"" + key.toStdString() + "\" configuration key. Defaulting to zeros.",
                     SkydelNotifierInterface::Type::WARNING);
  }
}

QJsonArray matrix3ToJsonArray(const Iml::Matrix3x3& matrix)
{
  QJsonArray array;

  for (size_t i = 0; i < matrix.rows(); ++i)
  {
    array.push_back(QJsonArray {matrix(i, 0), matrix(i, 1), matrix(i, 2)});
  }

  return array;
}

std::optional<Iml::Matrix3x3> jsonArrayToMatrix3(const QJsonArray& array)
{
  if (array.size() != 3)
  {
    return {};
  }

  Iml::Matrix3x3 matrix;
  size_t i = 0;

  for (const auto row : array)
  {
    if (!row.isArray())
    {
      return {};
    }

    const auto triplet = row.toArray();

    if (triplet.size() != 3)
    {
      return {};
    }

    blaze::row(matrix, i) = {triplet[0].toDouble(), triplet[1].toDouble(), triplet[2].toDouble()};
    ++i;
  }

  return matrix;
}

SerialPortSettings jsonToSerialPortSettings(const QJsonArray& array)
{
  assert(array.size() == 6);

  return {.baudRate = array[0].toInt(),
          .dataBits = array[1].toInt(),
          .parity = QSerialPort::Parity(array[2].toInt()),
          .stopBits = array[3].toInt(),
          .flowControl = QSerialPort::FlowControl(array[4].toInt()),
          .portName = array[5].toString()};
}

} // namespace

ImuConfiguration::ImuConfiguration() : m_notifier(nullptr), m_configuration()
{
}

void ImuConfiguration::update(const QJsonObject& newConfiguration)
{
  for (const auto& key : m_configuration.keys())
  {
    if (!newConfiguration.contains(key))
    {
      continue;
    }

    if (m_configuration.value(key).type() != newConfiguration.value(key).type())
    {
      if (m_notifier)
      {
        m_notifier->notify("The \"" + key.toStdString() +
                             "\" parameter in Skydel configuration has an unexpected type and will not be loaded. ",
                           SkydelNotifierInterface::Type::WARNING);
      }
      continue;
    }

    m_configuration.insert(key, newConfiguration.value(key));
  }
}

QJsonValue ImuConfiguration::getValue(const QString& key) const
{
  if (m_configuration.contains(key))
  {
    return m_configuration.value(key);
  }
  else
  {
    reportGetError(m_notifier, key);
    return {}; // Null
  }
}

void ImuConfiguration::addValue(const QString& key, const QJsonValue& value)
{
  if (m_configuration.contains(key))
  {
    if (m_notifier)
    {
      m_notifier->notify("The \"" + key.toStdString() +
                           "\" configuration key has already been added. The first added value will be used.",
                         SkydelNotifierInterface::Type::WARNING);
    }
  }
  else
  {
    m_configuration.insert(key, value);
  }
}

void ImuConfiguration::setValue(const QString& key, const QJsonValue& value)
{
  bool success = false;

  if (m_configuration.contains(key))
  {
    const auto currentValue = m_configuration.value(key);

    if (currentValue.type() == value.type())
    {
      success = true;

      if (currentValue != value)
      {
        m_configuration.insert(key, value);

        if (m_notifier)
        {
          m_notifier->setDirty();
        }
      }
    }
  }

  if (!success && m_notifier)
  {
    m_notifier->notify("Failed to set the \"" + key.toStdString() + "\" configuration key.",
                       SkydelNotifierInterface::Type::WARNING);
  }
}

Iml::Triplet ImuConfiguration::getTriplet(const QString& key) const
{
  Iml::Triplet triplet;
  QJsonValue value = getValue(key);

  if (!value.isNull())
  {
    bool success = false;

    if (value.isArray())
    {
      const auto array = value.toArray();

      if (array.size() == 3)
      {
        triplet = {array[0].toDouble(), array[1].toDouble(), array[2].toDouble()};
        success = true;
      }
    }

    if (!success)
    {
      reportGetError(m_notifier, key);
    }
  }

  return triplet;
}

void ImuConfiguration::addTriplet(const QString& key, const Iml::Triplet& triplet)
{
  QJsonArray array {triplet[0], triplet[1], triplet[2]};
  addValue(key, array);
}

void ImuConfiguration::setTriplet(const QString& key, const Iml::Triplet& triplet)
{
  QJsonArray array {triplet[0], triplet[1], triplet[2]};
  setValue(key, array);
}

Iml::Matrix3x3 ImuConfiguration::getMatrix3x3(const QString& key) const
{
  Iml::Matrix3x3 matrix;
  QJsonValue value = getValue(key);

  if (!value.isNull())
  {
    bool success = false;

    if (value.isArray())
    {
      const auto optionalMatrix = jsonArrayToMatrix3(value.toArray());

      if (optionalMatrix.has_value())
      {
        matrix = optionalMatrix.value();
        success = true;
      }
    }

    if (!success)
    {
      reportGetError(m_notifier, key);
    }
  }

  return matrix;
}

void ImuConfiguration::addMatrix3x3(const QString& key, const Iml::Matrix3x3& matrix)
{
  addValue(key, matrix3ToJsonArray(matrix));
}

void ImuConfiguration::setMatrix3x3(const QString& key, const Iml::Matrix3x3& matrix)
{
  setValue(key, matrix3ToJsonArray(matrix));
}

SerialPortSettings ImuConfiguration::getSerialPortSettings() const
{
  QJsonValue value = getValue(ImuConfigurationKeys::SERIAL_LOGGING_CONFIGURATION);
  assert(value.isArray());

  return jsonToSerialPortSettings(value.toArray());
}
void ImuConfiguration::addSerialPortSettings(const SerialPortSettings& serialPortSettings)
{
  QJsonArray array {serialPortSettings.baudRate,
                    serialPortSettings.dataBits,
                    serialPortSettings.parity,
                    serialPortSettings.stopBits,
                    serialPortSettings.flowControl,
                    serialPortSettings.portName};
  addValue(ImuConfigurationKeys::SERIAL_LOGGING_CONFIGURATION, array);
}
void ImuConfiguration::setSerialPortSettings(const SerialPortSettings& serialPortSettings)
{
  QJsonArray array {serialPortSettings.baudRate,
                    serialPortSettings.dataBits,
                    serialPortSettings.parity,
                    serialPortSettings.stopBits,
                    serialPortSettings.flowControl,
                    serialPortSettings.portName};
  setValue(ImuConfigurationKeys::SERIAL_LOGGING_CONFIGURATION, array);
}
