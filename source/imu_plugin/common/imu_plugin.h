#pragma once

#include "imu_configuration.h"
#include "skydel_plug_ins/skydel_core_interface.h"
#include "skydel_plug_ins/skydel_position_observer_interface.h"

namespace Iml
{
struct ImuData;
}

class ImuPlugin : public QObject, public SkydelCoreInterface, public SkydelPositionObserverInterface
{
  Q_OBJECT

public:
  ImuPlugin();
  virtual ~ImuPlugin() = default;

  inline void setLogPath(const QString& path) override { m_logPath = path; }
  inline void setInstanceIdentifier(uint32_t) override {}
  void setNotifier(SkydelNotifierInterface* notifier) override;
  void setConfiguration(const QString&, const QJsonObject& configuration) override;
  inline QJsonObject getConfiguration() const override { return m_configuration.toJson(); }
  inline void initialize() override {}
  SkydelRuntimePositionObserver* createRuntimePositionObserver() override;

protected:
  virtual void setDefaultConfiguration();
  virtual void onImuDataUpdate(Iml::ImuData& imuData) = 0;

signals:
  void configurationChanged(const ImuConfiguration& configuration);
  void simulationStarted();
  void simulationStopped();

protected:
  const QString PLUGIN_MENU_PATH = "/" + SKYDEL_WIDGET_SETTINGS_KEY + "/Vehicle/";
  ImuConfiguration m_configuration;

private:
  QString m_logPath;
  SkydelNotifierInterface* m_notifier;
};
