#pragma once

#include "body_dynamic.h"
#include "gravity.h"

namespace Iml
{

class BodyInertialDynamic : public BodyDynamic
{
public:
  BodyInertialDynamic(GravityModel gravityModel);
  InertialData getPosition() const override;

private:
  GravityModel m_gravityModel;
};

} // namespace Iml
