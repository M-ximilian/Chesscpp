#include "Headers/ui.h"
#include "Headers/Board.h"
bool postion_map = true;
bool space_calculation = true;
bool connected_pawns = false;
bool open_lines = true;
bool double_pawns = false;
bool endgame_close_king = true;
bool single_pawns = true;
bool open_bishops = true;





vector<tuple<int, int, int>> sort_moves(vector<tuple<int, int, int>>& moves, vector<float>& scores, bool multiply = false) {
    for (int i = 0; i < moves.size()-1; i ++) {
        for (int j = i+1; j > 0; j--) {
            int swap = j-1;
            if (float (1-2*multiply) * scores.at(swap) < float (1-2*multiply) * scores.at(j)) {
                iter_swap(moves.begin()+j, moves.begin()+swap);
                iter_swap(scores.begin()+j, scores.begin()+swap);

            }
        }
    }
    return moves;
}

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
        else if (raw_move == "pgn") {move = tuple(true, 7, 0, 0); return move;}
        else {
            int piece = convert_piece_type((raw_move.at(0))); // type of piece to move
            raw_piece = raw_move.at(0);
            raw_move = string(raw_move.begin()+1, raw_move.end());
            if (isalpha((char) raw_move.at(raw_move.length()-1))) {
                promotion = convert_piece_type(toupper(raw_move.at(raw_move.length()-1)));
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
            } else if (!try_pawn) {try_pawn = true; continue;}
            else {try_pawn = false; continue;}
        }
        if (old_pos == -1 || tar == -1) {try_pawn = true; continue;}
        move = tuple(false, old_pos, tar, promotion);
        return move;
    }
}

tuple<bool, int, int, int> Random_ui::move() {
    int piece;
    while (true) {
        piece = own_pieces.at(rand()%own_pieces.size());
        if (!piece_list[piece].get_real_moves().empty()) {break;}
    }

    int move = piece_list[piece].get_real_moves().at(rand()%piece_list[piece].get_real_moves().size());
    int promotion = -1;
    if (piece_list[piece].get_type() == 0 && ((to_play == 0 && (piece/8) == 1 && (move/8) == 0) || (to_play == 1 && (piece/8) == 6 && (move/8) == 7))){
        promotion = (int(rand()))%4+1;
    }
    return {false, piece, move, promotion};
}
int count_undo = 0, count_moves =0;
tuple<bool, int, int, int> Bot_ui::move() {
    tuple<int, int, int> best_move;
    float score;
    count_moves = 0;
    best_moves_this_iteration.clear();
    score = better_min_max(depth, -10000000, 10000000);
    best_move = best_moves_this_iteration.size()==1?best_moves_this_iteration.at(0):best_moves_this_iteration[rand()%(best_moves_this_iteration.size()-1)];
    //cout << count_moves << endl;
    //cout << game->get_fen()  << endl;
    //cout << get<0>(best_move) << " " << get<1>(best_move) << " " << get<2>(best_move) << " "<< score << endl;
    cout << "eval " << score/10 << endl;
    return {false, get<0>(best_move), get<1>(best_move), get<2>(best_move)};
}


