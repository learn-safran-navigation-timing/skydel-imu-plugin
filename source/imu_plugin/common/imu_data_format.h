#pragma once

#include <stdexcept>

enum ImuDataFormat
{
  CSV,
  JSON,
  STIM_IMU
};

const ImuDataFormat ALL_IMU_DATA_FORMAT[] = {ImuDataFormat::CSV, ImuDataFormat::JSON, ImuDataFormat::STIM_IMU};

inline const char* toString(ImuDataFormat format)
{
  switch (format)
  {
    case ImuDataFormat::CSV:
      return "CSV";
    case ImuDataFormat::JSON:
      return "JSON";
    case ImuDataFormat::STIM_IMU:
      return "STIM IMU";
  }
  throw std::runtime_error {"Unknown ImuDataFormat"};
}
