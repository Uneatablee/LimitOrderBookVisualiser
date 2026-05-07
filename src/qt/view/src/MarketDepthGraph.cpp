#include "../include/MarketDepthGraph.hpp"
#include "QBoxLayout"
#include "QPushButton"
#include "QtCharts/QBarSet"
#include "QtCharts/QStackedBarSeries"
#include "QtCharts/QChartView"
#include "QtCharts/QChart"
#include <QStringList>
// #include "../../viewmodel/ViewModel.hpp"
// #include "../../Intent/UserIntent.hpp"
// #include "../../viewstate/ViewState.hpp"
#include "QtCharts/QBarCategoryAxis"
#include "QtCharts/QValueAxis"
#include "MarketDepthDataMock.hpp"

MarketDepthGraph::MarketDepthGraph(/*ViewModel* view_model*/){
    auto graph_layout = new QHBoxLayout(this);

    QBarSet *setBids = new QBarSet("Bids");
    QBarSet *setAsks = new QBarSet("Asks");
    QStringList categories;

    setBids -> setColor(QColor(51,255,51, 127));
    setAsks -> setColor(QColor(255, 51, 51, 127));

    // view_model -> HandleIntent(Intent::ReloadData);
    //temporary data mock usage, just to test basic graph view
    //later to include MVI abstraction and use that mock in its target place

    auto temporary_market_depth = Mocks::UiMocks::GenerateMockSnapshot(100);

    std::vector<int> bidVolumes;
    std::vector<int> askVolumes;

    unsigned int bidsTotalVolume = 0;
    unsigned int asksTotalVolume = 0;

    bidVolumes.reserve(temporary_market_depth.bids_price_level_volumes.size());
    askVolumes.reserve(temporary_market_depth.asks_price_level_volumes.size());

    for (auto const& [price, volume] : temporary_market_depth.bids_price_level_volumes) {
        bidsTotalVolume += volume;
        bidVolumes.push_back(bidsTotalVolume);
    }

    for (auto const& [price, volume] : temporary_market_depth.asks_price_level_volumes) {
        asksTotalVolume += volume;
        askVolumes.push_back(asksTotalVolume);
    }

    std::sort(bidVolumes.begin(), bidVolumes.end(), [](int a, int b){
        return a > b;
    });

    for(size_t i = 0; i < bidVolumes.size(); ++i) {
        *setBids << bidVolumes[i];
        *setAsks << 0;
        categories << QString("B%1").arg(i);
    }

    for(size_t i = 0; i < askVolumes.size(); ++i) {
        *setBids << 0;
        *setAsks << askVolumes[i];
        categories << QString("A%1").arg(i);
    }

    auto series = new QStackedBarSeries(this);
    series -> append(setBids);
    series -> append(setAsks);

    series->setBarWidth(1.0);

    auto chart = new QChart();
    chart -> addSeries(series);

    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setLabelsVisible(false);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    auto chart_view = new QChartView(chart);

    graph_layout -> addWidget(chart_view);
}