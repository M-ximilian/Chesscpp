#ifndef UI
#define UI
#include "Pieces.h"

static const int eval_list[6] = {10, 30, 32, 50, 90, 900};
class Board;

class Ui {
public:
    explicit Ui(int color) {to_play = color;};
    void set_pieces(Piece[64], const bool[64]);
    int to_play;
    vector<int> own_pieces;
    bool piece_exists[64]{};
    Piece piece_list[64];
    static void replace(string &str, const string &from, const string &to) {
        size_t start_pos = str.find(from);
        if (start_pos != string::npos) {
            str.replace(start_pos, from.length(), to);
        }
    }


};

class Ascii_ui : public Ui {


public:
    explicit Ascii_ui(int color) : Ui(color) {}
    tuple<bool, int, int, int> move();
};

class Random_ui : public Ui {
public:
    explicit Random_ui(int color) : Ui(color) {};
    tuple<bool, int, int, int> move();
};

class Bot_ui: public Ui {
public:
    explicit Bot_ui(int color, int dep) : Ui(color) {depth = dep;};
    tuple<bool, int, int, int> move();
    void set_board(Board *b) {game = b;};
private:
    Board *game{};
    int depth;
    tuple<float, int, int, int> min_max(int local_depth, bool gameto_play, double alpha, double beta);
    static float evaluate(Piece *, const bool *);
    vector<tuple<int, int, int>> order_moves(int color);
};
#endif

