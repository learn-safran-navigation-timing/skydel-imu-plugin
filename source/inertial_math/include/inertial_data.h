#pragma once

#include "triplet.h"

namespace Iml
{

struct InertialData
{
  int64_t time = 0;                        // millisecond
  Triplet position = {0, 0, 0};            // meter (ECEF)
  Triplet velocity = {0, 0, 0};            // meter/second
  Triplet acceleration = {0, 0, 0};        // meter/second^2
  Triplet jerk = {0, 0, 0};                // meter/second^3
  Triplet attitude = {0, 0, 0};            // radian (roll, pitch, yaw)
  Triplet angularVelocity = {0, 0, 0};     // radian/second
  Triplet angularAcceleration = {0, 0, 0}; // radian/second^2
  Triplet angularJerk = {0, 0, 0};         // radian/second^3
};

} // namespace Iml
