#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "levelselector.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    levelSelector(nullptr)
{
    ui->setupUi(this);
    ui->StartButton->setStyleSheet
        (
                "QPushButton {"
                "  color: white;"
                "  background-color: blue;"
                "  border-radius: 10px;"
                "  padding: 10px;"
                "}"
                "QPushButton:hover {"
                "  background-color: red;"
                "}"
                "QPushButton:pressed {"
                "  background-color: yellow;"
                "}"
            );

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_StartButton_clicked()
{
    if (!levelSelector) {
           levelSelector = new LevelSelector;
       }
    levelSelector->show();
    this->hide();
}
