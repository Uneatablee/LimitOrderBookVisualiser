#include "../include/MarketPriceLevels.hpp"
#include "QPushButton"
#include "QBoxLayout"

MarketPriceLevels::MarketPriceLevels(){

    auto prices_layout = new QHBoxLayout(this);
    auto button_test = new QPushButton("Test", this);

    prices_layout -> addWidget(button_test);
}