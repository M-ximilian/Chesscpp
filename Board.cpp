#include "Headers/Board.h"

// Brett Konstruktor
Board::Board(const string &startpos, Interface *white, Interface *black,
             bool draw) {
    // Variablen initialisieren
    int count = 0, spaces = -1;
    draw_board = draw;
    delete ui_white;
    delete ui_black;
    ui_white = white, ui_black = black;

    // Umrechnen der Startposition in FEN-Notation in eigene, gespeicherte Werte
    for (unsigned char value: startpos) {
        int real_count = 8 * (7 - count / 8) + count % 8;
        int int_of_value = (int) value;
        if (count > 63) {
            if (int_of_value == 32) {
                spaces++;
                continue;
            } else {
                switch (spaces) {
                    case 0:
                        to_play = value == 'w'; // color to move
                        break;
                    case 1: // castles
                        switch (value) {
                            case 'K':
                                castles[0] = true;
                                break;
                            case 'Q':
                                castles[1] = true;
                                break;
                            case 'k':
                                castles[2] = true;
                                break;
                            case 'q':
                                castles[3] = true;
                                break;
                            default:
                                break;
                        }
                        break;
                    case 2:
                        if (value == '-') {
                            en_passant = -1;
                            continue;
                        } // en_passant
                        else if (int_of_value < 58) {
                            en_passant += int_of_value - 49;
                        } else {
                            en_passant = (int_of_value - 97) * 8;
                        }
                        break;
                    case 3: // 50 move rule
                        if (value == '-') {
                            mr_count = 0;
                        } else {
                            mr_count = int_of_value - 48;
                        }
                        break;
                    case 4: // half_moves
                        move_count = int_of_value - 48;
                        break;
                    default:
                        break;
                }
            }
            continue;
        }
        if (int_of_value != 47) {
            if (48 < int_of_value && int_of_value < 57) {
                for (int j = 0; j < int_of_value - 48; j++) {
                    piece_exists[real_count] = false;
                    count++;
                }
                continue;
            } else {
                piece_exists[real_count] = true;
                bool color = int_of_value <= 96;
                piece_list[real_count] = Piece(color, convert_piece_type((char) value));
                if (int_of_value == 75) {
                    king_positions[1] = real_count;
                } else if (int_of_value == 107) {
                    king_positions[0] = real_count;
                }
                count++;
            }
        }
    }
}

// Draw-Methode des Brettes
// Player auf 0 zu setzen zeichnet aus Schwarz's Perspektive
void Board::draw(int player) {
    if (!uci.empty()) {
        cout << uci.at(uci.size() - 1);
    }
    for (int i = 0; i < 64; i++) {
        int real_i =
                (player == 1 ? 8 * (7 - i / 8) + i % 8 : i / 8 * 8 + 7 - (i % 8));
        string to_print;
        to_print = (piece_exists[real_i] ? "  " : "# ");
        if (piece_exists[real_i]) {
            if (i % 8 == 7) {
                // Änderung zu z.B. "fancy_symbols" ändert Darstellung
                cout << fancier_symbols[piece_list[real_i].get_color() * 6 +
                                        piece_list[real_i].get_type()]
                     << endl;
            } else {
                cout << fancier_symbols[piece_list[real_i].get_color() * 6 +
                                        piece_list[real_i].get_type()]
                     << " ";
            }
        } else {
            if (i % 8 == 7) {
                cout << "# " << endl;
            } else {
                cout << "# "
                     << "";
            }
        }
    }
    cout << endl;
}

// Zentraler Spielcontroller
int Board::run() {
    while (true) {
        int game_end = update_moves();
        if (game_end >= 0) { // Break, Matt | Unentschieden
            if (draw_board) {
                draw(1);
            }
            return game_end; // Spiel vorbei
        }
        while (true) {
            // bool wahr heißt Systembefehl in erstem int, sonst Zug in Form [pos, tar, prom], Entwicklung normal -1
            tuple<bool, int, int, int> move_case;
            if (draw_board) {
                draw(1);
            }

            // Rufe move() des Spielers am Zug auf
            if (to_play) {
                ui_white->set_pieces(get_pl(), get_pe());
                move_case = ui_white->move();
            } else {
                ui_black->set_pieces(get_pl(), get_pe());
                move_case = ui_black->move();
            }
            // Bearbeiten von Steuerbefehlen
            if (get<0>(move_case)) {
                if (get<1>(move_case) == 6) {
                    return 6;
                } // neues Spiel
                else if (get<1>(move_case) == 1) {
                    undo();
                    undo();
                } // undo move
                else if (get<1>(move_case) == 7) {
                    cout << get_pgn() << endl;
                } // print portable game notation
                else {
                    continue;
                }

            } else {
                // Zug erhalten
                cout << "move " << (char) (get<1>(move_case) % 8 + 97)
                     << (get<1>(move_case) / 8 + 1) << " "
                     << (char) (get<2>(move_case) % 8 + 97)
                     << (get<2>(move_case) / 8 + 1) << " " << endl;
                // move == castling, prüfe ob möglich
                if (piece_exists[get<1>(move_case)] &&
                    piece_list[get<1>(move_case)].get_color() == to_play &&
                    piece_list[get<1>(move_case)].get_type() == 5 &&
                    get<1>(move_case) == 60 - 56 * to_play) {
                    if (get<2>(move_case) == 56 - 56 * to_play) {
                        move_case = {get<0>(move_case), get<1>(move_case),
                                     58 - 56 * to_play, get<3>(move_case)};
                    } else if (get<2>(move_case) == 63 - 56 * to_play) {
                        move_case = {get<0>(move_case), get<1>(move_case),
                                     62 - 56 * to_play, get<3>(move_case)};
                    }
                }
                // normaler Zug, prüfe, ob ziehende Figur aktuellem Spieler gehört und Promotion-Wert realistisch ist
                if (piece_exists[get<1>(move_case)] &&
                    piece_list[get<1>(move_case)].get_color() == to_play &&
                    get<3>(move_case) >= -1 && get<3>(move_case) < 5 &&
                    get<3>(move_case) != 0) {
                    make_move(get<1>(move_case), get<2>(move_case),
                              get<3>(move_case)); // normaler Zug gestattet und ausgeführt
                    break;
                }
            }
        }
    }
}

