#include "Headers/ui.h"
#include "Headers/Board.h"

// Parameter für die Nutzung zusätzlicher Evaluationen für den schwarzen Bot
bool space_calculation = true;
bool connected_pawns = true;
bool open_lines = true;
bool double_pawns = true;
bool endgame_close_king = true;
bool single_pawns = true;
bool open_bishops = true;


// Sortieren der Züge anhand es scores Vektors durch Bubble-Sort, ist multiply aktiviert, so werden die niedrigsten
// anstelle der höchsten Werte nach vorne sortiert (Sortierung im Sinne des schwarzen Spielers)
vector<tuple<int, int, int>>
sort_moves(vector<tuple<int, int, int>> &moves, vector<float> &scores, bool multiply = false) {
    for (int i = 0; i < moves.size() - 1; i++) {
        for (int j = i + 1; j > 0; j--) {
            int swap = j - 1;
            if (float(1 - 2 * multiply) * scores.at(swap) < float(1 - 2 * multiply) * scores.at(j)) {
                iter_swap(moves.begin() + j, moves.begin() + swap);
                iter_swap(scores.begin() + j, scores.begin() + swap);
            }
        }
    }
    return moves;
}

//setzt Figurenlisten des jeweiligen Spielers sowie den Vektor eigener Figuren | Für alle Interfaces
void Ui::set_pieces(Piece *pl, const bool *pe) {
    own_pieces.clear();
    for (int piece = 0; piece < 64; piece++) {
        if (*(pe + piece)) {
            piece_exists[piece] = true;
            piece_list[piece] = *(pl + piece);
            if ((pl + piece)->get_color() == to_play) {
                own_pieces.push_back(piece);
            }
        } else { piece_exists[piece] = false; }
    }
}

