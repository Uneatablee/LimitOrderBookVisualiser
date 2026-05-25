#include "gtest/gtest.h"
#include "OrderBook.hpp"

using namespace lobv::business_logic;

//Arrange
struct OrderBookUnitTest : testing::Test{
    OrderBook order_book{};
};

struct OrderParams{
    OrderId id;
    OrderType type;
    Side side;
    Price price;
    Quantity quantity;
};

struct CreateOrderUnitTest : OrderBookUnitTest, testing::WithParamInterface<OrderParams>{

};

//TESTS ----------->

TEST_P(CreateOrderUnitTest, OrderAddedToOrderBookIncreasesVolume){

    //Act
    auto params = GetParam();
    auto result = order_book.AddOrder(params.id, params.type, params.side, params.price, params.quantity);
    Quantity volume = order_book.GetVolumeAtPrice(params.side, params.price);
    Quantity expected_volume = params.quantity;

    //Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(volume, expected_volume);
}

TEST_P(CreateOrderUnitTest, OrderCanceledDecreasesVolume){

    //Act
    auto params = GetParam();
    order_book.AddOrder(params.id, params.type, params.side, params.price, params.quantity);
    auto result = order_book.CancelOrder(params.id);
    Quantity volume = order_book.GetVolumeAtPrice(params.side, params.price);
    Quantity expected_volume = 0;

    //Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(volume, expected_volume);
}

TEST_F(OrderBookUnitTest, OrdersCanAccumulateVolume){

    //Act
    order_book.AddOrder(1, OrderType::GoodForDay, Side::BuySide, 100, 30);
    order_book.AddOrder(2, OrderType::GoodForDay, Side::BuySide, 100, 55);
    order_book.AddOrder(3, OrderType::GoodForDay, Side::BuySide, 100, 80);

    auto three_placed_result = order_book.GetVolumeAtPrice(Side::BuySide, 100);

    order_book.CancelOrder(2);

    auto middle_cancel_result = order_book.GetVolumeAtPrice(Side::BuySide, 100);

    order_book.AddOrder(4, OrderType::GoodForDay, Side::BuySide, 100, 77);

    auto additional_add_result = order_book.GetVolumeAtPrice(Side::BuySide, 100);

    //Assert
    Quantity three_placed_result_expected = 165;
    Quantity middle_cancel_result_expected = 110;
    Quantity additional_add_result_expected = 187;

    EXPECT_EQ(three_placed_result_expected, three_placed_result);
    EXPECT_EQ(middle_cancel_result_expected, middle_cancel_result);
    EXPECT_EQ(additional_add_result_expected, additional_add_result);
}

TEST_F(OrderBookUnitTest, CanReturnHighestBid){
    //Act
    order_book.AddOrder(1, OrderType::GoodTilCancelled, Side::BuySide, 200, 50);
    order_book.AddOrder(2, OrderType::GoodTilCancelled, Side::BuySide, 100, 50);
    order_book.AddOrder(3, OrderType::GoodTilCancelled, Side::BuySide, 199, 50);
    order_book.AddOrder(4, OrderType::GoodTilCancelled, Side::BuySide, 201, 50);

    //Assert
    Price expected_highest_bid = 201;
    EXPECT_EQ(order_book.GetHighestBid(), expected_highest_bid);
}

TEST_F(OrderBookUnitTest, CanReturnLowestAsk){
    order_book.AddOrder(1, OrderType::GoodTilCancelled, Side::SellSide, 200, 50);
    order_book.AddOrder(2, OrderType::GoodTilCancelled, Side::SellSide, 100, 50);
    order_book.AddOrder(3, OrderType::GoodTilCancelled, Side::SellSide, 99, 50);
    order_book.AddOrder(4, OrderType::GoodTilCancelled, Side::SellSide, 201, 50);

    //Assert
    Price expected_lowest_ask = 99;
    EXPECT_EQ(order_book.GetLowestAsk(), expected_lowest_ask);
}

TEST_F(OrderBookUnitTest, OrdersCanMatch){

    //Act
    order_book.AddOrder(1, OrderType::GoodForDay, Side::SellSide, 100, 30);
    order_book.AddOrder(2, OrderType::GoodForDay, Side::SellSide, 110, 55);
    order_book.AddOrder(3, OrderType::GoodForDay, Side::SellSide, 120, 80);

    auto obtainable_price_check = order_book.CanMatch(Side::BuySide, 115);
    auto not_obtainable_price_check = order_book.CanMatch(Side::BuySide, 90);

    //Assert
    EXPECT_TRUE(obtainable_price_check);
    EXPECT_FALSE(not_obtainable_price_check);
}