// Updates für die Positionen von Figuren, alte Figur wird deaktiviert und an neue Pos geschrieben, bei Promotion wird
// zudem Typ geändert
void Board::update(int old, int target, Piece obj, int promotion = -1) {
    if (promotion != -1) {
        piece_exists[old] = false;
        piece_list[target] = Piece(to_play, promotion);
        piece_exists[target] = true;
    } else {
        piece_exists[old] = false;
        piece_list[target] = obj;
        piece_exists[target] = true;
    }
}

// Vereinfacht aufruf von make_move.
// Wandelt Tripel in benötigte 3 Parameter um und ruft make_move damit auf
void Board::make_move(tuple<int, int, int> move) {
    make_move(get<0>(move), get<1>(move), get<2>(move));
}

// macht Züge
void Board::make_move(int old, int target, int promotion) {

    // Zurücksetzen der Listen, falls Zug aus undo-Kette heraus stattfindet
    if (undo_count > 0) {
        for (int i = 0; i < undo_count; i++) {
            move_list.pop_back();
            positions.pop_back();
            uci.pop_back();
        }
        undo_count = 0;
    }

    Piece &obj = piece_list[old];
    int rook_old = -1, rook_new = -1;

    // Fälle für Rochade, en passant, Schlag- und normale Züge
    Move add; // Zugformat für Speicherung
    if (obj.get_type() == 5 && abs(target % 8 - old % 8) > 1) { // Rochaden
        if (target - old == 2) {
            rook_old = old / 8 * 8 + 7;
            rook_new = old / 8 * 8 + 5;
        } else if (target - old == -2) {
            rook_old = old / 8 * 8;
            rook_new = old / 8 * 8 + 3;
        }
        Piece &rook_obj = piece_list[rook_old];

        // erstellen des Zuges mit Turmobjekten und Positionen
        add = {3, old,
               target, rook_old,
               rook_new, -1,
               -1, mr_count,
               -1, {castles[0], castles[1], castles[2], castles[2]},
               obj, rook_obj};
        // Verschieben des Turmes an seine Zielposition
        update(rook_old, rook_new, rook_obj, -1);

    } else if (obj.get_type() == 0 && target == en_passant) { // en passant
        int tar_en_passant = old / 8 * 8 + target % 8;
        Piece &tar_obj = piece_list[tar_en_passant];
        piece_exists[tar_en_passant] = false; // deaktivieren der geschlagenen Figur, wird "manuell" gemacht

        // erstellen des Zuges
        add = {2, old,
               target, -1,
               -1, -1,
               -1, mr_count,
               en_passant, {castles[0], castles[1], castles[2], castles[2]},
               obj, {},
               tar_obj};

    } else if (piece_exists[target]) { // Schlagzug
        Piece &tar_obj = piece_list[target];

        //erstellen des Zuges für spätere Speicherung
        add = {1, old,
               target, -1,
               -1, -1,
               -1, mr_count,
               -1, {castles[0], castles[1], castles[2], castles[2]},
               obj, {},
               tar_obj};
        if (promotion != -1) { // Entwicklung
            // hinzufügen des Promotion-Wertes bei Entwicklung
            add = {5,
                   old,
                   target,
                   -1,
                   -1,
                   promotion,
                   obj.get_type(),
                   mr_count,
                   -1,
                   {castles[0], castles[1], castles[2], castles[2]},
                   obj,
                   {},
                   tar_obj};
        }
        mr_count = 0; // Zähler der 50-Zug-Regel wird bei Schlägen zurückgesetzt

    } else { // normaler Zug
        add = {0, old,
               target, -1,
               -1, -1,
               -1, mr_count,
               en_passant, {castles[0], castles[1], castles[2], castles[2]},
               obj, {},
               {}};
        if (promotion != -1) { // Entwicklung
            add = {4,
                   old,
                   target,
                   -1,
                   -1,
                   promotion,
                   obj.get_type(),
                   mr_count,
                   -1,
                   {castles[0], castles[1], castles[2], castles[2]},
                   obj,
                   {},
                   {}};
        }
    }
    move_list.push_back(add); // erstellter Zug wird gespeichert
    update(old, target, obj, promotion); // Figur wird bewegt
    vector<int> new_position;
    // Position für Stellungswiederholung wird erzeut und gespeichert
    for (int pos = 0; pos < 64; pos++) {
        new_position.push_back(piece_exists[pos]
                               ? piece_list[pos].get_color() * 6 +
                                 piece_list[pos].get_type() + 1
                               : 0);
    }
    for (bool castle: castles) {
        new_position.push_back(castle);
    }
    positions.push_back({new_position, en_passant});

    // Rochaderechte werden erneuert
    if (obj.get_type() == 5) {
        if (obj.get_color() == 1) {
            castles[0] = false;
            castles[1] = false;
        } else {
            castles[2] = false;
            castles[3] = false;
        }
        king_positions[obj.get_color()] = target;
    } else if (obj.get_type() == 3) {
        switch (old) {
            case 0:
                castles[1] = false;
                break;
            case 7:
                castles[0] = false;
                break;
            case 56:
                castles[3] = false;
                break;
            case 63:
                castles[2] = false;
                break;
            default:
                break;
        }
    }
    // Zug wird für PGN-Notation gespeichert
    string uci_move;
    uci_move.push_back(convert_piece_type(0, piece_list[old].get_type()));
    uci_move.push_back(char(old % 8 + 97));
    uci_move.push_back(char(old / 8 + 49));
    uci_move.push_back(char(target % 8 + 97));
    uci_move.push_back(char(target / 8 + 49));
    if (promotion != -1) {
        uci_move.push_back('=');
        uci_move.push_back(convert_piece_type(0, promotion));
    }

    uci.push_back(uci_move);

    // Zugzähler und 50-Züge-Zähler werden erhöht
    move_count++;
    mr_count++;

    // ändern des Spielers am Zug
    to_play = 1 - to_play;
}