// move-Funktion der für menschliche Interaktion gebauten Ui
tuple<bool, int, int, int> Ascii_ui::move() {
    bool got_move = false, try_pawn = false;
    tuple<bool, int, int, int> move;
    int old_pos, tar, promotion;
    string raw_move, raw_piece;

    // Wiederholt die Zugabfrage, solange kein ordentlicher Zug eingegeben wurde
    while (!got_move) {
        old_pos = -1, tar = -1;

        // Für den Fall eines Bauernzuges, bei dem das p für pawn weggelassen wurde, wird versucht, dieses hinzuzufügen
        // und dann auf einen gültigen Zug zu testen
        if (!try_pawn) {
            promotion = -1;
            cin >> raw_move;
            // Sonderzeichen, die in der Standardnotation auftreten, werden entfernt
            replace(raw_move, "=", ""); // Für Entwicklung
            replace(raw_move, "x", ""); // Für Schlagzüge
            replace(raw_move, "+", ""); // Für Schach
            replace(raw_move, "#", ""); // Für Matt
        } else { raw_move = "p" + raw_piece + raw_move; }


        // Hinzufügen von p bei Zügen der Länge 2, präventiv und erfolgreich bei "e4", nicht aber bei "cxd5"
        if (raw_move.length() == 2 && raw_move != "uu") { raw_move = "p" + raw_move; }

        // Abfangen von Rochadenotationen
        if (raw_move == "0-0" || raw_move == "O-O") {
            if (piece_exists[60 - to_play * 56] &&
                piece_list[60 - to_play * 56].pos_in_real_moves(short(62 - to_play * 56))) {
                move = tuple(false, 60 - to_play * 56, 62 - to_play * 56, -1);
                return move;
            }
        } else if (raw_move == "0-0-0" || raw_move == "O-O-O") {
            if (piece_exists[60 - to_play * 56] &&
                piece_list[60 - to_play * 56].pos_in_real_moves(short(58 - to_play * 56))) {
                move = tuple(false, 60 - to_play * 56, 58 - to_play * 56, -1);
                return move;
            }
        } else if (raw_move == "u") {
            move = tuple(true, 1, 0, 0); // Abfangen von Steuerbefehlen | Zug-Zurück (undo)
            return move;
        } else if (raw_move == "r") {
            move = tuple(true, 2, 0, 0); // Zug wiederholen (redo), nicht implementiert
            return move;
        } else if (raw_move == "uu") {
            move = tuple(true, 3, 0, 0); // eigenen und gegnerischen Zug zurück, nicht implementiert
            return move;
        } else if (raw_move == "to_last") {
            move = tuple(true, 4, 0, 0); // redo bis "aktuelle" Postion erreicht, nicht implementiert
            return move;
        } else if (raw_move == "to start") {
            move = tuple(true, 5, 0, 0); // undo bis zum Spielbeginn, nicht implementiert
            return move;
        } else if (raw_move == "new game") {
            move = tuple(true, 6, 0, 0); // startet neues Spiel, bitte nicht verwenden, funktioniert nicht ordentlich
            return move;
        } else if (raw_move == "pgn") {
            move = tuple(true, 7, 0, 0); // stellt das Spiel bis zum aktuellen Punkt in pgn-Notation dar
            return move;
        } else {
            // erhalt der Figur von der 0. Position des Zuges unter der Annahme, Bauernzüge seien durch p markiert
            int piece = convert_piece_type((raw_move.at(0))); // type of piece to move
            raw_piece = raw_move.at(0);
            raw_move = string(raw_move.begin() + 1, raw_move.end());

            // speichern von Entwicklungen
            if (isalpha((char) raw_move.at(raw_move.length() - 1))) {
                promotion = convert_piece_type(toupper(raw_move.at(raw_move.length() - 1)));
                raw_move.pop_back();
            }

            // Umgang bei Spezifikation einer Zeile, Spalte oder beidem (rad5)
            if ((raw_move.length() == 3 || raw_move.length() == 4) && !isalpha(raw_move.at(raw_move.length() - 1))) {

                // erhalte Zielfeld des Zuges
                tar = (int(unsigned(raw_move.at(raw_move.length() - 1))) - 49) * 8 +
                      int(unsigned(toupper(raw_move.at(raw_move.length() - 2)))) - 65;

                // Suche nach Startfeld unter gegebenen Information, Annahme, dass mehrere Figuren das Feld
                // betreten können
                int row = -1, col = -1;
                if (isalpha(raw_move.at(0))) {
                    // Behandelt den Fall, in dem Spalte gegeben ist
                    col = int(unsigned(toupper(raw_move.at(0)))) - 65;
                    // Behandelt den Fall, in dem zudem auch Zeile gegeben ist
                    if (raw_move.length() == 4) { row = int(unsigned(raw_move.at(1))) - 49; }
                } else {
                    // Behandelt den Fall, in dem nur Zeile gegeben ist
                    row = int(unsigned(raw_move.at(0))) - 49;
                }
                if (!(col == -1 || row == -1)) {
                    // Überprüfung des Startfeldes, wenn Zeile und Spalte gegeben wurden
                    if (find(own_pieces.begin(), own_pieces.end(), row * 8 + col) != own_pieces.end() &&
                        piece_list[row * 8 + col].get_type() == piece) { old_pos = row * 8 + col; }
                } else {
                    // Suche nach der fehlenden Information (Zeile/ Spalte) für den Fall, dass nur eins der
                    // beiden gegeben war
                    for (int pos: own_pieces) {
                        if ((row == pos / 8 || col == pos % 8) &&
                            find(piece_list[pos].get_real_moves().begin(), piece_list[pos].get_real_moves().end(),
                                 tar) != piece_list[pos].get_real_moves().end() &&
                            piece_list[pos].get_type() == piece) {
                            old_pos = pos;
                            got_move = true;
                            break;
                        }
                    }
                }

            } else if (raw_move.length() == 2) {
                // Fall, dass das Startfeld nicht spezifiziert wurde
                tar = (int(unsigned(raw_move.at(1))) - 49) * 8 + int(unsigned(toupper(raw_move.at(0)))) - 65;
                // Iteration durch alle Figuren, um jene zu finden, die das Zielfeld als Zug besitzt
                // Annahme, dass es nur eine derartige Figur gibt, sonst wird die erste gewählt
                for (int p = 0; p < 64; p++) {
                    if (find(own_pieces.begin(), own_pieces.end(), p) == own_pieces.end()) { continue; }
                    vector<int> moves = piece_list[p].get_real_moves();
                    if (find(moves.begin(), moves.end(), tar) != moves.end() && piece_list[p].get_type() == piece) {
                        old_pos = p;
                        got_move = true;
                        break;
                    }
                }
            } else if (!try_pawn) {
                // Wenn die Länge mit keiner gegebenen übereinstimmt, dann wird "p" vor den Zug geschrieben und erneut
                // derselbe Zug getestet
                try_pawn = true;
                continue;
            } else {
                // wenn es sich auch um keinen verkürzt notierten Bauernzug ("e4" statt "pe4") handelte, so wird ein
                // neuer Zug abgefragt, der aktuelle war ungültig
                try_pawn = false;
                continue;
            }
        }
        if (old_pos == -1 || tar == -1) {
            // wurde kein gültiges Startfeld gefunden, so wird "p" vor den Zug geschrieben und erneut versucht.
            try_pawn = true;
            continue;
        }
        // zurückgeben des Zuges
        move = tuple(false, old_pos, tar, promotion);
        return move;
    }
}

