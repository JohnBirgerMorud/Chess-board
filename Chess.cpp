#include "Chess.h"


void Chess::drawBoard() {
    printBoard(pieces, chessBoard);
}

bool Chess::close() {
    return chessBoard.should_close();
}



vector<Point> Chess::getDesiredMove() {
    Point wantToMoveTo; Point wantToMoveFrom;

       //Kjører helt til man får kordinatet. 

    wantToMoveFrom = chessBoard.get_mouse_coordinates();

    while(chessBoard.is_left_mouse_button_down()) {
        
        chessBoard.next_frame();
        drawBoard();
        
        string p1_text = "(" + to_string(wantToMoveFrom.x) + ", " + to_string(wantToMoveFrom.y) + ")";
        string p2_text = "(" + to_string(wantToMoveTo.x) + ", " + to_string(wantToMoveTo.y) + ")";
    

        chessBoard.draw_text({300, 300}, (p1_text)); 
        chessBoard.draw_text({400, 300}, (p2_text)); 
        wantToMoveTo = chessBoard.get_mouse_coordinates();

    }

    // Gjøre om til heltallskordinater.
    
    wantToMoveFrom.x = wantToMoveFrom.x * 8 / chessBoard.width();
    wantToMoveFrom.y = wantToMoveFrom.y * 8 / chessBoard.height();

    wantToMoveTo.x = wantToMoveTo.x * 8 / chessBoard.width();
    wantToMoveTo.y = wantToMoveTo.y * 8 / chessBoard.height();

    vector<Point> desiredMove = {wantToMoveFrom, wantToMoveTo};

    return desiredMove;

}

void Chess::Castle(Point moveFrom, Point moveTo) {
    if (moveTo.x < 4) {
        pieces.takePiece(moveFrom, {2, moveFrom.y});
        pieces.takePiece({0, moveFrom.y}, {3, moveFrom.y});
    }
    else {
        pieces.takePiece(moveFrom, {6, moveFrom.y});
        pieces.takePiece({7, moveFrom.y}, {5, moveFrom.y});
        }
}

bool Chess::checkIfHaveMoved(Point moveTo, Color playerColor) {
    if (moveTo.x < 4) {
        if (playerColor == Color::white) {
            if (!whiteLeft && !whiteKingMoved) return true;
        }
        else {
            if (!blackLeft && !blackKingMoved) return true;
            }
        }
    else {
        if (playerColor == Color::white) {
            if (!whiteRight && !whiteKingMoved) return true;
        }
        else {
            if (!blackRight && !blackKingMoved) return true;
        }
    }

    return false;
}

void Chess::updateIfMoved(Point MoveFrom) {
    int x = MoveFrom.x; int y = MoveFrom.y;

    if (x==0 && y==0)      {blackLeft=true;}
    else if (x==7 && y==0) {blackRight=true;}
    else if (x==4 && y==0) {blackKingMoved=true;}
    else if (x==0 && y==7) {whiteLeft=true;}
    else if (x==7 && y==7) {whiteRight=true;}
    else if (x==4 && y==7) {whiteKingMoved=true;}
}

