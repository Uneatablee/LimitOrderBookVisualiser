#pragma once
#include <cstdint>

namespace lobv::business_logic{

    enum class Side{
        BuySide,
        SellSide
    };

    using Price = int64_t;
    using OrderId = uint64_t;
    using Quantity = uint64_t;
}