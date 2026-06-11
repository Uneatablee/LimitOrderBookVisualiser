#include "../include/MarketDepthGraph.hpp"
#include "QBoxLayout"
#include "QPushButton"
#include "QtCharts/QBarSet"
#include "QtCharts/QStackedBarSeries"
#include "QtCharts/QChartView"
#include "QtCharts/QChart"
#include <QStringList>
#include "../../viewmodel/ViewModel.hpp"
#include "../../Intent/UserIntent.hpp"
#include "../../viewstate/ViewState.hpp"
#include "QtCharts/QBarCategoryAxis"
#include "QtCharts/QValueAxis"
#include "MarketDepthDataMock.hpp"

MarketDepthGraph::MarketDepthGraph(std::shared_ptr<ViewModel> view_model) : _view_model(view_model){
    SetupUi();

    connect(_view_model.get(), &ViewModel::StateChanged, this, &MarketDepthGraph::UpdateFromState);
}

void MarketDepthGraph::SetupUi(){
    auto graph_layout = new QHBoxLayout(this);

    _setBids = new QBarSet("Bids");
    _setAsks = new QBarSet("Asks");

    _setBids -> setColor(QColor(51,255,51, 127));
    _setAsks -> setColor(QColor(255, 51, 51, 127));

    auto series = new QStackedBarSeries(this);
    series -> append(_setBids);
    series -> append(_setAsks);

    series->setBarWidth(1.0);

    auto chart = new QChart();
    chart -> addSeries(series);
    QStringList categories;

    _axisX = new QBarCategoryAxis();
    _axisX->append(categories);
    _axisX->setLabelsVisible(false);
    chart->addAxis(_axisX, Qt::AlignBottom);
    series->attachAxis(_axisX);

    _axisY = new QValueAxis();
    _axisY->setRange(0, 14000);
    chart->addAxis(_axisY, Qt::AlignLeft);
    series->attachAxis(_axisY);

    auto chart_view = new QChartView(chart);
    graph_layout -> addWidget(chart_view);

}

void MarketDepthGraph::UpdateFromState(std::shared_ptr<ViewState> state){

    _setBids->remove(0, _setBids->count());
    _setAsks->remove(0, _setAsks->count());
    _axisX -> clear();

    unsigned int bidsTotalVolume = 0;
    unsigned int asksTotalVolume = 0;

    _bidVolumes.clear();
    _askVolumes.clear();

    for (auto const& [price, volume] : state -> bids_price_level_volumes) {
        bidsTotalVolume += volume;
        _bidVolumes.push_back(bidsTotalVolume);
    }

    for (auto const& [price, volume] : state -> asks_price_level_volumes) {
        asksTotalVolume += volume;
        _askVolumes.push_back(asksTotalVolume);
    }

    std::sort(_bidVolumes.begin(), _bidVolumes.end(), [](int a, int b){
        return a > b;
    });

    QStringList categories;

    for(size_t i = 0; i < _bidVolumes.size(); ++i) {
        *_setBids << _bidVolumes[i];
        *_setAsks << 0;
        categories << QString("B%1").arg(i);
    }

    for(size_t i = 0; i < _askVolumes.size(); ++i) {
        *_setBids << 0;
        *_setAsks << _askVolumes[i];
        categories << QString("A%1").arg(i);
    }

    _axisX->append(categories);

    unsigned int max_volume = std::max(bidsTotalVolume, asksTotalVolume);

    if(max_volume == 0) {
        max_volume = 100;
    }

    _axisY->setRange(0, max_volume * 1.1);
}