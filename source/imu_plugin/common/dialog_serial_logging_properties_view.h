#pragma once

#include <QDialog>
#include <QSerialPortInfo>

#include <vector>

namespace Ui
{
class DialogSerialLoggingPropertiesView;
}

struct SerialPortSettings;

class QListWidgetItem;
class QWidget;

class DialogSerialLoggingPropertiesView : public QDialog
{
public:
  explicit DialogSerialLoggingPropertiesView(const QString& serialPortName,
                                             const SerialPortSettings& settings,
                                             const std::vector<int>& validBaudRates,
                                             QWidget* parent = nullptr);
  ~DialogSerialLoggingPropertiesView();

  QSerialPortInfo selection() const;

  SerialPortSettings getSerialPortSettings();

  inline bool arePortSettingsValid() const { return m_arePortSettingsValid; };

private slots:
  void refreshList();
  void validatePortSettings();
  void on_listWidget_itemSelectionChanged();
  void on_listWidget_itemDoubleClicked(QListWidgetItem* item);

private:
  void initWidgets(const std::vector<int>& validBaudRates);
  void selectSerialPort(const QString& serialPortName);

  std::unique_ptr<Ui::DialogSerialLoggingPropertiesView> ui;
  std::vector<QSerialPortInfo> m_ports;
  bool m_arePortSettingsValid {false};
};