bool Chess::playerTurn(Player player, Point moveFrom, Point moveTo) {
    Pieces testBoard = pieces;
    vector<Point> doesNothing;
    bool sucsessfullMove = false;

    Color playerColor = player.getColor();
    Point kingPosition = pieces.getKingsPosition(playerColor);
    Color oppositColor = pieces.getOppositColor(kingPosition);
    
    bool tryCastle = pieces.rokade(moveFrom, moveTo);

    if (tryCastle && checkIfHaveMoved(moveTo, playerColor)) {
        Castle(moveFrom, moveTo);
        return true;
    }

    if (pieces.isMoveLegal(moveFrom, moveTo)) {
        testBoard.takePiece(moveFrom, moveTo);
        updateIfMoved(moveFrom);    // Sjekker om konge eller tårn har flyttet.

        doesNothing = {};
        if (!(testBoard.isCheck(testBoard.getKingsPosition(playerColor), doesNothing))) {
            pieces.takePiece(moveFrom, moveTo);
            sucsessfullMove = true;

            return sucsessfullMove;
            
        }
    }
    return false;
}

    
void Chess::playChess() {
    Player winner = noPlayer;
    bool gameFinished = false;

    while(!chessBoard.should_close()) {
        drawBoard(); 
        chessBoard.next_frame();
        
    if (gameFinished == false) {
        if (chessBoard.is_left_mouse_button_down()) {
            Point moveFrom; Point moveTo;
            vector<Point> move = getDesiredMove();
            moveFrom = move.at(0);
            moveTo = move.at(1);


            if (whitePlayer.getTurn() && pieces.getPieceColor(pieces.getPiece(moveFrom)) == Color::white) {
                bool sucsessfullTurn = playerTurn(whitePlayer, moveFrom, moveTo);
                if (sucsessfullTurn == true) {
                    if (isCheckMate(pieces.getKingsPosition(Color::black))) {
                        winner = whitePlayer;
                        gameFinished = true;
                    }

                    whitePlayer.turnOver(); 
                    blackPlayer.turnStarts();
                }
            }


            else if (blackPlayer.getTurn() && pieces.getPieceColor(pieces.getPiece(moveFrom)) == Color::black) {
                bool sucsessfullTurn = playerTurn(blackPlayer, moveFrom, moveTo);
                if (sucsessfullTurn == true) {
                    if (isCheckMate(pieces.getKingsPosition(Color::white))) {
                        winner = blackPlayer;
                        gameFinished = true;
                    }

                    blackPlayer.turnOver();
                    whitePlayer.turnStarts();
                }
            }
            }
    }

        else {
            // Her har noen vunnet!
            string win = "SJAKK MATT! " + winner.getName() + " vinner!";
            chessBoard.draw_text({chessBoard.width()/2, chessBoard.height()/2}, win);
        }   
    }    
}


bool Chess::isCheckMate(Point kingPos) {
    bool checkMate = true;
    Pieces testBoard = pieces;
    Color kingColor = testBoard.getPieceColor(testBoard.getPiece(kingPos));
    vector<Point> checkFrom;
    pieces.isCheck(kingPos, checkFrom);

    // Kongen er ikke i sjakk.
    if (checkFrom.size() == 0) {
        checkMate = false;
        return checkMate;
    }

    // Sjekke alle kombinasjoner av å flytte kongen.
    vector<Point> legalKingMoves = {
        {1,1},  {1,  0}, {1,-1}, 
        {0,1},  /* 😎 */ {0,-1}, 
        {-1,1}, {-1, 0}, {-1,-1}
    };

    for (Point p : legalKingMoves) {
        vector<Point> doesNothingVector;
        testBoard = pieces;
        if (testBoard.isMoveLegal(kingPos, {kingPos.x + p.x, kingPos.y + p.y})) {
            testBoard.takePiece(kingPos, {kingPos.x + p.x, kingPos.y + p.y});
            if (testBoard.isCheck(testBoard.getKingsPosition(kingColor), doesNothingVector) == false) {
                checkMate = false;
                return checkMate;
            }
        
        } 
    }

    // Poeng: siden kongen ikke har noen muligheter til å flytte unna uten å stå i sjakk, er alternativene
    // å enten ta brikken som sjakker, eller flytte brikke i mellom.

    // Hvis det sjakkes fra to eller flere steder nå, er det sjakkmatt.
    if (checkFrom.size() > 1) {
        checkMate = true;
        return checkMate;
    }

    // Herfra og under sjakkes det bare fra en plass.
    Point attPos = checkFrom.at(0);

    vector<Point> attVector;
    if (pieces.pointsOnDiagonal(kingPos, attPos).size() > 0) {
        cout << "Vet dronning angriper" << endl;
        attVector = pieces.pointsOnDiagonal(kingPos, attPos);
        for (auto p : attVector) {cout << p.x << ", " << p.y << endl;}
    }
    else if (pieces.pointsOnVertical(kingPos, attPos).size() > 0) {
        cout << "er også her av en grunn." << endl;
        attVector = pieces.pointsOnVertical(kingPos, attPos);
        cout << "att: " << attVector.at(0).x << attVector.at(0).y << endl;
    }
    else attVector = {attPos};

    if (attVector.size() > 0) {   // Vil være det i alle tilfeller utenom hest-angrep.
        cout << "tester diise" << endl;
        if (defendeFromDiagonal(kingPos, attPos, attVector)) {cout << "1. "; checkMate = false;}
        if (defendeFromVertical(kingPos, attPos, attVector)) {cout << "2. "; checkMate = false;}
        if (defendeWithHorse(kingPos, attPos, attVector))    {cout << "3. "; checkMate = false;}
        if (defendeWithPawn(kingPos, attPos, attVector))     {cout << "4. "; checkMate = false;}
    }
    else {
        if (defendeFromDiagonal(kingPos, attPos, attVector)) {cout << "5. "; checkMate = false;}
        if (defendeFromVertical(kingPos, attPos, attVector)) {cout << "6. "; checkMate = false;}
        if (defendeWithHorse(kingPos, attPos, attVector))    {cout << "7. "; checkMate = false;}
        if (defendeWithPawn(kingPos, attPos, attVector))     {cout << "8. "; checkMate = false;}
    }

    cout << "bool: " << checkMate << endl;
    return checkMate;
}


