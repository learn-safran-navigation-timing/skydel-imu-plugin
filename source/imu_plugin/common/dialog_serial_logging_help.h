#pragma once

#include <QDialog>

namespace Ui
{
class DialogSerialLoggingHelp;
}

class DialogSerialLoggingHelp : public QDialog
{
  Q_OBJECT

public:
  explicit DialogSerialLoggingHelp(QWidget* parent = nullptr);
  ~DialogSerialLoggingHelp();

private:
  std::unique_ptr<Ui::DialogSerialLoggingHelp> ui;
};
