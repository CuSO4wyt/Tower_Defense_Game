#include "waypoint.h"
#include "gamescene.h"
#include <QPoint>
#include <QPainter>

WayPoint::WayPoint(const QPoint &Position)
{
    MidPosition = Position;
    NextPositon = nullptr; //暂时未设置下一个航点

}

WayPoint* WayPoint::GetNextWayPoint()const //获得下一个航点
{
    return this->NextPositon;
}

void WayPoint::SetNextWayPoint(WayPoint *point)
{
    this->NextPositon = point;
}

const QPoint& WayPoint::GetThisWayPoint() const //返回本航点
{
    return this->MidPosition;
}

void WayPoint::DrawPoint(QPainter *painter) const
{

    QPoint point = this->MidPosition;
    painter->save();
    painter->setPen(Qt::green);
    painter->drawEllipse(point, 6, 6);
    painter->setPen(Qt::yellow);
    painter->drawEllipse(point, 2, 2);
    painter->restore();
}
