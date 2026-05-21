#include "QWidget"
#include "mainwindow.h"
#include <iostream>
#include <QApplication>
#include "boost/di.hpp"

#include "../view/include/OrderBookMainScreen.hpp"
#include "../view/include/MarketDepthGraph.hpp"
#include "../view/include/MarketPriceLevels.hpp"
#include "../viewmodel/ViewModel.hpp"

int main(int argc, char* argv[]){

    QApplication application(argc, argv);

    auto app_di_container = boost::di::make_injector(
        boost::di::bind<ViewModel>().in(boost::di::singleton)
    );

    auto main_window = app_di_container.create<std::unique_ptr<MainWindow>>();

    main_window -> show();
    return application.exec();
}