// Zug-Zurück-Funktion, für Minmax benötigt
void Board::undo() {
    undo_count++;
    move_count--;
    // auslesen von Werten aus dem letzten Zug
    Move last_move = move_list.at(move_list.size() - undo_count);
    mr_count = last_move.mr_count;
    en_passant = last_move.en_passant;

    // erhalten aller Eigenschaften des letzten Zuges
    int old_pos = last_move.pos,
    tar_pos = last_move.tar,
    rook_old_pos = last_move.rook_pos,
    rook_tar_pos = last_move.rook_tar,
    promotion_type = last_move.promotion;

    Piece obj = last_move.obj,
    tar_obj = last_move.tar_obj,
    rook_obj = last_move.rook_obj;

    // king_positions Array wenn nötig aktualisieren
    if (obj.get_type() == 5) {
        king_positions[obj.get_color()] = old_pos;
    }
    // wiederherstellen der Rochaderechte
    for (int i = 0; i < 4; i++) {
        castles[i] = last_move.castles[i];
    }
    // Indices: 0 normal, 1 Schlag, 2 en passant, 3 Rochade, 4 Entwicklung, 5 Entwicklung mit Schlag
    switch (last_move.type) {
        case 1:
        case 5:
            // schlagende Figur wird wiederhergestellt
            piece_list[old_pos] = obj;
            piece_exists[old_pos] = true;
            break;
        case 2:
            // Schlagende Figur wird wiederhergestellt, Zielfeld der schlagenden Figur ist nun leer
            piece_exists[tar_pos] = false;
            // ändert Reihe zwischen 3 und 4, Spalte bleibt
            tar_pos = 32 - 8 * to_play + tar_pos % 8;
            piece_list[old_pos] = obj;
            piece_exists[old_pos] = true;
            break;
        default:
            break;
    }
    switch (last_move.type) {
        case 1:
        case 2:
        case 5:
            // Wiederherstellen der geschlagenen Figur
            piece_list[tar_pos] = tar_obj;
            piece_exists[tar_pos] = true;
            break;
        case 3: // Rochade rückgängig machen
            update(tar_pos, old_pos, obj);
            update(rook_tar_pos, rook_old_pos, rook_obj);
            break;
        default:
            // Figur wird nur zurückbewegt
            update(tar_pos, old_pos, obj);
    }
    // Spieler am Zug ändert sich
    to_play = 1 - to_play;
}

