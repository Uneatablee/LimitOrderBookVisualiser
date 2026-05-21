#pragma once
#include <map>

struct ViewState{
        std::map<int, int, std::greater<int>> bids_price_level_volumes;
        std::map<int, int, std::less<int>> asks_price_level_volumes;
};

