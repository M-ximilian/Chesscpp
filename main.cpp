#include "Headers/Board.h"
#include <ctime>

int main() {
    int used_seed = time(0);
    int bot_depth = 3;
    srandom(1642350471);
    cout << used_seed << endl;
    string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    fen = "k7/6R1/7R/8/8/8/P6K/8 b - - 0 1";
    fen = "7K/1r6/r7/8/6k1/p7/8/8 b - - 0 1";
    //fen = "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1";

    for (int i = 0; i<100000; i++) {
        auto *ui_white = new Interface{0, new Ascii_ui(1), new Random_ui(1), new Bot_ui(1, bot_depth)},
             *ui_black = new Interface{2, new Ascii_ui(0), new Random_ui(0), new Bot_ui(0, bot_depth)};
        Board b = Board(fen, ui_white, ui_black);
        if (ui_white->type == 2) {ui_white->bui->set_board(&b);}
        if (ui_black->type == 2) {ui_black->bui->set_board(&b);}
        Board *test = &b;
        test->run();
        //int end = b.run();
        //cout << i << " " << end << endl;
        if (i == 164) {
            bool test1 = false;
        }
        if (i % 100 == 0) {
            cout << "total " << i << endl << ((float)i)/((float)(time(0)-used_seed)) << " games/s" << endl << endl;
        }
    }
        //cout << i << endl;


    return 0;
}


