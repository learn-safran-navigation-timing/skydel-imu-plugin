#pragma once

#include <QJsonObject>

#include "imu_serial_port_settings.h"
#include "matrix.h"
#include "skydel_plug_ins/skydel_notifier_interface.h"
#include "triplet.h"

class ImuConfiguration final
{
public:
  ImuConfiguration();
  ~ImuConfiguration() = default;

  inline void clear() { m_configuration = QJsonObject(); }
  inline void setNotifier(SkydelNotifierInterface* notifier) { m_notifier = notifier; }

  inline QJsonObject toJson() const { return m_configuration; }
  void update(const QJsonObject& newConfiguration);

  QJsonValue getValue(const QString& key) const;
  void addValue(const QString& key, const QJsonValue& value);
  void setValue(const QString& key, const QJsonValue& value);

  Iml::Triplet getTriplet(const QString& key) const;
  void addTriplet(const QString& key, const Iml::Triplet& triplet);
  void setTriplet(const QString& key, const Iml::Triplet& triplet);

  Iml::Matrix3x3 getMatrix3x3(const QString& key) const;
  void addMatrix3x3(const QString& key, const Iml::Matrix3x3& matrix);
  void setMatrix3x3(const QString& key, const Iml::Matrix3x3& matrix);

  SerialPortSettings getSerialPortSettings() const;
  void addSerialPortSettings(const SerialPortSettings& serialPortSettings);
  void setSerialPortSettings(const SerialPortSettings& serialPortSettings);

private:
  SkydelNotifierInterface* m_notifier;
  QJsonObject m_configuration;
};
