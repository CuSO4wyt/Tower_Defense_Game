#include "meleetower.h"
#include "gamescene.h"
#include "enemy.h"
#include "modifier.h"

MeleeTower::MeleeTower(QPoint Position, GameScene *gameScene, int Hurt, int AttackRange, int HP, int AttackSpeed, bool IsAlive, QString Path)
    : Tower(Path, Position, gameScene, IsAlive), myHurt(Hurt), myAttackRange(AttackRange), myHP(HP)
{
    myMaxHP = HP;
    myAttackSpeed  = AttackSpeed;
}

void MeleeTower::Draw(QPainter *painter)
{
    if(myKilled >= 2)
    {
        myPath = ":/img/img/Tower4.png";
    }
    painter->drawPixmap(myPosition - QPoint(63, 63), QPixmap(myPath));

    // 绘制塔的血条
    int barWidth = 50;
    int barHeight = 5;
    int xOffset = -25;
    int yOffset = -10;
    QPoint barTopLeft = myPosition + QPoint(xOffset, yOffset) - QPoint(63, 63);
    QPoint barBottomRight = barTopLeft + QPoint(barWidth, barHeight);
    QRect healthBarRect(barTopLeft, barBottomRight);

    painter->setBrush(Qt::gray);
    painter->drawRect(healthBarRect);

    int healthBarWidth = static_cast<int>(barWidth * (static_cast<double>(myHP) / myMaxHP));
    QRect currentHealthBarRect(barTopLeft, barTopLeft + QPoint(healthBarWidth, barHeight));
    painter->setBrush(Qt::red);
    painter->drawRect(currentHealthBarRect);

    // 绘制攻击线
    for (Enemy *enemy : attackTargets) {
        painter->setPen(Qt::green);
        painter->drawLine(myPosition, enemy->get_pos());
    }

    // 绘制词缀
        for (int i = 0; i < modifiers.size(); ++i) {
            painter->drawText(myPosition + QPoint(-50, -50 - i * 15), modifiers[i]->getName());
        }
}


void MeleeTower::AttackEnemy()
{
    QList<Enemy *> enemies = myGameScene->getEnemies();
    for (Enemy *enemy : enemies) {
        if ((myPosition - enemy->get_pos()).manhattanLength() <= myAttackRange) {

            if(modifiers.size() == 1)//群伤
            {
                if(dynamic_cast<AoeModifier*>(modifiers[0]))
                {
                    if (attackTargets.size() < 4 && !attackTargets.contains(enemy)) //修改最大攻击为3
                    {
                        attackTargets.append(enemy);
                    }
                }
                else {
                    if (attackTargets.size() < maxAttackTargets && !attackTargets.contains(enemy)) //没安装
                    {
                        attackTargets.append(enemy);
                    }
                }

            }

            else if(modifiers.size() == 2)
            {
                if(dynamic_cast<AoeModifier*>(modifiers[0])||dynamic_cast<AoeModifier*>(modifiers[1]))
                {
                    if (attackTargets.size() < 4 && !attackTargets.contains(enemy)) //修改最大攻击为3
                    {
                        attackTargets.append(enemy);
                    }

                }
                else {
                    if (attackTargets.size() < maxAttackTargets && !attackTargets.contains(enemy)) //没安装
                    {
                        attackTargets.append(enemy);
                    }
                }
            }

            else{ //无群伤
                if (attackTargets.size() < maxAttackTargets && !attackTargets.contains(enemy)) //没安装
                {
                    attackTargets.append(enemy);
                }
            }

            enemy->getDamage(myHurt); // 对敌人造成伤害

           if(modifiers.size()== 0)
           {
           }
           else if(modifiers.size()==1)
           {
               if(dynamic_cast<IceModifier*>(modifiers[0]))
               {
                   enemy->stop();
               }
            }
           else {
               if(dynamic_cast<IceModifier*>(modifiers[0])||dynamic_cast<IceModifier*>(modifiers[1]))
               {
                   enemy->stop();
               }
           }

            // 如果有冰系修饰器，停止敌人
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

void MeleeTower::takeDamage(int damage)
{
    myHP -= damage;
    if (myHP <= 0) {
        // 移除防御塔
        myGameScene->removeTower(this);
    }
}

int MeleeTower::getMyHP()
{
    return myHP;
}

int MeleeTower::getMyHurt()
{
    return myHurt;
}

int MeleeTower::getMyAttackRange()
{
    return myAttackRange;
}

bool MeleeTower::isMeleeModifier(Modifier *mod)
{
    return dynamic_cast<BerserkModifier*>(mod) || dynamic_cast<IceModifier*>(mod) || dynamic_cast<AoeModifier*>(mod);
}
