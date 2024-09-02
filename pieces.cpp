#include "pieces.h"


Pieces::Pieces() {

    chessPieces = {
        
        {'T', 'H', 'L', 'D', 'K', 'L', 'H', 'T'},
        {'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B'}, 
        {'-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-'},
        {'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b'}, 
        {'t', 'h', 'l', 'd', 'k', 'l', 'h', 't'}

    };
}

char Pieces::getPiece(Point p) {
    char q;
    if (0<=p.x && p.x<=7  &&  0<=p.y && p.y <= 7) {
        q = chessPieces.at(p.y).at(p.x);
        return q;
    }

    else {
        return 'x';
        }
}

bool Pieces::checkIfOposite(Point MoveFrom, Point MoveTo) {
    Color color1 = getPieceColor(getPiece(MoveFrom));
    Color color2 = getPieceColor(getPiece(MoveTo));

    if (color1 == Color::black && color2 == Color::white) return true; 
    else if (color1 == Color::white && color2 == Color::black) return true;
    else return false;
}

vector<Point> Pieces::pointsOnDiagonal(Point MoveFrom, Point MoveTo) {
    vector<Point> v;    // Alle punkt inkludert MoveTo.
    Point move = moveVector(MoveFrom, MoveTo);

    int directionX; int directionY;
    if (abs(move.x) == abs(move.y) && move.x != 0) {
        if (getPieceColor(getPiece(MoveFrom)) == Color::white) directionY = - move.y / abs(move.y);
        else directionY = move.y / abs(move.y);
        int directionX = move.x / abs(move.x);
        
        int j = directionX;
        for (int i = directionY; abs(i) <= abs(move.y); i+=directionY) {
            v.push_back({MoveFrom.x+j, MoveFrom.y + i});
            j += directionX;
        }
    } 

    return v;
}

vector<Point> Pieces::pointsOnVertical(Point MoveFrom, Point MoveTo) {
    vector<Point> v;    // Alle punkt inkludert MoveTo.
    Point move = moveVector(MoveFrom, MoveTo);
    
    int direction;
    if (!move.x == 0 || !move.y == 0) {
        direction = (move.x + move.y) / abs(move.x + move.y);
    }

    if (move.x == 0) {
        if (getPieceColor(getPiece(MoveFrom)) == Color::white) direction *= -1;
        for (int i = direction; abs(i) <= abs(move.y); i += direction) {
            v.push_back({MoveFrom.x, MoveFrom.y + i});
        }}
    
    else if (move.y == 0) {
        for (int i = direction; abs(i) <= abs(move.x); i += direction) {
            v.push_back({MoveFrom.x + i, MoveFrom.y});
        }
    }

    return v;

}

bool Pieces::pointInsideBoard(Point p) {
    if (p.x < 0  || p.x > 7     ||      p.y < 0 || p.y > 7) {
        return false;
    }
    else {
        return true;
    }
}

Point Pieces::moveVector(Point MoveFrom, Point MoveTo) {
    Point p;
    int changePositiveDirectionY = 1;
    if (getPieceColor(getPiece(MoveFrom)) == Color::white) changePositiveDirectionY = -1;
    p.x = MoveTo.x - MoveFrom.x;
    p.y = (MoveTo.y - MoveFrom.y) * changePositiveDirectionY;
    return p;
}

vector<vector<char>> Pieces::getChessPieces() {
    return chessPieces;
}


void Pieces::takePiece(Point a, Point b) {
    //takenPieces.push_back(defendingPiece);
    chessPieces.at(b.y).at(b.x) = getPiece({a.x, a.y});
    chessPieces.at(a.y).at(a.x) = '-';

    
}  


Color Pieces::getPieceColor(char piece) {
    if (isupper(piece)) {
        return Color::black;
    }
    
    else if (islower(piece)) {
        return Color::white;
    }

    else {
        return Color::tomato;
    }
}