// Interfaces des Bots, der zufällige Züge spielen soll
tuple<bool, int, int, int> Random_ui::move() {
    int piece;
    while (true) {
        // wähle eine Figur, welche noch Züge besitzt
        piece = own_pieces.at(rand() % own_pieces.size());
        if (!piece_list[piece].get_real_moves().empty()) { break; }
    }
    // wähle einen Zug
    int move = piece_list[piece].get_real_moves().at(rand() % piece_list[piece].get_real_moves().size());
    int promotion = -1;
    // wenn es sich um einen Bauern handelt, der sich mit dem Zug entwickelt, generiere zudem einen Figurenwert für
    // die Entwicklung
    if (piece_list[piece].get_type() == 0 && ((to_play == 0 && (piece / 8) == 1 && (move / 8) == 0) ||
                                              (to_play == 1 && (piece / 8) == 6 && (move / 8) == 7))) {
        promotion = (int(rand())) % 4 + 1;
    }
    // Zurückgeben des generierten Zuges
    return {false, piece, move, promotion};
}


// move-Funktion des Minmax-Spielers
tuple<bool, int, int, int> Bot_ui::move() {
    // Speichert besten Zug
    tuple<int, int, int> best_move;
    float score;
    // bereinigt beste-Züge-Vektor aus vorherigem Aufruf
    best_moves_this_iteration.clear();
    // Aufruf des Minmax-Algorithmus
    score = min_max(depth, -10000000, 10000000);
    // wähle einen der besten Züge
    best_move =
            best_moves_this_iteration.size() == 1 ? best_moves_this_iteration.at(0) : best_moves_this_iteration[rand() %
                                                                                                                (best_moves_this_iteration.size() -
                                                                                                                 1)];
    // Ausgabe der Evaluation
    cout << "eval " << score / 10 << endl;
    // Rückgabe des Zuges
    return {false, get<0>(best_move), get<1>(best_move), get<2>(best_move)};
}

