#pragma once

#include <stdexcept>

enum ImuDataFormat
{
  CSV,
  JSON
};

const ImuDataFormat ALL_IMU_DATA_FORMAT[] = {ImuDataFormat::CSV, ImuDataFormat::JSON};

inline const char* toString(ImuDataFormat format)
{
  switch (format)
  {
    case ImuDataFormat::CSV:
      return "CSV";
    case ImuDataFormat::JSON:
      return "JSON";
  }
  throw std::runtime_error {"Unknown ImuDataFormat"};
}
