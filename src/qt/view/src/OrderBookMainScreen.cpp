#include "../include/OrderBookMainScreen.hpp"
#include "../include/MarketDepthGraph.hpp"
#include "../include/MarketPriceLevels.hpp"

#include "QBoxLayout"

OrderBookMainScreen::OrderBookMainScreen(MarketDepthGraph* depth_graph, MarketPriceLevels* price_levels){

    auto main_layout = new QVBoxLayout(this);
    main_layout -> setContentsMargins(0, 0, 0, 0);
    main_layout -> setSpacing(0);

    depth_graph -> setParent(this);
    price_levels -> setParent(this);

    main_layout -> addWidget(depth_graph);
    main_layout -> addWidget(price_levels);
}