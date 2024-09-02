#pragma once
#include "std_lib_facilities.h"
#include "AnimationWindow.h"

class Pieces {
private:
    vector<vector<char>> chessPieces;
    vector<string> takenPieces;
    bool checkIfOposite(Point MoveFrom, Point MoveTo);

public:
    Pieces();
    char getPiece(Point p);
    vector<vector<char>> getChessPieces();
    void takePiece(Point a, Point b);

    Color getPieceColor(char piece);

    bool isMoveLegal(Point MoveFrom, Point MoveTo); // Skal velge ut rett funksjon med en switch. 
    bool isPawnMoveLegal(Point MoveFrom, Point MoveTo);
    bool isRookMoveLegal(Point MoveFrom, Point MoveTo);
    bool isHorseMoveLegal(Point MoveFrom, Point MoveTo);
    bool isBischopMoveLegal(Point MoveFrom, Point MoveTo);
    bool isQueenMoveLegal(Point MoveFrom, Point MoveTo);
    bool isKingMoveLegal(Point MoveFrom, Point MoveTo);

    Point getKingsPosition(Color color);
    bool isCheck(Point kingPosition, vector<Point>& checkFrom);

    Point moveVector(Point MoveFrom, Point MoveTo);

    vector<vector<Point>> getAllDiagonals(Point startPoint);
    vector<vector<Point>> getAllVertical(Point startPoint);
    vector<Point> pointsOnVertical(Point MoveFrom, Point MoveTo);
    vector<Point> pointsOnDiagonal(Point MoveFrom, Point MoveTo);

    Color getOppositColor(Point p);
    bool pointInsideBoard(Point p);
    void anpassant(Point MoveFrom, Point MoveTo);

    bool rokade(Point moveFrom, Point moveTo);


};