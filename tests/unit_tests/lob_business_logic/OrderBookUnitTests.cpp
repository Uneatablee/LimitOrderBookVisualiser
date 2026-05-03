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

// ------------------>

INSTANTIATE_TEST_SUITE_P(Default, CreateOrderUnitTest,
    testing::Values(
        OrderParams{1, OrderType::FillOrKill, Side::BuySide, 100, 50},
        OrderParams{1, OrderType::FillOrKill, Side::SellSide, 10, 200'000'000},
        OrderParams{1, OrderType::GoodForDay, Side::BuySide, 20, 0}
));

int main(int argc, char* argv[]){

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}