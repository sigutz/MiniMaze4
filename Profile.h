
#ifndef PROFILE_H
#define PROFILE_H

#include "Room.h"

class Profile {
    std::string name;
    std::vector<Room *> rooms;
    int NrSlots;

    static void SlowText(std::string line, int time = 30);

protected:
    Room *currentRoom;
    int LastRoomNr, CurrentRoomNr;
    int speed,balance;
    int dmg, HP, intel;
public:
Profile(std::string name="None",int LastRoomNr=0, int CurrentRoomNr=0,int NrSlots=3, int speed=0, int balance=0, int dmg=1, int HP=3, int intel=1);

    int GetSpeed();
    void SetSpeed(int speed);
    int GetCurrentRoomNr();
    Room *GetRoom();
    void MapDraw();
    void Debug();
    void SetCurrentRoom(int room);
    int GetLastRoom();
    void SetLastRoom(int room);

    std::string GetName();
    int GetCoins();
    void IncreaseBalance(int coins);
    void DecreaseBalance(int coins);
    void incNrSlots(int x=1);
    int GetNrSlots();

    void incDmg(int x=1);
    void incHP(int x=1);
    void incIntel(int x=1);
    void incSpeed(int x=10);

    int GetDmg();
    int GetHP();
    int GetIntel();
    int GetBalance();

    friend std::istream& operator>>(std::istream& is, Profile& profile);

};


#endif //PROFILE_H
