#include "Object.h"

Coord::Coord(int x, int y): x(x), y(y) {
}

Coord::Coord(const Coord &ob) {
    x = ob.x;
    y = ob.y;
}

Coord::~Coord() {
}

void Coord::SetX(int x) {
    this->x = x;
}

void Coord::SetY(int y) {
    this->y = y;
}

int Coord::GetX() const {
    return x;
}

int Coord::GetY() const {
    return y;
}

void Coord::SetCoord(int x, int y) {
    this->x = x;
    this->y = y;
}

void Coord::SetCoord(const Coord &c) {
    x = c.x;
    y = c.y;
}

Coord Coord::GetCoord() const {
    return Coord(x, y);
}

void Coord::IncX(int x) {
    this->x += x;
}

void Coord::IncY(int y) {
    this->y += y;
}

void Coord::DecX(int x) {
    this->x -= x;
}

void Coord::DecY(int y) {
    this->y -= y;
}

Coord Coord::operator=(const Coord &c) {
    x = c.x;
    y = c.y;
    return *this;
}

std::ostream &operator<<(std::ostream &os, const Coord &c) {
    os << "(" << c.x << "," << c.y << ")";
    return os;
}

Object::Object(int x, int y, char chr, int Type, bool isTakeble): Coord(x, y), Icon(chr), Type(Type),
                                                                  isTakeble(isTakeble) {
}

Object::Object(Coord xy, char chr, int Type, bool isTakeble): Coord(xy), Icon(chr), Type(Type), isTakeble(isTakeble) {
}

Object::Object(const Object &ob): Coord(ob), Icon(ob.Icon), Type(ob.Type), isTakeble(ob.isTakeble) {
}

Object::~Object() {
}

void Object::setIcon(char c) {
    Icon = c;
}

void Object::setType(int Type) {
    this->Type = Type;
}

char Object::getIcon() const {
    return Icon;
}

int Object::getType() const {
    return Type;
}

bool Object::getIsTakeble() const {
    return isTakeble;
}

void Object::setIsTakeble(bool isTakeble) {
    this->isTakeble = isTakeble;
}

bool Object::operator==(const Object &ob) const {
    return Type == ob.Type;
}

std::ostream &operator<<(std::ostream &os, const Object &ob) {
    os << ob.Icon;
    return os;
}

Gate::Gate(int pointingroom, int x, int y, char chr, bool rk, int Type): Coord(x, y), Object(x, y, chr, Type),
                                                                         reqKey(rk), roomNumber(pointingroom) {
}

Gate::Gate(const Gate &ob): Object(static_cast<const Object &>(ob)), reqKey(ob.reqKey), roomNumber(ob.roomNumber) {
}

int Gate::getRoomNumber() const {
    return roomNumber;
}

bool Gate::ReqKey() const {
    return reqKey;
}

void Gate::setReqKey(bool rk) {
    reqKey = rk;
}

void Gate::setRoomNumber(int roomNumber) {
    this->roomNumber = roomNumber;
}

Key::Key(int pointingroom, int x, int y, char chr, int Type):Coord(x, y), Object(x, y, chr, Type, true), roomNumber(pointingroom) {
}

Key::Key(const Key &ob): Object(static_cast<const Object &>(ob)), roomNumber(ob.roomNumber) {
}

int Key::getRoomNumber() const {
    return roomNumber;
}

void Key::setRoomNumber(int roomNumber) {
    this->roomNumber = roomNumber;
}

Coin::Coin(int value, int x, int y, char chr, int Type):Coord(x, y), Object(x, y, chr, Type, true), value(value) {
}

Coin::Coin(const Coin &ob): Object(static_cast<const Object &>(ob)), value(ob.value) {
}

int Coin::getValue() const {
    return value;
}

void Coin::setValue(int value) {
    this->value = value;
}

Slots::Slots(int NrSlots) {
    this->NrSlots = NrSlots;
    for (int i = 0; i < NrSlots; i++) {
        slots.push_back(new Object(0, 0, '-'));
    }
}

Slots::Slots(const Slots &ob) {
    NrSlots = ob.NrSlots;
    CurentPoz = ob.CurentPoz;
    state = ob.state;
    for (int i = 0; i < NrSlots; i++) {
        slots.push_back(new Object(*ob.slots[i]));
    }
}

Slots::~Slots() {
    for (auto &slot: slots) {
        delete slot;
    }
}

bool Slots::isOpen() const {
    return state;
}

