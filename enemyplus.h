#ifndef ENEMYPLUS_H
#define ENEMYPLUS_H

#include "enemy.h"

class Enemy;

class EnemyPlus:public Enemy
{
    Q_OBJECT
    friend SpeedModifier;//神速词缀

public:
    EnemyPlus(WayPoint *start, GameScene *gameScene,
                const QPixmap &sprite = QPixmap(":/img/img/EnemyPlus.png"));
    ~EnemyPlus();
};


#endif // ENEMYPLUS_H
