#ifndef ENEMY_H
#define ENEMY_H

#include <QObject>
#include <QPoint>
#include <QPixmap>
#include <QPainter>
#include <QSize>

#include "tower.h"
#include "waypoint.h"


class GameScene;
class SpeedModifier;


class Enemy : public QObject
{
    Q_OBJECT
    friend SpeedModifier;//神速词缀

public:
    Enemy(WayPoint *start, GameScene *gameScene,
          const QPixmap &sprite = QPixmap(":/img/img/Enemy.png"));
    ~Enemy();


    void draw(QPainter *painter) const;//绘图

    void move();//移动
    QPoint get_pos() const;//获取位置

    void getAttack(Tower *tower);
    void getDamage(int damage);
    void getRemoved();
    void getLostSight(Tower *tower);
    void reSetHp(int maxHp);
    void AttackTowers();
    void removeFromGame();//从游戏中移除
    void stop();
    void resume();

    //词缀相关
    void addModifier(Modifier *modifier);
    void removeModifier(Modifier *modifier);
    Modifier *removeModifier();
    void applyModifiers();
    QList<Modifier *> getModifiers() const;

    bool isAlive() const;

    bool isStopped() const;

    int getMyCurrentHP();
    void setMyCurrentHP(int HP);
    void setMyCurrentPosition(QPoint Position);
    void flashToNextWayPoint(); // 闪现到下一个航点,用于FlashModifier

private slots:
    void doActive();

protected:
    int m_maxHp;
    int m_currentHp;
    int m_walkingSpeed;
    bool m_active;
    bool m_stopped;

    int m_attackRange; // 攻击范围
    int m_maxAttackTowers; // 最大同时攻击塔数
    WayPoint *m_destinationWayPoint;
    GameScene *m_gameScene;
    QPoint m_pos;
    QPixmap m_sprite;
    QList<Tower *> attackTowersList;
    QList<Modifier *> modifiers; // 词缀列表

    static const QSize m_fixedSize;
};

#endif // ENEMY_H
