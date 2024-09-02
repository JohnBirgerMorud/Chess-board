#pragma once
#include "std_lib_facilities.h"
#include "AnimationWindow.h"
#include "pieces.h"
#include "graphics.h"
#include "Player.h"

class Chess {
private:
    Player whitePlayer{Color::white, Playervalue::player1, true, "White"};
    Player blackPlayer{Color::black, Playervalue::player2, false, "Black"};
    Player noPlayer{Color::tomato, Playervalue::player1, false, "No winner"};

    bool whiteRight; bool whiteLeft; bool whiteKingMoved; 
    bool blackRight; bool blackLeft; bool blackKingMoved;

public:
    AnimationWindow chessBoard{200, 200, 600, 600};
    void drawBoard();
    bool close();
    vector<Point> getDesiredMove();
    bool playerTurn(Player player, Point moveFrom, Point moveTo);

    void playChess();

    bool isCheckMate(Point kingPos);
    bool defendeFromDiagonal(Point kingPos, Point attPos, vector<Point> attackingLine);
    bool defendeFromVertical(Point kingPos, Point attPos, vector<Point> attackingLine);
    bool defendeWithPawn(Point kingPos, Point attPos, vector<Point> attackingLine);
    bool defendeWithHorse(Point kingPos, Point attPos, vector<Point> attackingLine);
    bool checkIfDefenseWorks(Pieces testBoard, Point kingPos);
    
    void Castle(Point moveFrom, Point moveTo);

    bool checkIfHaveMoved(Point moveTo, Color playerColor);
    void updateIfMoved(Point MoveFrom);
    Pieces pieces;
  
};