bool Pieces::isMoveLegal(Point MoveFrom, Point MoveTo) {
    char c = getPiece(MoveFrom);
    if (c == '-') return false;

    switch (tolower(c)) {
        case 'b': {return isPawnMoveLegal(MoveFrom, MoveTo);}
        case 't': {return isRookMoveLegal(MoveFrom, MoveTo);}
        case 'l': {return isBischopMoveLegal(MoveFrom, MoveTo);}
        case 'd': {return isQueenMoveLegal(MoveFrom, MoveTo);}
        case 'h': {return isHorseMoveLegal(MoveFrom, MoveTo);}
        case 'k': {return isKingMoveLegal(MoveFrom, MoveTo);}
        default : {return false;}
    }

    return false;
}

Point Pieces::getKingsPosition(Color color) {
    for (int i=0; i <= 7; i++) {
        for (int j=0; j<= 7; j++) {
            if (color == Color::white) {
                if (chessPieces.at(i).at(j) == 'k') {
                    return {j, i};
                }
            }
            else if (color == Color::black) {
                if (chessPieces.at(i).at(j) == 'K') {
                    return {j, i};
                }
            }
            }
        }
    
    return {0,0};
}


bool Pieces::isPawnMoveLegal(Point MoveFrom, Point MoveTo) {
    Color pieceColor = getPieceColor(getPiece(MoveFrom));
    Point move = moveVector(MoveFrom, MoveTo);  // Positiv retning nedover.

    if (!pointInsideBoard(MoveFrom) || !pointInsideBoard(MoveTo)) {
        return false;
    }

    // Teste om man tar brikke:
    if (abs(move.x) == 1 && move.y == 1) {
        if (checkIfOposite(MoveFrom, MoveTo)) {
            string s = "Det er good";
        }
        
        else return false;
    }

    else{
        //Første trekk.
        if (MoveFrom.y == 1 || MoveFrom.y == 6) {
            if ((move.x == 0) && (move.y == 1 || move.y == 2)) {
            }
            else {return false;}
            }
        // Andre trekk.
        else{
            if ((move.x == 0) && (move.y == 1)) {
                }
            else {return false;} }

        // Hvis man kommer hit sjekk at ingen brikker er i veien.
        for (auto v : pointsOnVertical(MoveFrom, MoveTo)) {
            if (getPiece(v) != '-') {
                return false;
            }
        }}

    // Her er trekket godkjent. Gjør om bonde til dronning om den er kommet over.
    if (MoveTo.y == 7) {
        chessPieces.at(6).at(MoveFrom.x) = 'D';
    }
    else if (MoveTo.y == 0) {
        chessPieces.at(1).at(MoveFrom.x) = 'd';
    }

    return true;
    }

bool Pieces::isRookMoveLegal(Point MoveFrom, Point MoveTo) {
    Point move = moveVector(MoveFrom, MoveTo);
    
    if (move.x == 0 || move.y == 0) {
        for (auto p : pointsOnVertical(MoveFrom, MoveTo)) {
            if (p.x != MoveTo.x || p.y != MoveTo.y) {
                if (getPiece(p) != '-') return false;
            }
            else {
                if (getPiece(p) == '-') return true;
                else if (checkIfOposite(MoveFrom, MoveTo)) {return true;}
            }
        }
    }

    return false;
}

bool Pieces::isBischopMoveLegal(Point MoveFrom, Point MoveTo) {
    for (auto p : pointsOnDiagonal(MoveFrom, MoveTo)) {
        if (p.x != MoveTo.x || p.y != MoveTo.y) {
            if (getPiece(p) != '-') return false; }
        
        else {
            if (getPiece(p) == '-') return true;
            else if (checkIfOposite(MoveFrom, MoveTo)) {return true;}
        }
    }

    return false;
}

bool Pieces::isQueenMoveLegal(Point MoveFrom, Point MoveTo) {
    vector<Point> v;
    if (pointsOnDiagonal(MoveFrom, MoveTo).size() != 0) {
        v = pointsOnDiagonal(MoveFrom, MoveTo);
    }
    else if (pointsOnVertical(MoveFrom, MoveTo).size() != 0) {
        v = pointsOnVertical(MoveFrom, MoveTo);
}
    else {return false;}

    for (auto p : v) {
        if (p.x != MoveTo.x || p.y != MoveTo.y) {
            if (getPiece(p) != '-') return false; }
        
        else {
            if (getPiece(p) == '-') return true;
            else if (checkIfOposite(MoveFrom, MoveTo)) {return true;}
        }
    }
    return false;

}

