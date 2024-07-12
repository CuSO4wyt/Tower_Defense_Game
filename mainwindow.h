#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "levelselector.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_StartButton_clicked();

private:
    Ui::MainWindow *ui;
    LevelSelector* levelSelector;


};

#endif // MAINWINDOW_H
