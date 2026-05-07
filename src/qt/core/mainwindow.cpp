#include "mainwindow.h"
#include "../view/include/OrderBookMainScreen.hpp"
#include "QWidget"
#include "QBoxLayout"

MainWindow::MainWindow(OrderBookMainScreen* orderbook_main_screen) : QMainWindow(nullptr)
{
    setWindowTitle("OrderBook Visualization");
    resize(1320, 870);

    QWidget* central_widget = new QWidget(this);
    setCentralWidget(central_widget);

    QBoxLayout* central_layout = new QBoxLayout(QBoxLayout::LeftToRight, central_widget);
    central_layout -> setContentsMargins(0, 0, 0, 0);
    central_layout -> setSpacing(0);

    orderbook_main_screen -> setParent(this);

    central_layout -> addWidget(orderbook_main_screen);
}

MainWindow::~MainWindow(){}