bool Chess::defendeFromDiagonal(Point kingPos, Point attPos, vector<Point> attackingLine) {
    bool defend = false;
    Color kingColor = pieces.getPieceColor(pieces.getPiece(kingPos));
    Color attColor = pieces.getOppositColor(kingPos);

    vector<Point> doNothing;


    for (Point p : attackingLine) {
        vector<vector<Point>> diagonalDefence = pieces.getAllDiagonals(p);
        for (vector vDef : diagonalDefence) {
            for (Point pDef : vDef) {
                if (pieces.getPieceColor(pieces.getPiece(pDef)) == attColor) break;
                else if (pieces.getPieceColor(pieces.getPiece(pDef)) == kingColor) {
                    char posDef = pieces.getPiece(pDef);
                    if (tolower(posDef) == 'q' || tolower(posDef) == 'l') {
                        Pieces testBoard = pieces;
                        testBoard.takePiece(pDef, p);
                        if (!testBoard.isCheck(kingPos, doNothing)) {
                            defend = true;
                            return defend;
                    }
                    }
                    else break;

                    }
                }
            }
        }
    return defend;
}

bool Chess::defendeFromVertical(Point kingPos, Point attPos, vector<Point> attackingLine) {
    bool defend = false;
    Color kingColor = pieces.getPieceColor(pieces.getPiece(kingPos));
    Color attColor = pieces.getOppositColor(kingPos);

    vector<Point> doNothing;

    for (Point p : attackingLine) {
        cout << "pLine: " << p.x << ", " << p.y << endl;
        vector<vector<Point>> diagonalDefence = pieces.getAllVertical(p);
        for (vector vDef : diagonalDefence) {
            for (Point pDef : vDef) {
                if (pieces.getPieceColor(pieces.getPiece(pDef)) == attColor) break;
                else if (pieces.getPieceColor(pieces.getPiece(pDef)) == kingColor) {
                    char posDef = pieces.getPiece(pDef);
                    if (tolower(posDef) == 'q' || tolower(posDef) == 't') {
                        Pieces testBoard = pieces;
                        testBoard.takePiece(pDef, p);
                        if (!testBoard.isCheck(kingPos, doNothing)) {
                            defend = true;
                            return defend;
                    }}
                    else break;
                }
            }
            }
            }   

    return defend;
}

bool Chess::defendeWithHorse(Point kingPos, Point attPos, vector<Point> attackingLine) {
    bool defend = false;
    Color kingColor = pieces.getPieceColor(pieces.getPiece(kingPos));
    Color attColor = pieces.getOppositColor(kingPos);

    vector<Point> doNothing;

    vector<Point> legalMoves = {
        {2,1}, {2,-1}, {1,2}, {1,-2},
        {-1,2}, {-1,-2}, {-2, 1}, {-2,-1}
    };

    for (Point p : attackingLine) {
        for (Point dir : legalMoves) {
            Point posHorse = {p.x + dir.x, p.y + dir.y};
            if (pieces.pointInsideBoard(posHorse)) {
                if (tolower(pieces.getPiece(posHorse)) == 'h' 
                    && pieces.getPieceColor(pieces.getPiece(posHorse)) == kingColor) {

                        Pieces testBoard = pieces;
                        testBoard.takePiece(posHorse, p);
                        if (!testBoard.isCheck(kingPos, doNothing)) {
                            defend = true;
                            return defend;
                       
                }
            }
        }
    }
    }
    return defend;
}

