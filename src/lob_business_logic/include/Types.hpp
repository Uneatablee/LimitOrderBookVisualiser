#pragma once
#include <cstdint>

namespace lobv::business_logic{

    enum class Side : uint8_t{
        BuySide,
        SellSide
    };

    enum class OrderType : uint8_t{
        GoodTilCancelled,
        ImmediateOrCancel,
        FillOrKill,
        GoodForDay
    };

    using Price = int64_t;
    using OrderId = uint64_t;
    using Quantity = uint64_t;
}