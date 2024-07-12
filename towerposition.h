#ifndef TOWERPOSITION_H
#define TOWERPOSITION_H

#include <QSize>
#include <QPainter>
#include <QString>
#include <QPoint>

class TowerPosition
{
 private:
    QPoint position; //防御塔左上角位置
    bool myHasTower; //有无防御塔放置
    QString myPath; //放置防御塔坑图片的路径
 public:
    TowerPosition(QPoint Position,QString path = ":/img/img/TowerPosition.png");
    ~TowerPosition() = default;

    bool HasTower() {return myHasTower;}//返回坑内是否有防御塔
    void SetTower(bool hasTower);//设置有防御塔
    bool InPosition(QPoint point); //检查点击位置是否在防御塔坑内
    void DrawTowerPosition(QPainter *painter);//加载防御塔图片

    QPoint GetMidPosition();
};

#endif // TOWERPOSITION_H
