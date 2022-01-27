#include "Headers/Board.h"
#include <ctime>
const string postions[20] = {"r1bqk1nr/pppp1ppp/2n5/2b1p3/2B1P3/2P2N2/PP1P1PPP/RNBQK2R b KQkq - 0 4",
                       "r1bqkbnr/pppp1ppp/2n5/1B2p3/4P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 3 3",
                       "r1bqkbnr/pp1p1ppp/2n5/1N2p3/4P3/8/PPP2PPP/RNBQKB1R b KQkq - 1 5",
                       "rnbqkbnr/pp2pppp/2p5/3p4/4P3/2N2N2/PPPP1PPP/R1BQKB1R b KQkq - 1 3",
                       "rnbqkb1r/ppp2ppp/4pn2/3p4/2PP4/5NP1/PP2PP1P/RNBQKB1R b KQkq - 0 4",
                       "r1bqk2r/pppp1ppp/2n2n2/8/1b1NP3/2N5/PPP2PPP/R1BQKB1R w KQkq - 3 6",
                       "1r3k2/5ppp/5n2/pNp1r3/P2p4/1P3P1P/2PR2P1/3R2K1 w - - 0 27",
                       "r1b2rk1/1p2bppp/p4n2/8/P1BR1B2/2N5/1P3PPP/R5K1 b - - 0 16",
                       "r2q1rk1/ppp2ppp/1bnp1n2/4p3/1PP1P1b1/P1NPBN2/4BPPP/R2Q1RK1 b - - 6 10",
                       "r1bqr1k1/pp3ppp/1bpp1nn1/4p3/4P3/2PP1N1P/PPBN1PP1/R1BQR1K1 w - - 1 12",
                       "2rq1rk1/1p1nbppp/p1b1p3/P3P3/1P1p1P2/6P1/2PQ1B1P/R2NRBK1 b - - 1 22",
                       "2R5/pp2rp2/5kp1/7p/7P/1P4P1/P2K1P2/8 b - - 2 29",
                       "rnbqkb1r/ppp2pp1/3p1n1p/8/2B1Pp1P/2N2N2/PPPP2P1/R1BQK2R b KQkq - 2 6",
                       "r1bqr1k1/ppp2ppp/3b4/3p4/3P4/1PPB4/PB1Q1PPP/4RRK1 b - - 2 15",
                       "rnbqkbnr/pppp1ppp/8/4p3/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 2",
                       "rnbqkbnr/ppp1pppp/8/3p4/3P4/8/PPP1PPPP/RNBQKBNR w KQkq - 0 2",
                       "r2q2k1/6p1/p3p1Q1/2p1n1N1/3p4/5r2/PP5P/6RK w - - 0 28",
                       "1k4r1/ppp2q1r/3p1p2/4p3/PPPnP1Q1/2KP2RN/5P2/R7 w - - 7 33",
                       "8/1R6/3rp3/3p4/K1kP4/4P3/1P6/8 b - - 0 35",
                       "4k3/3r4/4pBK1/3b1p2/5P2/2R3P1/1P3b2/8 w - - 3 40"};



int main() {
    int used_seed = time(0);
    int bot_depth = 3;
    int scores[6] = {0,0,0,0,0,0};
    srand(used_seed);
    cout << used_seed << endl;
    string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    //fen = "8/1k2r3/8/8/8/8/2K5/8 w - - 0 1";
    //fen = "7K/1r6/r7/8/6k1/p7/8/8 b - - 0 1";
    //fen = "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1";

    for (int i = 0; i<100000; i++) {
        auto *ui_white = new Interface{0, new Ascii_ui(1), new Random_ui(1), new Bot_ui(1, bot_depth)},
             *ui_black = new Interface{2, new Ascii_ui(0), new Random_ui(0), new Bot_ui(0, bot_depth)};
        Board b = Board(fen, ui_white, ui_black, true);
        if (ui_white->type == 0) {ui_white->bui->set_board(&b);}
        if (ui_black->type == 2) {ui_black->bui->set_board(&b);}
        int test = b.run();
        scores[test==6?5:test]++;
        ofstream Output("C:/Users/janne/OneDrive/Desktop/Chesscpp-main/scores4.txt");
        Output << scores[0] << " " << scores[1] << " " << scores[2] << " " << scores[3] << " " << scores[4] << " "<<scores[5];
        Output.close();
        if (i % 50 == 0) {
            cout << "total " << i << endl << ((float)i)/((float)(time(0)-used_seed)) << " games/s" << endl << endl;
        }
    }
        //cout << i << endl;
    return 0;
}


