#pragma once

#include <cstdint>
#include <vector>

#include "gravity.h"
#include "triplet.h"

namespace Iml
{
struct RawPositionData
{
  int64_t time;          // millisecond
  double position[3];    // meter
  double orientation[3]; // radian
};

struct InertialData
{
  int64_t time;                // millisecond
  Triplet position;            // meter
  Triplet velocity;            // meter/second
  Triplet acceleration;        // meter/second^2
  Triplet jerk;                // meter/second^3
  Triplet attitude;            // radian
  Triplet angularVelocity;     // radian/second
  Triplet angularAcceleration; // radian/second^2
  Triplet angularJerk;         // radian/second^3
};

class BodyDynamic
{
public:
  virtual ~BodyDynamic() = default;
  virtual inline InertialData getPosition() const { return m_datas[1]; }
  bool isAccelerationReady() const;
  bool isJerkReady() const;
  void pushPosition(const RawPositionData& rawPosition);

private:
  Triplet m_phaseCorrection;
  std::vector<InertialData> m_datas;
};

class BodyInertialDynamic : public BodyDynamic
{
public:
  BodyInertialDynamic(GravityModel gravityModel);
  InertialData getPosition() const override;

private:
  GravityModel m_gravityModel;
};

} // namespace Iml
