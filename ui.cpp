#include "Headers/ui.h"

void Ui::set_pieces(Piece *pl, const bool *pe) {
    own_pieces.clear();
    for (int piece = 0; piece < 64; piece++) {
        if (*(pe+piece)) {
            piece_exists[piece] = true;
            piece_list[piece] = *(pl+piece);
            if ((pl+piece)->get_color() == to_play) {
                own_pieces.push_back(piece);
            }
        } else {piece_exists[piece] = false;}

    }
}

tuple<bool, int, int, int> Ascii_ui::move() {
    bool got_move = false, try_pawn = false;
    tuple<bool, int, int, int> move;
    int old_pos, tar, promotion;
    string raw_move, raw_piece;
    while (!got_move) {
        old_pos = -1, tar = -1;

        if (!try_pawn) {
            promotion = -1;
            cin >> raw_move;
            replace(raw_move,"=", "");
            replace(raw_move,"x", "");
            replace(raw_move,"+", "");
            replace(raw_move,"#", "");}
        else {raw_move = "p"+raw_piece+raw_move;}

        if (raw_move.length() == 2 &&  raw_move != "uu") {raw_move = "p"+raw_move;}
        if (raw_move == "0-0" || raw_move == "O-O") {
            bool test1 = piece_exists[60-to_play*56], test2 = piece_list[60-to_play*56].pos_in_real_moves(short(62-to_play*56)), test3 = piece_list[60].pos_in_real_moves(62);
            if (piece_exists[60-to_play*56] && piece_list[60-to_play*56].pos_in_real_moves(short(62-to_play*56))) {move = tuple(false, 60-to_play*56, 62-to_play*56, -1); return move;}
        } else if (raw_move  == "0-0-0" || raw_move == "O-O-O") {
            if (piece_exists[60-to_play*56] && piece_list[60-to_play*56].pos_in_real_moves(short (58-to_play*56))) {move = tuple(false, 60-to_play*56, 58-to_play*56, -1); return move;}
        }
        else if (raw_move == "u") {move = tuple(true, 1, 0, 0); return move;}
        else if (raw_move == "r") {move = tuple(true, 2, 0, 0); return move;}
        else if (raw_move == "uu") {move = tuple(true, 3, 0, 0); return move;}
        else if (raw_move == "to_last") {move = tuple(true, 4, 0, 0); return move;}
        else if (raw_move == "to start") {move = tuple(true, 5, 0, 0); return move;}
        else if (raw_move == "new game") {move = tuple(true, 6, 0, 0); return move;}
        else {
            int piece = convert_piece_type((raw_move.at(0))); // type of piece to move
            raw_piece = raw_move.at(0);
            raw_move = string(raw_move.begin()+1, raw_move.end());
            if (isalpha(raw_move.at(raw_move.length()-1))) {
                promotion = convert_piece_type(convert_piece_type(1,toupper(raw_move.at(raw_move.length()-1))));
                raw_move.pop_back();
            } // handles and removes promotions
            if ((raw_move.length() == 3 || raw_move.length() == 4) && !isalpha(raw_move.at(raw_move.length()-1))) {
                tar = (int(unsigned(raw_move.at(raw_move.length()-1)))-49)*8+int(unsigned(toupper(raw_move.at(raw_move.length()-2))))-65; // specific old_pos is given
                int row = -1, col = -1;
                if (isalpha(raw_move.at(0))) {
                    col = int (unsigned (toupper(raw_move.at(0)))) -65;
                    if (raw_move.length() == 4) {row = int (unsigned (raw_move.at(1)))-49;}
                } else {
                    row = int (unsigned (raw_move.at(0)))-49;
                }
                if (!(col == -1 || row == -1)) {
                    if (find(own_pieces.begin(), own_pieces.end(), row*8+col) != own_pieces.end() && piece_list[row*8+col].get_type() == piece) {old_pos = row*8+col;}
                } else {
                    for (int pos: own_pieces) {
                        if ((row == pos/8 || col == pos%8) && find(piece_list[pos].get_real_moves().begin(), piece_list[pos].get_real_moves().end(), tar) != piece_list[pos].get_real_moves().end() && piece_list[pos].get_type() == piece) {
                            old_pos = pos;
                            got_move = true;
                            break;
                        }
                    }
                }

            } else if (raw_move.length() == 2) {
                tar = (int(unsigned(raw_move.at(1)))-49)*8+int(unsigned(toupper(raw_move.at(0))))-65;
                for (int p = 0; p<64; p++) {
                    if (find(own_pieces.begin(), own_pieces.end(), p) == own_pieces.end()) {continue;}
                    vector<int> moves = piece_list[p].get_real_moves();
                    if (find(moves.begin(), moves.end(), tar) != moves.end() && piece_list[p].get_type() == piece) {
                        old_pos = p;
                        got_move = true;
                        break;
                    }
                }
            } else {try_pawn = false; continue;}
        }
        if (old_pos == -1 || tar == -1) {try_pawn = true; continue;}
        move = tuple(false, old_pos, tar, promotion);
        return move;
    }
}

tuple<bool, int, int, int> Random_ui::move() {
    int piece;
    while (true) {
        piece = own_pieces.at(random()%own_pieces.size());
        if (!piece_list[piece].get_real_moves().empty()) {break;}
    }

    int move = piece_list[piece].get_real_moves().at(random()%piece_list[piece].get_real_moves().size());
    int promotion = -1;
    if (piece_list[piece].get_type() == 0 && ((to_play == 0 && (piece/8) == 1 && (move/8) == 0) || (to_play == 1 && (piece/8) == 6 && (move/8) == 7))){
        promotion = (int( random()))%4+1;
    }
    return tuple<bool, int, int, int>(false, piece, move, promotion);
}

