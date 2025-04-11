#pragma once

#include <memory>
#include <vector>

#include "imu_dynamic.h"
#include "skydel_plug_ins/skydel_position_observer_interface.h"

class ImuConfiguration;
class ImuLogger;

class ImuRuntimeObjects final : public SkydelRuntimePositionObserver
{
public:
  ImuRuntimeObjects(const QString& logPath,
                    const ImuConfiguration& configuration,
                    const std::function<void(Iml::ImuData&)>& updateImuDataFunction);
  ~ImuRuntimeObjects() = default;

  void pushPosition(const TimedPosition& p) override;

  inline void connectToView(QWidget*) override {}

private:
  void push(const TimedPosition& p);
  void log(const Iml::ImuData& data);

  std::vector<std::unique_ptr<ImuLogger>> m_loggers;
  Iml::ImuDynamic m_dynamics;
  std::function<void(Iml::ImuData&)> m_updateImuDataFunction;
};
