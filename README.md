# HTU21D C++ Library

A simple library for reading temperature and relative humidity using HTU21D sensor.

## Supported Platforms

Currently supported platform but not limited to:

* Arduino (See src/HTU21D-Arduino-12C-HAL.h and src/HTU21D-Arduino-I2C-HAL.cpp files)

Support for more platform may be added later on, but for now you can:

* Implement the I2C HAL interface (See src/interfaces/HTU21D-I2C-HAL.h file)
* Implement the delay function interface (See src/interfaces/Delay-HAL.h file)


## Usage

### Arduino

```c++
#include <Arduino.h>
#include <Wire.h>
#include <HTU21D.h>
#include <HTU21D-Arduino-I2C-HAL.h>

HTU21D::ArduinoI2CHAL i2cHal(&Wire, 0x40);
HTU21D::Sensor sensor(&i2cHal, delay);

void setup()
{
    Serial.begin(9600);
    Wire.begin();
}

void loop()
{
    auto temp = sensor.measureTemperature();
    auto hum = sensor.measureHumidity();

    if (temp.first == HTU21D::ERROR_NONE)
    {
        Serial.print("Temperature: ");
        Serial.println(temp.second);
    }

    if (hum.first == HTU21D::ERROR_NONE)
    {
        Serial.print("Humidity: ");
        Serial.println(hum.second);
    }

    delay(1000);
}
```