// legal_moves
void Board::generate_piece_moves(int position) {
    Piece &piece = piece_list[position];
    piece.set_pins({});
    int type_for_pointer; // Typ wie in possible-moves-array verwendet
    if (piece.get_type() == 0) { type_for_pointer = (int) (!(bool) piece.get_color()); }
    else { type_for_pointer = piece.get_type() + 1; }

    vector<int> *piece_moves = possible_moves[type_for_pointer][position];// genutzte possible-moves
    vector<int> legal, pins, defend, update; // neue Vektoren für alle Züge
    int same_kind; // Für eventuell kollidierende Figur(en)

    // Unterscheidung: Bauer oder kein Bauer?
    if (piece.get_type() != 0) {
        vector<int> *pointer_other_king; // Züge des fremden Königs, um zu-nahe-kommen der Könige zu unterbinden

        // König oder nicht?
        bool piece_is_king = true;
        if (position == king_positions[0]) { pointer_other_king = possible_moves[6][king_positions[1]]; }
        else if (position == king_positions[1]) { pointer_other_king = possible_moves[6][king_positions[0]]; }
        else { piece_is_king = false; }

        // setzen der Abbruchbedingungen des hinzufügen zu den Arrays
        bool pin_stopped, other_stopped, king_found = false, no_king_add, own_color_in_pins = false;

        // iterieren durch jede Blickrichtung
        for (int l = 0; l < 8; l++) {
            pin_stopped = false;
            other_stopped = false;
            own_color_in_pins = false;
            pins = {};
            for (int move: *(piece_moves + l)) { // iterieren durch jeden Zug
                if (pin_stopped && other_stopped) { break; } // stoppen, falls eh nirgendwo hinzugefügt werden könnte

                // same_kind ermitteln, 1 für selbe Farbe, 0 für fremde, -1 für keine Figur auf Feld
                if (piece_exists[move] &&
                    piece_list[move].get_color() == piece.get_color()) { same_kind = 1; }
                else if (piece_exists[move]) { same_kind = 0; }
                else { same_kind = -1; }

                // Zug nicht hinzufügen, wenn er für fremden König zugänglich ist (Könige würden nebeneinander stehen)
                no_king_add = false;
                if (piece_is_king) {
                    for (int r = 0; r < 8; r++) {
                        int opponent_move;
                        if (!(r + pointer_other_king)->empty()) {
                            opponent_move = (r + pointer_other_king)->at(0);
                        }
                        else { continue; }
                        if (opponent_move == move) {
                            no_king_add = true;
                            break;
                        }
                    }
                }
                if (!other_stopped) { // für alles bis auf Fesselungen
                    // Update beinhaltet alle Positionen, auf denen Figuren Aktualisierungen erfahren müssten, falls,
                    // falls die Figur sich bewegt, bzw. alle Positionen, für die diese Figur geupdated werden müsste,
                    // wenn sich eine Figur von einem oder auf eins der Felder bewegen würde
                    update.push_back(move);
                    if (same_kind == 1) {
                        // defend enthält zusätzlich zu normalen Zügen auch je die erste eigene Figur in jeder
                        // Blickrichtung, d.h. alle Figuren und Felder, die die eigene Figur verteidigt.
                        defend.push_back(move);
                        other_stopped = true; // Figur getroffen
                    } else if (same_kind == 0) {
                        other_stopped = true;
                        if (!no_king_add) { // damit Könige sich nicht zu nahe kommen
                            // legal_moves beinhaltet alle pseudolegalen Züge ohne Beachtung von Schach, Fesselungen,
                            // Rochaden und en passant
                            legal.push_back(move);
                        }
                    } else {
                        // nichts getroffen
                        if (!no_king_add) {
                            legal.push_back(move);
                        }
                        defend.push_back(move);
                    }
                }
                // Pin Liste
                if (!pin_stopped) {
                    if (same_kind == 0) {
                        //Zug wird gespeichert, wenn der feindliche König gefunden wurde, sind potenziell Figuren
                        // gefesselt, die Liste muss nicht weitergeführt wurde.
                        pins.push_back(move);
                        if (move == king_positions[abs(piece.get_color() - 1)]) {
                            king_found = true;
                            pin_stopped = true;
                        }
                    } else if (same_kind == 1) {
                        // erlaubt eine eigene Figur in der pin-Liste, nötig um en passant Pins ordentlich zu erkennen
                        if (own_color_in_pins) { pin_stopped = true; }
                        else {
                            own_color_in_pins = true;
                            pins.push_back(move);
                        }
                    }
                }
            }
            if (king_found) {
                // en passant Pins nur bei genau 3 Figuren (König eingeschlossen) möglich, sonst darf keine eigene Figur
                // dabei sein
                if (own_color_in_pins && pins.size() == 3 || !own_color_in_pins) {
                    piece.set_pins(pins);
                    king_found = false;
                }

            }
        }
    } else {
        // Bauern
        // iterieren über alle 3 Blickrichtungen
        for (int l = 0; l < 3; l++) {
            vector<int> &line = piece_moves[l];
            if (line.empty()) { continue; }

            // erster (von max. 2) Zügen der Sichtlinie, entscheidend dafür,ob 2. überhaupt möglich sein kann
            int move = line.at(0);

            // same_kind setzen
            if (piece_exists[move] &&
                piece_list[move].get_color() == piece.get_color()) { same_kind = 1; }
            else if (piece_exists[move]) { same_kind = 0; }
            else { same_kind = -1; }

            // ein Zug oder 2 (von Grundreihe aus) möglich?
            if (line.size() == 1) {
                update.push_back(move);
                if (position % 8 == move % 8) {
                    // Zug geradeaus, Feld muss leer sein
                    if (same_kind == -1) { legal.push_back(move); }
                } else {
                    // Schräger Zug
                    defend.push_back(move);
                    if (same_kind == 0) { legal.push_back(move); }
                    if (move == king_positions[1 - piece.get_color()]) {
                        piece.set_pins({move});
                    }
                }
            } else {
                // 2 Feld langer Zug
                bool stop = false;
                update.push_back(move);
                if (same_kind == -1) { legal.push_back(move); }
                else { stop = true; }
                // Prüfen des 2. Zuges, sofern der erste möglich war
                if (!stop) {
                    move = line.at(1);
                    update.push_back(move);
                    if (!piece_exists[move]) { legal.push_back(move); }
                }
            }
        }
    }
    // setzen aller neuen Züge
    piece.set_view(piece_moves);
    piece.set_legal_moves(legal);
    piece.set_defend(defend);
    piece.set_update(update);
}

