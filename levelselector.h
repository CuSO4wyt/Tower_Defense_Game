#ifndef LEVELSELECTOR_H
#define LEVELSELECTOR_H

#include <QWidget>

#include "gamescene.h"

namespace Ui {
class LevelSelector;
}

class LevelSelector : public QWidget
{
    Q_OBJECT

public:
    explicit LevelSelector(QWidget *parent = nullptr);
    ~LevelSelector();


private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::LevelSelector *ui;
    GameScene * gameScene;
};

#endif // LEVELSELECTOR_H
