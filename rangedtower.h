#ifndef RANGEDTOWER_H
#define RANGEDTOWER_H

#include "tower.h"

class RangedTower : public Tower
{
public:
    RangedTower(QPoint Position, GameScene *gameScene, int Hurt, int AttackRange, int HP,int AttackSpeed, bool IsAlive = true, QString Path = ":/img/img/Tower1.png");
    void Draw(QPainter *painter) override;
    void AttackEnemy() override;
    int getMyHurt();
    int getMyHP();
    int getMyAttackRange();
    bool isRangedModifier(Modifier *mod);


private:
    int myHurt;
    int myAttackRange;
    int myHP;
};

#endif // RANGEDTOWER_H