//Minmax-Algorithmus
float Bot_ui::min_max(int local_depth, float alpha, float beta) {
    // Evaluation, wenn Suchtiefe erreicht
    if (local_depth == 0) { return evaluate(game->get_pl(), game->get_pe(), game->move_count); }
    // sortieren aller eigenen Züge nach vermuteter Stärke
    vector<tuple<int, int, int>> moves = order_moves(game->to_play);

    // Suchbeginn
    float max_score = -10000000;
    float min_score = 10000000;
    // Simulation jedes Zuges
    for (const auto& move: moves) {
        game->make_move(move);
        // aktualisieren der Züge und Überprüfung des Spielendes
        int game_end = game->update_moves();
        switch (game_end) {
            case -1:
                break;
            case 0: // Schwarz gewinnt
                game->undo(); // Zug rückgängig machen
                game->update_moves(); // Züge aktualisieren, um Artefaktzüge in nächstem Simulationsschritt zu verhindern

                // Zug wird zu besten Zügen hinzugefügt, wenn beste Züge noch leer sind
                // oder Zug besser als alle vorherigen ist
                if (local_depth == depth &&
                    (best_moves_this_iteration.empty() ||
                     (to_play == 0 && (-100000 + (float) (depth - local_depth + 1)) < min_score) ||
                     (to_play == 1 && (-100000 + (float) (depth - local_depth + 1)) > max_score))) {
                    best_moves_this_iteration = {move};
                } else if (local_depth == depth &&
                           ((to_play == 0 && (-100000 + (float) (depth - local_depth + 1)) == min_score) ||
                            (to_play == 1 && (-100000 + (float) (depth - local_depth + 1)) ==
                                             max_score))) { best_moves_this_iteration.push_back(move); }

                // gibt willkürlichen, niedrigen Wert mit Beachtung der aktuellen Suchtiefe zurück
                return (-100000) + (float) (depth - local_depth + 1);
            case 1: // Weiß gewinnt
                game->undo();
                game->update_moves();
                if (local_depth == depth &&
                    (best_moves_this_iteration.empty() ||
                     (to_play == 0 && (100000 - (float) (depth - local_depth + 1)) < min_score) ||
                     (to_play == 1 && (100000 - (float) (depth - local_depth + 1)) > max_score))) {
                    best_moves_this_iteration = {move};
                } else if (local_depth == depth &&
                           ((to_play == 0 && (100000 - (float) (depth - local_depth + 1)) == min_score) ||
                            (to_play == 1 && (100000 - (float) (depth - local_depth + 1)) ==
                                             max_score))) { best_moves_this_iteration.push_back(move); }

                // gibt willkürlichen, hohen Wert mit Beachtung der aktuellen Suchtiefe zurück
                return (100000) - (float) (depth - local_depth + 1);
            default: // draw
                game->undo();
                game->update_moves();
                if (local_depth == depth && (best_moves_this_iteration.empty() || (to_play == 0 && 0 < min_score) ||
                                             (to_play == 1 && 0 > max_score))) { best_moves_this_iteration = {move}; }
                else if (local_depth == depth && ((to_play == 0 && 0 == min_score) || (to_play == 1 && 0 ==
                                                                                                       max_score))) {
                    best_moves_this_iteration.push_back(move);
                }

                // gibt 0 zurück
                return 0;
        }
        // Rekursiver Aufruf der nächsten Suchtiefe
        float eval = min_max(local_depth - 1, alpha, beta);


        // Rückgängigmachen des letzten Zuges und aktualisieren aller Züge
        game->undo();
        game->update_moves();

        // Unterscheidung in weiße oder schwarze Suchebene
        if (game->to_play) {
            // Test, ob gefundener Zug bester Zug ist
            if (eval > max_score) {
                max_score = eval;
                // Hinzufügen zu besten Zügen, sofern oberste Suchtiefe wieder erreicht
                if (local_depth == depth) { best_moves_this_iteration = {move}; }
            } else if (eval == max_score && local_depth == depth) {
                // Hinzufügen des Zuges wenn bester Zug, aber nicht besser als vorher bester
                best_moves_this_iteration.push_back(move);
            }
            // Aktualisierung des Alpha-Wertes
            alpha = max(alpha, eval);
            // Anwendung des Alpha-Beta-Prunings
            if (beta <= alpha) { break; }
        } else {
            // dasselbe für Schwarz
            if (eval < min_score) {
                min_score = eval;
                if (local_depth == depth) { best_moves_this_iteration = {move}; }
            } else if (eval == min_score && local_depth == depth) {
                best_moves_this_iteration.push_back(move);
            }
            beta = min(beta, eval);
            if (beta <= alpha) { break; }
        }
    }
    // Rückgabe der besten Evaluation
    if (game->to_play) { return max_score; }
    else { return min_score; }
}

