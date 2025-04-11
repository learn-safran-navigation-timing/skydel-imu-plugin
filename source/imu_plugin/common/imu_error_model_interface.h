#pragma once

#include "imu_configuration.h"
#include "imu_data.h"
#include "skydel_core_interface.h"

class ImuErrorModelInterface
{
public:
  virtual ~ImuErrorModelInterface() = default;

  virtual void setDefaultConfiguration(ImuConfiguration& configuration) = 0;
  virtual SkydelWidgets createUI(const ImuConfiguration& configuration) = 0;

  virtual void onSimulationStart() = 0;
  virtual void onSimulationStop() = 0;
  virtual void onConfigurationUpdate() = 0;

  virtual void setEnabled(bool enabled) = 0;
  virtual void applyErrors(Iml::ImuData& imuData) = 0;
};
