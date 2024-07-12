#include <QVector2D>
#include <cmath>


#include "enemy.h"
#include "waypoint.h"
#include "gamescene.h"
#include "tower.h"
#include "meleetower.h"
#include "modifier.h"

//计算距离
inline bool collisionWithCircle(QPoint p1, int r1, QPoint p2, int r2)
{
    int xx = p1.x() - p2.x();
    int yy = p1.y() - p2.y();
    int dis = sqrt(xx * xx + yy * yy);
    return dis < r1 + r2;
}


const QSize Enemy::m_fixedSize(30, 30);

Enemy::Enemy(WayPoint *startWayPoint, GameScene *gameScene, const QPixmap &sprite)
    : QObject(0), m_gameScene(gameScene), m_pos(startWayPoint->GetThisWayPoint()), m_sprite(sprite), m_stopped(false), m_attackRange(200), m_maxAttackTowers(1)
{
    m_maxHp = 150;
    m_currentHp = m_maxHp;
    m_walkingSpeed = 2;
    m_active = true;
    m_destinationWayPoint = startWayPoint->GetNextWayPoint();
}

Enemy::~Enemy()
{
    attackTowersList.clear();
    m_destinationWayPoint = nullptr;
    m_gameScene = nullptr;
}

void Enemy::draw(QPainter *painter) const
{
    painter->save();
    static const int healthBarWidth = m_fixedSize.width();
    QPoint healthBarPoint = m_pos + QPoint(-m_fixedSize.width() / 2, -m_fixedSize.height());
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::red);
    QRect healthBarBackRect(healthBarPoint, QSize(healthBarWidth, 2));
    painter->drawRect(healthBarBackRect);

    painter->setBrush(Qt::green);
    QRect healthBarRect(healthBarPoint, QSize((double)m_currentHp / m_maxHp * healthBarWidth, 2));
    painter->drawRect(healthBarRect);

    QPoint tmp(m_pos.x() - m_fixedSize.width() / 2, m_pos.y() - m_fixedSize.height() / 2);
    painter->drawPixmap(tmp.x(), tmp.y(), m_sprite);
    painter->restore();

    // 绘制攻击线
    for (Tower *tower : attackTowersList) {
        painter->setPen(Qt::red);
        painter->drawLine(m_pos, tower->getPosition());
    }

    // 绘制词缀名称
    for (int i = 0; i < modifiers.size(); ++i) {
        painter->drawText(m_pos + QPoint(-50, -50 - i * 15), modifiers[i]->getName());
    }

    painter->drawText(m_pos + QPoint(-50, -50), QString::number(m_currentHp));
}

void Enemy::move()
{
    if (!m_active || m_stopped) {
        return;
    }

    // 检查是否有近战塔在攻击范围内
    bool towerInRange = false;
    for (Tower *tower : m_gameScene->getTowers()) {
        if (collisionWithCircle(m_pos, m_attackRange, tower->getPosition(), tower->getSize().width() / 2)) {
            towerInRange = true;
            //stop();
            AttackTowers();
            break;
        }
    }



    if (collisionWithCircle(m_pos, 4, m_destinationWayPoint->GetThisWayPoint(), 4)) // 到达了航点
    {
        if (m_destinationWayPoint->GetNextWayPoint()) // 如果还有下一个航点
        {
            m_pos = m_destinationWayPoint->GetThisWayPoint();
            m_destinationWayPoint = m_destinationWayPoint->GetNextWayPoint();
        } else //到达终点
        {
            m_gameScene->Lost();
        }
    } else // 还在路上
    {
        QPoint targetPoint = m_destinationWayPoint->GetThisWayPoint();
        double movementSpeed = m_walkingSpeed;
        QVector2D normalized(targetPoint - m_pos);
        normalized.normalize();
        m_pos = m_pos + normalized.toPoint() * movementSpeed;
    }
}

void Enemy::doActive()
{
    m_active = true;
}


QPoint Enemy::get_pos() const
{
    return m_pos;
}

void Enemy::getAttack(Tower *tower)
{
    attackTowersList.push_back(tower);
}

void Enemy::getDamage(int damage)
{
    m_currentHp -= damage;
    if (m_currentHp <= 0) {
        removeFromGame(); // 调用移除敌人的函数
    }
}

void Enemy::removeFromGame()
{
    Q_ASSERT(this);
    m_gameScene->removeEnemy(this);
}

void Enemy::getLostSight(Tower *tower)
{
    attackTowersList.removeOne(tower);
}

void Enemy::reSetHp(int maxHp)
{
    m_maxHp = maxHp;
    m_currentHp = maxHp;
}

void Enemy::AttackTowers()
{
    if(m_stopped) return;

    QList<MeleeTower *> towers = m_gameScene->getMeleeTowers();
    for (MeleeTower *tower : towers) {
        if ((m_pos - tower->getPosition()).manhattanLength() <= m_attackRange) {
            if (attackTowersList.size() < m_maxAttackTowers && !attackTowersList.contains(tower)) {
                attackTowersList.append(tower);
            }
            tower->takeDamage(10); // 对防御塔造成伤害
        }
    }

    // 移除死亡的防御塔
    for (int i = 0; i < attackTowersList.size(); ++i) {
        if (!attackTowersList[i]->isAlive()) {
            attackTowersList.removeAt(i);
            --i;
        }
    }
}

bool Enemy::isAlive() const
{
    return m_currentHp > 0;
}

void Enemy::stop()
{
    m_stopped = true;
    m_walkingSpeed = 0;
    m_attackRange = 0;

}

void Enemy::resume()
{
    m_stopped = false;
    m_walkingSpeed = 2;
}

bool Enemy::isStopped() const
{
    return m_stopped;
}

int Enemy::getMyCurrentHP()
{
    return m_currentHp;
}

void Enemy::setMyCurrentHP(int HP)
{
    m_currentHp = HP;
}

void Enemy::setMyCurrentPosition(QPoint Position)
{
    m_pos = Position;
}

void Enemy::addModifier(Modifier *modifier)
{
    if (modifiers.size() < 2) {
        modifiers.append(modifier);
        applyModifiers();
    }
}

void Enemy::removeModifier(Modifier *modifier)
{
    modifiers.removeOne(modifier);
    applyModifiers();
}

Modifier *Enemy::removeModifier()
{
    if (!modifiers.isEmpty()) {
        Modifier *modifier = modifiers.takeLast();
        return modifier;
    }
    return nullptr;
}

void Enemy::applyModifiers()
{
    //m_walkingSpeed = 3; // 重置移动速度为基础速度
    m_attackRange = 100; // 重置攻击范围为基础范围

    for (Modifier *modifier : modifiers) {
        modifier->apply(this);
    }
}

QList<Modifier *> Enemy::getModifiers() const
{
    return modifiers;
}

void Enemy::flashToNextWayPoint()
{
    if (m_destinationWayPoint && m_destinationWayPoint->GetNextWayPoint()) {
        m_pos = m_destinationWayPoint->GetThisWayPoint();
        m_destinationWayPoint = m_destinationWayPoint->GetNextWayPoint();
    }
}
