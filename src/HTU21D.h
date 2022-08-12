#ifndef HTU21D_SENSOR_H
#define HTU21D_SENSOR_H

#include "interfaces/HTU21D-I2C-HAL.h"
#include "interfaces/Delay-HAL.h"
#include <utility>

namespace HTU21D
{
    enum ErrorType {
        ERROR_NONE,
        ERORR_TIMEOUT,
        ERROR_CRC_INVALID,
        ERORR_WRITE_FAILURE,
    };

    using MeasurementResult = std::pair<ErrorType, double>;

    class Sensor
    {
    public:
        Sensor(I2CHAL *i2cHal, DelayFunction delayFn);

        MeasurementResult measureTemperature();
        MeasurementResult measureHumidity();

    private:
        using CommandResult = std::pair<ErrorType, BytesVec>;
        CommandResult getCommandResult(uint8_t command);

    private:
        I2CHAL *hal_;
        DelayFunction delay_;
    };
};

#endif