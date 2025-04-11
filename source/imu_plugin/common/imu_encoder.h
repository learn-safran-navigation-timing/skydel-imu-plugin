#pragma once

#include "imu_data.h"

class QByteArray;

constexpr auto NB_BITS_PER_BYTE = 8;
constexpr auto SIZE_DATAGRAM_BYTES = 28;

constexpr auto DEFAULT_SEED = 0xFFFFFFFF;

constexpr auto GRAVITY = 9.80665;

constexpr auto GYRO_LSB_VALUE = 0.22 / 3600.0;       // deg/s
constexpr auto ACCELEROMETER_10G_LSB_VALUE = 1.9E-6; // g

QByteArray encodeImuDatagram(const Iml::ImuData& data);
