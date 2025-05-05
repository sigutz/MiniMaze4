//
// Created by silvi on 5/4/2025.
//

#ifndef PLAYER_H
#define PLAYER_H

#include "Profile.h"

class Player : public Profile, public Object {
    int Direction;
    char chr[4] = {'>', '^', 'v', '<'};
    float movementAcumulator = 0.0; // asta este pentru viitor sa nu uit eu, o folosesc pentru fps uri
public:
    Player(Profile &motherprofile);
    void ClearPlayer();
    int GetDirection();
    void SetDirectionE();
    void SetDirectionN();
    void SetDirectionS();
    void SetDirectionW();
    void MoveE(int &colisionType);
    void MoveN(int &colisionType);
    void MoveS(int &colisionType);
    void MoveW(int &colisionType);
    Coord GetPlayerFront();

    void Attack();
    void TakeDamage(int damage);
    bool IsAlive() const;
};


#endif //PLAYER_H
