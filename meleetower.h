#ifndef MELEETOWER_H
#define MELEETOWER_H

#include "tower.h"

class MeleeTower : public Tower
{
public:
    MeleeTower(QPoint Position, GameScene *gameScene, int Hurt,
               int AttackRange, int HP, int AttackSpeed,
               bool IsAlive = true, QString Path = ":/img/img/Tower2.png");


    void Draw(QPainter *painter) override; //绘图函数
    void AttackEnemy() override;
    void takeDamage(int damage);
    int getMyHP(); //get my hp
    int getMyHurt();//get my hurt
    int getMyAttackRange();//get my attack range
    bool isMeleeModifier(Modifier *mod); //判断是否是近战塔词缀

private:
    int myHurt;
    int myAttackRange;
    int myHP;
    int myMaxHP;

};

#endif // MELEETOWER_H