// Evaluation
float Bot_ui::evaluate(Piece *pl, const bool *pe, const int &move_count) {

    // Parameter zur Speicherung für spezielle Eval-Funktionen
    float eval = 0; // Eval-Wert

    bool queens = false; // Existenz von Damen | Könige im Endspiel
    bool all[2] = {false, false}; // Existenz von Spielsteinen (ohne König) pro Spieler | Könige im Endspiel
    int kings[2]; // Position der Könige | Könige im Endspiel
    int space_map[64]{0}; // Kontrolle über Felder | Raum (Space)
    int pawn_map[2][8]{0}; // Anzahl der Bauern pro Spalte und Spieler-Farbe | Doppelbauern, Freibauern, offene Linien
    vector<int> rook_pos[2]; // Positionen aller Türme pro Spieler | offene Linien
    int bishopsnknights = 0; // Differenz von Läufern-gegnerischen Läufern, dasselbe für Springer | Läufer und Springer



    // Hauptschleife
    for (int i = 0; i < 64; i++) {
        if (!(*(pe + i))) { continue; }
        Piece &piece = *(pl + i);

        // Materialaddition für König bei "Könige im Endspiel"
        if (piece.get_type() == 5 && to_play == 0 && endgame_close_king) {
            eval += eval_list[piece.get_type()] * (float) (-1 + 2 * piece.get_color());
            kings[piece.get_color()] = i;
        } else {
            // Standardaddition
            eval += eval_list[piece.get_type()] * (float) (-1 + 2 * piece.get_color()) +
                    0.4 * eval_positions[piece.get_color() * 6 + piece.get_type()][i];
        }
        // Setzen von all und queens, d.h. Existenz von Figuren/ Damen
        if (piece.get_type() != 5) {
            all[piece.get_color()] = true;
            if (piece.get_type() == 4) { queens = true; }
        }
        // Bei Raumberechnung wird für jeden Zug 1 zum jeweiligen Zielfeld addiert/ abgezogen (je nach Farbe)
        if (space_calculation && to_play == 0) {
            int multiplier = piece.get_color() == 1 ? 1 : -1;
            for (int move: piece.get_defend()) {
                space_map[move] += multiplier;
            }
        }
        // Belohnung verbundener Bauern
        if (connected_pawns && to_play == 0 && piece.get_type() == 0) {
            for (int column: {-1, 1}) {
                for (int row: {-1, 0, 1}) {
                    if (i % 8 + column > -1 && i % 8 + column < 8 && i / 8 + row > -1 && i / 8 + row < 8) {
                        if ((pl + ((i / 8 + row) * 8 + i % 8 + column))->get_type() == 0 &&
                            (pl + ((i / 8 + row) * 8 + i % 8 + column))->get_color() == piece.get_color()) {
                            eval += (float) (piece.get_color() == 1 ? 1 : -1) *
                                    (column == (piece.get_color() == 0 ? 1 : -1) ? 0.2 : 0.1);
                            break;
                        }
                    }
                }
            }
        }
        // bei weiß +1 für Läufer, -1 für Pferdchen, für schwarz anders herum,
        // d.h. je höher (für weiß) der Wert, desto besser sind offene Linien für den Spieler
        if (open_bishops && to_play == 0 && piece.get_type() == 2) {
            bishopsnknights += -1 + 2 * piece.get_color();
        } else if (open_bishops && to_play == 0 && piece.get_type() == 1) {
            bishopsnknights += 1 - 2 * piece.get_color();
        }
        // Bauern in pawn_map eintragen
        if ((double_pawns || open_lines || single_pawns || open_bishops) && to_play == 0 && piece.get_type() == 0) {
            pawn_map[piece.get_color()][i % 8]++;
        }
        // Türme in rook_pos eintragen
        if (open_lines && to_play == 0 && piece.get_type() == 3) {
            rook_pos[piece.get_color()].push_back(i);
        }
    }


    // Beginn der Auswertung der erstellten Arrays
    if (space_calculation && to_play == 0) {
        int full_space = 0;
        int square;
        //Raumboni für Schwarz
        for (int sq = 0; sq < 32; sq++) {
            square = space_map[sq];
            full_space += square < 0 ? -1 : 0;
        }
        // Raumboni für Weiß
        for (int sq = 32; sq < 64; sq++) {
            square = space_map[sq];
            full_space += square > 0 ? 1 : 0;
        }
        // 0.1 für jedes Feld kontrollierte Feld, d.h. maximal 3.2
        eval += (float) full_space * 0.1;
    }
    // Doppelbauern, ab dem 2. eigenen Bauern auf jeder Spalte -0.1 für jeden weiteren Bauern der Spalte
    if (double_pawns && to_play == 0) {
        for (int &i: pawn_map[0]) { eval += (i > 1 ? (float) i / 10 : 0); }
        for (int &i: pawn_map[1]) { eval -= (i > 1 ? (float) i / 10 : 0); }
    }
    // offene und halboffene Linien
    if (open_lines && to_play == 0) {
        // schwarze offene Linien
        for (int &i: rook_pos[0]) {
            if (pawn_map[0][i] == 0) {
                if (pawn_map[1][i] == 0) {
                    eval -= 0.1;
                }
            }
        }
        // weiße offene Linien
        for (int &i: rook_pos[1]) {
            if (pawn_map[1][i] == 0) {
                if (pawn_map[0][i] == 0) {
                    eval += 0.1;
                }
            }
        }
        // Halboffene Linien
        for (int pawn = 0; pawn < 8; pawn++) {
            if (pawn_map[1][pawn] == 0 && pawn_map[0][pawn] != 0) {
                eval += 0.1;
            } else if (pawn_map[0][pawn] == 0 && pawn_map[1][pawn] != 0) {
                eval -= 1;
            }
        }
    }
    // Freibauern, 0.2 pro Bauern
    if (single_pawns && to_play == 0) {
        // weiße Freibauern
        for (int pawn = 0; pawn < 8; pawn++) {
            if (pawn_map[1][pawn] > 0 && pawn_map[0][pawn] == 0 && pawn == 0 ||
                pawn_map[0][pawn - 1] == 0 && pawn == 7 || pawn_map[0][pawn + 1] == 0) { eval += 0.2; }
        }
        // schwarze Freibauern
        for (int pawn = 0; pawn < 8; pawn++) {
            if (pawn_map[0][pawn] > 0 && pawn_map[1][pawn] == 0 && pawn == 0 ||
                pawn_map[1][pawn - 1] == 0 && pawn == 7 || pawn_map[1][pawn + 1] == 0) { eval -= 0.2; }
        }
    }
    // Läufer und Springer, 0.1 Punkte mal dem zuvor ausgerechneten Multiplikator pro offener Linie, da dieser bei
    // 2 Läufern gegen 2 Pferde =4 wäre, bei 8 offenen Linien = 0.4*8=3.2, ist der Multiplikator auf ±2 begrenzt.
    if (open_bishops && to_play == 0) {
        for (int pawn = 0; pawn < 8; pawn++) {
            if (pawn_map[0][pawn] == 0 && pawn_map[1][pawn] == 0) {
                eval += 0.1 * (bishopsnknights < 0 ? max(-2, bishopsnknights) : min(2, bishopsnknights));
            }
        }
    }

    // Kalkulationen für Könige im Enspiel
    if (endgame_close_king && to_play == 0) {
        // wenn es noch Damen gibt, dann wird die Postionskarte verwendet
        if (queens) { eval += eval_positions[11][kings[1]] - eval_positions[5][kings[0]]; }
        else {
            if (!all[0]) {
                // hat der Gegner keine Figuren mehr, so ist es gut,
                // wenn sein König nah am eigenen und möglichst am Rand steht
                eval += (float) (7 - max(abs(kings[0] / 8 - kings[1] / 8), abs(kings[0] % 8 - kings[1] % 8)));
                eval += (float) 0.5 *
                        (float) (max(7 - kings[0] / 8, kings[0] / 8) + max(7 - kings[0] % 8, kings[0] % 8));
            }
            if (!all[1]) {
                eval -= (float) (7 - max(abs(kings[0] / 8 - kings[1] / 8), abs(kings[0] % 8 - kings[1] % 8)));
                eval -= (float) 0.5 *
                        (float) (max(7 - kings[1] / 8, kings[1] / 8) + max(7 - kings[1] % 8, kings[1] % 8));
            }
        }
    }
    // Rückgabe des (gerundeten) Eval-Wertes, die Rundung dient dem Ausgleich von float-Rechenfehlern
    return roundf(eval * 100) / 100;
}

