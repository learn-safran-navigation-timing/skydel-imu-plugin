#include "imu_view.h"

#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>

#include "gravity.h"
#include "imu_data_format.h"
#include "ui_imu_view.h"

ImuView::ImuView(const ImuConfiguration& configuration, QWidget* parent) : QWidget(parent), m_ui(new Ui::ImuView)
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

  setConfiguration(configuration);

  connect(m_ui->checkBoxFileLogging, &QCheckBox::clicked, this, &ImuView::checkBoxFileLoggingClicked);
  connect(m_ui->comboBoxFileLogging,
          QOverload<int>::of(&QComboBox::activated),
          this,
          &ImuView::comboBoxFileLoggingActivated);
  connect(m_ui->checkBoxNetworkLogging, &QCheckBox::clicked, this, &ImuView::checkBoxNetworkLoggingClicked);
  connect(m_ui->comboBoxNetworkLogging,
          QOverload<int>::of(&QComboBox::activated),
          this,
          &ImuView::comboBoxNetworkLoggingActivated);
  connect(m_ui->lineEditNetworkLogging, &QLineEdit::textChanged, this, &ImuView::lineEditNetworkLoggingTextChanged);
  connect(m_ui->spinBoxNetworkLogging,
          QOverload<int>::of(&QSpinBox::valueChanged),
          this,
          &ImuView::spinBoxNetworkLoggingValueChanged);
  connect(m_ui->comboBoxGravityModel,
          QOverload<int>::of(&QComboBox::activated),
          this,
          &ImuView::comboBoxGravityModelActivated);
}

ImuView::~ImuView()
{
  delete m_ui;
}

void ImuView::setConfiguration(const ImuConfiguration& configuration)
{
  {
    QSignalBlocker blocker {m_ui->checkBoxFileLogging};
    m_ui->checkBoxFileLogging->setChecked(configuration.getValue("fileLoggingEnabled").toBool());
  }
  {
    QSignalBlocker blocker {m_ui->comboBoxFileLogging};
    m_ui->comboBoxFileLogging->setCurrentIndex(configuration.getValue("fileLoggingFormat").toInt());
  }
  {
    QSignalBlocker blocker {m_ui->checkBoxNetworkLogging};
    m_ui->checkBoxNetworkLogging->setChecked(configuration.getValue("networkLoggingEnabled").toBool());
  }
  {
    QSignalBlocker blocker {m_ui->comboBoxNetworkLogging};
    m_ui->comboBoxNetworkLogging->setCurrentIndex(configuration.getValue("networkLoggingFormat").toInt());
  }
  {
    QSignalBlocker blocker {m_ui->lineEditNetworkLogging};
    m_ui->lineEditNetworkLogging->setText(configuration.getValue("networkLoggingAddress").toString());
  }
  {
    QSignalBlocker blocker {m_ui->spinBoxNetworkLogging};
    m_ui->spinBoxNetworkLogging->setValue(configuration.getValue("networkLoggingPort").toInt());
  }
  {
    QSignalBlocker blocker {m_ui->comboBoxGravityModel};
    m_ui->comboBoxGravityModel->setCurrentIndex(configuration.getValue("gravityModel").toInt());
  }
}
