#pragma once

#include "imu_data.h"

class ImuLogger
{
public:
  virtual ~ImuLogger() = default;
  virtual void log(const Iml::ImuData& data) = 0;
};
