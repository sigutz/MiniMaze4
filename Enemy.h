//
// Created by silvi on 5/4/2025.
//

#ifndef ENEMY_H
#define ENEMY_H
#include "Object.h"

class Player;

class Enemy : virtual public Object {
protected:
    int Health;
    int Speed;
    int PlayerRange;
    static int totalEnemiesDefeated;
    int maxHealth;

public:
    Enemy(int hp = 100, int speed = 1, int range = 5, int x = 0, int y = 0, char icon = 'E');

    Enemy(const Enemy &ob);

    virtual ~Enemy();

    virtual void Attack(Player &player) = 0;

    virtual void Move(Player &player) = 0;

    virtual void TakeDamage(int dmg) = 0;

    virtual void Display() const;

    int GetHealth() const;

    int GetSpeed() const;

    int GetPlayerRange() const;

    void SetHealth(int health);

    void SetSpeed(int speed);

    void SetPlayerRange(int range);

    bool IsAlive() const;

    static int GetTotalEnemies();

    Enemy &operator=(const Enemy &ob);
};

class Range : virtual public Enemy {
protected:
    int RangeDmg;
    int RangeActions;

public:
    Range(int rangeDmg = 10, int rangeActions = 3, int hp = 100, int speed = 1, int range = 8);

    Range(const Range &ob);

    virtual ~Range();

    virtual void RangedAttack(Player &player);

    virtual void AimAtPlayer(Player &player);

    int GetRangeDmg() const { return RangeDmg; }
    void SetRangeDmg(int dmg) { RangeDmg = dmg; }
    int GetRangeActions() const { return RangeActions; }
    void SetRangeActions(int actions) { RangeActions = actions; }

    Range &operator=(const Range &ob);
};

class Melee : virtual public Enemy {
protected:
    int MeleeDmg;
    int AttacksBeforeCritical;
    int currentAttacks;

public:
    Melee(int meleeDmg = 20, int attacksBeforeCritical = 3, int hp = 150, int speed = 2, int range = 1);

    Melee(const Melee &ob);

    virtual ~Melee();

    virtual void MeleeAttack(Player &player);

    virtual bool IsCriticalHit();

    int GetMeleeDmg() const { return MeleeDmg; }
    void SetMeleeDmg(int dmg) { MeleeDmg = dmg; }
    int GetAttacksBeforeCritical() const { return AttacksBeforeCritical; }
    void SetAttacksBeforeCritical(int attacks) { AttacksBeforeCritical = attacks; }

    Melee &operator=(const Melee &ob);
};

class Ghost : virtual public Enemy {
protected:
    int DmgReduction;
    int IntelReduction;
    int LightReduction;
    int SpeedReduction;
    int RangeReduction;
    int Proximity;

public:
    Ghost(int dmgReduction = 50, int intelReduction = 20, int lightReduction = 30,
          int speedReduction = 40, int rangeReduction = 10, int proximity = 3,
          int hp = 80, int speed = 3, int range = 4);

    Ghost(const Ghost &ob);

    virtual ~Ghost();

    virtual void ApplyDebuffs(Player &player);

    virtual bool IsInProximity(Player &player);

    virtual void PhaseThrough();

    Ghost &operator=(const Ghost &ob);
};

class Tank : virtual public Enemy {
protected:
    int Armor;
    bool hasShield;
    int ShieldDuration;
    int ShieldAmount;

public:
    Tank(int armor = 20, int shieldDuration = 5, int shieldAmount = 50,
         int hp = 300, int speed = 1, int range = 2);

    Tank(const Tank &ob);

    virtual ~Tank();

    virtual void ActivateShield();
    virtual void UpdateShield();
    virtual int CalculateDamageReduction(int incomingDmg);

    int GetArmor() const { return Armor; }
    void SetArmor(int armor) { Armor = armor; }
    bool HasShield() const { return hasShield; }
    int GetShieldAmount() const { return ShieldAmount; }

    Tank& operator=(const Tank &ob);
};

class Totem : public Ghost, public Tank {
protected:
    int TotemRange;
    bool isAnchored;
    int AuraStrength;

public:
    Totem(int totemRange = 5, int auraStrength = 20);

    Totem(const Totem &ob);

    virtual ~Totem();

    virtual void Attack(Player& player) override;
    virtual void Move(Player& player) override;
    virtual void TakeDamage(int dmg) override;

    virtual void CreateAura();
    virtual void DrainPlayer(Player& player);
    virtual void SummonShadow();

    bool GetIsAnchored() const { return isAnchored; }
    void SetIsAnchored(bool anchored) { isAnchored = anchored; }
    int GetAuraStrength() const { return AuraStrength; }

    Totem& operator=(const Totem &ob);
};

class TankMelee : public Tank, public Melee {
protected:
    int ChargeRange;
    bool isCharging;
    int ShieldBashDmg;

public:
    TankMelee(int chargeRange = 4, int shieldBashDmg = 30);

    TankMelee(const TankMelee &ob);

    virtual ~TankMelee();

    virtual void Attack(Player& player) override;
    virtual void Move(Player& player) override;
    virtual void TakeDamage(int dmg) override;

    virtual void ChargeAttack(Player& player);
    virtual void ShieldBash(Player& player);
    virtual void Parry();

    bool GetIsCharging() const { return isCharging; }
    void SetIsCharging(bool charging) { isCharging = charging; }
    int GetShieldBashDmg() const { return ShieldBashDmg; }

    TankMelee& operator=(const TankMelee &ob);
};
#endif //ENEMY_H
