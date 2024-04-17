#pragma once

#include "imu_configuration.h"
#include "imu_dynamic.h"
#include "skydel_plug_ins/skydel_plugin.h"

// Plugin implementation
class ImuPlugin : public QObject, public SkydelCoreInterface, public SkydelPositionObserverInterface
{
  Q_OBJECT

public:
  // SkydelCoreInterface
  inline void setLogPath(const QString& path) override { m_logPath = path; }
  inline void setNotifier(SkydelNotifierInterface* notifier) override { m_skydelNotifier = notifier; }
  void setConfiguration(const QString&, const QJsonObject& configuration) override;
  inline QJsonObject getConfiguration() const override { return m_configuration.getConfiguration(); }
  SkydelWidgets createUI() override;
  inline void initialize() override {}

  // SkydelPositionObserverInterface
  SkydelRuntimePositionObserver* createRuntimePositionObserver() override
  {
    return m_configuration.isEnabled() ? new ImuDynamic(m_configuration, m_logPath) : nullptr;
  }

signals:
  void configurationChanged();

private:
  QString m_logPath;
  SkydelNotifierInterface* m_skydelNotifier;
  ImuConfiguration m_configuration;
};

REGISTER_SKYDEL_PLUGIN(ImuPlugin)
