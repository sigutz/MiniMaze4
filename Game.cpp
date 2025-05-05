//
// Created by silvi on 5/4/2025.
//

#include "Game.h"
#include <conio.h>

void Game::ConsoleGoToXY(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord); // Seteaza cursorul la coordonatele x, y
}

void Game::GetConsoleSize(int &width, int &height) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

void Game::ClearConsoleLine() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    COORD coord = {0, csbi.dwCursorPosition.Y};
    DWORD written;
    FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', csbi.srWindow.Right - csbi.srWindow.Left + 1,
                               coord, &written);
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Game::PlayRoom(Slots &inv) {
    system("cls");
    bool inGame = true;
    int width, height;
    GetConsoleSize(width, height);
    Player P(*currentProfile);
    Room *currentRoom = currentProfile->GetRoom();
    int blockType = 0;

    ConsoleGoToXY(width - 20, 0);
    std::cout << currentProfile->GetName();
    ConsoleGoToXY(width - 20, 1);
    std::cout << "Balance:" << currentProfile->GetCoins() << "c\n";
    ConsoleGoToXY(width - 20, 2);
    std::cout << "HP: " << P.GetHP() << "/" << currentProfile->GetHP() << std::endl;
    ConsoleGoToXY(width - 20, 3);
    std::cout << "Damage: " << P.GetDmg() << std::endl;

    ConsoleGoToXY(0, height - currentRoom->GetDisplayConsoleInfoSize() - 2);
    currentRoom->DisplayConsoleInfo();
    ConsoleGoToXY(0, 0);
    currentProfile->MapDraw();
    std::cout << inv;

    while (inGame) {
        ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &inputRecord, 1, &events);
        if (inputRecord.EventType == KEY_EVENT && inputRecord.Event.KeyEvent.bKeyDown) {
            if (!inv.isOpen())
                switch (inputRecord.Event.KeyEvent.wVirtualKeyCode) {
                    case VK_UP:
                    case 0x57:
                        P.MoveN(blockType);
                        break;
                    case VK_DOWN:
                    case 0x53:
                        P.MoveS(blockType);
                        break;
                    case VK_LEFT:
                    case 0x41:
                        P.MoveW(blockType);
                        break;
                    case VK_RIGHT:
                    case 0x44:
                        P.MoveE(blockType);
                        break;
                    case 0x49: // I
                        inv.Open();
                        break;
                    case 0x45: // E
                    {
                        ClearConsoleLine();
                        Object *temp = currentRoom->GetObjectAt(P.GetPlayerFront());

                        if (temp->getIsTakeble()) {
                            if (!inv.isFull()) {
                                inv.AddObject(temp);
                                currentRoom->SetObjectAt(P.GetPlayerFront().GetX(), P.GetPlayerFront().GetY(),
                                                         new Object());
                            } else {
                                std::cout << "Inventory full";
                            }
                        } else {
                            if (temp->getType() == 2) {
                                Gate *gate = dynamic_cast<Gate *>(temp);
                                if (gate && gate->ReqKey()) {
                                    Object *CPO = inv.GetObjectAt(inv.GetCurentPoz()); // Obiectul de pe pozitia curenta
                                    if (CPO->getType() == 3) {
                                        Key *keyItem = dynamic_cast<Key *>(CPO);
                                        if (keyItem && keyItem->getRoomNumber() == gate->getRoomNumber()) {
                                            gate->setReqKey(false);
                                            inv.RemoveObject(inv.GetCurentPoz());
                                            currentProfile->SetCurrentRoom(gate->getRoomNumber());
                                            inGame = false;
                                            currentRoom->ClearPlayer();
                                        } else {
                                            std::cout << "You need a key to unlock this gate\n";
                                        }
                                    } else {
                                        std::cout << "You need a key to unlock this gate\n";
                                    }
                                } else if (gate) {
                                    if (gate->getRoomNumber() < 0)
                                        running = false;
                                    currentRoom->ClearPlayer();
                                    currentProfile->SetCurrentRoom(gate->getRoomNumber());
                                    inGame = false;
                                }
                            }
                            if (temp->getType() == 5) {
                                currentProfile->IncreaseBalance(inv.SaveCoins());
                            }
                            if (temp->getType() == 6) {
                                bool buying = true;
                                int x;
                                while (buying) {
                                    while (true) {
                                        system("cls");
                                        std::cout << "Balance:" << currentProfile->GetCoins() << "c\n";
                                        std::cout << "Buy Menu:\n";
                                        std::cout << "1. Buy Slot\n" << "Current Slots: " << inv.GetNrSlots() << '\n'
                                                << "Price: " << 10 * inv.GetNrSlots() << '\n';
                                        std::cout << "2. Increase Speed\n" << "Current Speed: " << currentProfile->
                                                GetSpeed() << '\n'
                                                << "Price: 5\n";
                                        std::cout << "3. Increase HP\n" << "Current HP: " << currentProfile->GetHP() <<
                                                '\n'
                                                << "Price: 5\n";
                                        std::cout << "4. Increase Damage\n" << "Current Damage: " << currentProfile->
                                                GetDmg() << '\n'
                                                << "Price: 5\n";
                                        std::cout << "0. Back\n";

                                        try {
                                            std::cin >> x;
                                            if (std::cin.fail()) {
                                                throw std::invalid_argument("Invalid input");
                                            }
                                            if (x < 0 || x > 4) {
                                                throw std::out_of_range("Input out of range");
                                            }
                                            break;
                                        } catch (const std::invalid_argument &e) {
                                            std::cout << "Invalid input. Please enter a number between 0 and 4.\n";
                                            system("pause");
                                        }
                                        catch (const std::out_of_range &e) {
                                            std::cout << "Input out of range. Please enter a number between 0 and 4.\n";
                                            system("pause");
                                        }
                                    }


                                    switch (x) {
                                        case 1:
                                            if (currentProfile->GetCoins() >= 10 * inv.GetNrSlots()) {
                                                currentProfile->incNrSlots();
                                                inv.IncNrSlots();
                                                currentProfile->DecreaseBalance(10 * inv.GetNrSlots());
                                            } else {
                                                std::cout << "Not enough coins\n";
                                            }
                                            system("pause");
                                            break;
                                        case 2:
                                            if (currentProfile->GetCoins() >= 5) {
                                                P.incSpeed(5);
                                                currentProfile->incSpeed(5);
                                                currentProfile->DecreaseBalance(5);
                                            } else {
                                                std::cout << "Not enough coins\n";
                                            }
                                            system("pause");
                                            break;
                                        case 3:
                                            if (currentProfile->GetCoins() >= 5) {
                                                P.incHP();
                                                currentProfile->incHP();
                                                currentProfile->DecreaseBalance(5);
                                            } else {
                                                std::cout << "Not enough coins\n";
                                            }
                                            system("pause");
                                            break;
                                        case 4:
                                            if (currentProfile->GetCoins() >= 5) {
                                                P.incDmg();
                                                currentProfile->incDmg();
                                                currentProfile->DecreaseBalance(5);
                                            } else {
                                                std::cout << "Not enough coins\n";
                                            }
                                            system("pause");
                                            break;
                                        case 0:
                                            buying = false;
                                            system("cls");
                                            break;
                                    }
                                }
                            }
                        }
                        break;
                    }
                    case 0x51: // Q
                    {
                        Object *currentItem = inv[inv.GetCurentPoz()];
                        if (currentItem->getType() != 0) {
                            Coord PF = P.GetPlayerFront();
                            Object *temp = currentRoom->GetObjectAt(PF);
                            if (temp->getType() == 0) {
                                Object *Dropped = nullptr;

                                // Create the correct type of object based on the current item's type
                                if (currentItem->getType() == 3) {
                                    // Key
                                    Key *keyItem = dynamic_cast<Key *>(currentItem);
                                    if (keyItem) {
                                        Dropped = new Key(*keyItem);
                                        Object* baseObj = static_cast<Object*>(Dropped);
                                        baseObj->SetCoord(PF);
                                    }
                                } else if (currentItem->getType() == 4) {
                                    // Coin
                                    Coin *coinItem = dynamic_cast<Coin *>(currentItem);
                                    if (coinItem) {
                                        Dropped = new Coin(*coinItem);
                                        Object* baseObj = static_cast<Object*>(Dropped); // am facut o asa pentru upcasting
                                        baseObj->SetCoord(PF);
                                    }
                                } else {
                                    // For regular objects
                                    Dropped = new Object(*currentItem);
                                    Dropped->SetCoord(PF);
                                }

                                if (Dropped) {
                                    currentRoom->SetObjectAt(PF.GetX(), PF.GetY(), Dropped);
                                    inv.RemoveObject(inv.GetCurentPoz());
                                }
                            } else {
                                std::cout << "No space to drop" << std::endl;
                            }
                        } else {
                            std::cout << "No item to drop" << std::endl;
                        }
                        break;
                    }
                    case VK_ESCAPE:
                        running = false;
                        break;
                    case 0x5A: // Z pentru atac
                    {
                        ClearConsoleLine();
                        P.Attack();
                        break;
                    }
                }
            else
                switch (inputRecord.Event.KeyEvent.wVirtualKeyCode) {
                    case VK_LEFT:
                    case 0x41:
                        inv.DecCurentPoz();
                        break;
                    case VK_RIGHT:
                    case 0x44:
                        inv.IncCurentPoz();
                        break;
                    case 0x49:
                        inv.Close();
                        break;
                }
            ConsoleGoToXY(width - 20, 0);
            std::cout << currentProfile->GetName();
            ConsoleGoToXY(width - 20, 1);
            std::cout << "Balance:" << currentProfile->GetCoins() << "c\n";
            ConsoleGoToXY(width - 20, 2);
            std::cout << "HP: " << P.GetHP() << "/" << currentProfile->GetHP() << std::endl;
            ConsoleGoToXY(width - 20, 3);
            std::cout << "Damage: " << P.GetDmg() << std::endl;

            ConsoleGoToXY(0, height - currentRoom->GetDisplayConsoleInfoSize() - 2);
            currentRoom->DisplayConsoleInfo();
            ConsoleGoToXY(0, 0);
            currentProfile->MapDraw();
            ClearConsoleLine();
            std::cout << inv;
            if (!P.IsAlive()) {
                std::cout << "Game Over! You died!" << std::endl;
                system("pause");
                running = false;
            }
            Sleep(160 - P.GetSpeed());
        }
    }
}

