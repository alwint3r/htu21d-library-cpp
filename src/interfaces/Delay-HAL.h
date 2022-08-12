#ifndef HTU21D_DELAYER_HAL
#define HTU21D_DELAYER_HAL

#include <functional>
#include <cstdint>

namespace HTU21D
{
    using DelayFunction = std::function<void(uint32_t)>;
};

#endif
