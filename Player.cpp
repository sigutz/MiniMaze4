//
// Created by silvi on 5/4/2025.
//

#include "Player.h"

Player::Player(Profile &motherprofile): Profile(motherprofile), Object(0,0,'?',-1) {
    Coord OriginalPoz;
    if (LastRoomNr != CurrentRoomNr) {
        OriginalPoz = currentRoom->GetGate(LastRoomNr);
        if (!OriginalPoz.GetX()) {
            y = OriginalPoz.GetY();
            x = 1;
            Direction = 0;
        } else if (!OriginalPoz.GetY()) {
            y = 1;
            x = OriginalPoz.GetX();
            Direction = 2;
        } else if (OriginalPoz.GetY() == currentRoom->GetHeight()-2) {
            y = OriginalPoz.GetY() - 1;
            x = OriginalPoz.GetX();
            Direction = 1;
        } else {
            y = OriginalPoz.GetY();
            x = OriginalPoz.GetX() - 1;
            Direction = 3;
        }
    } else {
        y = currentRoom->GetHeight() / 2 - 1;
        x = currentRoom->GetWidth() / 2;
        Direction = 0;
    }
    setIcon(chr[Direction]);
    currentRoom->SetObjectAt(x, y, this);
}

void Player::ClearPlayer() {
    currentRoom->ClearPlayer();
}

int Player::GetDirection() {
    return Direction;
}

void Player::SetDirectionE() {
    Direction = 0;
    setIcon(chr[Direction]);
}

void Player::SetDirectionN() {
    Direction = 1;
    setIcon(chr[Direction]);
}

void Player::SetDirectionS() {
    Direction = 2;
    setIcon(chr[Direction]);
}

void Player::SetDirectionW() {
    Direction = 3;
    setIcon(chr[Direction]);
}

void Player::MoveE(int &colisionType) {
    if (Direction != 0) {
        SetDirectionE();
        return;
    }
    colisionType = currentRoom->GetBlockType(x + 1, y);
    if (!colisionType)
        currentRoom->SwapObjects(x, y, x + 1, y);

}

void Player::MoveN(int &colisionType) {

    if (Direction != 1) {
        SetDirectionN();
        return;
    }
    colisionType = currentRoom->GetBlockType(x, y - 1);
    if (!colisionType)
        currentRoom->SwapObjects(x, y, x, y - 1);

}

void Player::MoveS(int &colisionType) {
    if (Direction != 2) {
        SetDirectionS();
        return;
    }
    colisionType = currentRoom->GetBlockType(x, y + 1);
    if (!colisionType)
        currentRoom->SwapObjects(x, y, x, y + 1);

}

void Player::MoveW(int &colisionType) {

    if (Direction != 3) {
        SetDirectionW();
        return;
    }
    colisionType = currentRoom->GetBlockType(x - 1, y);
    if (!colisionType)
        currentRoom->SwapObjects(x, y, x - 1, y);

}
Coord Player::GetPlayerFront() {
        Coord Temp(0,0);
        switch (Direction) {
            case 0:
                Temp.SetX(x + 1);
                Temp.SetY(y);
                break;
            case 1:
                Temp.SetX(x);
                Temp.SetY(y - 1);
                break;
            case 2:
                Temp.SetX(x);
                Temp.SetY(y+1);
                break;
            case 3:
                Temp.SetX(x-1);
                Temp.SetY(y);
                break;
        }
        return Temp;
}
void Player::Attack() {
    Coord target = GetPlayerFront();
    Object* targetObject = currentRoom->GetObjectAt(target);
    if (targetObject && targetObject->getType() == 10) {
        Enemy* enemy = dynamic_cast<Enemy*>(targetObject);
        if (enemy) {
            enemy->TakeDamage(dmg);
            std::cout << "You hit the enemy for " << dmg << " damage!" << std::endl;
            if (!enemy->IsAlive()) {
                std::cout << "Enemy defeated!" << std::endl;
                currentRoom->SetObjectAt(target.GetX(), target.GetY(), new Object());
            }
        }
    } else {
        std::cout << "Nothing to attack!" << std::endl;
    }
}
void Player::TakeDamage(int damage) {
    HP -= damage;
    std::cout << "You took " << damage << " damage! HP: " << HP << std::endl;
    if (HP < 0) HP = 0;
}

bool Player::IsAlive() const {
    return HP > 0;
}
