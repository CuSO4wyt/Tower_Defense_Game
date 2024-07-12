#ifndef WAYPOINT_H
#define WAYPOINT_H
#include <QPoint>
#include <QList>
#include <QPainter>

class WayPoint
{
private:
    QPoint MidPosition;  //中心点
    WayPoint * NextPositon;
public:
    WayPoint(const QPoint &Position);
    WayPoint * GetNextWayPoint() const; //获得下一个航点
    void SetNextWayPoint(WayPoint *point);
    const QPoint& GetThisWayPoint() const;
    void DrawPoint(QPainter * painter) const; //参数painter在gamescene中创建了

};

#endif // WAYPOINT_H