bool Pieces::isHorseMoveLegal(Point MoveFrom, Point MoveTo) {
    Color pieceColor = getPieceColor(getPiece(MoveFrom));
    Point move = moveVector(MoveFrom, MoveTo);

    vector<Point> legalMoves = {
        {2,1}, {2,-1}, {1,2}, {1,-2},
        {-1,2}, {-1,-2}, {-2, 1}, {-2,-1}
    };

    for (auto p : legalMoves) {
        if (move.x == p.x && move.y == p.y) {
            if (pointInsideBoard(MoveTo)) {
                if (getPiece(MoveTo) == '-' || checkIfOposite(MoveFrom, MoveTo)) {
                    return true;
                }
            }
        }
    }

    return false;

}

bool Pieces::isKingMoveLegal(Point MoveFrom, Point MoveTo) {
    Color pieceColor = getPieceColor(getPiece(MoveFrom));
    Point move = moveVector(MoveFrom, MoveTo);
    
    vector<Point> legalMoves = {
        {1,1}, {1,0}, {1,-1}, 
        {0,1}, {0,-1}, 
        {-1,1}, {-1,0}, {-1,-1}
    };

    for (auto p : legalMoves) {
        if (move.x == p.x && move.y == p.y) {
            if (pointInsideBoard(MoveTo)) {
                if (getPiece(MoveTo) == '-' || checkIfOposite(MoveFrom, MoveTo)) {
                    return true;
                }
            }
        }
    }
    return false;
}


bool Pieces::isCheck(Point kingPosition, vector<Point>& checkFrom) {
    Color pieceColor = getPieceColor(getPiece(kingPosition));
    bool check = false;
    // Sjakk av bonde.
    int direction = 1;
    if (pieceColor == Color::white) direction = -1;

    
    vector<Point> checkByPawn = {{1, direction}, {-1, direction}};
    for (auto p : checkByPawn) {
        p = {kingPosition.x + p.x, kingPosition.y + p.y};
        if ((tolower(getPiece(p)) == 'b') && checkIfOposite(kingPosition, p)) {
            checkFrom.push_back(p);
            check = true;
        }
    }
    
    vector<Point> checkByHorse = {
        {2,1}, {2,-1}, {1,2},
        {1,-2},        {-1,2},
        {-1,-2}, {-2, 1}, {-2,-1}};

    for (auto p : checkByHorse) {
        p = {kingPosition.x + p.x, kingPosition.y + p.y};
        if (pointInsideBoard(p)){
        if (tolower(getPiece(p)) == 'h' && checkIfOposite(kingPosition, p)) {
            checkFrom.push_back(p);
            check = true;
        }}
    }

    // Sjekk vertikaler for tårn og dronning.
    vector<vector<Point>> allVerticals = getAllVertical(kingPosition);
    

    for (vector v : allVerticals) {
        for (Point p : v) {
            //cout << "p: (" << p.x << ", " << p.y << ")" << endl;
            if (getPieceColor(getPiece(p)) == pieceColor) {
                break;
            }
            if (getPieceColor(getPiece(p)) == getOppositColor(kingPosition)) {
                if ((tolower(getPiece(p)) == 'd') || (tolower(getPiece(p)) == 't')) {
                    checkFrom.push_back(p);
                    check = true;
                        }

                else break;            
            }
        }
    }

    // Sjekk diagonaler for løpere og dronning.
    vector<vector<Point>> allDiagonals = getAllDiagonals(kingPosition);

    for (vector v : allDiagonals) {
        for (Point p : v) {
            if (getPieceColor(getPiece(p)) == pieceColor) {
                break;
            }
            if (getPieceColor(getPiece(p)) == getOppositColor(kingPosition)) {
                if ((tolower(getPiece(p)) == 'd') || (tolower(getPiece(p)) == 'l')) {
                    checkFrom.push_back(p);
                    check = true;
                                    }
                else break;
            }
        }
    }

    // Sjakk av motsatt konge:
    vector<Point> legalKingMoves = {
        {1,1}, {1,0}, {1,-1}, 
        {0,1}, {0,-1}, 
        {-1,1}, {-1,0}, {-1,-1}
    };
    for (Point p : legalKingMoves) {
        Point posKing = {kingPosition.x + p.x, kingPosition.y + p.y};
        if (tolower(getPiece(posKing)) == 'k') {
            if (getPieceColor(getPiece(posKing)) == getOppositColor(kingPosition)) {
                check = true;
            }
        }
        }
    return check;
}

