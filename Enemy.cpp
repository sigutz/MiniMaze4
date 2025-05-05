
#include "Enemy.h"
#include "Player.h"
#include <cmath>
int Enemy::totalEnemiesDefeated = 0;
Enemy::Enemy(int hp, int speed, int range, int x, int y, char icon)
    : Object(x, y, icon, 10, false), Health(hp), Speed(speed),
      PlayerRange(range), maxHealth(hp) {}

Enemy::Enemy(const Enemy &ob)
    : Object(static_cast<const Object&>(ob)), Health(ob.Health), Speed(ob.Speed),
      PlayerRange(ob.PlayerRange), maxHealth(ob.maxHealth) {}

Enemy::~Enemy() {
    if (Health <= 0) {
        totalEnemiesDefeated++;
    }
}

void Enemy::Display() const {
    std::cout << "Enemy Type: " << getType() << "\n";
    std::cout << "Health: " << Health << "/" << maxHealth << "\n";
    std::cout << "Speed: " << Speed << "\n";
    std::cout << "Range: " << PlayerRange << "\n";
    std::cout << "Position: (" << GetX() << ", " << GetY() << ")\n";
}

int Enemy::GetHealth() const { return Health; }
int Enemy::GetSpeed() const { return Speed; }
int Enemy::GetPlayerRange() const { return PlayerRange; }
void Enemy::SetHealth(int health) { Health = health; }
void Enemy::SetSpeed(int speed) { Speed = speed; }
void Enemy::SetPlayerRange(int range) { PlayerRange = range; }
int Enemy::GetTotalEnemies() { return totalEnemiesDefeated; }

bool Enemy::IsAlive() const {
    return Health > 0;
}

Enemy &Enemy::operator=(const Enemy &ob) {
    if (this != &ob) {
        Object::operator=(ob);
        Health = ob.Health;
        Speed = ob.Speed;
        PlayerRange = ob.PlayerRange;
        maxHealth = ob.maxHealth;
    }
    return *this;
}

Range::Range(int rangeDmg, int rangeActions, int hp, int speed, int range)
    : Enemy(hp, speed, range, 0, 0, 'R'), RangeDmg(rangeDmg), RangeActions(rangeActions) {}

Range::Range(const Range &ob)
    : Enemy(ob), RangeDmg(ob.RangeDmg), RangeActions(ob.RangeActions) {}

Range::~Range() {}

void Range::RangedAttack(Player &player) {
    int distance = std::abs(player.GetX() - GetX()) + std::abs(player.GetY() - GetY());
    if (distance <= PlayerRange) {
        player.TakeDamage(RangeDmg);
        std::cout << "Range enemy attacks for " << RangeDmg << " damage!" << std::endl;
    }
}

void Range::AimAtPlayer(Player &player) {
    int dx = player.GetX() - GetX();
    int dy = player.GetY() - GetY();

    if (dx > 0) setIcon('>');
    else if (dx < 0) setIcon('<');
    else if (dy > 0) setIcon('v');
    else if (dy < 0) setIcon('^');
}

Melee::Melee(int meleeDmg, int attacksBeforeCritical, int hp, int speed, int range)
    : Enemy(hp, speed, range, 0, 0, 'M'), MeleeDmg(meleeDmg),
      AttacksBeforeCritical(attacksBeforeCritical), currentAttacks(0) {}

Melee::Melee(const Melee &ob)
    : Enemy(ob), MeleeDmg(ob.MeleeDmg),
      AttacksBeforeCritical(ob.AttacksBeforeCritical),
      currentAttacks(ob.currentAttacks) {}

Melee::~Melee() {}

void Melee::MeleeAttack(Player &player) {
    int distance = std::abs(player.GetX() - GetX()) + std::abs(player.GetY() - GetY());
    if (distance <= PlayerRange) {
        currentAttacks++;
        if (IsCriticalHit()) {
            player.TakeDamage(MeleeDmg * 2);
            std::cout << "Critical hit! Melee enemy deals " << (MeleeDmg * 2) << " damage!" << std::endl;
            currentAttacks = 0;
        } else {
            player.TakeDamage(MeleeDmg);
            std::cout << "Melee enemy deals " << MeleeDmg << " damage!" << std::endl;
        }
    }
}

bool Melee::IsCriticalHit() {
    return currentAttacks >= AttacksBeforeCritical;
}

