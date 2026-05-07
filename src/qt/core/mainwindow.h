#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QMainWindow>

class OrderBookMainScreen;

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit MainWindow(OrderBookMainScreen*);
    ~MainWindow();
};

#endif