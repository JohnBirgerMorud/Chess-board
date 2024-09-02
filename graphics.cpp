#include "graphics.h"
#include "pieces.h"
#include <string>

std::string p0 = "/Users/johnbirgermorud/Documents/c++/Chess/brikker/";
Image imB(p0+"svart_bonde.png");
Image imT(p0+"svart_born.png");
Image imH(p0+"svart_hest.png");
Image imL(p0+"svart_løper.png");
Image imD(p0+"svart_dronning.png");
Image imK(p0+"svart_konge.png");
Image imb(p0+"hvit_bonde.png");
Image imt(p0+"hvit_tårn.png");
Image imh(p0+"hvit_hest.png");
Image iml(p0+"hvit_løper.png");
Image imd(p0+"hvit_dronning.png");
Image imk(p0+"hvit_konge.png");

map<string, Image> graphicPieces {
    {"B", imB},
    {"T", imT},
    {"H", imH},
    {"L", imL},
    {"D", imD},
    {"K", imK},
    {"b", imb},
    {"t", imt},
    {"h", imh},
    {"l", iml},
    {"d", imd},
    {"k", imk}
};


void printBoard(Pieces& chessPieces, AnimationWindow& chessBoard) {
    int colorCode = 0;
    int yCord = -1;
    int xCord = 0;

        for(int i=0; i<chessBoard.height(); i+= chessBoard.height()/8.0) {
            yCord ++;
            xCord = -1;
            colorCode ++;
            for (int j=0; j<chessBoard.width(); j+= chessBoard.width()/8.0) {
                xCord ++;
                Color color;
                if (colorCode % 2 == 1) color = Color::saddle_brown;
                else color = Color::burly_wood;

                chessBoard.draw_rectangle(
                    {i, j}, chessBoard.width()/8, chessBoard.height()/8, color);
                
                char p = chessPieces.getPiece({yCord, xCord});
                string q = to_string(p);
                if (graphicPieces.count(q) == 1) {

                    chessBoard.draw_image({i,j}, graphicPieces.at(q), chessBoard.width()/8, chessBoard.height()/8);
                }
                
                //chessBoard.draw_image({i+chessBoard.width() / 16 ,j+chessBoard.height()/16}, q);


                colorCode ++;
            }
        }
}

