#pragma once

#include "imu_plugin.h"
#include "skydel_plug_ins/skydel_plugin.h"

class ImuBasicPlugin final : public ImuPlugin
{
  Q_OBJECT

public:
  ImuBasicPlugin();
  ~ImuBasicPlugin() = default;

  SkydelWidgets createUI() override;

private:
  inline void onImuDataUpdate(Iml::ImuData&) override {}
};

REGISTER_SKYDEL_PLUGIN(ImuBasicPlugin)
