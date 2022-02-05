#include "Headers/Pieces.h"

// Constructor einer normalen Figur
Piece::Piece(int g_color, int g_type) {
    type = g_type;
    color = g_color;
    for (auto &i: view) {
        i = {};
    }
}

// "leere" Spielfigur
Piece::Piece() {
    color = (short) 7;
    type = (short) 7;
}

// getter - Methoden für Zugriffe auf Figuren
int Piece::get_color() const { return color; }

int Piece::get_type() const { return type; }

// gibt zurück, ob sich eine Position in einer der 8 Sichtlinien befindet und wenn ja, in welcher
// erhält dazu die gesuchte Position pos und die Position der Figur selbst piece
tuple<bool, vector<int> &> Piece::pos_in_view(const int &piece, const int &pos) {
    int begin = 0, end = 0;

    // für Positionen kleiner der eigenen müssen nur die ersten vier view-Arrays betrachtet werden,
    // sonst nur die letzten, bei Bauern stehen alle 3 Sichtachsen in den ersten 3 Sichtlinien
    if (pos < piece || type == 0) { begin = 0, end = 4; }
    else { begin = 4, end = 8; }
    for (int i = begin; i < end; i++) {

        if (find(view[i].begin(), view[i].end(), pos) != view[i].end()) {
            return {true, view[i]};
        }
    }
    // Rückgabe von false, falls pos nicht gefunden wurde
    return {false, view[0]};
}

const vector<int> &Piece::get_legal_moves() { return legal_moves; } // getter-Methoden für alle Arrays der Figur
const vector<int> &Piece::get_real_moves() { return real_moves; }

const vector<int> &Piece::get_pins() { return pins; }

const vector<int> &Piece::get_defend() { return legal_defend; }

const vector<int> &Piece::get_update() { return update; }


// überprüft, ob sich eine Position in den real-Moves der Figur befindet
bool Piece::pos_in_real_moves(const short int &pos) {
    vector<int>::iterator in;
    in = find(real_moves.begin(), real_moves.end(), pos);
    if (in != real_moves.end()) {
        return true;
    } else {
        return false;
    }
}

// Überprüft, ob sich eine Position in den legal-defend-Zügen einer Figur befindet
bool Piece::pos_in_defend(const short int &pos) {
    vector<int>::iterator in;
    in = find(legal_defend.begin(), legal_defend.end(), pos);
    if (in != legal_defend.end()) {
        return true;
    } else {
        return false;
    }
}

//setter-Methoden
void Piece::set_legal_moves(const vector<int> &moves) { legal_moves = moves; }

void Piece::set_pins(const vector<int> &moves) { pins = moves; }

void Piece::set_defend(const vector<int> &moves) { legal_defend = moves; }

void Piece::add_defend(vector<int> new_moves) {
    legal_defend.insert(legal_defend.end(), new_moves.begin(), new_moves.end());
}

void Piece::set_update(const vector<int> &moves) { update = moves; }

// für die 8 View Vektoren wird ein Pointer auf den ersten der Vektoren gegeben
void Piece::set_view(const vector<int> *first_group) {
    for (int i = 0; i < 8; i++) {
        view[i] = *(first_group + i);
    }
}

// die real-Moves besitzen zusätzliche Methoden, neben dem Setzen gibt es eine zum Entfernen und Hinzufügen von Zügen
void Piece::set_real_moves(
        const vector<int> &new_real_moves) { real_moves = new_real_moves; } // set real moves separately from generate moves
void Piece::add_real_moves(vector<int> new_real_moves) {
    real_moves.insert(real_moves.end(), new_real_moves.begin(), new_real_moves.end());
}

void Piece::pop_real_moves(const vector<int> &to_remove) {
    for (auto &elem: to_remove) {
        vector<int>::iterator in;
        in = find(real_moves.begin(), real_moves.end(), elem);
        if (in != real_moves.end()) {
            real_moves.erase(in);
        }
    }
}