Melee& Melee::operator=(const Melee &ob) {
    if (this != &ob) {
        Enemy::operator=(ob);
        MeleeDmg = ob.MeleeDmg;
        AttacksBeforeCritical = ob.AttacksBeforeCritical;
        currentAttacks = ob.currentAttacks;
    }
    return *this;
}
Ghost::Ghost(int dmgReduction, int intelReduction, int lightReduction,
             int speedReduction, int rangeReduction, int proximity,
             int hp, int speed, int range)
    : Enemy(hp, speed, range, 0, 0, 'G'), DmgReduction(dmgReduction),
      IntelReduction(intelReduction), LightReduction(lightReduction),
      SpeedReduction(speedReduction), RangeReduction(rangeReduction),
      Proximity(proximity) {}

Ghost::Ghost(const Ghost &ob)
    : Enemy(ob), DmgReduction(ob.DmgReduction), IntelReduction(ob.IntelReduction),
      LightReduction(ob.LightReduction), SpeedReduction(ob.SpeedReduction),
      RangeReduction(ob.RangeReduction), Proximity(ob.Proximity) {}

Ghost::~Ghost() {}

void Ghost::ApplyDebuffs(Player &player) {
    std::cout << "Ghost applies debuffs to the player!" << std::endl;
}
bool Ghost::IsInProximity(Player &player) {
    int distance = std::abs(player.GetX() - GetX()) + std::abs(player.GetY() - GetY());
    return distance <= Proximity;
}

void Ghost::PhaseThrough() {
    std::cout << "Ghost phases through solid objects!" << std::endl;
}
Ghost& Ghost::operator=(const Ghost &ob) {
    if (this != &ob) {
        Enemy::operator=(ob);
        DmgReduction = ob.DmgReduction;
        IntelReduction = ob.IntelReduction;
        LightReduction = ob.LightReduction;
        SpeedReduction = ob.SpeedReduction;
        RangeReduction = ob.RangeReduction;
        Proximity = ob.Proximity;
    }
    return *this;
}

Tank::Tank(int armor, int shieldDuration, int shieldAmount, int hp, int speed, int range)
    : Enemy(hp, speed, range, 0, 0, 'T'), Armor(armor), hasShield(false),
      ShieldDuration(shieldDuration), ShieldAmount(shieldAmount) {}

Tank::Tank(const Tank &ob)
    : Enemy(ob), Armor(ob.Armor), hasShield(ob.hasShield),
      ShieldDuration(ob.ShieldDuration), ShieldAmount(ob.ShieldAmount) {}

Tank::~Tank() {}

void Tank::ActivateShield() {
    hasShield = true;
    std::cout << "Tank activated its shield!" << std::endl;
}

void Tank::UpdateShield() {
    if (hasShield) {
        ShieldDuration--;
        if (ShieldDuration <= 0) {
            hasShield = false;
            ShieldDuration = 5; // Reset duration
            std::cout << "Tank's shield has dissipated!" << std::endl;
        }
    }
}

int Tank::CalculateDamageReduction(int incomingDmg) {
    int reducedDmg = incomingDmg - Armor;
    if (hasShield) {
        if (ShieldAmount >= reducedDmg) {
            ShieldAmount -= reducedDmg;
            reducedDmg = 0;
        } else {
            reducedDmg -= ShieldAmount;
            ShieldAmount = 0;
            hasShield = false;
        }
    }
    return reducedDmg > 0 ? reducedDmg : 0;
}

Tank& Tank::operator=(const Tank &ob) {
    if (this != &ob) {
        Enemy::operator=(ob);
        Armor = ob.Armor;
        hasShield = ob.hasShield;
        ShieldDuration = ob.ShieldDuration;
        ShieldAmount = ob.ShieldAmount;
    }
    return *this;
}

Totem::Totem(int totemRange, int auraStrength)
    : Enemy(200, 0, 6, 0, 0, 'O'), Ghost(), Tank(), TotemRange(totemRange),
      isAnchored(true), AuraStrength(auraStrength) {
    setIcon('&'); // Totem icon
}

Totem::Totem(const Totem &ob)
    : Enemy(ob), Ghost(ob), Tank(ob), TotemRange(ob.TotemRange),
      isAnchored(ob.isAnchored), AuraStrength(ob.AuraStrength) {}

