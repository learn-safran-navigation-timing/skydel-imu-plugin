#include "imu_runtime_objects.h"

#include <QDir>

#include "imu_configuration.h"
#include "imu_configuration_keys.h"
#include "imu_logger.h"
#include "imu_logger_file.h"
#include "imu_logger_network.h"
#include "imu_logger_serial.h"

namespace
{

auto createFileLogger(const QString& logPath, const ImuConfiguration& configuration)
{
  return std::make_unique<ImuFileLogger>(static_cast<ImuDataFormat>(
                                           configuration.getValue(ImuConfigurationKeys::FILE_LOGGING_FORMAT).toInt()),
                                         logPath + QDir::separator() + "imu_dynamic");
}

auto createNetworkLogger(const ImuConfiguration& configuration)
{
  return std::make_unique<ImuNetworkLogger>(
    static_cast<ImuDataFormat>(configuration.getValue(ImuConfigurationKeys::NETWORK_LOGGING_FORMAT).toInt()),
    configuration.getValue(ImuConfigurationKeys::NETWORK_LOGGING_ADDRESS).toString(),
    static_cast<uint16_t>(configuration.getValue(ImuConfigurationKeys::NETWORK_LOGGING_PORT).toInt()));
}

auto createSerialLogger(const ImuConfiguration& configuration)
{
  return std::make_unique<ImuSerialLogger>(
    static_cast<ImuDataFormat>(configuration.getValue(ImuConfigurationKeys::SERIAL_LOGGING_FORMAT).toInt()),
    configuration.getSerialPortSettings());
}

std::vector<std::unique_ptr<ImuLogger>> createLoggers(const QString& logPath, const ImuConfiguration& configuration)
{
  std::vector<std::unique_ptr<ImuLogger>> loggers;

  if (configuration.getValue(ImuConfigurationKeys::FILE_LOGGING_ENABLED).toBool())
  {
    loggers.push_back(createFileLogger(logPath, configuration));
  }

  if (configuration.getValue(ImuConfigurationKeys::NETWORK_LOGGING_ENABLED).toBool())
  {
    loggers.push_back(createNetworkLogger(configuration));
  }

  if (configuration.getValue(ImuConfigurationKeys::SERIAL_LOGGING_ENABLED).toBool())
  {
    loggers.push_back(createSerialLogger(configuration));
  }

  return loggers;
}

Iml::ImuDynamic createDynamics(const auto& configuration)
{
  return Iml::ImuDynamic(static_cast<Iml::GravityModel>(
                           configuration.getValue(ImuConfigurationKeys::GRAVITY_MODEL).toInt()),
                         configuration.getTriplet(ImuConfigurationKeys::LEVEL_ARM),
                         configuration.getTriplet(ImuConfigurationKeys::ORIENTATION));
}

} // namespace

ImuRuntimeObjects::ImuRuntimeObjects(const QString& logPath,
                                     const ImuConfiguration& configuration,
                                     const std::function<void(Iml::ImuData&)>& updateImuDataFunction) :
  m_loggers(createLoggers(logPath, configuration)),
  m_dynamics(createDynamics(configuration)),
  m_updateImuDataFunction(updateImuDataFunction)
{
}

void ImuRuntimeObjects::pushPosition(const TimedPosition& p)
{
  if (!m_loggers.empty())
  {
    push(p);

    if (m_dynamics.isReady())
    {
      auto imuData = m_dynamics.getData();

      m_updateImuDataFunction(imuData);

      log(imuData);
    }
  }
}

void ImuRuntimeObjects::push(const TimedPosition& p)
{
  m_dynamics.pushPosition(p.time,
                          {p.position.x, p.position.y, p.position.z},
                          {p.orientation.roll, p.orientation.pitch, p.orientation.yaw});
}

void ImuRuntimeObjects::log(const Iml::ImuData& data)
{
  for (const auto& logger : m_loggers)
  {
    logger->log(data);
  }
}
