#include "imu_plugin.h"

#include <QHostAddress>

#include "gravity.h"
#include "imu_configuration_keys.h"
#include "imu_data.h"
#include "imu_data_format.h"
#include "imu_runtime_objects.h"

ImuPlugin::ImuPlugin() : QObject(), m_configuration(), m_logPath(), m_notifier(nullptr)
{
  qRegisterMetaType<Iml::ImuData>("Iml::ImuData");
}

void ImuPlugin::setNotifier(SkydelNotifierInterface* notifier)
{
  m_notifier = notifier;
  m_configuration.setNotifier(notifier);
}

void ImuPlugin::setConfiguration(const QString&, const QJsonObject& configuration)
{
  m_configuration.clear();
  setDefaultConfiguration();
  m_configuration.update(configuration);
  emit configurationChanged(m_configuration);
}

void ImuPlugin::setDefaultConfiguration()
{
  m_configuration.addValue(ImuConfigurationKeys::FILE_LOGGING_ENABLED, false);
  m_configuration.addValue(ImuConfigurationKeys::FILE_LOGGING_FORMAT, ImuDataFormat::CSV);
  m_configuration.addValue(ImuConfigurationKeys::NETWORK_LOGGING_ENABLED, false);
  m_configuration.addValue(ImuConfigurationKeys::NETWORK_LOGGING_FORMAT, ImuDataFormat::JSON);
  m_configuration.addValue(ImuConfigurationKeys::NETWORK_LOGGING_ADDRESS,
                           QHostAddress(QHostAddress::LocalHost).toString());
  m_configuration.addValue(ImuConfigurationKeys::NETWORK_LOGGING_PORT, 161);
  m_configuration.addValue(ImuConfigurationKeys::GRAVITY_MODEL, Iml::GravityModel::WGS84);
  m_configuration.addTriplet(ImuConfigurationKeys::LEVEL_ARM, Iml::Triplet {0, 0, 0});
  m_configuration.addTriplet(ImuConfigurationKeys::ORIENTATION, Iml::Triplet {0, 0, 0});
  m_configuration.addValue(ImuConfigurationKeys::SERIAL_LOGGING_ENABLED, false);
  m_configuration.addValue(ImuConfigurationKeys::SERIAL_LOGGING_FORMAT, ImuDataFormat::STIM_IMU);
  m_configuration.addSerialPortSettings(SerialPortSettings());
}

SkydelRuntimePositionObserver* ImuPlugin::createRuntimePositionObserver()
{
  auto runtimeObjects = new ImuRuntimeObjects(m_logPath, m_configuration, [this](auto data) {
    this->onImuDataUpdate(data);
  });

  connect(runtimeObjects, &QObject::destroyed, this, [this]() { emit simulationStopped(); });

  emit simulationStarted();

  return runtimeObjects;
}