TEST_F(OrderBookUnitTest, SamePriceTakerOrderDecreasesMakerOrderSize){
    //Act
    order_book.AddOrder(1, OrderType::GoodForDay, Side::SellSide, 100, 30);
    order_book.MatchOrder(Side::BuySide, 100, 20);

    //Assert
    Quantity maker_quantity = order_book.GetVolumeAtPrice(Side::SellSide, 100);
    Quantity expected_maker_quantity = 10;

    EXPECT_EQ(maker_quantity, expected_maker_quantity);
}

TEST_F(OrderBookUnitTest, SamePriceMakerOrderDecreasesTakerOrderSize){
    //Act
    order_book.AddOrder(1, OrderType::GoodForDay, Side::SellSide, 100, 30);
    order_book.MatchOrder(Side::BuySide, 100, 20);

    //Assert
    Quantity taker_quantity = order_book.GetVolumeAtPrice(Side::BuySide, 100);
    Quantity expected_taker_quantity = 0;

    EXPECT_EQ(taker_quantity, expected_taker_quantity);
}

TEST_F(OrderBookUnitTest, MatchOrderCanBeCalledRepeatedlyOnBuySide){
    //Act
    order_book.AddOrder(1, OrderType::GoodForDay, Side::SellSide, 100, 100);
    order_book.AddOrder(2, OrderType::GoodForDay, Side::SellSide, 100, 20);
    order_book.AddOrder(3, OrderType::GoodForDay, Side::SellSide, 100, 211);
    order_book.AddOrder(4, OrderType::GoodForDay, Side::SellSide, 90, 30);
    order_book.AddOrder(5, OrderType::GoodForDay, Side::SellSide, 91, 15);
    order_book.AddOrder(6, OrderType::GoodForDay, Side::SellSide, 120, 10);

    order_book.MatchOrder(Side::BuySide, 100, 80);
    order_book.MatchOrder(Side::BuySide, 120, 37);

    //Assert
    Quantity expected_100_ask_price_level_quantity = 259;
    Quantity _100_ask_price_level_quantity = order_book.GetVolumeAtPrice(Side::SellSide, 100);

    EXPECT_EQ(_100_ask_price_level_quantity, expected_100_ask_price_level_quantity);
}

TEST_F(OrderBookUnitTest, MatchOrderCanBeCalledRepeatedlyOnSellSide){
    //Act
    order_book.AddOrder(1, OrderType::GoodForDay, Side::BuySide, 100, 100);
    order_book.AddOrder(2, OrderType::GoodForDay, Side::BuySide, 120, 10);
    order_book.AddOrder(3, OrderType::GoodForDay, Side::BuySide, 130, 43);
    order_book.AddOrder(4, OrderType::GoodForDay, Side::BuySide, 100, 10);
    order_book.AddOrder(5, OrderType::GoodForDay, Side::BuySide, 100, 60);
    order_book.AddOrder(6, OrderType::GoodForDay, Side::BuySide, 90, 12);


    order_book.MatchOrder(Side::SellSide, 100, 80);
    order_book.MatchOrder(Side::SellSide, 95, 43);

    //Assert
    Quantity expected_100_bid_price_level_quantity = 100;
    Quantity _100_bid_price_level_quantity = order_book.GetVolumeAtPrice(Side::BuySide, 100);

    EXPECT_EQ(_100_bid_price_level_quantity, expected_100_bid_price_level_quantity);
}

TEST_F(OrderBookUnitTest, ExcesiveQuantityOnOrderMatchIsAddedToOrderBook){
    //Act
    order_book.AddOrder(1, OrderType::GoodForDay, Side::BuySide, 100, 100);
    order_book.AddOrder(2, OrderType::GoodForDay, Side::BuySide, 120, 10);
    order_book.AddOrder(3, OrderType::GoodForDay, Side::BuySide, 130, 43);

    order_book.MatchOrder(Side::SellSide, 110, 63);
    order_book.MatchOrder(Side::SellSide, 110, 5);

    //Assert
    Quantity expected_sell_side_excesive = 15;
    Quantity sell_side_excesive = order_book.GetVolumeAtPrice(Side::SellSide, 110);

    EXPECT_EQ(expected_sell_side_excesive, sell_side_excesive);
}

// ------------------>

INSTANTIATE_TEST_SUITE_P(Default, CreateOrderUnitTest,
    testing::Values(
        OrderParams{1, OrderType::FillOrKill, Side::BuySide, 100, 50},
        OrderParams{1, OrderType::FillOrKill, Side::SellSide, 10, 200'000'000},
        OrderParams{1, OrderType::GoodForDay, Side::BuySide, 20, 0}
));