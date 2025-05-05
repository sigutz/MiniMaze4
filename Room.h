#ifndef ROOM_H
#define ROOM_H
#include "Object.h"
#include "Enemy.h"
class Room {
    int roomNumber;
    std::vector<std::vector<Object*>> Map;
    std::vector<std::pair<int,std::string>> legend;
    std::vector<std::string> consoleInfo;
    std::vector<Gate *> gates;
public:
    Room(std::string filePath);
    ~Room();

    void Draw() const;
    void DebugMap();
    void SetObjectAt(int x, int y, Object* ob);
    void SwapObjects(int x1, int y1, int x2, int y2);

    int GetRoomNumber() const;
    int GetBlockType(int x, int y) const;
    int GetHeight() const;
    int GetWidth() const;
    void DisplayConsoleInfo() const;
    int GetDisplayConsoleInfoSize() const;

    void ClearPlayer();

    Coord GetGate(int RoomNumber);
    Object* GetObjectAt(int x, int y) const;
    Object* GetObjectAt(Coord c) const;
};



#endif //ROOM_H
