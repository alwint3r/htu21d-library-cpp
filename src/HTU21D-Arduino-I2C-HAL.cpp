#include "HTU21D-Arduino-I2C-HAL.h"

#if defined(ARDUINO)

namespace HTU21D
{

    ArduinoI2CHAL::ArduinoI2CHAL(TwoWire *wire, uint8_t address) : wire_(wire), addr_(address)
    {
    }

    HTU21D::ReadResult ArduinoI2CHAL::read(size_t length)
    {
        auto res = wire_->requestFrom(addr_, length);
        if (res < length)
        {
            return std::make_pair(res, HTU21D::BytesVec{});
        }

        auto data = HTU21D::BytesVec{};
        data.reserve(res);
        for (auto i = 0; i < res; i++)
        {
            data.push_back(Wire.read());
        }

        return std::make_pair(res, data);
    }

    HTU21D::WriteResult ArduinoI2CHAL::write(uint8_t data)
    {
        wire_->beginTransmission(addr_);
        wire_->write(data);
        return wire_->endTransmission();
    }

    HTU21D::WriteResult ArduinoI2CHAL::write(HTU21D::RegAddress addr, uint8_t data)
    {
        wire_->beginTransmission(addr_);
        wire_->write(addr);
        wire_->write(data);
        return wire_->endTransmission();
    }

};

#endif