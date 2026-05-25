#include <iostream>
#include "OrderBook.hpp"

using namespace lobv::business_logic;

int main(){
    OrderBook order_book;

    order_book.AddOrder(1, OrderType::GoodForDay, Side::SellSide, 100, 100);
    order_book.AddOrder(2, OrderType::GoodForDay, Side::SellSide, 100, 20);
    order_book.AddOrder(3, OrderType::GoodForDay, Side::SellSide, 100, 211);
    order_book.AddOrder(4, OrderType::GoodForDay, Side::SellSide, 90, 30);
    order_book.AddOrder(5, OrderType::GoodForDay, Side::SellSide, 91, 15);
    order_book.AddOrder(6, OrderType::GoodForDay, Side::SellSide, 120, 10);

    order_book.MatchOrder(Side::BuySide, 100, 80);
    order_book.MatchOrder(Side::BuySide, 120, 37);

    //Assert
    Quantity _100_ask_price_level_quantity = order_book.GetVolumeAtPrice(Side::SellSide, 100);
    Quantity expected_100_ask_price_level_quantity = 259;
}