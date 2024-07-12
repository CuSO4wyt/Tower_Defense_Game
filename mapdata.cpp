#include "MapData.h"

MapData::MapData()
{
}

void MapData::addWayPoint(const QPoint &wayPoint, int pathIndex)
{
    while (wayPoints.size() <= pathIndex) {
        wayPoints.append(QList<QPoint>()); // 确保 wayPoints 的大小足够大
    }
    wayPoints[pathIndex].append(wayPoint);
}

void MapData::addMeleeTowerPosition(const QPoint &position)
{
    meleeTowerPositions.append(position);
}

void MapData::addRangedTowerPosition(const QPoint &position)
{
    rangedTowerPositions.append(position);
}

QJsonObject MapData::toJson() const
{
    QJsonObject json;

    QJsonArray wayPointsArray;
    for (const QList<QPoint> &path : wayPoints) {
        QJsonArray pathArray;
        for (const QPoint &wayPoint : path) {
            QJsonObject wayPointJson;
            wayPointJson["x"] = wayPoint.x();
            wayPointJson["y"] = wayPoint.y();
            pathArray.append(wayPointJson);
        }
        wayPointsArray.append(pathArray);
    }
    json["wayPoints"] = wayPointsArray;

    QJsonArray meleeTowerArray;
    for (const QPoint &position : meleeTowerPositions) {
        QJsonObject positionJson;
        positionJson["x"] = position.x();
        positionJson["y"] = position.y();
        meleeTowerArray.append(positionJson);
    }
    json["meleeTowerPositions"] = meleeTowerArray;

    QJsonArray rangedTowerArray;
    for (const QPoint &position : rangedTowerPositions) {
        QJsonObject positionJson;
        positionJson["x"] = position.x();
        positionJson["y"] = position.y();
        rangedTowerArray.append(positionJson);
    }
    json["rangedTowerPositions"] = rangedTowerArray;

    return json;
}

void MapData::fromJson(const QJsonObject &json)
{
    wayPoints.clear();
    meleeTowerPositions.clear();
    rangedTowerPositions.clear();

    QJsonArray wayPointsArray = json["wayPoints"].toArray();
    for (int i = 0; i < wayPointsArray.size(); ++i) {
        QJsonArray pathArray = wayPointsArray[i].toArray();
        QList<QPoint> path;
        for (int j = 0; j < pathArray.size(); ++j) {
            QJsonObject wayPointJson = pathArray[j].toObject();
            QPoint wayPoint(wayPointJson["x"].toInt(), wayPointJson["y"].toInt());
            path.append(wayPoint);
        }
        wayPoints.append(path);
    }

    QJsonArray meleeTowerArray = json["meleeTowerPositions"].toArray();
    for (int i = 0; i < meleeTowerArray.size(); ++i) {
        QJsonObject positionJson = meleeTowerArray[i].toObject();
        QPoint position(positionJson["x"].toInt(), positionJson["y"].toInt());
        meleeTowerPositions.append(position);
    }

    QJsonArray rangedTowerArray = json["rangedTowerPositions"].toArray();
    for (int i = 0; i < rangedTowerArray.size(); ++i) {
        QJsonObject positionJson = rangedTowerArray[i].toObject();
        QPoint position(positionJson["x"].toInt(), positionJson["y"].toInt());
        rangedTowerPositions.append(position);
    }
}

QList<QList<QPoint>> MapData::getWayPoints() const
{
    return wayPoints;
}

QList<QPoint> MapData::getMeleeTowerPositions() const
{
    return meleeTowerPositions;
}

QList<QPoint> MapData::getRangedTowerPositions() const
{
    return rangedTowerPositions;
}
