#include "body_inertial_dynamic.h"

#include "sensor.h"
#include "transformation.h"

namespace Iml
{

BodyInertialDynamic::BodyInertialDynamic(GravityModel gravityModel) : m_gravityModel(gravityModel)
{
}

InertialData BodyInertialDynamic::getPosition() const
{
  InertialData position;

  if (isAccelerationReady())
  {
    position = BodyDynamic::getPosition();
    auto llaPosition = ecefToLla(position.position);
    position.acceleration =
      idealAccelerometer(llaPosition, position.velocity, position.acceleration, position.attitude, m_gravityModel);
    position.angularVelocity = idealGyroscope(llaPosition,
                                              position.velocity,
                                              position.attitude,
                                              position.angularVelocity);
  }

  return position;
}

} // namespace Iml
