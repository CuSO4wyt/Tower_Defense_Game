#ifndef MAPDATA_H
#define MAPDATA_H

#include <QList>
#include <QPoint>
#include <QJsonObject>
#include <QJsonArray>

class MapData
{
public:
    MapData();

    //添加数据
    void addWayPoint(const QPoint &wayPoint, int pathIndex);
    void addMeleeTowerPosition(const QPoint &position);
    void addRangedTowerPosition(const QPoint &position);

    QJsonObject toJson() const;
    void fromJson(const QJsonObject &json);

    //获得数据
    QList<QList<QPoint>> getWayPoints() const;
    QList<QPoint> getMeleeTowerPositions() const;
    QList<QPoint> getRangedTowerPositions() const;

private:
    QList<QList<QPoint>> wayPoints; // 使用 QList 的 QList 保存不同路径的航点
    QList<QPoint> meleeTowerPositions; //保存近战塔位置
    QList<QPoint> rangedTowerPositions; //保存远战塔位置
};

#endif // MAPDATA_H

