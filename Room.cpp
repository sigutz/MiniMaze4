#include "Room.h"

Room::Room(std::string filePath) {
    try {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file: " + filePath);
        }
        std::string line, location;
        while (std::getline(file, line)) {
            location = line.substr(0, line.find(':'));
            std::string value = line.substr(line.find(':') + 1);
            if (location == "RoomNumber") {
                roomNumber = stoi(value);
            } else if (location == "Legend")
                while (std::getline(file, line))
                    if (line.substr(0, 4) == "DONE") break;
                    else
                        legend.push_back(std::make_pair(stoi(line.substr(0, line.find(":"))),
                                                        line.substr(line.find(":") + 1)));
            else if (location == "ConsoleInfo") {
                while (std::getline(file, line))
                    if (line.substr(0, 4) == "DONE") break;
                    else consoleInfo.push_back(line);
            }
            else if (location == "Map") {
                int y = 0;
                while (std::getline(file, line)) {
                    std::vector<Object *> row;
                    std::istringstream iss(line);
                    int nr;
                    int x = 0;
                    while (iss >> nr) {
                        if (!nr)
                            row.push_back(new Object(x, y));
                        else if (nr == 1) {
                            // wall
                            if (x % 2)
                                row.push_back(new Object(x, y, '=', 1));
                            else
                                row.push_back(new Object(x, y, '#', 1));
                        } else {
                            for (auto &l: legend) {
                                if (l.first == nr) {
                                    std::string TypeOfObject = l.second.substr(0, l.second.find('='));
                                    std::string Description = l.second.substr(l.second.find('=') + 1);
                                    if (TypeOfObject == "Gate") {
                                        int pointingRoom = 0;
                                        bool reqKey = false;
                                        char icon = Description[0];
                                        Description = Description.substr(1);
                                        if (Description[0] == '*') {
                                            reqKey = true;
                                            Description = Description.substr(1);
                                        }
                                        std::string stringNum;
                                        for (auto &c: Description) {
                                            if (isdigit(c))
                                                stringNum += c;
                                        }
                                        pointingRoom = stoi(stringNum);
                                        if (Description[0] =='-')
                                            pointingRoom*=-1;
                                        Gate *gt = new Gate(pointingRoom, x, y, icon, reqKey);
                                        row.push_back(gt);
                                        gates.push_back(gt);
                                    } else if (TypeOfObject == "Key") {
                                        int pointingRoom = 0;
                                        std::string stringNum;
                                        for (auto &c: Description) {
                                            if (isdigit(c))
                                                stringNum += c;
                                        }
                                        pointingRoom = stoi(stringNum);
                                        row.push_back(new Key(pointingRoom, x, y));
                                    } else if (TypeOfObject == "Coin") {
                                        int value = 0;
                                        std::string stringNum;
                                        for (auto &c: Description) {
                                            if (isdigit(c))
                                                stringNum += c;
                                        }
                                        value = stoi(stringNum);
                                        row.push_back(new Coin(value, x, y));
                                    } else if (TypeOfObject == "Bank") {
                                        row.push_back(new Object(x, y, '$', 5));
                                    } else if (TypeOfObject == "BuyMenu") {
                                        row.push_back(new Object(x, y, 'M', 6));
                                    } else {
                                        row.push_back(new Object(x, y, '?', nr));
                                    }
                                    break;
                                }
                            }
                        }
                        x++;
                    }
                    Map.push_back(row);
                    y++;
                }
            } else {
                std::cout << "Unknown location: " << location << std::endl;
                system("pause");
            }
        }
        file.close();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return;
    }
}

Room::~Room() {
    for (auto &row: Map) {
        for (auto &obj: row) {
            delete obj;
        }
        row.clear();
    }
    Map.clear();
}

void Room::Draw() const {
    for (const auto &row: Map) {
        for (const auto &obj: row) {
            std::cout << *obj;
        }
        std::cout << '\n';
    }
}

void Room::DebugMap() {
    for (const auto &row: Map) {
        for (const auto &obj: row) {
            std::cout << obj->getType() << ' ';
        }
        std::cout << '\n';
    }
}

void Room::SetObjectAt(int x, int y, Object *ob) {
    delete Map[y][x];
    Map[y][x] = ob;
}

void Room::SwapObjects(int x1, int y1, int x2, int y2) {
    Map[y1][x1]->SetX(x2);
    Map[y1][x1]->SetY(y2);
    Map[y2][x2]->SetX(x1);
    Map[y2][x2]->SetY(y1);
    std::swap(Map[y1][x1], Map[y2][x2]);
}

int Room::GetRoomNumber() const {
    return roomNumber;
}

int Room::GetBlockType(int x, int y) const {
    if (y >= 0 && y < Map.size() && x >= 0 && x < Map[y].size()) {
        return Map[y][x]->getType();
    }
    return -1;
}

int Room::GetHeight() const {
    return Map.size();
}

int Room::GetWidth() const {
    return Map.size() > 0 ? Map[0].size() : 0;
}

void Room::ClearPlayer() {
    for (auto &row: Map) {
        for (auto &obj: row) {
            if (obj->getType() == -1) {
                delete obj;
                obj = new Object(0, 0);
            }
        }
    }
}

Coord Room::GetGate(int RoomNumber) {
    for (auto &gate: gates) {
        if (gate->getRoomNumber() == RoomNumber) {
            return gate->GetCoord();
        }
    }
    return Coord(0, 0);
}

Object *Room::GetObjectAt(int x, int y) const {
    return Map[y][x];
}

Object *Room::GetObjectAt(Coord c) const {
    return Map[c.GetY()][c.GetX()];
}

void Room::DisplayConsoleInfo() const {
    for (const auto &info: consoleInfo) {
        std::cout << info << '\n';
    }
}
int Room::GetDisplayConsoleInfoSize() const {
    return consoleInfo.size();
}
