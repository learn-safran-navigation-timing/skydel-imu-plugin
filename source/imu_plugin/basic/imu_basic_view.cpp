#include "imu_basic_view.h"

#include "gravity.h"
#include "imu_configuration_keys.h"
#include "imu_data_format.h"
#include "ui_imu_basic_view.h"

ImuBasicView::ImuBasicView(const ImuConfiguration& configuration, QWidget* parent) :
  QWidget(parent),
  m_ui(new Ui::ImuBasicView())
{
  m_ui->setupUi(this);

  for (auto imuDataFormat : ALL_IMU_DATA_FORMAT)
  {
    m_ui->comboBoxFileLogging->addItem(toString(imuDataFormat));
    m_ui->comboBoxNetworkLogging->addItem(toString(imuDataFormat));
  }

  for (auto gravityModel : Iml::ALL_GRAVITY_MODEL)
  {
    m_ui->comboBoxGravityModel->addItem(toString(gravityModel));
  }

  m_ui->spinBoxNetworkLogging->setMaximum(65535);

  onConfigurationUpdate(configuration);

  connect(m_ui->checkBoxFileLogging, &QCheckBox::clicked, this, &ImuBasicView::checkBoxFileLoggingClicked);
  connect(m_ui->comboBoxFileLogging,
          QOverload<int>::of(&QComboBox::activated),
          this,
          &ImuBasicView::comboBoxFileLoggingActivated);
  connect(m_ui->checkBoxNetworkLogging, &QCheckBox::clicked, this, &ImuBasicView::checkBoxNetworkLoggingClicked);
  connect(m_ui->comboBoxNetworkLogging,
          QOverload<int>::of(&QComboBox::activated),
          this,
          &ImuBasicView::comboBoxNetworkLoggingActivated);
  connect(m_ui->lineEditNetworkLogging,
          &QLineEdit::textChanged,
          this,
          &ImuBasicView::lineEditNetworkLoggingTextChanged);
  connect(m_ui->spinBoxNetworkLogging,
          QOverload<int>::of(&QSpinBox::valueChanged),
          this,
          &ImuBasicView::spinBoxNetworkLoggingValueChanged);
  connect(m_ui->comboBoxGravityModel,
          QOverload<int>::of(&QComboBox::activated),
          this,
          &ImuBasicView::comboBoxGravityModelActivated);
}

ImuBasicView::~ImuBasicView()
{
  delete m_ui;
}

void ImuBasicView::onConfigurationUpdate(const ImuConfiguration& configuration)
{
  {
    QSignalBlocker blocker {m_ui->checkBoxFileLogging};
    m_ui->checkBoxFileLogging->setChecked(configuration.getValue(ImuConfigurationKeys::FILE_LOGGING_ENABLED).toBool());
  }
  {
    QSignalBlocker blocker {m_ui->comboBoxFileLogging};
    m_ui->comboBoxFileLogging->setCurrentIndex(
      configuration.getValue(ImuConfigurationKeys::FILE_LOGGING_FORMAT).toInt());
  }
  {
    QSignalBlocker blocker {m_ui->checkBoxNetworkLogging};
    m_ui->checkBoxNetworkLogging->setChecked(
      configuration.getValue(ImuConfigurationKeys::NETWORK_LOGGING_ENABLED).toBool());
  }
  {
    QSignalBlocker blocker {m_ui->comboBoxNetworkLogging};
    m_ui->comboBoxNetworkLogging->setCurrentIndex(
      configuration.getValue(ImuConfigurationKeys::NETWORK_LOGGING_FORMAT).toInt());
  }
  {
    QSignalBlocker blocker {m_ui->lineEditNetworkLogging};
    m_ui->lineEditNetworkLogging->setText(
      configuration.getValue(ImuConfigurationKeys::NETWORK_LOGGING_ADDRESS).toString());
  }
  {
    QSignalBlocker blocker {m_ui->spinBoxNetworkLogging};
    m_ui->spinBoxNetworkLogging->setValue(configuration.getValue(ImuConfigurationKeys::NETWORK_LOGGING_PORT).toInt());
  }
  {
    QSignalBlocker blocker {m_ui->comboBoxGravityModel};
    m_ui->comboBoxGravityModel->setCurrentIndex(configuration.getValue(ImuConfigurationKeys::GRAVITY_MODEL).toInt());
  }
}

void ImuBasicView::simulationStarted()
{
  m_ui->checkBoxFileLogging->setEnabled(false);
  m_ui->comboBoxFileLogging->setEnabled(false);

  m_ui->checkBoxNetworkLogging->setEnabled(false);
  m_ui->lineEditNetworkLogging->setEnabled(false);
  m_ui->comboBoxNetworkLogging->setEnabled(false);
  m_ui->spinBoxNetworkLogging->setEnabled(false);

  m_ui->comboBoxGravityModel->setEnabled(false);
}

void ImuBasicView::simulationStopped()
{
  m_ui->checkBoxFileLogging->setEnabled(true);
  m_ui->comboBoxFileLogging->setEnabled(true);

  m_ui->checkBoxNetworkLogging->setEnabled(true);
  m_ui->lineEditNetworkLogging->setEnabled(true);
  m_ui->comboBoxNetworkLogging->setEnabled(true);
  m_ui->spinBoxNetworkLogging->setEnabled(true);

  m_ui->comboBoxGravityModel->setEnabled(true);
}
