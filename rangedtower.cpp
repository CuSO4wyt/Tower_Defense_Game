#include "rangedtower.h"
#include "gamescene.h"
#include "enemy.h"
#include "modifier.h"

RangedTower::RangedTower(QPoint Position, GameScene *gameScene, int Hurt,int HP, int AttackRange, int AttackSpeed, bool IsAlive, QString Path)
    : Tower(Path, Position, gameScene, IsAlive), myHurt(Hurt),myHP(HP), myAttackRange(AttackRange)
{
    myAttackSpeed = AttackSpeed;
}

void RangedTower::Draw(QPainter *painter)
{

    if(myKilled >= 2)
    {
        myPath = ":/img/img/Tower3.png";
    }
    painter->drawPixmap(myPosition - QPoint(63,63), QPixmap(myPath));

    // 绘制塔的血条
    int barWidth = 50;
    int barHeight = 5;
    int xOffset = -25;
    int yOffset = -10;
    QPoint barTopLeft = myPosition + QPoint(xOffset, yOffset)- QPoint(63,63);
    QPoint barBottomRight = barTopLeft + QPoint(barWidth, barHeight);
    QRect healthBarRect(barTopLeft, barBottomRight);

    painter->setBrush(Qt::gray);
    painter->drawRect(healthBarRect);

    int healthBarWidth = static_cast<int>(barWidth * (static_cast<double>(myHealth) / myMaxHealth));
    QRect currentHealthBarRect(barTopLeft, barTopLeft + QPoint(healthBarWidth, barHeight));
    painter->setBrush(Qt::red);
    painter->drawRect(currentHealthBarRect);

    // 绘制攻击线
    for (Enemy *enemy : attackTargets) {
        painter->setPen(Qt::green);
        painter->drawLine(myPosition, enemy->get_pos());
    }

     //绘制词缀
    for (int i = 0; i < modifiers.size(); ++i) {
            painter->drawText(myPosition + QPoint(-50, -50 - i * 15), modifiers[i]->getName());
        }
}

void RangedTower::AttackEnemy()
{
    QList<Enemy *> enemies = myGameScene->getEnemies();
    for (Enemy *enemy : enemies) {
        if ((myPosition - enemy->get_pos()).manhattanLength() <= myAttackRange) {
            if (attackTargets.size() < maxAttackTargets && !attackTargets.contains(enemy)) {
                attackTargets.append(enemy);
            }
            enemy->getDamage(myHurt); // 对敌人造成伤害
            if (!modifiers.size()) //有放血词缀
            { enemy->getDamage(myHurt);//二次伤害
            }
        }
    }

    // 移除死亡的敌人
    for (int i = 0; i < attackTargets.size(); ++i) {
        if (!attackTargets[i]->isAlive()) {
            attackTargets.removeAt(i);
            myKilled++;
            --i;
        }
    }
}


int RangedTower::getMyHP()
{
    return myHP;
}

int RangedTower::getMyHurt()
{
    return myHurt;
}

int RangedTower::getMyAttackRange()
{
    return myAttackRange;
}


bool RangedTower::isRangedModifier(Modifier *mod)
{
    return dynamic_cast<BleedModifier*>(mod);
}