tuple<float, int, int, int> Bot_ui::min_max(int local_depth, double alpha, double beta, tuple<int,int,int> last_move) {
    //if (count_moves % 1000 == 0) {cout << count_moves << endl;}
    double min_score = (numeric_limits<float>::max()), max_score = (-numeric_limits<float>::max());
    tuple<double, int,int, int> best_move;
    if (local_depth == 0) {return {evaluate(game->get_pl(), game->get_pe(), game->move_count), get<0>(last_move), get<1>(last_move), get<2>(last_move)};}
    // sort all possible moves by estimated (hardcoded) strength
    vector<tuple<int,int,int>> new_moves = order_moves(game->to_play);
    //go through all moves
    for (tuple<int,int,int> move:new_moves) {
        // move and update
        game->make_move(get<0>(move), get<1>(move), get<2>(move));
        int game_end = game->update_moves();
        // test if game over
        switch (game_end) {
            case -1:
                break;
            case 0:
                game->undo();
                return {(game->to_play ? (-numeric_limits<double>::max())+depth-local_depth:(numeric_limits<double>::max())-depth+local_depth), get<0>(move), get<1>(move), get<2>(move)};
            case 1:
                game->undo();
                return {(game->to_play ? (numeric_limits<double>::max())-depth+local_depth:(-numeric_limits<double>::max())+depth-local_depth), get<0>(move), get<1>(move), get<2>(move)};
            default:
                game->undo();
                return {0, get<0>(move), get<1>(move), get<2>(move)};
        }

        // prepare player specific stuff for next depth
        count_moves++;
        best_move = min_max(local_depth-1, alpha, beta, move);
        //best_move = (game->to_play ? min_max(local_depth-1, false, alpha, min_score): min_max(local_depth-1, true, max_score, beta));
        game->undo();
        game->update_moves();
        count_undo++;
        double &score = get<0>(best_move);
        if (game->to_play == 1) {
            max_score = max(max_score, score);
            alpha = max(alpha, score);
            if (beta <= alpha) {break;}
            /*if (score > max_score) {
                max_score = score;
                if (local_depth == depth) {
                    best_move = {score, get<0>(move), get<1>(move), get<2>(move)};
                }
                if (max_score >= beta) {break;}
            } else if (score > -infinity()) {best_move = {score, get<0>(move), get<1>(move), get<2>(move)};}*/
        } else {
            min_score = min(min_score, score);
            beta = min(beta, score);
            if (beta <= alpha) {break;}
            /*if (score < min_score) {
                min_score = score;
                if (local_depth == depth) {
                    best_move = {score, get<0>(move), get<1>(move), get<2>(move)};
                }
                if (min_score <= alpha) {break;}
            } else if (score < infinity()) {best_move = {score, get<0>(move), get<1>(move), get<2>(move)};}*/
        }
    }
    if (game->to_play) {return {max_score, get<1>(best_move), get<2>(best_move), get<3>(best_move)};}
    else {return {min_score, get<1>(best_move), get<2>(best_move), get<3>(best_move)};}
}

float Bot_ui::better_min_max(int local_depth, float alpha, float beta, bool capture_sort, bool pre_sort, int end_depth) {
    if (local_depth == 0) {return evaluate(game->get_pl(), game->get_pe(), game->move_count);}
    vector<tuple<int, int, int>> moves;
    if (local_depth == 0 && !capture_sort) {
        moves = capture_order(game->to_play);
        capture_sort = true;
        if (moves.empty()) {return evaluate(game->get_pl(), game->get_pe(), game->move_count);}
        else {local_depth = 1;}
    }
    else {moves = order_moves(game->to_play);}

    float max_score = -10000000;
    float min_score = 10000000;
    for (auto move:moves) {
        game->make_move(move);
        int game_end = game->update_moves();
        switch (game_end) {
            case -1:
                break;
            case 0: // weiß ist matt (schwarz gewinnt)
                game->undo();
                game->update_moves();
                if (pre_sort && local_depth == depth == end_depth) {best_moves_this_iteration.push_back(move); pre_scores.push_back(-100000+(depth-local_depth+1));}
                else {
                    if (local_depth == depth && (best_moves_this_iteration.empty() || (to_play == 0 && (-100000 +(float) (depth-local_depth+1)) < min_score) ||  (to_play == 1 && (-100000 +(float) (depth-local_depth+1)) > max_score))) {best_moves_this_iteration = {move};}
                    else if (local_depth == depth && ((to_play == 0 && (-100000 +(float) (depth-local_depth+1)) == min_score) || (to_play == 1 && (-100000 +(float) (depth-local_depth+1)) == max_score))) {best_moves_this_iteration.push_back(move);}
                }
                    return (-100000) +(float) (depth-local_depth+1);
            case 1:
                game->undo();
                game->update_moves();
                if (pre_sort && local_depth == depth == end_depth) {best_moves_this_iteration.push_back(move); pre_scores.push_back(100000-(depth-local_depth+1));}
                else {
                    if (local_depth == depth && (best_moves_this_iteration.empty() || (to_play == 0 && (100000 - (float) (depth - local_depth + 1)) < min_score) || (to_play == 1 && (100000 - (float) (depth - local_depth + 1)) > max_score))) {best_moves_this_iteration = {move};}
                    else if (local_depth == depth && ((to_play == 0 && (100000 - (float) (depth - local_depth + 1)) == min_score) || (to_play == 1 && (100000 - (float) (depth - local_depth + 1)) == max_score))) {best_moves_this_iteration.push_back(move);}
                }
                return (100000)-(float) (depth-local_depth+1);
            default:
                game->undo();
                game->update_moves();
                if (pre_sort && local_depth == depth == end_depth) {best_moves_this_iteration.push_back(move); pre_scores.push_back(0);}
                else {
                    if (local_depth == depth && (best_moves_this_iteration.empty() ||(to_play == 0 && 0 < min_score) || (to_play == 1 && 0 > max_score))) {best_moves_this_iteration = {move};}
                    else if (local_depth == depth && ((to_play == 0 && 0 == min_score) || (to_play == 1 && 0 == max_score))) {best_moves_this_iteration.push_back(move);}
                    }
                return 0;
        }
        float eval = better_min_max(local_depth-1, alpha, beta, capture_sort);
        if (depth == local_depth) {
            //cout << get<0>(move) << " " << get<1>(move) << " " << eval << endl;
        }
        if (local_depth == 3) {
            bool test = false;
        }
        //cout << "2nd eval " << eval << endl;
        if (pre_sort && local_depth == end_depth) {best_moves_this_iteration.push_back(move); pre_scores.push_back(eval);}

        game->undo();
        game->update_moves();
        count_moves++;
        if (game->to_play) {
            if (eval > max_score) {
                max_score = eval;
                if (local_depth == depth && !pre_sort) {best_moves_this_iteration = {move};}
            }
            else if (eval == max_score && local_depth == depth && !pre_sort) {
                best_moves_this_iteration.push_back(move);
            }
            alpha = max(alpha, eval);
            if (beta <= alpha) {break;}
        } else {
            if (eval < min_score) {
                min_score = eval;
                if (local_depth == depth && !pre_sort) {best_moves_this_iteration = {move};}
            }
            else if (eval == min_score && local_depth == depth && !pre_sort) {
                best_moves_this_iteration.push_back(move);
            }
            beta = min(beta, eval);
            if (beta <= alpha) {break;}
        }

    }
    if (game->to_play) {return max_score;}
    else {return min_score;}
}

