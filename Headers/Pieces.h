#include "main.h"

class Piece {
public:
    Piece(int g_color, int g_type); // constructor initialises all as empty vectors
    Piece();
    [[nodiscard]] int get_color() const;
    [[nodiscard]] int get_type() const;


    bool pos_in_legal_moves(const short int &); // methods to see if pos in specific list
    bool pos_in_real_moves(const short int &);
    bool pos_in_pins(const short int &);
    bool pos_in_defend(const short int &);
    bool pos_in_update(const short int &);
    tuple<bool, vector<int> &> pos_in_view(const int &, const int &); // returns bool and sub_vector containing pos if in, else false and last element of array, also used as view get-method

    const vector<int> & get_legal_moves(); // get-methods
    const vector<int> & get_real_moves();
    const vector<int> & get_pins();
    const vector<int> & get_defend();
    const vector<int> & get_update();


    void set_legal_moves(const vector<int>&);
    void set_pins(const vector<int>&);
    void set_defend(const vector<int>&);
    void add_defend(vector<int> new_moves);
    void set_update(const vector<int>&);
    void set_view(const vector<int> *);
    void set_real_moves(const vector<int>& new_real_moves); // set real moves separately from generate moves
    void add_real_moves(vector<int> new_real_moves);
    void pop_real_moves(const vector<int>&);

    //void test_set_view(vector<int>[8]); // test method for manipulating lists
private:
    int color, type;// 0-5, p, n, b, r, q, k
    vector<int> legal_moves = {}, real_moves = {}, pins = {}, view[8] = {}, legal_defend = {}, update = {};


};
struct Move {
    int type{}; //normal, takes, ep, castle, pro, pro capture (0-5)
    int pos{}, tar{}, rook_pos{}, rook_tar{}, promotion{}, promotion_old{};
    bool en_passant{}, castles[4]{};
    Piece obj, rook_obj , tar_obj;
};
struct Position {
    tuple<int, int> position[64];
    int castles[4];
    int en_passant;
};
static int convert_piece_type(char piece) {
    int int_piece = (int)toupper(piece);
    switch (int_piece) {
        case 80: return 0; //pawn
        case 78: return 1; //knight
        case 66: return 2; //bishop
        case 82: return 3; //rook
        case 81: return 4; //queen
        case 75: return 5; //king
        default: return -1; //others can't happen
    }
}
static char convert_piece_type(int color, int piece) {
    return (char)((7.0/6)*pow(piece, 5)-15.125*pow(piece, 4)+(815.0/12)*pow(piece, 3)-120.375*pow(piece, 2)+773.0*piece/12+80+(1-color)*32);
}