vector<vector<Point>> Pieces::getAllDiagonals(Point startPoint) {
    vector<vector<Point>> allDiags;

    // Bevege seg i en retning til man finner endepunktet. Bruk så getDiag-funksjon.
    vector<Point> direction = {{1,1}, {1,-1}, {-1,1}, {-1,-1}};

    Point point;
    for (Point dir : direction) {
        point = startPoint;
        //cout << "starting point: " << point.x << ", " << point.y <<  endl;
        
        while (0<=point.x+dir.x && point.x+dir.x<=7 && 0<=point.y+dir.y && point.y+dir.y <= 7) {
            point.x += dir.x; 
            point.y += dir.y;
            //cout << "Point after iteration: " << point.x << ", " << point.y <<  endl;
        }
        vector<Point> diag = pointsOnDiagonal(startPoint, point);
        allDiags.push_back(diag);
    }

    return allDiags;
}


Color Pieces::getOppositColor(Point p) {
    Color color = getPieceColor(getPiece(p));

    if (color == Color::black) return Color::white;
    else if (color == Color::white) return Color::black;
    else return color;
}

vector<vector<Point>> Pieces::getAllVertical(Point startPoint) {
    vector<vector<Point>> allVerticals;
    allVerticals.push_back(pointsOnVertical(startPoint, {0, startPoint.y}));
    allVerticals.push_back(pointsOnVertical(startPoint, {7, startPoint.y}));
    allVerticals.push_back(pointsOnVertical(startPoint, {startPoint.x, 0}));
    allVerticals.push_back(pointsOnVertical(startPoint, {startPoint.x, 7}));

    return allVerticals;
}

    
bool Pieces::rokade(Point moveFrom, Point moveTo) {
    vector<Point> castle;
    if (moveFrom.x==4 && moveFrom.y==7 && getPiece(moveFrom)=='k') {
        cout << "hvit" << endl;
        castle = {{0,7}, {1,7}, {2,7}, {6,7}, {7,7}};
    }
    else if (moveFrom.x==4 && moveFrom.y==0 && getPiece(moveFrom)=='K') {
        cout << "svart " << endl;
        castle = {{0,0}, {1,0}, {2,0}, {6,0}, {7,0}};
    }

    else return false;


    Pieces testBoard;
    // Rokade er ønsket, men er det lovlig? Finner ut nedenfor.
    vector<Point> rokadeVec;
    vector<Point> doNothingVec;
    for (Point p : castle) {
        if (moveTo.x == p.x && moveTo.y == p.y) {
            if (p.x < moveFrom.x) {
                rokadeVec = pointsOnVertical(moveFrom, {2, moveFrom.y});
                if (getPiece({1, moveFrom.y}) != '-' || tolower(getPiece({0, moveFrom.y})) != 't') {
                    return false; //Det er ikke lov at det står brikker i mellom.
                }
            }
            else {
                rokadeVec = pointsOnVertical(moveFrom, {6, moveFrom.y});
                if (tolower(getPiece({7, moveFrom.y})) != 't') {
                    cout << "se hit" << endl;
                    return false;
                }
            }
        
            
            testBoard.chessPieces = chessPieces;
            Point lastPos = moveFrom;

            for (Point pRok : rokadeVec) {
                testBoard.takePiece(lastPos, pRok);
                if (testBoard.isCheck(pRok, doNothingVec) || getPiece(pRok) != '-') {
                    cout << "fucker her!" << endl;
                    return false; // Ikke love om kongen er i sjakk på veien. Ikke lov med brikke i veien.
                }
                lastPos = pRok;
            }
        // Her er rokade godkjent. 
        return true;

        }
    }
    

    return false;
        
}