void Game::SlowText(std::string line, int time) {
    for (char c: line) {
        std::cout << c;
        if (_kbhit()) {
            time = 0;
        }
        Sleep(time);
    }
    std::cout << std::endl;
}

void Game::CreateProfile() {
    system("cls");
    std::string name;
    std::cout << "COMAND CENTER:\n";
    SlowText("GOD... TEAM, TEAM, we have a survivor! Please remind us of your name.\n", 70);
    Sleep(50);
    fputs("\e[?25h", stdout); // activeaza cursorul
    std::cout << "->";
    std::getline(std::cin, name);
    system("cls");
    fputs("\e[?25l", stdout); // dezactiveaza cursorul
    std::cout << "COMAND CENTER:\n";
    SlowText(name + ", I'm sorry to inform you are the only known survivor.\n", 70);
    Sleep(50);
    SlowText(
        "I know it's harsh, but you are here with a mission... mankind is putting all its hope in you and you alone."
        " We cannot send more reinforcements until the next cycle, which is a few years away, and we don't have that much time remaining."
        " So please, do this for humanity, for your family, for your daughters. We will be here to guide you and help you.",
        70);
    Sleep(50);
    SlowText(
        "Mars is a very dangerous place. We don't know much about your explosion, and we know very little about THE NEST."
        + name + ", you must retrieve the fuse parts and reactivate the Sun Stabilizer System."
        " As you well know, the SSS is the foundation of the Solar Alliance; it is its whole core."
        " We can't afford another civil war, not now, with the Separatist powers rising and more and more systems doubting"
        " the stability and prosperity of the Solar Alliance.", 70);
    Sleep(50);
    SlowText("If you are ready, we may proceed. Press any key to let us know you are there and to start the process.",
             70);
    system("pause>null");
    profiles.push_back(new Profile(name));
    currentProfile = profiles.back();
    system("cls");
}

Game::Game() {
    Profile A;
    std::cin >> A;
    currentProfile = new Profile(A);
    running = true;
    profiles.push_back(currentProfile);
}

Game::~Game() {
}

void Game::GameLoop() {
    Slots Inventory(3);
    while (running)
        PlayRoom(Inventory);
    std::cout << "Game Over!" << std::endl;
}

void Game::Run() {
    GameLoop();
}
