#include "dialog_serial_logging_help.h"

#include "ui_dialog_serial_logging_help.h"

DialogSerialLoggingHelp::DialogSerialLoggingHelp(QWidget* parent) :
  QDialog(parent),
  ui(std::make_unique<Ui::DialogSerialLoggingHelp>())
{
  ui->setupUi(this);
}

DialogSerialLoggingHelp::~DialogSerialLoggingHelp() = default;
