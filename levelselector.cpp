#include "levelselector.h"
#include "ui_levelselector.h"
#include "gamescene.h"

LevelSelector::LevelSelector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LevelSelector),
    gameScene(nullptr)
{
    ui->setupUi(this);

}

LevelSelector::~LevelSelector()
{
    delete ui;
}

void LevelSelector::on_pushButton_clicked()
{
    if(!gameScene)
    {
        gameScene = new GameScene;
    }
    gameScene->show();
    this->hide();
}


void LevelSelector::on_pushButton_3_clicked()
{
    if(!gameScene)
    {
        gameScene = new GameScene(nullptr, 2);
    }
    gameScene->show();
    this->hide();
}

void LevelSelector::on_pushButton_2_clicked()
{
    if(!gameScene)
    {
        gameScene = new GameScene(nullptr, 3);
    }
    gameScene->show();
    this->hide();
}
