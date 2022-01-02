#include "Pieces.h"


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
    explicit Random_ui(int color) : Ui(color) {}
    tuple<bool, int, int, int> move();
};

class Bot_ui: public Ui {
private:
    int en_pasant, move_count, king_positions[2];
    bool castles[4];
    vector<Position> positions;

    Move last_move;
    vector<Move> move_list;
};


