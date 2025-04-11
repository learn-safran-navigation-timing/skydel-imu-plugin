#include "body_dynamic.h"

#include "angle_unwrap.h"

namespace
{
constexpr size_t MIN_BUFFER_SIZE_FOR_ACCELERATION = 4;
constexpr size_t MIN_BUFFER_SIZE_FOR_JERK = 5;
} // namespace

namespace Iml
{
bool BodyDynamic::isAccelerationReady() const
{
  return m_datas.size() >= MIN_BUFFER_SIZE_FOR_ACCELERATION;
}

bool BodyDynamic::isJerkReady() const
{
  return m_datas.size() >= MIN_BUFFER_SIZE_FOR_JERK;
}

void BodyDynamic::pushPosition(int64_t time, const Triplet& position, const Triplet& attitude)
{
  InertialData data = {
    time,
    position,
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0},
    attitude,
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0},
  };

  if (!m_datas.empty())
  {
    data.attitude = unwrap(m_datas.back().attitude + m_phaseCorrection, data.attitude, m_phaseCorrection);
  }

  if (isJerkReady())
  {
    m_datas.erase(m_datas.begin());
  }

  m_datas.push_back(data);

  if (isAccelerationReady())
  {
    auto dt_1_0 = (m_datas[1].time - m_datas[0].time) / 1000.0;
    auto dt_2_0 = (m_datas[2].time - m_datas[0].time) / 1000.0;
    auto dt_3_1 = (m_datas[3].time - m_datas[1].time) / 1000.0;

    m_datas[1].velocity = (m_datas[2].position - m_datas[0].position) / dt_2_0;
    m_datas[2].velocity = (m_datas[3].position - m_datas[1].position) / dt_3_1;

    m_datas[1].acceleration = (m_datas[2].velocity - m_datas[0].velocity) / dt_2_0;

    m_datas[1].angularVelocity = (m_datas[1].attitude - m_datas[0].attitude) / dt_1_0;

    m_datas[1].angularAcceleration = (m_datas[1].angularVelocity - m_datas[0].angularVelocity) / dt_1_0;

    m_datas[1].angularJerk = (m_datas[1].angularAcceleration - m_datas[0].angularAcceleration) / dt_1_0;

    if (isJerkReady())
    {
      auto dt_4_2 = (m_datas[4].time - m_datas[2].time) / 1000.0;

      m_datas[3].velocity = (m_datas[4].position - m_datas[2].position) / dt_4_2;

      m_datas[2].acceleration = (m_datas[3].velocity - m_datas[1].velocity) / dt_3_1;

      m_datas[1].jerk = (m_datas[2].acceleration - m_datas[0].acceleration) / dt_2_0;
    }
  }
}

} // namespace Iml
