#ifndef OBJECT_H
#define OBJECT_H
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <windows.h>

class Coord {
protected:
    int x,y;
public:
    Coord(int x=0,int y=0);
    Coord(const Coord& c);
    virtual ~Coord();

    void SetX(int x);
    void SetY(int y);
    int GetX() const;
    int GetY() const;
    void SetCoord(int x,int y);
    void SetCoord(const Coord& c);
    Coord GetCoord() const;

    void IncX(int x=1);
    void IncY(int y=1);
    void DecX(int x=1);
    void DecY(int y=1);

    Coord operator=(const Coord& c);
    friend std::ostream& operator<<(std::ostream& os, const Coord& c);
};

class Object:virtual public Coord {
protected:
    char Icon;
    int Type;
    bool isTakeble=false;
public:
    Object(int x=0,int y=0,char chr=' ',int Type=0,bool isTakeble=false);
    Object(Coord xy,char chr=' ',int Type=0,bool isTakeble=false);
    Object(const Object& ob);
    virtual ~Object();

    virtual void Display();

    void setIcon(char c);
    void setType(int Type);
    char getIcon() const;
    int getType() const;
    bool getIsTakeble() const;
    void setIsTakeble(bool isTakeble);

    bool operator==(const Object& ob) const;
    friend std::ostream& operator<<(std::ostream& os, const Object& ob);
};

class Gate : public Object {
    bool reqKey=false;
    int roomNumber=0;
public:
    Gate(int pointingroom=0,int x=0,int y=0,char chr=' ',bool rk=false,int Type=2);
    Gate(const Gate& ob);

    int getRoomNumber() const;
    bool ReqKey() const;
    void setReqKey(bool rk);
    void setRoomNumber(int roomNumber);
};

class Key : public Object {
    int roomNumber=0;
public:
    Key(int pointingroom=0,int x=0,int y=0,char chr='k',int Type=3);
    Key(const Key& ob);
    void Display();
    int getRoomNumber() const;
    void setRoomNumber(int roomNumber);
};
class Coin : public Object {
    int value=1;
public:
    Coin(int value, int x=0,int y=0,char chr='c',int Type=4);
    Coin(const Coin& ob);
    void Display();
    int getValue() const;
    void setValue(int value);
};

class Slots{
    bool state=false;
    int NrSlots=0;
    int CurentPoz=0;
    std::vector<Object*> slots;
public:
    Slots(int NrSlots=5);
    Slots(const Slots& ob);
    ~Slots();

    bool isOpen() const;
    bool isFull() const;
    void Open();
    void Close();
    void SetState();

    void SetNrSlots(int NrSlots);
    int GetNrSlots() const;
    void SetCurentPoz(int CurentPoz);
    int GetCurentPoz() const;
    void IncNrSlots(int NrSlots=1);

    Object *GetObjectAt(int poz) const;

    void IncCurentPoz(int CurentPoz=1);
    void DecCurentPoz(int CurentPoz=1);

    void AddObject(Object* item);
    void RemoveObject(int poz);

    int SaveCoins();

    Slots operator=(const Slots& ob);
    Slots operator+=(Object* item);
    Slots operator-=(int poz);
    Object* operator[](int poz) const;
    friend std::ostream& operator<<(std::ostream& os, const Slots& ob);
};
#endif //OBJECT_H
