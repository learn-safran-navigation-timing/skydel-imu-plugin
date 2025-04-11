#include "imu_encoder.h"

#include <QByteArray>

#include <cstdint>
#include <limits>
#include <numbers>

namespace
{
template<typename TypeValue, typename TypeLsb>
void encodeValue(TypeValue value,
                 TypeLsb lsb,
                 size_t numberBytes,
                 uint64_t startingByte,
                 QByteArray& datagram) requires std::is_arithmetic_v<TypeValue>&& std::is_arithmetic_v<TypeLsb>
{
  int64_t valueRounded = round(value / lsb);
  for (uint64_t byte = 0; byte < numberBytes; byte++)
  {
    if (static_cast<int>(byte + startingByte) >= datagram.size())
    {
      return;
    }
    datagram[static_cast<int32_t>(byte + startingByte)] = 0xFF & (valueRounded >>
                                                                  ((numberBytes - (byte + 1)) * NB_BITS_PER_BYTE));
  }
}

void encodeAngularVelocity(const Iml::ImuData& data, QByteArray& datagram)
{
  encodeValue(Iml::roll(data.angularVelocity) * 180 / std::numbers::pi, GYRO_LSB_VALUE, 3, 1, datagram);
  encodeValue(Iml::pitch(data.angularVelocity) * 180 / std::numbers::pi, GYRO_LSB_VALUE, 3, 4, datagram);
  encodeValue(Iml::yaw(data.angularVelocity) * 180 / std::numbers::pi, GYRO_LSB_VALUE, 3, 7, datagram);
}

void encodeAcceleration(const Iml::ImuData& data, QByteArray& datagram)
{
  encodeValue(Iml::x(data.acceleration) / GRAVITY, ACCELEROMETER_10G_LSB_VALUE, 3, 11, datagram);
  encodeValue(Iml::y(data.acceleration) / GRAVITY, ACCELEROMETER_10G_LSB_VALUE, 3, 14, datagram);
  encodeValue(Iml::z(data.acceleration) / GRAVITY, ACCELEROMETER_10G_LSB_VALUE, 3, 17, datagram);
}

} // namespace

namespace
{

unsigned char reverse8Bits(unsigned char b)
{
  b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
  b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
  b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
  return b;
}

uint32_t reverse4Bytes(uint32_t x)
{
  return reverse8Bits((x >> 24) & 0xFF) << 0L | reverse8Bits((x >> 16) & 0xFF) << 8L |
         reverse8Bits((x >> 8) & 0xFF) << 16L | reverse8Bits((x >> 0) & 0xFF) << 24L;
}

consteval auto generateTable(uint32_t polynomial = 0x04C11DB7)
{
  std::array<uint32_t, 256> table {};
  for (uint32_t byte = 0; byte < table.size(); byte++)
  {
    uint32_t c = byte << 24;
    for (size_t bit = 0; bit < NB_BITS_PER_BYTE; bit++)
    {
      if (c & 0x80000000)
      {
        c = (c << 1) ^ polynomial;
      }
      else
      {
        c <<= 1;
      }
    }
    table[byte] = c;
  }
  return table;
}

constexpr auto LOOKUP_TABLE = generateTable();

uint32_t crc32(const QByteArray& buffer, uint32_t length, uint32_t seed = DEFAULT_SEED)
{
  uint32_t c = seed;

  if (buffer.size() < static_cast<int>(length))
  {
    length = buffer.size();
  }

  for (size_t i = 0; i < length; ++i)
  {
    c = LOOKUP_TABLE[((c >> 24) ^ reverse8Bits(buffer.at(static_cast<int>(i)))) & 0xFF] ^ (c << 8);
  }
  return reverse4Bytes(~c);
}

} // namespace

QByteArray encodeImuDatagram(const Iml::ImuData& data)
{
  QByteArray datagram {
    "\x91\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
    SIZE_DATAGRAM_BYTES};

  encodeAngularVelocity(data, datagram);
  encodeAcceleration(data, datagram);

  encodeValue(data.time % (std::numeric_limits<uint8_t>::max() + 1), 1, 1, 21, datagram);

  auto crcRes = crc32(datagram, SIZE_DATAGRAM_BYTES - 4, DEFAULT_SEED);

  encodeValue(crcRes, 1, 4, 24, datagram);

  return datagram;
}
