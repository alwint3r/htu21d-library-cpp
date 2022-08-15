#ifndef HTU21D_I2C_HAL_HPP
#define HTU21D_I2C_HAL_HPP

#include <utility>
#include <cstdint>
#include <vector>

namespace HTU21D
{
    using BytesVec = std::vector<uint8_t>;
    enum WriteResult
    {
        I2C_WRITE_OK,
        I2C_ERROR_WRITE_FAILURE,
        I2C_ERROR_WRITE_TIMEOUT,
        I2C_WRITE_ERROR_UNKNOWN
    };

    enum ReadStatus
    {
        I2C_READ_OK,
        I2C_READ_INSUFFICIENT_DATA,
        I2C_READ_ERROR_UNKNOWN,
    };

    struct ReadResult
    {
        ReadStatus status;
        BytesVec data;
    };

    class I2CHAL
    {
    public:
        I2CHAL() {}
        virtual ~I2CHAL() {}

        virtual ReadResult read(size_t length) = 0;
        virtual WriteResult write(uint8_t data) = 0;
        virtual WriteResult write(uint8_t addr, uint8_t data) = 0;
    };
};

#endif