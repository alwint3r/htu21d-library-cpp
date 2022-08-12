#include "HTU21D.h"

static constexpr auto CMD_MEASURE_TEMPERATURE_NO_HOLD = 0xF3;
static constexpr auto CMD_MEASURE_HUMIDITY_NO_HOLD = 0xF5;

// https://stackoverflow.com/questions/51752284/how-to-calculate-crc8-in-c
static uint8_t crc8(uint8_t *data, size_t len)
{
    uint8_t crc = 0x00;
    size_t i, j;
    for (i = 0; i < len; i++)
    {
        crc ^= data[i];
        for (j = 0; j < 8; j++)
        {
            if ((crc & 0x80) != 0)
                crc = (uint8_t)((crc << 1) ^ 0x31);
            else
                crc <<= 1;
        }
    }
    return crc;
}

namespace HTU21D
{
    Sensor::Sensor(I2CHAL *i2cHal, DelayFunction delayFunction) : hal_(i2cHal), delay_(delayFunction)
    {
    }

    MeasurementResult Sensor::measureHumidity()
    {
        auto res = getCommandResult(CMD_MEASURE_HUMIDITY_NO_HOLD);
        if (res.first != ERROR_NONE)
        {
            return std::make_pair(res.first, 0.0);
        }

        auto msb = res.second[0];
        auto lsb = res.second[1];

        uint8_t cleanedLsb = lsb & ~(0b00001111);
        uint16_t raw = msb << 8 | cleanedLsb;
        auto value = -6 + (125 * (raw / 65536.0));

        return std::make_pair(ERROR_NONE, value);
    }

    MeasurementResult Sensor::measureTemperature()
    {
        auto res = getCommandResult(CMD_MEASURE_TEMPERATURE_NO_HOLD);
        if (res.first != ERROR_NONE)
        {
            return std::make_pair(res.first, 0.0);
        }

        auto msb = res.second[0];
        auto lsb = res.second[1];

        uint8_t cleanedLsb = lsb & ~(3);
        uint16_t raw = msb << 8 | cleanedLsb;
        auto value = -46.85 + (175.72 * (raw / 65536.0));

        return std::make_pair(ERROR_NONE, value);
    }

    Sensor::CommandResult Sensor::getCommandResult(uint8_t command)
    {
        auto res = hal_->write(command);
        if (res != 0)
        {
            return std::make_pair(ERORR_WRITE_FAILURE, BytesVec{});
        }

        delay_(50);
        auto retryCount = 0;
        auto maxRetry = 30;

        while (true)
        {
            auto readResult = hal_->read(3);
            if (readResult.first < 3)
            {
                delay_(50);
                if (retryCount++ >= maxRetry)
                {
                    break;
                }

                continue;
            }

            auto crc = readResult.second[2];

            auto calculatedCrc = crc8(readResult.second.data(), 2);
            if (crc != calculatedCrc)
            {
                return std::make_pair(ERROR_CRC_INVALID, BytesVec{});
            }

            return std::make_pair(ERROR_NONE, readResult.second);
        }

        return std::make_pair(ERORR_TIMEOUT, BytesVec{});
    }
};