// Zugsortierung
vector<tuple<int, int, int>> Bot_ui::order_moves(int color) {
    Piece *pl = game->get_pl();
    bool *pe = game->get_pe();
    vector<tuple<int, int, int>> ordered_moves;
    vector<float> scores;
    // ordnet jedem Zug einen score-Wert zu
    for (int i = 0; i < 64; i++) {
        if (!*(pe + i)) { continue; }
        Piece &piece = *(pl + i);
        if (color != piece.get_color()) { continue; }
        // positive Werte für Entwicklungen (Materialdifferenz aus entwickelter Figur und "verlorenem" Bauern)
        if (piece.get_type() == 0 && i / 8 == 1 + 5 * color) {
            for (int move: piece.get_real_moves()) {
                for (int prom = 1; prom < 5; prom++) {
                    ordered_moves.emplace_back(i, move, prom);
                    if (*(pe + move)) {
                        // höherer Werte für Entwicklung mit Schlag
                        scores.push_back(eval_list[prom] - eval_list[0] - eval_list[(pl + move)->get_type()]);
                    } else { scores.push_back(eval_list[prom] - eval_list[0]); } // Normale Entwicklung
                }
            }
        } else {
            for (int move: piece.get_real_moves()) {
                ordered_moves.emplace_back(i, move, -1);
                if (*(pe + move)) {
                    // Bonus für Schlagzüge (Materialdifferenz, Annahme, das schlagende Figur verloren wird)
                    scores.push_back(eval_list[(pl + move)->get_type()] - eval_list[piece.get_type()]);
                } else if (color == 1 &&
                           (move / 8 + 1 < 8 && move % 8 - 1 > -1 && *(pe + move + 7) &&
                            (pl + move + 7)->get_color() != color && (pl + move + 7)->get_type() == 0 ||
                            move / 8 + 1 < 8 && move % 8 + 1 < 8 && *(pe + move + 9) &&
                            (pl + move + 9)->get_color() != color && (pl + move + 9)->get_type() == 0)) {
                    scores.push_back(-eval_list[((pl + move)->get_type())] - eval_list[0]);
                    // Minuspunkte, wenn Figur schräg vor feindlichem Bauern steht, d.h. von diesem geschlagen werden
                    // könnte (minus Materialdifferenz aus eigener Figur und Bauer)
                } else if (color == 0 &&
                           (move / 8 - 1 > -1 && move % 8 - 1 > -1 && *(pe + move - 9) &&
                            (pl + move - 9)->get_color() != color && (pl + move - 9)->get_type() == 0 ||
                            move / 8 - 1 > -1 && move % 8 + 1 < 8 && *(pe + move - 7) &&
                            (pl + move - 7)->get_color() != color && (pl + move - 7)->get_type() == 0)) {
                    scores.push_back(-eval_list[((pl + move)->get_type())] - eval_list[0]);
                    // exakt dasselbe wie darüber, aber ein Fall mit oder wäre zu unlesbar gwesen
                } else { scores.push_back(0); /* Normaler Zug erhält Wertung von 0*/}
            }
        }
    }
    // Züge und scores werden sortiert zurückgegeben
    return sort_moves(ordered_moves, scores);
}

