#pragma once

#include <cstdint>
#include <vector>

#include "inertial_data.h"
#include "triplet.h"

namespace Iml
{

class BodyDynamic
{
public:
  virtual ~BodyDynamic() = default;
  virtual inline InertialData getPosition() const { return m_datas[1]; }
  bool isAccelerationReady() const;
  bool isJerkReady() const;
  void pushPosition(int64_t time,             // milliseconds
                    const Triplet& position,  // meters (ECEF)
                    const Triplet& attitude); // randians (roll, pitch, yaw)

private:
  Triplet m_phaseCorrection;
  std::vector<InertialData> m_datas;
};

} // namespace Iml
