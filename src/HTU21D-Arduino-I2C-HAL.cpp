#include "HTU21D-Arduino-I2C-HAL.h"

#if defined(ARDUINO)

namespace HTU21D
{

    ArduinoI2CHAL::ArduinoI2CHAL(TwoWire *wire, uint8_t address) : wire_(wire), addr_(address)
    {
    }

    HTU21D::ReadResult ArduinoI2CHAL::read(size_t length)
    {
        HTU21D::ReadResult result = {};
        result.status = HTU21D::I2C_READ_OK;
        result.data = HTU21D::BytesVec{};

        auto res = wire_->requestFrom(addr_, length);
        if (res < length)
        {
            result.status = HTU21D::I2C_READ_INSUFFICIENT_DATA;
            return result;
        }

        result.data.reserve(res);
        for (auto i = 0; i < res; i++)
        {
            result.data.push_back(Wire.read());
        }

        return result;
    }

    HTU21D::WriteResult ArduinoI2CHAL::write(uint8_t data)
    {
        return doWrite((const uint8_t*)&data, 1);
    }

    HTU21D::WriteResult ArduinoI2CHAL::write(uint8_t addr, uint8_t data)
    {
        const uint8_t buffer[2] = { addr, data };
        return doWrite(buffer, 2);
    }

    HTU21D::WriteResult ArduinoI2CHAL::doWrite(const uint8_t *data, size_t length)
    {
        wire_->beginTransmission(addr_);
        for (auto i = 0; i < length; i++)
        {
            wire_->write(data[i]);
        }

        auto res = wire_->endTransmission();
        if (res == 0)
        {
            return HTU21D::I2C_WRITE_OK;
        }
        else if (res == 2)
        {
            return HTU21D::I2C_ERROR_WRITE_FAILURE;
        }
        else if (res == 5)
        {
            return HTU21D::I2C_ERROR_WRITE_TIMEOUT;
        }
        else
        {
            return HTU21D::I2C_WRITE_ERROR_UNKNOWN;
        }
    }
};

#endif