// all_moves
int Board::update_moves() {
    // Spielende durch 50-Züge-Regel
    if (mr_count > 99) {
        return 4;
    }
    // en passant aus der fen behalten
    if (!move_list.empty()) {
        en_passant = -1;
    }
    // nutzung der differenz aus Anzahl der Züge bis jetzt und dem undo count um zu überprüfen, ob man (wieder) am
    // Spielbeginn ist
    int udc = (int) move_list.size() - undo_count;

    // Variablen setzen
    bool heavy_piece = false,
    local_en_passant = false,
    long_possible = false,
    short_possible = false,
    check = false,
    any_moves = false;

    vector<Move> last_moves = {};

    vector<int> updated_positions,
    updated_pieces,
    update_by_pin,
    update_by_king;
    vector<tuple<int, int>> all_on_board;

    // Referenzen auf Könige
    Piece &king = piece_list[king_positions[to_play]],
            other_king = piece_list[king_positions[abs(to_play - 1)]];
    int king_pos = king_positions[to_play],
            other_king_pos = king_positions[abs(to_play - 1)];

    // Zu aktualisierende Figuren erhalten
    if (udc > 0) {

        if (undo_count == 1) {
            // Letzter und zuletzt rückgängig gemachter Zug werden für Aktualisierungen genutzt
            last_moves.push_back(move_list.at(move_list.size() - 1));
            last_moves.push_back(move_list.back());
        } else if (undo_count > 1) {
            // Letzter und zuletzt rückgängig gemachter Zug werden für Aktualisierungen genutzt
            last_moves.push_back(move_list.at(move_list.size() - undo_count + 1));
            last_moves.push_back(move_list.at(move_list.size() - undo_count));
        } else {
            // Nutzt nur den letzten Zug
            last_moves.push_back(move_list.back());
        }

        for (const Move &last_move: last_moves) {
            // updated_positions speichert alle Positionen, die sich geändert haben, werden sie von einer Figur gesehen,
            // so wird diese aktualisiert
            updated_positions.push_back(last_move.pos);
            updated_positions.push_back(last_move.tar);
            // Speichert gezogene Figur
            updated_pieces.push_back(last_move.tar);
            if (last_move.type == 3) {
                // dasselbe für Türme bei Rochaden
                updated_positions.push_back(last_move.rook_pos);
                updated_positions.push_back(last_move.rook_tar);
                updated_pieces.push_back(last_move.rook_tar);
            } else if (last_move.type == 4) {
                // geschlagene Figur im Falle von en passant
                updated_positions.push_back(last_move.pos / 8 * 8 + last_move.tar % 8);
            }
        }
    }
    // Iterieren über jede Figur
    for (int i = 0; i < 64; i++) {
        if (!piece_exists[i]) {
            continue;
        }
        Piece &piece = piece_list[i];
        if (!heavy_piece) {
            // Suche nach schweren Figuren für Unentschieden durch zu wenig Material
            if (piece.get_type() != 5) {
                all_on_board.emplace_back(piece.get_type(), i);
                if (piece.get_type() == 0 || piece.get_type() == 3 ||
                    piece.get_type() == 4) {
                    heavy_piece = true;
                }
            }
        }
        // Spielbeginn?
        if (udc > 0) {
            // wenn die Figur den König sieht, dann wird sie immer aktualisiert
            bool see_other_king =
                    get<0>(piece.pos_in_view(i, king_positions[!to_play]));
            if (piece.get_color() == to_play && see_other_king) {
                update_by_king.push_back(i);
            } else if (!updated_positions.empty()) {
                tuple<bool, vector<int>> see_king = piece.pos_in_view(i, king_pos),
                        see_org_pos =
                        piece.pos_in_view(i, updated_positions[0]),
                        see_new_pos =
                        piece.pos_in_view(i, updated_positions[1]);
                // Updaten, falls der König aus dem Sichtfeld gelaufen ist
                if (get<0>(see_king) && get<0>(see_org_pos) &&
                    get<1>(see_king) == get<1>(see_org_pos)) {
                    update_by_pin.push_back(i);
                } else if (get<0>(see_king) && get<0>(see_new_pos) &&
                           get<1>(see_king) == get<1>(see_new_pos)) {
                    // oder wenn der König in das Sichtfeld gelaufen ist
                    update_by_pin.push_back(i);
                }
            }
            // Updaten, wenn ein Element aus updated_positions gesehen wird
            bool compare_positions = false;
            for (int p: piece.get_update()) {
                for (int p1: updated_positions) {
                    if (p == p1) {
                        compare_positions = true;
                        break;
                    }
                }
                if (compare_positions) {
                    break;
                }
            }
            // Bedingungen für Updates, d.h. all die zuvor erstellten Listen, oder, wenn die Figur der König selbst ist
            if (find(updated_pieces.begin(), updated_pieces.end(), i) !=
                updated_pieces.end() ||
                find(update_by_pin.begin(), update_by_pin.end(), i) !=
                update_by_pin.end() ||
                find(update_by_king.begin(), update_by_king.end(), i) !=
                update_by_king.end() ||
                compare_positions || i == king_pos) {
                generate_piece_moves(i);
            }
        } else {
            // Bei Spielbeginn alle Figuren aktualisieren
            generate_piece_moves(i);
        }
        // Real-moves durch legal-moves ersetzen, um eventuelle Fesselungen letzter Züge aufzuheben
        piece.set_real_moves(piece.get_legal_moves());
    }
    if (!heavy_piece) {
        // Sind weniger als 2 Leichtfiguren auf dem Brett, so ist das Spiel unentschieden
        if (all_on_board.size() < 2) {
            return 6;
        } else if (all_on_board.size() == 2) {
            // Unentschieden gilt auch, wenn sich nur noch 2 Läufer derselben Feldfarbe auf dem Spielbrett befinden
            if (get<0>(all_on_board.at(0)) == 2 && get<0>(all_on_board.at(1)) == 2 &&
                (get<1>(all_on_board.at(0)) / 8 + get<1>(all_on_board.at(0)) % 8 +
                 get<1>(all_on_board.at(1)) / 8 + get<1>(all_on_board.at(1)) % 8) %
                2 ==
                0) {
                return 6;
            }
        }
    }
    // Ende der Updates

    // en passant wird gesetzt, wenn sich im letzten Zug ein Bauer zwei Felder weit bewegt hat
    if (udc > 0 &&
        move_list.at(move_list.size() - undo_count - 1).obj.get_type() == 0) {
        Move last_move = move_list.at(move_list.size() - undo_count - 1);
        int old_pos = last_move.pos, old_rank = old_pos / 8, file = old_pos % 8,
                new_rank = last_move.tar / 8;
        if (old_rank == 1 + 5 * to_play && new_rank == 3 + to_play) {
            // beide Nachbarfelder werden auf Bauern überprüft
            for (int i: {-1, 1}) {
                if (0 < file + i && file + i < 8) {
                    int pawn = new_rank * 8 + file + i;
                    if (piece_exists[pawn]) {
                        if (piece_list[pawn].get_type() == 0) {
                            if (piece_list[new_rank * 8 + file].get_color() !=
                                piece_list[pawn].get_color()) {
                                // dem gefundenen Bauern wird der en passant Zug hinzugefügt
                                piece_list[pawn].add_real_moves({8 * (2 + 3 * to_play) + file});
                                piece_list[pawn].add_defend({8 * (2 + 3 * to_play) + file});
                                local_en_passant = true;
                            }
                        }
                    }
                }
            }
        }
        if (local_en_passant) {
            // die Klassenvariable wird gesetzt, sodass en passant in makemove erkannt werden kann
            en_passant = 8 * (2 + 3 * to_play) + file;
        }
    } else if (move_list.empty() && en_passant > -1) {
        // en passant Züge aus der FEN werden intern verarbeitet und gespeichert
        for (int pawn = 0; pawn < 2; pawn++) {
            int pos = en_passant + 8 - (16 * to_play) - 1 + (2 * pawn);
            if (piece_exists[pos] && piece_list[pos].get_color() == to_play &&
                piece_list[pos].get_type() == 0) {
                piece_list[pos].add_real_moves({en_passant});
                piece_list[pos].add_defend({en_passant});
            }
        }
    }


    // Vorbereitungen für die lange Rochade
    int long_c[2] = {59 - to_play * 56, 58 - to_play * 56};
    // Rochaderecht
    if (castles[(1 - to_play) * 2 + 1]) {
        // Turm existiert
        if (piece_exists[(7 - to_play * 7) * 8]) {
            Piece &obj = piece_list[(7 - to_play * 7) * 8];
            // Turm ist auch ein Turm
            if (obj.get_type() == 3 &&
                obj.get_color() == to_play) {
                long_possible = true;
                // Test, ob alle Rochadefelder auch frei sind
                for (int tile: {(7 - to_play * 7) * 8 + 1, (7 - to_play * 7) * 8 + 2,
                                (7 - to_play * 7) * 8 + 3}) {
                    if (piece_exists[tile]) {
                        long_possible = false;
                        break;
                    }
                }
            }
        }
        if (long_possible) {
            // Zug zum König hinzufügen
            king.add_real_moves({long_c[1]});
        }
    }

    // Vorbereitungen für die kurze Rochade, Semantik gleich wie lange Rochade (nur andere Felder)
    int short_c[2] = {(7 - to_play * 7) * 8 + 5, (7 - to_play * 7) * 8 + 6};
    if (castles[(1 - to_play) * 2]) {
        if (piece_exists[(7 - to_play * 7) * 8 + 7]) {
            Piece &obj = piece_list[(7 - to_play * 7) * 8 + 7];
            if (obj.get_type() == 3 &&
                obj.get_color() == to_play) {
                short_possible = true;
                for (int tile: short_c) {
                    if (piece_exists[tile]) {
                        short_possible = false;
                        break;
                    }
                }
            }
        }
        if (short_possible) {
            king.add_real_moves({short_c[1]});
        }
    }

    // Hauptschleife für Fesselung und Schach
    vector<int> block_king;
    for (int piece_position = 0; piece_position < 64; piece_position++) {
        if (!piece_exists[piece_position]) {
            continue;
        }
        Piece &piece = piece_list[piece_position];
        // Findet jeweils König der Farbe der aufgerufenen Figur
        Piece &used_king = (find(update_by_king.begin(), update_by_king.end(),
                                 piece_position) != update_by_king.end()
                            ? other_king
                            : king);
        int &used_king_pos = (find(update_by_king.begin(), update_by_king.end(),
                                   piece_position) != update_by_king.end()
                              ? other_king_pos
                              : king_pos);
        // Sonderfall für pins von en passant, hier stehen 2 Figuren zwischen König und fesselnder Figur
        bool en_passant_pin = false;

        // Kann eine eigene Figur gepinnt sein?
        if ((king.get_color() != piece.get_color() ||
             used_king_pos == other_king_pos) &&
            !piece.get_pins().empty() && piece.get_pins().size() < 4) {
            // Sonderfall: Pin von en passant Bauer
            if (piece.get_pins().size() == 3 && en_passant != -1 &&
                piece_list[piece.get_pins().at(0)].get_type() == 0 &&
                piece_list[piece.get_pins().at(0)].get_color() == to_play &&
                piece_list[piece.get_pins().at(0)].pos_in_real_moves(en_passant)) {
                en_passant_pin = true;
                piece_list[piece.get_pins().at(0)].pop_real_moves({en_passant});
            } else if (piece.get_pins().size() == 3 && en_passant != -1 &&
                       piece_list[piece.get_pins().at(1)].get_type() == 0 &&
                       piece_list[piece.get_pins().at(1)].get_color() == to_play &&
                       piece_list[piece.get_pins().at(1)].pos_in_real_moves(
                               en_passant)) {
                en_passant_pin = true;
                piece_list[piece.get_pins().at(1)].pop_real_moves({en_passant});
            }
            if (!en_passant_pin) {
                // Normaler Pin | Schach
                const vector<int> &pin_line = piece.get_pins(),
                        view_line = get<1>(piece.pos_in_view(piece_position,
                                                             pin_line.at(0)));

                int range_begin = 0, range_end = 0, len = (int) pin_line.size();
                bool is_king;
                vector<int> all_to_remove, new_all_moves;
                // bei Schach verlieren alle Figuren die Züge, die das Schach nicht blocken
                if (len == 1) {
                    range_begin = 0;
                    range_end = 64;
                    check = true;
                } else {
                    // bei Fesselungen nur die gefesselte Figur
                    range_begin = pin_line.at(0);
                    range_end = range_begin + 1;
                }

                for (int pin_pos = range_begin; pin_pos < range_end; pin_pos++) {
                    if (!piece_exists[pin_pos] ||
                        piece_list[pin_pos].get_color() != used_king.get_color()) {
                        continue;
                    }
                    // Züge werden entfernt, wenn sie nicht blocken
                    Piece &pinned_piece = piece_list[pin_pos];
                    is_king =
                            pin_pos == king_positions[0] || pin_pos == king_positions[1];
                    vector<int> new_moves = {};
                    bool move_in, block_behind, take_attacker;
                    // durchlaufe alle Züge der betroffenen Figur
                    for (int move: pinned_piece.get_real_moves()) {
                        move_in = find(view_line.begin(), view_line.end(), move) !=
                                  view_line.end();
                        take_attacker = move == piece_position; //um die feindliche Figur schlagen zu können
                        // damit der König nicht durch nach hinten gehen ein Schach verlassen kann
                        block_behind = move <= max(piece_position, used_king_pos) &&
                                       move >= min(piece_position, used_king_pos);
                        if (!is_king && (move_in || take_attacker) && block_behind ||
                            is_king && !move_in) {
                            // Hinzufügen eines blockenden Zugs
                            new_moves.push_back(move);
                        } else {
                            // entfernen der Züge im Schach beim König
                            all_to_remove.push_back(move);
                        }
                    }
                    // neue Züge setzen
                    pinned_piece.set_real_moves(new_moves);
                }
            }
        }
        // Rochade durch Schach verhindern
        if (king.get_color() != piece.get_color()) {
            bool do_next;
            vector<int> to_erase;
            if (long_possible) {
                do_next = false;
                for (int &lo: long_c) {
                    if (piece.pos_in_defend((short) lo) || do_next) {
                        to_erase.push_back(lo);
                        do_next = true;
                    }
                }
            }
            if (short_possible) {
                do_next = false;
                for (int &sho: short_c) {
                    if (piece.pos_in_defend((short) sho) || do_next) {
                        to_erase.push_back(sho);
                        do_next = true;
                    }
                }
            }
            king.pop_real_moves(to_erase); // Illegale Königszüge entfernen
            block_king.insert(block_king.end(), piece.get_defend().begin(),
                              piece.get_defend().end());
        }
    }
    vector<int> new_real;
    // entfernen von Rochadezügen, wenn Rochade durch Schach verlaufen würde
    for (int move: king.get_real_moves()) {
        if (find(block_king.begin(), block_king.end(), move) == block_king.end()) {
            new_real.push_back(move);
        }
    }
    king.set_real_moves(new_real);
    //entfernen von Rochadezügen bei Schach
    if (check) {
        king.pop_real_moves({king_pos + 2, king_pos - 2});
    }

    // Test, ob der Spieler noch Züge hat
    for (int pos = 0; pos < 64; pos++) {
        if (!piece_exists[pos] || piece_list[pos].get_color() != to_play) {
            continue;
        }
        if (!piece_list[pos].get_real_moves().empty()) {
            any_moves = true;
            break;
        }
    }

    // Spielende testen
    if (!any_moves) {
        if (check) {
            return (int) !to_play;
        } // Matt
        else {
            return 2;
        } // Unentschieden
    } else if (positions.size() > 5) {
        // Stellungswiederholung
        int rep = 1;
        Position compare = positions.at(positions.size() - 1 - undo_count);
        for (int pos = 0; pos < positions.size() - undo_count - 1; pos++) {
            if (positions.at(pos).position == compare.position &&
                positions.at(pos).en_passant == compare.en_passant) {
                rep++;
                if (rep == 3) {
                    return 3;
                }
            }
        }
        return -1;
    } else {
        return -1;
    } // standard return
}

