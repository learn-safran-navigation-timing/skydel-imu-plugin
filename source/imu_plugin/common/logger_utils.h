#pragma once

#include <QByteArray>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

#include "imu_data.h"
#include "imu_data_format.h"

static const QString ELASPED_JSON_KEY = "elapsed";
static const QString ACCELERATION_JSON_KEY = "acceleration";
static const QString ANGULAR_VELOCITY_JSON_KEY = "angularvelocity";

static const QString ELAPSED_TIME = "Elapsed Time (ms)";
static const QJsonArray ACCELERATION = {"Acceleration X (m/s^2)", "Acceleration Y (m/s^2)", "Acceleration Z (m/s^2)"};
static const QJsonArray ANGULAR_VELOCITY = {"Angular Velocity Roll (rad/s)",
                                            "Angular Velocity Pitch (rad/s)",
                                            "Angular Velocity Yaw (rad/s)"};

static const QString csvFileExtension = ".csv";
static const QString jsonFileExtension = ".json";
static const QString stimImuFileExtension = ".bin";

static const QString csvHeader = ELAPSED_TIME + ',' + ACCELERATION.at(0).toString() + ',' +
                                 ACCELERATION.at(1).toString() + ',' + ACCELERATION.at(2).toString() + ',' +
                                 ANGULAR_VELOCITY.at(0).toString() + ',' + ANGULAR_VELOCITY.at(1).toString() + ',' +
                                 ANGULAR_VELOCITY.at(2).toString() + '\n';

static const QString jsonHeader = QJsonDocument(QJsonObject({{ELASPED_JSON_KEY, ELAPSED_TIME},
                                                             {ACCELERATION_JSON_KEY, ACCELERATION},
                                                             {ANGULAR_VELOCITY_JSON_KEY, ANGULAR_VELOCITY}}))
                                    .toJson();

static const QString stimImuHeader = "";

const QString getFormattedData(const Iml::ImuData& data, ImuDataFormat dataFormat);
const QByteArray getFormattedDataByteArray(const Iml::ImuData& data, ImuDataFormat dataFormat);