Totem::~Totem() {}

void Totem::Attack(Player& player) {
    if (IsInProximity(player)) {
        ApplyDebuffs(player);
        CreateAura();
        DrainPlayer(player);
    } else {
        SummonShadow();
    }
}

void Totem::Move(Player& player) {
    if (!isAnchored) {
        // Totemele nu prea se misca :)) ma gandesc sa fac ceva logica cu teleport on hit
    }
}
void Totem::TakeDamage(int dmg) {
    int reducedDmg = CalculateDamageReduction(dmg);
    reducedDmg -= DmgReduction / 2;
    if (reducedDmg < 0) reducedDmg = 0;

    Health -= reducedDmg;
    if (Health < 0) Health = 0;
}
void Totem::CreateAura() {
    std::cout << "Totem creates a corruption aura!" << std::endl;
}
void Totem::DrainPlayer(Player& player) {
    int distance = std::abs(player.GetX() - GetX()) + std::abs(player.GetY() - GetY());
    if (distance <= TotemRange) {
        int drainAmount = AuraStrength / (distance + 1);
        player.TakeDamage(drainAmount);
        std::cout << "Totem drains " << drainAmount << " health!" << std::endl;
    }
}

void Totem::SummonShadow() {
    std::cout << "Totem summons a shadow minion!" << std::endl;
}

Totem& Totem::operator=(const Totem &ob) {
    if (this != &ob) {
        Ghost::operator=(ob);
        Tank::operator=(ob);
        TotemRange = ob.TotemRange;
        isAnchored = ob.isAnchored;
        AuraStrength = ob.AuraStrength;
    }
    return *this;
}
TankMelee::TankMelee(int chargeRange, int shieldBashDmg)
    : Enemy(400, 2, 2, 0, 0, 'W'), Tank(), Melee(), ChargeRange(chargeRange),
      isCharging(false), ShieldBashDmg(shieldBashDmg) {
    setIcon('+');
}
TankMelee::TankMelee(const TankMelee &ob)
    : Enemy(ob), Tank(ob), Melee(ob), ChargeRange(ob.ChargeRange),
      isCharging(ob.isCharging), ShieldBashDmg(ob.ShieldBashDmg) {}

TankMelee::~TankMelee() {}

void TankMelee::Attack(Player& player) {
    int distance = std::abs(player.GetX() - GetX()) + std::abs(player.GetY() - GetY());

    if (distance <= 1) {
        if (IsCriticalHit()) {
            ShieldBash(player);
        } else {
            MeleeAttack(player);
        }
    } else if (distance <= ChargeRange && !isCharging) {
        ChargeAttack(player);
    }

    UpdateShield();
}

void TankMelee::Move(Player& player) {
    int dx = player.GetX() - GetX();
    int dy = player.GetY() - GetY();
    int distance = std::abs(dx) + std::abs(dy);

    if (distance > 1 && !isCharging) {
        if (std::abs(dx) > std::abs(dy)) {
            if (dx > 0) SetX(GetX() + 1);
            else SetX(GetX() - 1);
        } else {
            if (dy > 0) SetY(GetY() + 1);
            else SetY(GetY() - 1);
        }
    }
}
void TankMelee::TakeDamage(int dmg) {
    int reducedDmg = CalculateDamageReduction(dmg);
    Health -= reducedDmg;
    if (Health < 0) Health = 0;
}

void TankMelee::ChargeAttack(Player& player) {
    isCharging = true;
    std::cout << "TankMelee charges towards you!" << std::endl;
    player.TakeDamage(MeleeDmg * 2);
    isCharging = false;
}

void TankMelee::ShieldBash(Player& player) {
    std::cout << "TankMelee performs a devastating shield bash!" << std::endl;
    player.TakeDamage(ShieldBashDmg + MeleeDmg);
}

void TankMelee::Parry() {
    ActivateShield();
    std::cout << "TankMelee enters a defensive stance!" << std::endl;
    Armor += 10;
}

TankMelee& TankMelee::operator=(const TankMelee &ob) {
    if (this != &ob) {
        Tank::operator=(ob);
        Melee::operator=(ob);
        ChargeRange = ob.ChargeRange;
        isCharging = ob.isCharging;
        ShieldBashDmg = ob.ShieldBashDmg;
    }
    return *this;
}