#include "Headers/Board.h"
#include <ctime>

int main() {
    int used_seed = time(0);
    srandom(used_seed);
    cout << used_seed << endl;
    string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    for (int i = 0; i<100000; i++) {
        Random_ui *ui_white = new Random_ui(1), *ui_black = new Random_ui(0);
        Board b = Board(fen, ui_white, ui_black);
        int end = b.run();
        //cout << i << " " << end << endl;
        if (i % 1000 == 0) {
            cout << "total " << i << endl << ((float)i)/((float)(time(0)-used_seed)) << " games/s" << endl << endl;
        }
    }
        //cout << i << endl;


    return 0;
}


