#ifndef UI
#define UI
#include "Pieces.h"

// Wetigkeiten der Figuren fuer Minimax
static const float eval_list[6] = {10, 30, 32, 50, 90, 900};
// Bewertungen der Qualität jeder Brettposition fuer jede Figur
static const float eval_positions[12][64] = {{0, 0, 0, 0, 0, 0, 0, 0, -5, -5, -5, -5, -5, -5, -5, -5, -1, -1, -2, -3, -3, -2, -1, -1, -0.5, -0.5, -1, -2.5,
                                               -2.5, -1, -0.5, -0.5, 0, 0, 0, -2, -2, 0, 0, 0, -0.5, 0.5, 1, 0, 0, 1, 0.5, -0.5, -0.5, -1, -1, 2, 2, -1, -1,
                                               -0.5, 0, 0, 0, 0, 0, 0, 0, 0}, // p
                                             {5, 4, 3, 3, 3, 3, 4, 5, 4, 2, 0, 0, 0, 0, 2, 4, 3, 0, -1, -1.5, -1.5, -1, 0, 3, 3, -0.5, -1.5, -2, -2, -1.5,
                                              -0.5, 3, 3, 0, -1.5, -2, -2, -1.5, 0, 3, 3, -0.5, -1, -1.5, -1.5, -1, -0.5, 3, 4, 2, 0, -0.5, -0.5, 0, 2, 4,
                                              5, 4, 3, 3, 3, 3, 4, 5}, // n
                                             {2, 1, 1, 1, 1, 1, 1, 2, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, -0.5, -1, -1, -0.5, 0, 1, 1, -0.5, -0.5, -1, -1, -0.5,
                                              -0.5, 1, 1, 0, -1, -1, -1, -1, 0, 1, 1, -1, -1, -1, -1, -1, -1, 1, 1, -0.5, 0, 0, 0, 0, -0.5, 1, 2, 1, 1, 1,
                                              1, 1, 1, 2}, // b
                                             {0, 0, 0, 0, 0, 0, 0, 0, -0.5, -1, -1, -1, -1, -1, -1, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0,
                                              0.5, 0.5, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 0.5, 0, 0, 0, -0.5, -0.5,
                                              0, 0, 0}, // r
                                             {2, 1, 1, 0.5, 0.5, 1, 1, 2, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, -0.5, -0.5, -0.5, -0.5, 0, 1, 0, 0, -0.5, -0.5,
                                              -0.5, -0.5, 0, 0, 0, 0, -0.5, -0.5, -0.5, -0.5, 0, 0, 1, 0, 0, -0.5, -0.5, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 2,
                                              1, 1, 0.5, 0.5, 1, 1, 2}, // q
                                             {3, 4, 4, 5, 5, 4, 4, 3, 3, 4, 4, 5, 5, 4, 4, 3, 3, 4, 4, 5, 5, 4, 4, 3, 3, 4, 4, 5, 5, 4, 4, 3, 2, 3, 3, 4, 4,
                                              3, 3, 2, 1, 2, 2, 2, 2, 2, 2, 1, -2, -2, 0, 0, 0, 0, -2, -2, -2, -3, -1, 0, 0, -1, -3, -2}, // k
                                             {0, 0, 0, 0, 0, 0, 0, 0, 0.5, 1, 1, -2, -2, 1, 1, 0.5, 0.5, -0.5, -1, 0, 0, -1, -0.5, 0.5, 0, 0, 0, 2, 2, 0, 0,
                                              0, 0.5, 0.5, 1, 2.5, 2.5, 1, 0.5, 0.5, 1, 1, 2, 3, 3, 2, 1, 1, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0,
                                              0}, // P
                                             {-5, -4, -3, -3, -3, -3, -4, -5,
                                              -4, -2, 0, 0.5, 0.5, 0, -2, -4,
                                              -3, 0.5, 1, 1.5, 1.5, 1, 0.5, -3,
                                              -3, 0, 1.5, 2, 2, 1.5, 0, -3,
                                              -3, 0.5, 1.5, 2, 2, 1.5, 0.5, -3,
                                              -3, 0, 1, 1.5, 1.5, 1, 0, -3,
                                              -4, -2, 0, 0, 0, 0, -2, -4,
                                              -5, -4, -3, -3, -3, -3, -4, -5}, // N
                                             {-2, -1, -1, -1, -1, -1, -1, -2,
                                              -1, 0.5, 0, 0, 0, 0, 0.5, -1,
                                              -1, 1, 1, 1, 1, 1, 1, -1,
                                              -1, 0, 1, 1, 1, 1, 0, -1,
                                              -1, 0.5, 0.5, 1, 1, 0.5, 0.5, -1,
                                              -1, 0, 0.5, 1, 1, 0.5, 0, -1,
                                              -1, 0, 0, 0, 0, 0, 0, -1,
                                              -2, -1, -1, -1, -1, -1, -1, -2}, // B
                                             {0, 0, 0, 0.5, 0.5, 0, 0, 0,
                                              -0.5, 0, 0, 0, 0, 0, 0, -0.5,
                                              -0.5, 0, 0, 0, 0, 0, 0, -0.5,
                                              -0.5, 0, 0, 0, 0, 0, 0, -0.5,
                                              -0.5, 0, 0, 0, 0, 0, 0, -0.5,
                                              -0.5, 0, 0, 0, 0, 0, 0, -0.5,
                                              0.5, 1, 1, 1, 1, 1, 1, 0.5,
                                              0, 0, 0, 0, 0, 0, 0, 0}, // R
                                             {-2, -1, -1, -0.5, -0.5, -1, -1, -2,
                                              -1, 0, 0, 0, 0, 0, 0, -1,
                                              -1, 0, 0, 0.5, 0.5, 0, 0, -1,
                                              0, 0, 0.5, 0.5, 0.5, 0.5, 0, 0,
                                              0, 0, 0.5, 0.5, 0.5, 0.5, 0, 0,
                                              -1, 0, 0.5, 0.5, 0.5, 0.5, 0, -1,
                                              -1, 0, 0, 0, 0, 0, 0, -1,
                                              -2, -1, -1, -0.5, -0.5, -1, -1, -2}, // Q
                                             {2, 3, 1, 0, 0, 1, 3, 2,
                                              2, 2, 0, 0, 0, 0, 2, 2,
                                              -1, -2, -2, -2, -2, -2, -2, -1,
                                              -2, -3, -3, -4, -4, -3, -3, -2,
                                              -3, -4, -4, -5, -5, -4, -4, -3,
                                              -3, -4, -4, -5, -5, -4, -4, -3,
                                              -3, -4, -4, -5, -5, -4, -4, -3,
                                              -3, -4, -4, -5, -5, -4, -4, -3}}; // K

