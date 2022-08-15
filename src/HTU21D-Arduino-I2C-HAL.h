#ifndef ARDUINO_HTU21D_I2C_HAL_H
#define ARDUINO_HTU21D_I2C_HAL_H

#if defined(ARDUINO)

#include <Arduino.h>
#include <Wire.h>
#include "interfaces/HTU21D-I2C-HAL.h"

namespace HTU21D
{
    class ArduinoI2CHAL : public HTU21D::I2CHAL
    {
    public:
        ArduinoI2CHAL(TwoWire *wire, uint8_t address);
        virtual ~ArduinoI2CHAL() = default;

        virtual HTU21D::ReadResult read(size_t length);
        virtual HTU21D::WriteResult write(uint8_t data);
        virtual HTU21D::WriteResult write(uint8_t addr, uint8_t data);
    
    private:
        HTU21D::WriteResult doWrite(const uint8_t* data, size_t length);

    private:
        TwoWire *wire_;
        uint8_t addr_;
    };
};

#endif

#endif