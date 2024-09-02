#pragma once
#include "std_lib_facilities.h"
#include "AnimationWindow.h"



enum class Playervalue {
    player1,
    player2
};



class Player {
private:
    Color color;
    Playervalue value;
    bool turn;
    string name;

public:
    Player(Color initColor, Playervalue initValue, bool initTurn, string initName);
    Color getColor() {return color;}
    Playervalue getPlayer() {return value;}
    bool getTurn() {return turn; }

    void turnOver() {turn = false;}
    void turnStarts() {turn = true;}
    string getName() {return name;}
};