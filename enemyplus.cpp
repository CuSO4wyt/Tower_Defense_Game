#include <QVector2D>
#include <cmath>

#include "enemy.h"
#include "enemyplus.h"
#include "waypoint.h"
#include "gamescene.h"
#include "tower.h"
#include "meleetower.h"
#include "modifier.h"

EnemyPlus::EnemyPlus(WayPoint *start, GameScene *gameScene, const QPixmap &sprite)
    : Enemy(start, gameScene, sprite)
{
    // 增加最大血量和当前血量
    m_maxHp *= 3; // 强敌的血量是普通敌人的三倍
    m_currentHp = m_maxHp;
}

EnemyPlus::~EnemyPlus()
{
    attackTowersList.clear();
    m_destinationWayPoint = nullptr;
    m_gameScene = nullptr;
}
