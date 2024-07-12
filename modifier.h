#ifndef MODIFIER_H
#define MODIFIER_H

#include <QString>

class Tower;
class Enemy;

class Modifier
{
public:
    Modifier(const QString &name);
    virtual ~Modifier() {}
    virtual void apply(Tower *tower) = 0;
    virtual void apply(Enemy *enemy) = 0;
    QString getName() const;

private:
    QString name;
};

// 近战塔词缀

//狂暴
class BerserkModifier : public Modifier
{
public:
    BerserkModifier();
    void apply(Tower *tower) override;
    void apply(Enemy *enemy) override;
};

//冰系
class IceModifier : public Modifier
{
public:
    IceModifier();
    void apply(Tower *tower) override;
    void apply(Enemy *enemy) override;
};

//群伤
class AoeModifier : public Modifier
{
public:
    AoeModifier();
    void apply(Tower *tower) override;
    void apply(Enemy *enemy) override;
};

// 远程塔词缀

//放血
class BleedModifier : public Modifier
{
public:
    BleedModifier();
    void apply(Tower *tower) override;
    void apply(Enemy *enemy) override;
};

// 敌方单位词缀

//闪现
class FlashModifier : public Modifier
{
public:
    FlashModifier();
    void apply(Tower *tower) override;
    void apply(Enemy *enemy) override;
};

//神速
class SpeedModifier : public Modifier
{
public:
    SpeedModifier();
    void apply(Tower *tower) override;
    void apply(Enemy *enemy) override;
};

#endif // MODIFIER_H
