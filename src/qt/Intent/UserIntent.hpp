#pragma once
#include <variant>

namespace Intents::UserIntent{

    //Intents
    struct UpdateDataIntent{};
    struct ChangeMarketDepthIntent{
        unsigned int _market_depth;
    };
    struct GenerateMarketDepthMockDataIntent{
        unsigned int _market_depth;
    };

    using Intent = std::variant<
        UpdateDataIntent,
        ChangeMarketDepthIntent,
        GenerateMarketDepthMockDataIntent>;

};

