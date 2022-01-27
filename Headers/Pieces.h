#ifndef PIECES
#define PIECES
#include "main.h"

class Piece {
public:
    Piece(int g_color, int g_type); // Initialisierung einer Figur, 0-schwarz, 1-weiss
    // 0-5 fuer Bauer, Springer, Laeufer, Turm, Dame, Koenig
    Piece();
    [[nodiscard]] int get_color() const; // getter Funktionen
    [[nodiscard]] int get_type() const;

    bool pos_in_legal_moves(const short int &); // prueft Anwesenheit eines Feldes in der jeweiligen Liste der Figur
    bool pos_in_real_moves(const short int &);
    bool pos_in_pins(const short int &);
    bool pos_in_defend(const short int &);
    bool pos_in_update(const short int &);
    tuple<bool, vector<int> &> pos_in_view(const int &, const int &); // returns bool and sub_vector containing pos if in, else false and last element of array, also used as view get-method

    const vector<int> & get_legal_moves(); // mehr getter Funktionen
    const vector<int> & get_real_moves();
    const vector<int> & get_pins();
    const vector<int> & get_defend();
    const vector<int> & get_update();

    void set_legal_moves(const vector<int>&); // setter Funktionen
    void set_pins(const vector<int>&);
    void set_defend(const vector<int>&);
    void add_defend(vector<int> new_moves);
    void set_update(const vector<int>&);
    void set_view(const vector<int> *);
    void set_real_moves(const vector<int>& new_real_moves); // set fuer Ueberschreiben
    void add_real_moves(vector<int> new_real_moves); // add fuer hinzufuegen
    void pop_real_moves(const vector<int>&); // pop um zu entfernen

private:
    int color, type;// 0-5, p, n, b, r, q, k
    // legal moves fuer alle Zuege ohne Pins, en passant, rochade
    // real moves fuer alle erlaubten Zuege
    // pins fuer die Sichtlinie, in der sich der Konig befindet
    // view fuer alle alle Zuege ohne Hindernisserkennung nach Richtung sortiert
    // legal defend wie legal_moves + erste eigene Figur jeder Sichtlinie, fuer Bauern nur Diagonalzuege
    // update wie legal defend, für Bauern aber alle Zuege
    vector<int> legal_moves = {}, real_moves = {}, pins = {}, view[8] = {}, legal_defend = {}, update = {};
};
struct Move {
    int type{}; //normaler Zug, Schlag, En Passant, Rochade, Entwicklung, Schlag + Entwicklung (0-5)
    int pos{}, tar{}, rook_pos{}, rook_tar{}, promotion{}, promotion_old{}, mr_count{}, en_passant{};
    bool castles[4]{}; // alles was den Zug ausmacht
    Piece obj, rook_obj , tar_obj; // Zu speichernde Figuren
};
struct Position {
    vector<int> position; // Vector aller Positionen + Rochaderechte
    int en_passant; // En Passant Feld
};
static int convert_piece_type(char piece) {
    int int_piece = (int)toupper(piece);
    switch (int_piece) {
        case 80: return 0; //Bauer
        case 78: return 1; //Springer
        case 66: return 2; //Laeufer
        case 82: return 3; //Turm
        case 81: return 4; //Dame
        case 75: return 5; //Koenig
        default: return -1; //kann nicht eintreten
    }
}
static char convert_piece_type(int color, int piece) {
    switch (piece) {
        case 0:
            return (char) (112- 32*color); //Bauer
        case 1:
            return (char) (110 - 32*color); //Springer
        case 2:
            return (char) (98 - 32*color); //Laeufer
        case 3:
            return (char) (114 - 32*color); //Turm
        case 4:
            return (char) (113 - 32*color); //Dame
        case 5:
            return (char) (107 - 32*color); //Koenig
        default:
            return -1; //kann nicht eintreten
    }
}

#endif