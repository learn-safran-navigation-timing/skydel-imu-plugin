#pragma once

#include "triplet.h"

namespace Iml
{

struct ImuData
{
  int64_t time = 0;                    // millisecond
  Triplet acceleration = {0, 0, 0};    // meter/second^2
  Triplet angularVelocity = {0, 0, 0}; // radian/second
};

} // namespace Iml
