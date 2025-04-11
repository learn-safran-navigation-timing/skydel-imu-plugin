#pragma once

#include <QWidget>

#include "imu_configuration.h"

namespace Ui
{
class ImuBasicView;
}

class ImuBasicView final : public QWidget
{
  Q_OBJECT

public:
  ImuBasicView(const ImuConfiguration& configuration, QWidget* parent = nullptr);
  ~ImuBasicView();

  void onConfigurationUpdate(const ImuConfiguration& configuration);

public slots:
  void simulationStarted();
  void simulationStopped();

signals:
  void checkBoxFileLoggingClicked(bool checked);
  void comboBoxFileLoggingActivated(int index);
  void checkBoxNetworkLoggingClicked(bool checked);
  void comboBoxNetworkLoggingActivated(int index);
  void lineEditNetworkLoggingTextChanged(const QString& text);
  void spinBoxNetworkLoggingValueChanged(int i);
  void comboBoxGravityModelActivated(int index);

private:
  Ui::ImuBasicView* m_ui;
};