// Definition der Klasse Board, um deren Funtionen im Anschluss uebergeben zu koennen
class Board;
// Allgemeine Definition eines Interfaces
class Ui {
public:
    // Definition der Farbe
    explicit Ui(int color) {to_play = color;};
    // Boardposition aktualisieren
    void set_pieces(Piece[64], const bool[64]);
    int to_play; // Farbe der Ui
    vector<int> own_pieces; // Felder mit eigenen Figuren
    bool piece_exists[64]{}; // Kopie der Figurenliste
    Piece piece_list[64]; // Kopie der Figurexistezliste
    // funktion fuer einfache Textersetzung
    static void replace(string &str, const string &from, const string &to) {
        size_t start_pos = str.find(from);
        if (start_pos != string::npos) {
            str.replace(start_pos, from.length(), to);
        }
    }
};
// Ascii Ui: einfaches Interface, um Zuege zu uebergeben
class Ascii_ui : public Ui {
public:
    explicit Ascii_ui(int color) : Ui(color) {} // Konstrukton
    tuple<bool, int, int, int> move(); // Zugfunktion
};
// Ui, die zufaellige Zuege macht
class Random_ui : public Ui {
public:
    explicit Random_ui(int color) : Ui(color) {}; // Konstruktor
    tuple<bool, int, int, int> move(); // Zug Funktion
};
// Ui fuer Minmax Algorithmus
class Bot_ui: public Ui {
public:
    explicit Bot_ui(int color, int dep) : Ui(color) {depth = dep;}; // Konstruktor mit Suchtiefe
    tuple<bool, int, int, int> move(); // Zug Funktion
    void set_board(Board *b) {game = b;}; // Funktion, um Zugriff auf das Brett zu erhalten
private:
    Board *game{};
    int depth;
    vector<tuple<int, int, int>> best_moves_this_iteration;
    float evaluate(Piece *, const bool *, const int &move_count);
    vector<tuple<int, int, int>> order_moves(int color);
    float min_max(int local_depth, float alpha, float beta, bool capture_sort = false, int end_depth = 0);
    vector<float> pre_scores;
    vector<tuple<int, int, int>> capture_order(int color);
};
#endif
