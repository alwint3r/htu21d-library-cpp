#ifndef HTU21D_I2C_HAL_HPP
#define HTU21D_I2C_HAL_HPP

#include <utility>
#include <cstdint>
#include <vector>

namespace HTU21D
{
    using BytesVec = std::vector<uint8_t>;
    using ReadResult = std::pair<int, BytesVec>;
    using WriteResult = int;
    using RegAddress = uint8_t;

    class I2CHAL
    {
    public:
        I2CHAL() {}
        virtual ~I2CHAL() {}

        virtual ReadResult read(size_t length) = 0;
        virtual WriteResult write(uint8_t data) = 0;
        virtual WriteResult write(RegAddress addr, uint8_t data) = 0;
    };
};

#endif