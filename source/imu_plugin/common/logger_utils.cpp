#include "logger_utils.h"

#include <QJsonDocument>
#include <QTextStream>

#include "imu_encoder.h"

namespace
{

const QString getFormattedDataInCsvFormat(const Iml::ImuData& data)
{
  QString formattedData;
  QTextStream stream(&formattedData);
  stream.setRealNumberPrecision(15);
  stream << data.time << ',' << Iml::x(data.acceleration) << ',' << Iml::y(data.acceleration) << ','
         << Iml::z(data.acceleration) << ',' << Iml::roll(data.angularVelocity) << ','
         << Iml::pitch(data.angularVelocity) << ',' << Iml::yaw(data.angularVelocity) << '\n';

  return formattedData;
}

const QByteArray getFormattedDataInJsonFormat(const Iml::ImuData& data, bool compact)
{
  QJsonObject formattedData;
  formattedData.insert(ELASPED_JSON_KEY, static_cast<double>(data.time));
  formattedData.insert(ACCELERATION_JSON_KEY,
                       QJsonArray {Iml::x(data.acceleration), Iml::y(data.acceleration), Iml::z(data.acceleration)});
  formattedData.insert(ANGULAR_VELOCITY_JSON_KEY,
                       QJsonArray {Iml::roll(data.angularVelocity),
                                   Iml::pitch(data.angularVelocity),
                                   Iml::yaw(data.angularVelocity)});

  QJsonDocument jsonDocument(formattedData);

  return jsonDocument.toJson(compact ? QJsonDocument::JsonFormat::Compact : QJsonDocument::JsonFormat::Indented);
}

} // namespace

const QString getFormattedData(const Iml::ImuData& data, ImuDataFormat dataFormat)
{
  switch (dataFormat)
  {
    case ImuDataFormat::CSV:
      return getFormattedDataInCsvFormat(data);
    case ImuDataFormat::JSON:
      return getFormattedDataInJsonFormat(data, false);
    case ImuDataFormat::STIM_IMU:
      return QString(encodeImuDatagram(data));
  }

  return "INVALID";
}

const QByteArray getFormattedDataByteArray(const Iml::ImuData& data, ImuDataFormat dataFormat)
{
  switch (dataFormat)
  {
    case ImuDataFormat::CSV:
      return getFormattedDataInCsvFormat(data).toUtf8();
    case ImuDataFormat::JSON:
      return getFormattedDataInJsonFormat(data, true);
    case ImuDataFormat::STIM_IMU:
      return encodeImuDatagram(data);
  }

  return {};
}