float Bot_ui::evaluate(Piece *pl, const bool *pe, const int &move_count) {
    bool queens = false;
    bool all[2] = {false, false};
    int kings[2];
    float eval = 0;
    float material = 0;
    int space_map[64];
    int pawn_map[2][8];
    int pawn_colors[2][2] = {{0, 0}, {0, 0}};
    vector<int> rook_pos[2];
    int bishopsnknights = 0;
    for (int &i:space_map) {i = 0;}
    for (int &i:pawn_map[0]) {i = 0;}
    for (int &i:pawn_map[1]) {i = 0;}
    for (int i = 0; i<64; i++) {
        if (!(*(pe+i))) {continue;}
        Piece &piece = *(pl+i);
        if (piece.get_type() != 5) {material += eval_list[piece.get_type()];}
    }

    for (int i = 0; i <64; i++) {
        if (!(*(pe+i))) {continue;}
        Piece &piece = *(pl+i);
        if (piece.get_type() == 5 && to_play == 0 && endgame_close_king) {
            eval += eval_list[piece.get_type()] * (float) (-1+2*piece.get_color());
            kings[piece.get_color()] = i;


        } else {
            eval += eval_list[piece.get_type()] * (float) (-1+2*piece.get_color()) + 0.4*eval_positions[piece.get_color()*6+piece.get_type()][i];
        }
        if (piece.get_type() != 5) {
            all[piece.get_color()] = true;
            if (piece.get_type() == 4) {queens = true;}
        }
        if (space_calculation && to_play == 0) {
            int multiplier = piece.get_color()==1?1:-1;
            for (int move:piece.get_defend()) {
                space_map[move]+=multiplier;
            }
        }
        if (connected_pawns && to_play == 0 &&  piece.get_type() == 0) {
            for (int column:{-1,1}) {
                for (int row:{-1,0,1}) {
                    if (i%8+column>-1 && i%8+column<8 && i/8+row>-1 && i/8+row<8) {
                        if ((pl+((i/8+row)*8+i%8+column))->get_type() == 0 && (pl+((i/8+row)*8+i%8+column))->get_color() == piece.get_color()) {
                            eval += (float)(piece.get_color() == 1?1:-1) * (column == (piece.get_color() == 0?1:-1)?0.2:0.1);
                            break;
                        }
                    }
                }
            }
        }
        // Laeuferfarben speichern
        if (open_bishops && to_play == 0 && piece.get_type() == 2) {
            bishopsnknights += -1+2*piece.get_color();
        } else if (open_bishops && to_play == 0 && piece.get_type() == 1) {
            bishopsnknights += 1-2*piece.get_color();
        }
        // Vorraussetzung fuer alles, was mit Bauern zu tun hat
        if ((double_pawns || open_lines || single_pawns || open_bishops) && to_play == 0 && piece.get_type() == 0) {
            pawn_map[piece.get_color()][i%8]++;
        }
        if (open_lines && to_play == 0 && piece.get_type() == 3) {
            rook_pos[piece.get_color()].push_back(i);
        }
    }
    // Beginn der Auswertung der erstellten Arrays
    if (space_calculation && to_play == 0) {
        int full_space = 0;
        int square;
        for (int sq = 0; sq < 32; sq++) {
            square = space_map[sq];
            full_space += square < 0 ? -1 : 0;
        }
        for (int sq = 32; sq < 64; sq++) {
            square = space_map[sq];
            full_space += square > 0 ? 1 : 0;
        }
        eval += (float) full_space * 0.1;
    }
    // Doppelbauern
    if (double_pawns && to_play == 0) {
        for (int &i:pawn_map[0]) {eval += (i>1?(float) i/10:0);}
        for (int &i:pawn_map[1]) {eval -= (i>1?(float) i/10:0);}
    }
    // offene und halboffene Linien
    if (open_lines && to_play == 0) {
        // schwarze offene Linien
        for (int &i:rook_pos[0]) {
            if (pawn_map[0][i] == 0) {
                if (pawn_map[1][i] == 0) {
                    eval -= 0.1;
                }
            }
        }
        // weisse offene Linien
        for (int &i:rook_pos[1]) {
            if (pawn_map[1][i] == 0) {
                if (pawn_map[0][i] == 0) {
                    eval += 0.1;
                }
            }
        }
        // Halboffene Linien
        for (int pawn = 0; pawn < 8; pawn ++) {
             if (pawn_map[1][pawn] == 0 && pawn_map[0][pawn] != 0) {
                eval += 0.1;
            } else if (pawn_map[0][pawn] == 0 && pawn_map[1][pawn] != 0) {
                eval-=1;
            }
        }
    }

    if (single_pawns && to_play == 0) {
        // weisse einzelbauern (past pawns)
        for (int pawn = 0; pawn < 8; pawn++) {
            if (pawn_map[1][pawn] > 0 && pawn_map[0][pawn] == 0 && pawn == 0 || pawn_map[0][pawn-1] == 0 && pawn == 7 || pawn_map[0][pawn+1] == 0) {eval += 0.2;}
        }
        // schwarze Einzelbauern
        for (int pawn = 0; pawn < 8; pawn++) {
            if (pawn_map[0][pawn] > 0 && pawn_map[1][pawn] == 0 && pawn == 0 || pawn_map[1][pawn-1] == 0 && pawn == 7 || pawn_map[1][pawn+1] == 0) {eval -= 0.2;}
        }
    }

    if (open_bishops && to_play == 0) {
        for (int pawn = 0; pawn < 8; pawn++) {
            if (pawn_map[0][pawn] == 0 && pawn_map[1][pawn] == 0) {eval += 0.1*(bishopsnknights<0?max(-2,bishopsnknights):min(2, bishopsnknights));}
        }
    }
    if (endgame_close_king && to_play == 0) {
        if (queens) {eval += eval_positions[11][kings[1]]-eval_positions[5][kings[0]];}
        else {
            if (!all[0]) {
                eval +=  (float) (7-max(abs(kings[0]/8 - kings[1]/8), abs(kings[0]%8-kings[1]%8)));
                eval += (float) 0.5 * (float) (max(7- kings[0]/8,  kings[0]/8) + max(7- kings[0]%8,  kings[0]%8));
            } if (!all[1]) {
                eval -= (float) (7 - max(abs(kings[0] / 8 - kings[1] / 8), abs(kings[0] % 8 - kings[1] % 8)));
                eval -= (float) 0.5 * (float) (max(7- kings[1]/8,  kings[1]/8) + max(7- kings[1]%8,  kings[1]%8));
            }
        }
    }
    //cout << roundf(eval*100)/100 << endl;
    return roundf(eval*100)/100;
}
vector<tuple<int, int, int>> Bot_ui::order_moves(int color) {
    Piece *pl = game->get_pl();
    bool *pe = game->get_pe();
    vector<tuple<int, int, int>> ordered_moves;
    vector<float> scores;
    for (int i = 0; i < 64; i++) {
        if (!*(pe+i)) {continue;}
        Piece &piece = *(pl+i);
        if (color != piece.get_color()) {continue;}
        if (piece.get_type() == 0 && i/8 == 1+5*color) {
            for (int move:piece.get_real_moves()) {
                for (int prom = 1; prom < 5; prom++) {
                    ordered_moves.emplace_back(i, move, prom);
                    if (*(pe+move)) {scores.push_back(eval_list[prom]- eval_list[0]- eval_list[(pl+move)->get_type()]);}
                    else {scores.push_back(eval_list[prom]-eval_list[0]);}
                }
            }
        } else {
            for (int move:piece.get_real_moves()) {
                ordered_moves.emplace_back(i, move, -1);
                if (*(pe+move)) {scores.push_back(eval_list[(pl+move)->get_type()]-eval_list[piece.get_type()]);}
                else if (color == 1 &&
                         (move/8+1 < 8 && move%8-1 > -1 && *(pe+move+7) && (pl+move+7)->get_color() != color && (pl+move+7)->get_type() == 0 ||
                         move/8+1 < 8 && move%8+1 < 8 && *(pe+move+9) && (pl+move+9)->get_color() != color && (pl+move+9)->get_type() == 0)) {
                    scores.push_back(-eval_list[((pl+move)->get_type())]-eval_list[0]);
                }
                else if (color == 0 &&
                         (move/8-1 >-1 && move%8-1 > -1 && *(pe+move-9) && (pl+move-9)->get_color() != color && (pl+move-9)->get_type() == 0 ||
                          move/8-1 >-1 && move%8+1 < 8 && *(pe+move-7) && (pl+move-7)->get_color() != color && (pl+move-7)->get_type() == 0)) {
                    scores.push_back(-eval_list[((pl+move)->get_type())]-eval_list[0]);
                }
                else {scores.push_back(0);}
            }
        }
    }
    return sort_moves(ordered_moves, scores);
}
vector<tuple<int, int, int>> Bot_ui::better_order_moves(int color) {
    better_min_max((depth>3?depth-3:1), -10000000, 10000000, true, (depth>3?depth-3:1));
    return sort_moves(best_moves_this_iteration, pre_scores, !color);
}
vector<tuple<int, int, int>> Bot_ui::unordered_moves(int color) {
    Piece *pl = game->get_pl();
    bool *pe = game->get_pe();
    vector<tuple<int, int, int>> ordered_moves;
    for (int i = 0; i < 64; i++) {
        if (!*(pe + i)) { continue; }
        Piece &piece = *(pl + i);
        if (color != piece.get_color()) { continue; }
        if (piece.get_type() == 0 && i / 8 == 1 + 5 * color) {
            for (int move : piece.get_real_moves()) {
                for (int prom = 1; prom < 5; prom++) {
                    ordered_moves.emplace_back(i, move, prom);
                }
            }
        } else {
            for (int move : piece.get_real_moves()) {
                ordered_moves.emplace_back(i, move, -1);
            }
        }
    }
    return ordered_moves;
}
vector<tuple<int, int, int>> Bot_ui::capture_order(int color) {
    Piece *pl = game->get_pl();
    bool *pe = game->get_pe();
    vector<tuple<int, int, int>> ordered_moves;
    for (int i = 0; i < 64; i++) {
        if (!*(pe+i)) {continue;}
        Piece &piece = *(pl+i);
        if (color != piece.get_color()) {continue;}
        if (piece.get_type() == 0 && i/8 == 1+5*color) {
            for (int move:piece.get_real_moves()) {
                for (int prom = 1; prom < 5; prom++) {
                    if (*(pe+move)) {ordered_moves.emplace_back(i, move, prom);}
                }
            }
        } else {
            for (int move:piece.get_real_moves()) {

                if (*(pe+move)) {ordered_moves.emplace_back(i, move, -1);}
            }
        }
    }
    return ordered_moves;
}


