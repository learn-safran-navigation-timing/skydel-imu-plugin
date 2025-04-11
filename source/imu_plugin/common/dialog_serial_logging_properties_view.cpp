#include "dialog_serial_logging_properties_view.h"

#include <QMessageBox>
#include <QPushButton>

#include "imu_serial_port_settings.h"
#include "imu_serial_port_utils.h"
#include "ui_dialog_serial_logging_properties_view.h"

namespace
{
constexpr auto NOT_AVAILABLE_MESSAGE = "Info Not Available";
}

DialogSerialLoggingPropertiesView::DialogSerialLoggingPropertiesView(const QString& serialPortName,
                                                                     const SerialPortSettings& settings,
                                                                     QWidget* parent) :
  QDialog(parent),
  ui(std::make_unique<Ui::DialogSerialLoggingPropertiesView>())
{
  ui->setupUi(this);

  initWidgets();

  ui->baudRate->setCurrentText(QString::number(settings.baudRate));
  ui->dataBits->setValue(settings.dataBits);
  if (settings.stopBits > 0 && settings.stopBits <= ui->stopBitsGroup->buttons().size())
  {
    ui->stopBitsGroup->buttons().at(settings.stopBits - 1)->click();
  }
  ui->parity->setCurrentText(SerialPortUtils::parityString(settings.parity));
  ui->flowControl->setCurrentText(SerialPortUtils::flowControlString(settings.flowControl));

  setWindowTitle("IMU Serial Port Configuration");

  auto* refresh = new QPushButton("Refresh List");
  connect(refresh, &QPushButton::clicked, this, &DialogSerialLoggingPropertiesView::refreshList);
  ui->buttonBox->addButton(refresh, QDialogButtonBox::ActionRole);

  connect(ui->buttonBox->button(QDialogButtonBox::Ok),
          &QPushButton::clicked,
          this,
          &DialogSerialLoggingPropertiesView::validatePortSettings);

  refreshList();
  selectSerialPort(serialPortName);
}

DialogSerialLoggingPropertiesView::~DialogSerialLoggingPropertiesView() = default;

void DialogSerialLoggingPropertiesView::initWidgets()
{
  ui->baudRate->clear();
  for (auto bd : SerialPortUtils::ValidBaudRates)
  {
    ui->baudRate->addItem(QString::number(bd));
  }

  ui->dataBits->clear();
  ui->dataBits->setMinimum(SerialPortUtils::MIN_DATA_BIT);
  ui->dataBits->setMaximum(SerialPortUtils::MAX_DATA_BIT);

  ui->parity->clear();
  for (auto pr : SerialPortUtils::ParityValues)
  {
    ui->parity->addItem(SerialPortUtils::parityString(pr));
  }

  ui->flowControl->clear();
  for (auto fc : SerialPortUtils::FlowControlValues)
  {
    ui->flowControl->addItem(SerialPortUtils::flowControlString(fc));
  }

  ui->baudRate->setCurrentText(QString::number(SerialPortUtils::DEFAULT_BAUD_RATE));
  ui->dataBits->setValue(SerialPortUtils::DEFAULT_DATA_BIT);
  ui->parity->setCurrentText(SerialPortUtils::parityString(SerialPortUtils::DEFAULT_PARITY));
  auto* stopBitRadioBtn = SerialPortUtils::DEFAULT_STOP_BIT == 1 ? ui->radioButtonStopBits1 : ui->radioButtonStopBits2;
  stopBitRadioBtn->setChecked(true);
  ui->flowControl->setCurrentText(SerialPortUtils::flowControlString(SerialPortUtils::DEFAULT_FLOW_CONTROL));
}

QSerialPortInfo DialogSerialLoggingPropertiesView::selection() const
{
  return (ui->listWidget->currentRow() >= 0) ? m_ports.at(ui->listWidget->currentRow()) : QSerialPortInfo {};
}

SerialPortSettings DialogSerialLoggingPropertiesView::getSerialPortSettings()
{
  return {.baudRate = ui->baudRate->currentText().toInt(),
          .dataBits = ui->dataBits->value(),
          .parity = SerialPortUtils::parseParityString(ui->parity->currentText()),
          .stopBits = ui->stopBitsGroup->buttons().indexOf(ui->stopBitsGroup->checkedButton()) +
                      1, // Left radio button = 1, right = 2
          .flowControl = SerialPortUtils::parseFlowControlString(ui->flowControl->currentText()),
          .portName = selection().portName()};
}

void DialogSerialLoggingPropertiesView::refreshList()
{
  auto lastSelectedPort = selection();
  ui->label_desciption->clear();
  ui->label_manufacturer->clear();
  ui->label_product_id->clear();
  ui->label_vendor_id->clear();
  ui->listWidget->clear();

  auto listSerialPortInfo = QSerialPortInfo::availablePorts();

  m_ports = std::vector<QSerialPortInfo>(listSerialPortInfo.begin(), listSerialPortInfo.end());

  ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!m_ports.empty());

  for (const auto& port : m_ports)
  {
    ui->listWidget->addItem(port.portName());
    if (port.portName() == lastSelectedPort.portName())
    {
      ui->listWidget->setCurrentRow(ui->listWidget->count() - 1);
    }
  }
}

void DialogSerialLoggingPropertiesView::validatePortSettings()
{
  const auto serialPortSettings = getSerialPortSettings();
  QSerialPort serialPort;

  serialPort.setPortName(serialPortSettings.portName);
  serialPort.setBaudRate(serialPortSettings.baudRate);
  serialPort.setFlowControl(serialPortSettings.flowControl);
  serialPort.setDataBits(static_cast<QSerialPort::DataBits>(serialPortSettings.dataBits));
  serialPort.setStopBits(static_cast<QSerialPort::StopBits>(serialPortSettings.stopBits));
  serialPort.setParity(serialPortSettings.parity);

  if (!serialPort.open(QSerialPort::WriteOnly))
  {
    QMessageBox::critical(this, "IMU Serial Port Connection Error", serialPort.errorString());
    ui->listWidget->setCurrentRow(-1);
    m_arePortSettingsValid = false;
  }
  else
  {
    m_arePortSettingsValid = true;
  }

  serialPort.close();
}

void DialogSerialLoggingPropertiesView::on_listWidget_itemSelectionChanged()
{
  QSerialPortInfo info = m_ports.at(ui->listWidget->currentRow());
  ui->label_desciption->setText(info.description());
  ui->label_manufacturer->setText(info.manufacturer());
  if (info.hasProductIdentifier())
    ui->label_product_id->setText(QString::number(info.productIdentifier()));
  else
    ui->label_product_id->setText(NOT_AVAILABLE_MESSAGE);
  if (info.hasVendorIdentifier())
    ui->label_vendor_id->setText(QString::number(info.vendorIdentifier()));
  else
    ui->label_vendor_id->setText(NOT_AVAILABLE_MESSAGE);
  ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!info.isBusy());
}

void DialogSerialLoggingPropertiesView::on_listWidget_itemDoubleClicked(QListWidgetItem*)
{
  if (!m_ports.at(ui->listWidget->currentRow()).isBusy())
    accept();
}

void DialogSerialLoggingPropertiesView::selectSerialPort(const QString& serialPortName)
{
  int index = 0;
  for (const auto& port : m_ports)
  {
    if (port.portName() == serialPortName)
    {
      ui->listWidget->setCurrentRow(index);
      break;
    }
    ++index;
  }
}
