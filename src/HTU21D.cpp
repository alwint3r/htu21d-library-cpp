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
        MeasurementResult result = {};
        result.status = SENSOR_OK;
        result.data = 0.0;

        auto res = getCommandResult(CMD_MEASURE_HUMIDITY_NO_HOLD);
        if (res.first != SENSOR_OK)
        {
            result.status = res.first;
            return result;
        }

        auto msb = res.second[0];
        auto lsb = res.second[1];

        uint8_t cleanedLsb = lsb & ~(0b00001111);
        uint16_t raw = msb << 8 | cleanedLsb;
        auto value = -6 + (125 * (raw / 65536.0));

        result.data = value;

        return result;
    }

    MeasurementResult Sensor::measureTemperature()
    {
        MeasurementResult result = {};
        result.status = SENSOR_OK;
        result.data = 0.0;

        auto res = getCommandResult(CMD_MEASURE_TEMPERATURE_NO_HOLD);
        if (res.first != SENSOR_OK)
        {
            result.status = res.first;
            return result;
        }

        auto msb = res.second[0];
        auto lsb = res.second[1];

        uint8_t cleanedLsb = lsb & ~(3);
        uint16_t raw = msb << 8 | cleanedLsb;
        auto value = -46.85 + (175.72 * (raw / 65536.0));

        result.data = value;

        return result;
    }

    Sensor::CommandResult Sensor::getCommandResult(uint8_t command)
    {
        auto res = hal_->write(command);
        if (res != I2C_WRITE_OK)
        {
            return std::make_pair(HTU21D::SENSOR_ERROR_COMMUNICATION, BytesVec{});
        }

        delay_(50);

        auto retryCount = 0;
        auto maxRetry = 30;
        while (retryCount < maxRetry)
        {
            auto readResult = hal_->read(3);
            if (readResult.status == I2C_READ_INSUFFICIENT_DATA)
            {
                delay_(50);
                retryCount++;
                continue;
            }

            auto crc = readResult.data[2];

            auto calculatedCrc = crc8(readResult.data.data(), 2);
            if (crc != calculatedCrc)
            {
                return std::make_pair(SENSOR_ERROR_DATA_CORRUPT, BytesVec{});
            }

            return std::make_pair(SENSOR_OK, readResult.data);
        }

        return std::make_pair(SENSOR_ERROR_COMMUNICATION, BytesVec{});
    }
};