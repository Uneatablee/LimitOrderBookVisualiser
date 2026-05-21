#pragma once
#include "QWidget"
#include "QBarSet"
#include "QBarCategoryAxis"
#include "QValueAxis"
#include <memory>
#include "../../viewstate/ViewState.hpp"

class ViewModel;

class MarketDepthGraph : public QWidget{
    Q_OBJECT

public:
    MarketDepthGraph(std::shared_ptr<ViewModel>);
    void UpdateFromState(std::shared_ptr<ViewState>);

private:
    void SetupUi();

    std::shared_ptr<ViewModel> _view_model;
    std::vector<int> _bidVolumes;
    std::vector<int> _askVolumes;

    //QT OWNERSHIPS
    QBarSet* _setBids;
    QBarSet* _setAsks;
    QBarCategoryAxis* _axisX;
    QValueAxis* _axisY;
};