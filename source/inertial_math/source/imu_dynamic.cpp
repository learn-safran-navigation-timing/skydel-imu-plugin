#include "imu_dynamic.h"

#include "sensor.h"
#include "transformation.h"

namespace
{
constexpr Iml::Triplet ZERO_TRIPLET {0, 0, 0};
}

namespace Iml
{

ImuDynamic::ImuDynamic(GravityModel gravityModel, const Triplet& leverArm, const Triplet& orientation) :
  m_bodyDynamic(),
  m_imuData(),
  m_gravityModel(gravityModel),
  m_imuBodyPosition((leverArm != ZERO_TRIPLET) ? std::make_unique<Triplet>(leverArm) : nullptr),
  m_bodyToImuFrameRotation((orientation != ZERO_TRIPLET) ? std::make_unique<RotationMatrix>(
                                                             blaze::trans(rotationMatrixForXYZRotation(-orientation)))
                                                         : nullptr)
{
}

void ImuDynamic::pushPosition(int64_t time, const Triplet& position, const Triplet& attitude)
{
  m_bodyDynamic.pushPosition(time, position, attitude);

  if (isReady())
  {
    const auto inertialData = m_bodyDynamic.getPosition();
    auto imuEcefPosition = inertialData.position;

    if (m_imuBodyPosition)
    {
      const auto vehicleLlaPosition = ecefToLla(inertialData.position);
      const auto imuNedPosition = bodyToNedFromZYXRotation(inertialData.attitude, *m_imuBodyPosition);
      imuEcefPosition += nedToEcef(vehicleLlaPosition, imuNedPosition);
    }

    const auto imuLlaPosition = ecefToLla(imuEcefPosition);
    auto measuredAcceleration = idealAccelerometer(imuLlaPosition,
                                                   inertialData.velocity,
                                                   inertialData.acceleration,
                                                   inertialData.attitude,
                                                   m_gravityModel);
    auto measuredAngularVelocity = idealGyroscope(imuLlaPosition,
                                                  inertialData.velocity,
                                                  inertialData.attitude,
                                                  inertialData.angularVelocity);

    if (m_imuBodyPosition)
    {
      const auto bodyAngularVelocity = nedToBody(inertialData.attitude, inertialData.angularVelocity);
      const auto bodyAngularAcceleration = nedToBody(inertialData.attitude, inertialData.angularAcceleration);
      const auto bodyCentrifugalAcceleration = blaze::cross(bodyAngularVelocity,
                                                            blaze::cross(bodyAngularVelocity, *m_imuBodyPosition));
      const auto bodyEulerAcceleration = blaze::cross(bodyAngularAcceleration, *m_imuBodyPosition);
      measuredAcceleration -= (bodyCentrifugalAcceleration + bodyEulerAcceleration);
    }

    m_imuData.time = inertialData.time;
    m_imuData.acceleration = measuredAcceleration;
    m_imuData.angularVelocity = measuredAngularVelocity;

    if (m_bodyToImuFrameRotation)
    {
      m_imuData.acceleration = rotate(*m_bodyToImuFrameRotation, m_imuData.acceleration);
      m_imuData.angularVelocity = rotate(*m_bodyToImuFrameRotation, m_imuData.angularVelocity);
    }
  }
}

} // namespace Iml
