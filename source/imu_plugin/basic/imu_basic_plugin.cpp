#include "imu_basic_plugin.h"

#include "imu_basic_view.h"
#include "imu_configuration_keys.h"

namespace
{
const QString PLUGIN_NAME = "Basic IMU";
}

ImuBasicPlugin::ImuBasicPlugin() : ImuPlugin()
{
  setDefaultConfiguration();
}

SkydelWidgets ImuBasicPlugin::createUI()
{
  auto* view = new ImuBasicView(m_configuration);

  connect(view, &ImuBasicView::checkBoxFileLoggingClicked, [this](bool checked) {
    m_configuration.setValue(ImuConfigurationKeys::FILE_LOGGING_ENABLED, checked);
  });

  connect(view, &ImuBasicView::comboBoxFileLoggingActivated, [this](int index) {
    m_configuration.setValue(ImuConfigurationKeys::FILE_LOGGING_FORMAT, index);
  });

  connect(view, &ImuBasicView::checkBoxNetworkLoggingClicked, [this](bool checked) {
    m_configuration.setValue(ImuConfigurationKeys::NETWORK_LOGGING_ENABLED, checked);
  });

  connect(view, &ImuBasicView::comboBoxNetworkLoggingActivated, [this](int index) {
    m_configuration.setValue(ImuConfigurationKeys::NETWORK_LOGGING_FORMAT, index);
  });

  connect(view, &ImuBasicView::lineEditNetworkLoggingTextChanged, [this](const QString& text) {
    m_configuration.setValue(ImuConfigurationKeys::NETWORK_LOGGING_ADDRESS, text);
  });

  connect(view, &ImuBasicView::spinBoxNetworkLoggingValueChanged, [this](int i) {
    m_configuration.setValue(ImuConfigurationKeys::NETWORK_LOGGING_PORT, i);
  });

  connect(view, &ImuBasicView::comboBoxGravityModelActivated, [this](int index) {
    m_configuration.setValue(ImuConfigurationKeys::GRAVITY_MODEL, index);
  });

  connect(this, &ImuPlugin::simulationStarted, view, &ImuBasicView::simulationStarted);
  connect(this, &ImuPlugin::simulationStopped, view, &ImuBasicView::simulationStopped);

  connect(this, &ImuPlugin::configurationChanged, [this, view]() { view->onConfigurationUpdate(m_configuration); });

  return {{.widget = view, .path = PLUGIN_MENU_PATH, .name = PLUGIN_NAME}};
}
