#include "Profile.h"
#include <conio.h>
void Profile::SlowText(std::string line, int time) {
    for (char c : line) {
        std::cout << c;
        if (_kbhit()) {
            time = 0;
        }
        Sleep(time);
    }
    std::cout << std::endl;
}
Profile::Profile(std::string name,int LastRoomNr, int CurrentRoomNr,int NrSlots, int speed, int balance, int dmg, int HP, int intel): LastRoomNr(LastRoomNr),
    CurrentRoomNr(CurrentRoomNr), speed(speed), balance(balance), NrSlots(NrSlots), dmg(dmg), HP(HP), intel(intel), name(name) {
    rooms.clear();
    rooms.push_back(new Room("Rooms/0.mm"));
    rooms.push_back(new Room("Rooms/1.mm"));
    rooms.push_back(new Room("Rooms/2.mm"));
    rooms.push_back(new Room("Rooms/3.mm"));
    rooms.push_back(new Room("Rooms/4.mm"));
    rooms.push_back(new Room("Rooms/5.mm"));
    rooms.push_back(new Room("Rooms/6.mm"));
    for (auto room: rooms) {
        if (room->GetRoomNumber() == CurrentRoomNr) {
            currentRoom = room;
        }
    }
}
int Profile::GetSpeed() {
    return speed;
}
void Profile::SetSpeed(int speed) {
    this->speed = speed;
}
int Profile::GetCurrentRoomNr() {
    return CurrentRoomNr;
}
Room *Profile::GetRoom() {
    return currentRoom;
}
void Profile::MapDraw() {
    currentRoom->Draw();
}
void Profile::Debug() {
    currentRoom->DebugMap();
}
void Profile::SetCurrentRoom(int room) {
    LastRoomNr = CurrentRoomNr;
    CurrentRoomNr = room;
    for (auto room: rooms) {
        if (room->GetRoomNumber() == CurrentRoomNr) {
            currentRoom = room;
        }
    }
}
int Profile::GetLastRoom() {
    return LastRoomNr;
}
void Profile::SetLastRoom(int room) {
    LastRoomNr = CurrentRoomNr;
}
void Profile::incNrSlots(int x) {
    NrSlots += x;
}
int Profile::GetNrSlots() {
    return NrSlots;
}
std::string Profile::GetName() {
    return name;
}
int Profile::GetCoins() {
    return balance;
}
void Profile::IncreaseBalance(int coins) {
    balance += coins;
}
void Profile::incDmg(int x) {
    dmg += x;
}
void Profile::incHP(int x) {
    HP += x;
}
void Profile::incIntel(int x) {
    intel += x;
}
void Profile::incSpeed(int x) {
    speed += x;
}
int Profile::GetDmg() {
    return dmg;
}
int Profile::GetHP() {
    return HP;
}
int Profile::GetIntel() {
    return intel;
}
int Profile::GetBalance() {
    return balance;
}
void Profile::DecreaseBalance(int coins) {
    balance -= coins;
}
std::istream& operator>>(std::istream& is, Profile& profile) {
    system("cls");
    fputs("\e[?25h", stdout); // activeaza cursorul

    std::cout << "COMAND CENTER:\n";
    Profile::SlowText("GOD... TEAM, TEAM, we have a survivor! Please remind us of your name.\n", 70);

    Sleep(50);
    std::cout << "->";
    std::getline(is, profile.name);

    system("cls");
    fputs("\e[?25l", stdout); // dezactiveaza cursorul

    std::cout << "COMAND CENTER:\n";
    Profile::SlowText(profile.name + ", I'm sorry to inform you are the only known survivor.\n", 70);
    Sleep(50);
    Profile::SlowText("I know it's harsh, but you are here with a mission... mankind is putting all its hope in you and you alone."
                      " We cannot send more reinforcements until the next cycle, which is a few years away, and we don't have that much time remaining."
                      " So please, do this for humanity, for your family, for your daughters. We will be here to guide you and help you.", 70);
    Sleep(50);
    Profile::SlowText("Mars is a very dangerous place. We don't know much about your explosion, and we know very little about THE NEST."
                      + profile.name + ", you must retrieve the fuse parts and reactivate the Sun Stabilizer System."
                      " As you well know, the SSS is the foundation of the Solar Alliance; it is its whole core."
                      " We can't afford another civil war, not now, with the Separatist powers rising and more and more systems doubting"
                      " the stability and prosperity of the Solar Alliance.", 70);
    Sleep(50);
    Profile::SlowText("If you are ready, we may proceed. Press any key to let us know you are there and to start the process.", 70);

    system("pause>null");

    profile.LastRoomNr = 0;
    profile.CurrentRoomNr = 0;
    profile.NrSlots = 3;
    profile.speed = 0;
    profile.balance = 0;
    profile.dmg = 1;
    profile.HP = 3;
    profile.intel = 1;

    profile.rooms.clear();
    profile.rooms.push_back(new Room("Rooms/0.mm"));
    profile.rooms.push_back(new Room("Rooms/1.mm"));
    profile.rooms.push_back(new Room("Rooms/2.mm"));
    profile.rooms.push_back(new Room("Rooms/3.mm"));
    profile.rooms.push_back(new Room("Rooms/4.mm"));
    profile.rooms.push_back(new Room("Rooms/5.mm"));
    profile.rooms.push_back(new Room("Rooms/6.mm"));

    for (auto room : profile.rooms) {
        if (room->GetRoomNumber() == profile.CurrentRoomNr) {
            profile.currentRoom = room;
        }
    }

    system("cls");
    return is;
}