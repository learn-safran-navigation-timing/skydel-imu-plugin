#pragma once

#include "triplet.h"

namespace Iml
{
/**
 * @brief Convert triplet from geocentric Earth-centered Earth-fixed (ECEF) to geodetic (LLA).
 *
 * @param ecef ECEF triplet in meter.
 * @return LLA triplet in radian, radian and meter.
 */
Triplet ecefToLla(const Triplet& ecef);

/**
 * @brief Convert triplet from geocentric Earth-centered Earth-fixed (ECEF) to local north-east-down (NED).
 *
 * @param llaOrigin Geodetic coordinate (LLA) of local NED system origin in radian, radian and meter.
 * @param ecef ECEF triplet in meter.
 * @return NED triplet in meter.
 *
 * @attention This function only rotates vectors. It does not convert an ECEF position to an LLA-NED coordinates pair.
 */
Triplet ecefToNed(const Triplet& llaOrigin, const Triplet& ecef);

/**
 * @brief Convert triplet from local north-east-down (NED) to body via x-y-z axis rotation.
 *
 * @param llaOrigin Geodetic coordinate (LLA) of local NED system origin in
 * radian, radian and meter.
 * @param ned NED triplet in meter.
 * @return ECEF triplet in meter.
 *
 * @attention This function only rotates vectors. It does not convert an LLA-NED coordinates pair to an ECEF position.
 */
Triplet nedToEcef(const Triplet& llaOrigin, const Triplet& ned);

/**
 * @brief Convert triplet from local north-east-down (NED) to body via x-y-z
 * axis rotation.
 *
 * @param attitude Attitude triplet in radian.
 * @param ned      NED triplet.
 * @return Body triplet.
 */
Triplet nedToBodyFromXYZRotation(const Triplet& attitude, const Triplet& ned);

/**
 * @brief Convert triplet from body to local north-east-down (NED) via z-y-x
 * axis rotation.
 *
 * @param attitude Attitude triplet in radian.
 * @param ned      NED triplet.
 * @return Body triplet.
 */
Triplet bodyToNedFromZYXRotation(const Triplet& attitude, const Triplet& body);

/**
 * @brief Convert angular velocity from local north-east-down (NED) to body.
 *
 * @param attitude Attitude triplet in radian.
 * @param ned      NED triplet.
 * @return Body triplet.
 */
Triplet nedToBody(const Triplet& attitude, const Triplet& ned);

} // namespace Iml