bool Slots::isFull() const {
    for (int i = 0; i < NrSlots; i++) {
        if (slots[i]->getType() == 0)
            return false;
    }
    return true;
}

void Slots::Open() {
    state = true;
}

void Slots::Close() {
    state = false;
}

void Slots::SetNrSlots(int NrSlots) {
    for (int i = this->NrSlots - 1; i < NrSlots; i++) {
        slots.push_back(new Object(0, 0, '-'));
    }
    this->NrSlots = NrSlots;
}

int Slots::GetNrSlots() const {
    return NrSlots;
}

void Slots::SetCurentPoz(int CurentPoz) {
    this->CurentPoz = CurentPoz;
}

int Slots::GetCurentPoz() const {
    return CurentPoz;
}

void Slots::IncNrSlots(int NrSlots) {
    this->NrSlots += NrSlots;
    for (int i = 0; i < NrSlots; i++) {
        slots.push_back(new Object(0, 0, '-'));
    }
}

void Slots::AddObject(Object *item) {
    for (int i = 0; i < NrSlots; i++) {
        if (*slots[i] == Object(0, 0, '-')) {
            delete slots[i]; // Delete the existing empty slot

            if (item->getType() == 3) {
                // Key
                Key *key = dynamic_cast<Key *>(item);
                if (key) slots[i] = new Key(*key);
                else slots[i] = new Object(*item);
            } else if (item->getType() == 4) {
                // Coin
                Coin *coin = dynamic_cast<Coin *>(item);
                if (coin) slots[i] = new Coin(*coin);
                else slots[i] = new Object(*item);
            } else {
                slots[i] = new Object(*item);
            }

            CurentPoz = i;
            return;
        }
    }
}

void Slots::RemoveObject(int poz) {
    if (poz >= 0 && poz < NrSlots) {
        delete slots[poz];
        slots[poz] = new Object(0, 0, '-');
    } else {
        std::cout << "Invalid position!" << std::endl;
    }
}

Slots Slots::operator=(const Slots &ob) {
    if (this != &ob) {
        for (auto &slot: slots) {
            delete slot;
        }
        slots.clear();
        NrSlots = ob.NrSlots;
        for (int i = 0; i < NrSlots; i++) {
            slots.push_back(new Object(*ob.slots[i]));
        }
    }
    return *this;
}

std::ostream &operator<<(std::ostream &os, const Slots &ob) {
    if (ob.state) {
        os << "<  ";
        for (int i = 0; i < ob.NrSlots; i++)
            if (i == ob.CurentPoz)
                os << "[ " << *ob.slots[i] << " ]";
            else
                os << "[" << *ob.slots[i] << "]";
        os << "  >" << '\n';
    } else {
        os << "<";
        for (int i = 0; i < ob.NrSlots; i++)
            if (i == ob.CurentPoz)
                os << "[ " << *ob.slots[i] << " ]";
            else
                os << "[" << *ob.slots[i] << "]";
        os << '>' << '\n';
    }
    if (ob.slots[ob.CurentPoz]->getType())
        ob.slots[ob.CurentPoz]->Display();
    else {
        os<<"\n";
    }
    return os;
}

void Slots::SetState() {
    if (state)
        state = false;
    else state = true;
}

Object *Slots::operator[](int poz) const {
    if (poz >= 0 && poz < NrSlots) {
        return slots[poz];
    } else {
        std::cout << "Invalid position!" << std::endl;
        return nullptr;
    }
}

void Slots::IncCurentPoz(int CurentPoz) {
    this->CurentPoz = (this->CurentPoz + CurentPoz) % NrSlots;
}

void Slots::DecCurentPoz(int CurentPoz) {
    this->CurentPoz = (this->CurentPoz - CurentPoz + NrSlots) % NrSlots;
}
void Object::Display() {
    std::cout<<x<<' '<<y<<'\n';
    std::cout<<Type<<'\n';
}
void Key::Display() {
    std::cout<<"Key for the door number: "<<roomNumber<<'\n';
}
void Coin::Display() {
    std::cout<<"Coin of value: "<<value<<'\n';
}
Object* Slots::GetObjectAt(int poz) const {
        return slots[poz];
}
int Slots::SaveCoins() {
    int totalValue = 0;
    for (int i = 0; i < NrSlots; i++) {
        if (slots[i]->getType() == 4) { // Coin type
            Coin *coin = dynamic_cast<Coin *>(slots[i]);
            if (coin) {
                totalValue += coin->getValue();
            }
            RemoveObject(i);
        }
    }
    std::cout<<totalValue<<" coins added to the bank\n";
    return totalValue;
}