Piece *Board::get_pl() { return &piece_list[0]; } // erhalte Pointer auf piece-Liste und Existenz-Liste

bool *Board::get_pe() { return &piece_exists[0]; }

// returnt Forsyth–Edwards Notation der aktuellen Position
string Board::get_fen() {
    string output;
    int count_empty = 0;
    int i;
    // Position
    for (int wrong_i = 0; wrong_i < 64; wrong_i++) {
        i = 8 * (7 - wrong_i / 8) + wrong_i % 8;
        if (piece_exists[i]) {
            if (count_empty > 0) {
                output.append(to_string(count_empty));
                count_empty = 0;
            }
            output.push_back(convert_piece_type(piece_list[i].get_color(),
                                                piece_list[i].get_type()));
        } else {
            if (i % 8 == 7) {
                output.append(to_string(count_empty + 1));
                count_empty = 0;
            } else {
                count_empty++;
            }
        }
        if (i % 8 == 7 && wrong_i != 63) {
            output.append("/");
        }
    }
    // Spieler am Zug
    output.append((to_play == 0 ? " b " : " w "));
    // Rochaderechte
    char castles_chars[4] = {'K', 'Q', 'k', 'q'};
    bool any_castles = false;
    for (i = 0; i < 4; i++) {
        if (castles[i]) {
            output.push_back(castles_chars[i]);
            any_castles = true;
        }
    }
    // Fall, dass Rochade nicht möglich
    if (!any_castles) {
        output.append("-");
    }
    output.append(" ");
    // en passant
    if (en_passant > -1) {
        output.push_back((char) (en_passant / 8 + 97));
        output.append(to_string(en_passant % 8 + 1));
    } else {
        output.append("-");
    }
    // Züge seit Schlag- oder Bauernzug
    output.append(" " + to_string(mr_count) + " ");
    // Halbzüge
    output.append(to_string(move_count));

    return output;
}

// returnt portable game notation
string Board::get_pgn() {
    string all;
    // zusammensetzen aller gespeicherten Züge in einen einzelnen String
    for (int i = 0; i < uci.size() - undo_count; i++) {
        if (i % 2 == 0) {
            all += " " + to_string(i / 2 + 1) + ".";
        }
        all += " " + uci.at(i);
    }
    return all;
}
