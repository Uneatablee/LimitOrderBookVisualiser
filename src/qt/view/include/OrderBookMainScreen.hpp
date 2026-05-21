#pragma once
#include "QWidget"

class MarketDepthGraph;
class MarketPriceLevels;

class OrderBookMainScreen : public QWidget{

    Q_OBJECT

public:
    OrderBookMainScreen(MarketDepthGraph*, MarketPriceLevels*);
};