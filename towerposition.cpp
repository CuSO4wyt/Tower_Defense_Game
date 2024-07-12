
#include "towerposition.h"
#include "QString"

#define SIZE_TOWER 125

TowerPosition::TowerPosition(QPoint Position,QString path)
{
    myHasTower = false;
    position = Position;
    myPath = path;
}

QPoint TowerPosition::GetMidPosition()
{
    QPoint tmp;
    tmp.setX(position.x()+SIZE_TOWER/2);
    tmp.setY(position.y()+SIZE_TOWER/2);
    return tmp;
}

void TowerPosition::SetTower(bool hasTower)
{
    myHasTower = hasTower;
}

bool TowerPosition::InPosition(QPoint point)
{
    bool xInHere=point.x()>position.x() && point.x()<position.x()+SIZE_TOWER;
    bool yInHere=point.y()>position.y() && point.y()<position.y()+SIZE_TOWER;
    return xInHere && yInHere;

}

void TowerPosition::DrawTowerPosition(QPainter *painter)
{
    painter->drawPixmap(position.x(),position.y(),myPath);
}
