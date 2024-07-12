#include "tower.h"
#include "modifier.h"
#include "gamescene.h"
#include <QMenu>
Tower::Tower(QString Path, QPoint Position, GameScene *gameScene, bool IsAlive)
    : myPath(Path), myPosition(Position), myGameScene(gameScene), towerIsAlive(IsAlive), myHealth(100), myMaxHealth(100), maxAttackTargets(1)
{
    myKilled = 0;
}


void Tower::takeDamage(int damage)
{
    myHealth -= damage;
    if (myHealth <= 0) {
        towerIsAlive = false;
    }
}


bool Tower::isAlive() const
{
    return towerIsAlive;
}

QPoint Tower::getPosition() const
{
    return myPosition;
}

QSize Tower::getSize() const
{
    return QSize(125, 125);
}

void Tower::addModifier(Modifier *modifier)
{
    if (modifiers.size() < 2) //最多安装两个词缀
    {
        modifiers.append(modifier);
        applyModifiers();
    }
}

Modifier *Tower::removeModifier()
{
    if (!modifiers.isEmpty()) {
        Modifier *modifier = modifiers.takeLast();
        return modifier;
    }
    return nullptr;
}

Modifier *Tower::removeModifier(Modifier *modifier)
{
    if (modifiers.contains(modifier)) {
        modifiers.removeOne(modifier);
        return modifier;
    }
    return nullptr;
}

void Tower::contextMenuEvent(QContextMenuEvent *event)
{
    myGameScene->currentTower = this;
    myGameScene->contextMenu->exec(event->globalPos());
}



void Tower::applyModifiers()
{

    for (Modifier *modifier : modifiers) {
        modifier->apply(this);
    }
}


QList<Modifier *> Tower::getModifiers() const
{
    return modifiers;
}
