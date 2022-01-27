#include "Headers/Pieces.h"
Piece::Piece(int g_color, int g_type) {
    type = g_type;
    color = g_color;
    for (auto & i : view) {
        i = {};
    }
}
Piece::Piece() {
    color = (short) 7;
    type = (short) 7;
}

int Piece::get_color() const {return color;}
int Piece::get_type() const {return type;}

tuple<bool, vector<int> &> Piece::pos_in_view(const int &piece, const int &pos) {
    int begin = 0, end = 0;

    if (pos < piece || type == 0) {begin = 0, end = 4;}
    else {begin = 4, end = 8;}
    for (int i = begin; i < end; i++) {

        if (find(view[i].begin(), view[i].end(), pos) != view[i].end()) {
            return {true, view[i]};
        }
    }
    return {false, view[0]};
}
const vector<int> & Piece::get_legal_moves() {return legal_moves;}; // get-methods
const vector<int> & Piece::get_real_moves() {return real_moves;};
const vector<int> & Piece::get_pins() {return pins;};
const vector<int> & Piece::get_defend() {return legal_defend;};
const vector<int> & Piece::get_update() {return update;};

bool Piece::pos_in_legal_moves(const short int & pos) {
    vector<int>::iterator in;
    in = find(legal_moves.begin(), legal_moves.end(), pos);
    if (in != legal_moves.end()) {
        return true;
    } else {
        return false;
    }
}
bool Piece::pos_in_real_moves(const short int & pos) {
    vector<int>::iterator in;
    in = find(real_moves.begin(), real_moves.end(), pos);
    if (in != real_moves.end()) {
        return true;
    } else {
        return false;
    }
}
bool Piece::pos_in_pins(const short int & pos) {
    vector<int>::iterator in;
    in = find(pins.begin(), pins.end(), pos);
    if (in != pins.end()) {
        return true;
    } else {
        return false;
    }
}
bool Piece::pos_in_defend(const short int & pos) {
    vector<int>::iterator in;
    in = find(legal_defend.begin(), legal_defend.end(), pos);
    if (in != legal_defend.end()) {
        return true;
    } else {
        return false;
    }
}
bool Piece::pos_in_update(const short int & pos) {
    vector<int>::iterator in;
    in = find(update.begin(), update.end(), pos);
    if (in != update.end()) {
        return true;
    } else {
        return false;
    }
}
void Piece::set_legal_moves(const vector<int>& moves) {legal_moves = moves;}
void Piece::set_pins(const vector<int>& moves) {pins = moves;}
void Piece::set_defend(const vector<int>& moves) {legal_defend = moves;}
void Piece::add_defend(vector<int> new_moves) {legal_defend.insert(legal_defend.end(),new_moves.begin(), new_moves.end());}
void Piece::set_update(const vector<int>& moves) {update = moves;}
void Piece::set_view(const vector<int> * first_group) {
    for (int i = 0; i<8; i++) {
        view[i] = *(first_group+i);
    }
}
void Piece::set_real_moves(const vector<int>& new_real_moves) {real_moves = new_real_moves;} // set real moves separately from generate moves
void Piece::add_real_moves(vector<int> new_real_moves) {real_moves.insert(real_moves.end(),new_real_moves.begin(), new_real_moves.end());}
void Piece::pop_real_moves(const vector<int>& to_remove) {
    for (auto & elem:to_remove) {
        vector<int>::iterator in;
        in = find(real_moves.begin(), real_moves.end(), elem);
        if (in != real_moves.end()) {
            real_moves.erase(in);
        }
    }
}
