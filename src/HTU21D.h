#ifndef HTU21D_SENSOR_H
#define HTU21D_SENSOR_H

#include "interfaces/HTU21D-I2C-HAL.h"
#include "interfaces/Delay-HAL.h"
#include <utility>

namespace HTU21D
{
    enum MeasurementStatus {
        SENSOR_OK,
        SENSOR_ERROR_COMMUNICATION,
        SENSOR_ERROR_DATA_CORRUPT,
        SENSOR_ERROR_UNKNOWN,
    };

    struct MeasurementResult {
        MeasurementStatus status;
        double data;
    };

    class Sensor
    {
    public:
        Sensor(I2CHAL *i2cHal, DelayFunction delayFn);

        MeasurementResult measureTemperature();
        MeasurementResult measureHumidity();

    private:
        using CommandResult = std::pair<MeasurementStatus, BytesVec>;
        CommandResult getCommandResult(uint8_t command);

    private:
        I2CHAL *hal_;
        DelayFunction delay_;
    };
};

#endif