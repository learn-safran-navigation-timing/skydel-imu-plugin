#pragma once

#include <cstdint>
#include <memory>

#include "body_dynamic.h"
#include "gravity.h"
#include "imu_data.h"
#include "rotation_matrix.h"
#include "triplet.h"

namespace Iml
{

class ImuDynamic
{
public:
  ImuDynamic(GravityModel gravityModel,
             const Triplet& leverArm,     // meters (body)
             const Triplet& orientation); // radians (rotation angles around the body X, Y and Z axes respectively)

  inline ImuData getData() const { return m_imuData; }
  inline bool isReady() const { return m_bodyDynamic.isJerkReady(); }
  void pushPosition(int64_t time,             // milliseconds
                    const Triplet& position,  // meters (ECEF)
                    const Triplet& attitude); // randians (roll, pitch, yaw)

private:
  BodyDynamic m_bodyDynamic;
  ImuData m_imuData;
  const GravityModel m_gravityModel;
  const std::unique_ptr<Triplet> m_imuBodyPosition;
  const std::unique_ptr<RotationMatrix> m_bodyToImuFrameRotation;
};

} // namespace Iml
