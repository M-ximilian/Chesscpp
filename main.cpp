#include "Headers/Board.h"
#include <ctime>

int main() {
    int used_seed = time(0);
    int bot_depth = 5;
    int scores[6] = {0,0,0,0,0,0};
    srandom(used_seed);
    srand(used_seed);
    cout << used_seed << endl;
    string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    //fen = "rn3rk1/ppp1qp1p/3p4/6p1/2P1n3/P1B1PQ2/1PP2PPP/R4RK1 w - - 1 16";
    //fen = "7K/1r6/r7/8/6k1/p7/8/8 b - - 0 1";
    //fen = "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1";

    for (int i = 0; i<100000; i++) {
        auto *ui_white = new Interface{2, new Ascii_ui(1), new Random_ui(1), new Bot_ui(1, bot_depth)},
             *ui_black = new Interface{2, new Ascii_ui(0), new Random_ui(0), new Bot_ui(0, bot_depth)};
        Board b = Board(fen, ui_white, ui_black);
        if (ui_white->type == 2) {ui_white->bui->set_board(&b);}
        if (ui_black->type == 2) {ui_black->bui->set_board(&b);}
        int test = b.run();
        scores[test==6?5:test]++;
        ofstream Output("D:/Programing/CLion/Chess/scores.txt");
        Output << scores[0] << " " << scores[1] << " " << scores[2] << " " << scores[3] << " " << scores[4] << " "<<scores[5];
        Output.close();
        if (i % 50 == 0) {
            cout << "total " << i << endl << ((float)i)/((float)(time(0)-used_seed)) << " games/s" << endl << endl;
        }
    }
        //cout << i << endl;


    return 0;
}


