#ifndef TOWER_H
#define TOWER_H

#include <QObject>
#include <QPoint>
#include <QSize>
#include <QString>
#include <QPainter>
#include <QList>


class GameScene;
class Enemy;
class Modifier;
class BerserkModifier;

class Tower : public QObject
{
    Q_OBJECT
    friend BerserkModifier;

public:
    Tower(QString Path, QPoint Position, GameScene *gameScene, bool IsAlive);
    virtual void Draw(QPainter *painter) = 0; //绘图函数
    virtual void AttackEnemy() = 0;//攻击敌人
    void takeDamage(int damage);//自己被攻击
    bool isAlive() const;
    QPoint getPosition() const;
    QSize getSize() const;
    void addModifier(Modifier *modifier); //增加词缀

    void applyModifiers(); //应用词缀

    QList<Modifier *> getModifiers() const;
    Modifier *removeModifier();//卸下词缀
    Modifier *removeModifier(Modifier *modifier); //根据具体的Modifier卸下


protected:
    QString myPath;//图片路径
    QPoint myPosition;//位置
    GameScene *myGameScene;//指向游戏
    bool towerIsAlive;//是否存活
    int myAttackRange;//攻击范围
    int myDamage;//伤害
    int myHealth;//目前血量
    int myMaxHealth;//最高血量
    int myAttackSpeed;//攻击速度
    QList<Enemy *> attackTargets; // 攻击目标列表
    int maxAttackTargets; // 最多攻击目标数
    QList<Modifier *> modifiers;//词缀槽
    int myKilled;

    void contextMenuEvent(QContextMenuEvent *event);
};

#endif // TOWER_H