// Enten ta angripende brikke. Eller flytte en eller to fram i vertikal retning. 
bool Chess::defendeWithPawn(Point kingPos, Point attPos, vector<Point> attackingLine) {
    bool defend = false;
    Color kingColor = pieces.getPieceColor(pieces.getPiece(kingPos));
    Color attColor = pieces.getOppositColor(kingPos);

    Point attPiece = attackingLine.at(attackingLine.size()-1);
    vector<Point> doNothing;

    // Ta angripende brikke med bonde. 
    vector<Point> posWhitePawn = {{1, 1}, {-1,1}};
    vector<Point> posBlackPawn = {{1, -1}, {-1,-1}};

    if (attColor==Color::white) {
        for (Point dir : posBlackPawn) {
            if (pieces.pointInsideBoard({attPiece.x + dir.x, attPiece.y + dir.y})) {
                Point posPawn = {attPiece.x + dir.x, attPiece.y + dir.y};
                Color posPawnColor = pieces.getPieceColor(pieces.getPiece(posPawn));
                if (tolower(pieces.getPiece(posPawn)) == 'b' && posPawnColor == kingColor) {
                    Pieces testBoard = pieces;
                    testBoard.takePiece(posPawn, attPiece);
                    if (!testBoard.isCheck(kingPos, doNothing)) {
                        defend = true;
                        return defend;
                }
            }
        } 
    }}

    else if (attColor==Color::black) {
        for (Point dir : posWhitePawn) {
            if (pieces.pointInsideBoard({attPiece.x + dir.x, attPiece.y + dir.y})) {
                Point posPawn = {attPiece.x + dir.x, attPiece.y + dir.y};
                Color posPawnColor = pieces.getPieceColor(pieces.getPiece(posPawn));
                if (tolower(pieces.getPiece(posPawn)) == 'b' && posPawnColor == kingColor) {
                    Pieces testBoard = pieces;
                    testBoard.takePiece(posPawn, attPiece);
                    if (!testBoard.isCheck(kingPos, doNothing)) {
                        defend = true;
                        return defend;
                }
            }
        } 
    }}

    // Forsvare ved å flytte bonde fremover.
    attackingLine.pop_back();

    for (Point p : attackingLine) {
    cout << "attack: " << p.x << "(, " << p.y << ")" << endl;
    if (attColor == Color::black) {
            Point posPawn; Point anotherPosPawn = {7,7};
            if (pieces.pointInsideBoard({p.x, p.y + 1})) {
                posPawn = {p.x, p.y + 1};
            }
            if (p.y == 4) {anotherPosPawn = {p.x, p.y + 2};}

            if (pieces.getPiece(posPawn) == 'b')  {
                Pieces testBoard = pieces;
                testBoard.takePiece(posPawn, p);
                if (!testBoard.isCheck(kingPos, doNothing)) {
                    defend = true;
                    return defend;
            }}
            
            if (pieces.getPiece(anotherPosPawn) == 'b') {
                Pieces testBoard = pieces;
                testBoard.takePiece(anotherPosPawn, p);
                if (!testBoard.isCheck(kingPos, doNothing)) {
                    defend = true;
                    return defend;
            }}}

    else if (attColor == Color::white) {
            Point posPawn; Point anotherPosPawn = {0,0};
            if (pieces.pointInsideBoard({p.x, p.y - 1})) {
                posPawn = {p.x, p.y + 1};
            }
            if (p.y == 3) {anotherPosPawn = {p.x, p.y - 2};}

            if (pieces.getPiece(posPawn) == 'B') {
                Pieces testBoard = pieces;
                testBoard.takePiece(posPawn, p);
                if (!testBoard.isCheck(kingPos, doNothing)) {
                    defend = true;
                    return defend;
                }
            }
            if (pieces.getPiece(anotherPosPawn) == 'B') {
                Pieces testBoard = pieces;
                testBoard.takePiece(anotherPosPawn, p);
                if (!testBoard.isCheck(kingPos, doNothing)) {
                    defend = true;
                    return defend;
                }
            }
            }
        }
    defend = false;
    return defend;
    }
    

    





