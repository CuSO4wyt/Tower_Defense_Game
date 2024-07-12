#include "modifier.h"
#include "tower.h"
#include "enemy.h"
#include <QTimer>

Modifier::Modifier(const QString &name)
    : name(name)
{
}

QString Modifier::getName() const
{
    return name;
}

// 近战塔词缀实现

//狂暴
BerserkModifier::BerserkModifier(): Modifier("狂暴"){}

void BerserkModifier::apply(Tower *tower)
{
    tower->myDamage *= 2;
    tower->myAttackSpeed /= 2;
}


void BerserkModifier::apply(Enemy *enemy){} //事实上不会发生，留空


//冰系
IceModifier::IceModifier(): Modifier("冰系"){}


void IceModifier::apply(Tower *tower){}//事实上不会发生，留空

void IceModifier::apply(Enemy *enemy)
{
    enemy->stop(); // 使敌人停止移动和攻击
}


//群伤
AoeModifier::AoeModifier(): Modifier("群伤"){}


void AoeModifier::apply(Tower *tower)
{
    //在攻击逻辑中实现
}

void AoeModifier::apply(Enemy *enemy){}//事实上不会发生，留空

// 远程塔词缀实现

//放血
BleedModifier::BleedModifier(): Modifier("放血"){}


void BleedModifier::apply(Tower *tower){}//事实上不会发生，留空

void BleedModifier::apply(Enemy *enemy)
{
    // 敌人受到放血效果，持续掉血，在攻击逻辑中实现
}

// 敌方单位词缀实现
FlashModifier::FlashModifier(): Modifier("闪现"){}


void FlashModifier::apply(Tower *tower){}//事实上不会发生，留空


void FlashModifier::apply(Enemy *enemy)
{
    // 闪现效果
     enemy->flashToNextWayPoint();
     enemy->removeModifier(this);
}


//神速
SpeedModifier::SpeedModifier(): Modifier("神速"){}


void SpeedModifier::apply(Tower *tower){}//事实上不会发生，留空

void SpeedModifier::apply(Enemy *enemy)//使enemy的移动速度加倍
{
    enemy->m_walkingSpeed *= 2;
}
