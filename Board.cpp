#include "Headers/Board.h"


//, const int * (ui_white)(), const int * (ui_black)()
Board::Board(const string & startpos, Interface *white, Interface *black) {
    int count = 0, spaces = -1;
    delete ui_white, ui_black;
    ui_white = white, ui_black = black;
    for (unsigned char value: startpos) {
        int real_count = 8*(7-count / 8)+count%8;
        int int_of_value = (int) value;
        if (count > 63) {
            if (int_of_value == 32) {
                spaces++;
                continue;
            }
            else {
                switch (spaces) {
                    case 0:
                        to_play = value == 'w'; //color to move
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
                        if (value == '-') {en_passant = -1; continue;} // en_passant
                        else if (int_of_value < 58) {en_passant += int_of_value-49;}
                        else {en_passant = (int_of_value-97)*8;}
                        break;
                    case 3: // 50 move rule
                        if (value == '-') {mr_count = 0;}
                        else {mr_count = int_of_value-48;}
                        break;
                    case 4: // half_moves
                        move_count = int_of_value-48;
                        break;
                    default:
                        break;
                }
            }
            continue;
        }
        if (int_of_value != 47) {
            if (48 < int_of_value && int_of_value < 57) {
                for (int j = 0; j < int_of_value-48; j++) {
                    piece_exists[real_count] = false;
                    count++;
                }
                continue;
            } else {
                piece_exists[real_count] = true;
                bool color = int_of_value <= 96;
                piece_list[real_count] = Piece(color, convert_piece_type((char) value));
                if (int_of_value == 75) {king_positions[1] = real_count;}
                else if (int_of_value == 107) {king_positions[0] = real_count;}
                count++;
            }
        }
    }

}
void Board::draw(int player) {
    for (int i = 0; i < 64; i++) {
        int real_i = (player==1? 8*(7-i/8) + i%8: i/8*8 + 7-(i%8));
        string to_print;
        to_print = (piece_exists[real_i] ? "  " : "\u25a1 ");
        if (piece_exists[real_i]) {
            if (i % 8 == 7) { cout << fancier_symbols[piece_list[real_i].get_color()*6+piece_list[real_i].get_type()] << endl; }
            else { cout << fancier_symbols[piece_list[real_i].get_color()*6+piece_list[real_i].get_type()] << " "; }
        } else {
            if (i % 8 == 7) { cout << "\u25a1 " << endl; }

            else { cout << "\u25a1 " << ""; }
        }
    }
    cout << endl;
}

//run
int Board::run()  {
    while (true) {
        int game_end = update_moves();
        if (game_end >= 0) {
            //draw(1);
            return game_end; //game over
        }
        while (true) {
            tuple<bool, int, int, int> move_case; // bool true is system command in first int else move as [pos, tar, prom], prom default is -1
            //draw(1);
            //cout << move_count << get_fen() << endl;

            if (to_play) {
                ui_white->set_pieces(get_pl(), get_pe());
                move_case = ui_white->move();
            } else {
                ui_black->set_pieces(get_pl(), get_pe());
                move_case = ui_black->move();
            }
            if (get<0>(move_case)) {
                if (get<1>(move_case) == 6) {return 6;} // new game requested
                else if (get<1>(move_case) == 1) {undo();undo();}
                else {continue;}
            } else {
                //cout << "move " << (char) (get<1>(move_case)%8 + 97) <<  (get<1>(move_case)/8 + 1) << " " << (char) (get<2>(move_case)%8 + 97) <<  (get<2>(move_case)/8 + 1) << " " << endl;
                if (piece_exists[get<1>(move_case)] && piece_list[get<1>(move_case)].get_color() == to_play && piece_list[get<1>(move_case)].get_type() == 5 && get<1>(move_case) == 60-56*to_play) {
                    if (get<2>(move_case) == 56-56*to_play) {move_case = {get<0>(move_case),get<1>(move_case), 58-56*to_play, get<3>(move_case)};}
                    else if (get<2>(move_case) == 63-56*to_play) {move_case = {get<0>(move_case),get<1>(move_case), 62-56*to_play, get<3>(move_case)};}
                }
                if (piece_exists[get<1>(move_case)] && piece_list[get<1>(move_case)].get_color() == to_play && piece_list[get<1>(move_case)].pos_in_real_moves(get<2>(move_case)) && get<3>(move_case) >= -1 && get<3>(move_case) < 5 && get<3>(move_case) != 0) {
                    make_move(get<1>(move_case),get<2>(move_case),get<3>(move_case)); // normal move
                    break;
                }
            }
        }
    }
}

void Board::update(int old, int target, Piece obj , int promotion = -1) {
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
void Board::make_move(tuple<int,int,int> move) {
    make_move(get<0>(move), get<1>(move), get<2>(move));
}
void Board::make_move(int old, int target, int promotion) {

    if (undo_count > 0) {
        for (int i = 0; i< undo_count; i++) {
            move_list.pop_back();
            positions.pop_back();
        }
        undo_count = 0;
    }

    Piece & obj = piece_list[old];
    int rook_old = -1, rook_new = -1;


    // cases for castles, en-passant, normal capture, normal move
    Move add;
    if (obj.get_type() == 5 && abs(target%8-old%8) > 1) {
        if (target-old == 2) {
            rook_old = old/8*8+7; rook_new = old/8*8+5;
        } else if (target-old == -2) {
            rook_old = old/8*8; rook_new = old/8*8+3;
        }
        Piece & rook_obj = piece_list[rook_old];
        add = {3, old, target, rook_old, rook_new, -1, -1, mr_count,
                -1, {castles[0],castles[1],castles[2],castles[2]} , obj, rook_obj};
        /*add.type = 3; add.pos = old; add.tar = target; add.rook_pos = rook_old; add.rook_tar = rook_new;
        add.obj = obj; add.rook_obj = rook_obj; add.en_passant = -1;
        add.castles[0] = castles[0]; add.castles[1] = castles[1]; add.castles[2] = castles[2]; add.castles[3] = castles[3];*/
        update(rook_old, rook_new, rook_obj, -1);

    } else if (obj.get_type() == 0 && target == en_passant) {
        int tar_en_passant = old/8*8+target%8;
        Piece & tar_obj = piece_list[tar_en_passant];
        piece_exists[tar_en_passant] = false;
        add = {2, old, target, -1, -1, -1, -1, mr_count,
               en_passant, {castles[0],castles[1],castles[2],castles[2]} , obj, {}, tar_obj};
        //add.type = 2; add.pos = old; add.tar = target; add.obj = obj; add.tar_obj = tar_obj; add.en_passant = en_passant;

    } else if (piece_exists[target]) {
        Piece & tar_obj = piece_list[target];
        add = {1, old, target, -1, -1, -1, -1, mr_count,
               -1, {castles[0],castles[1],castles[2],castles[2]} , obj, {}, tar_obj};
        //add.type = 5; add.pos = old; add.tar = target; add.obj = obj; add.tar_obj = tar_obj;
        if (promotion != -1) {
            add = {5, old, target, -1, -1, promotion, obj.get_type(), mr_count,
                   -1, {castles[0],castles[1],castles[2],castles[2]} , obj, {}, tar_obj};
        }
        piece_exists[target] = false;
        mr_count = 0;

    } else {
        add = {0, old, target, -1, -1, -1, -1, mr_count,
               en_passant, {castles[0],castles[1],castles[2],castles[2]} , obj, {}, {}};
        //add.type = 5; add.pos = old; add.tar = target; add.obj = obj;
        if (promotion != -1) {
            add = {4, old, target, -1, -1, promotion, obj.get_type(), mr_count,
                   -1, {castles[0],castles[1],castles[2],castles[2]} , obj, {}, {}};
            //add.promotion = promotion, add.promotion_old = obj.get_type();
        }

    }
    move_list.push_back(add);
    update(old, target, obj, promotion);
    vector<int> new_position;
    for (int pos = 0; pos < 64; pos++) {
        new_position.push_back(piece_exists[pos]?piece_list[pos].get_color()*6+piece_list[pos].get_type()+1:0);
    }
    for (bool castle : castles) {
        new_position.push_back(castle);
    }
    positions.push_back({new_position, en_passant});
    /*for (int i = 0; i < 64; i++) {
        new_position.position[i] = (piece_exists[i]? tuple<int, int>(piece_list[i].get_color(), piece_list[i].get_type()): tuple<int, int>(-1,-1));
        if (i<4) {new_position.castles[i] = castles[i];}
    }

    new_position.en_passant = en_passant;

    if (obj.get_type() == 0) {
        mr_count = 0;
        //positions.clear();
    }*/
    // draw rules and castles updates
    if (obj.get_type() == 5) {
        if (obj.get_color() == 1) {castles[0] = false; castles[1] = false;}
        else {castles[2] = false; castles[3] = false;}
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
    move_count ++;
    mr_count ++;
    //positions.push_back(new_position);

    to_play = 1-to_play;
}

void Board::undo() {
    undo_count++;
    move_count--;
    Move last_move = move_list.at(move_list.size()-undo_count);
    mr_count = last_move.mr_count;
    en_passant = last_move.en_passant;

    int old_pos = last_move.pos, tar_pos = last_move.tar, rook_old_pos = last_move.rook_pos, rook_tar_pos = last_move.rook_tar, promotion_type = last_move.promotion;
    Piece obj = last_move.obj, tar_obj = last_move.tar_obj, rook_obj = last_move.rook_obj;

    // update king_positions
    if (obj.get_type() == 5) {king_positions[obj.get_color()] = old_pos;}
    // restore castles states
    for (int i = 0; i <4; i++) {
        castles[i] = last_move.castles[i];
    }
    // indices: 0 normal, 1 takes, 2 ep, 3 castle, 4 pro, 5 pro capture
    switch (last_move.type) {
        case 1: case 5:
            piece_list[old_pos] = obj;
            piece_exists[old_pos] = true;
            break;
        case 2:
            piece_exists[tar_pos] = false;
            // differs row between 3 and 4, keeps file
            tar_pos = 32-8*to_play+tar_pos%8;
            piece_list[old_pos] = obj;
            piece_exists[old_pos] = true;
            break;
        default:
            break;
    }
    switch (last_move.type) {
        case 1: case 2: case 5:
            piece_list[tar_pos] = tar_obj;
            piece_exists[tar_pos] = true;
            break;
        case 3: // undo castles
            update(tar_pos, old_pos, obj);
            update(rook_tar_pos, rook_old_pos, rook_obj);
            break;
        default:
            update(tar_pos, old_pos, obj);
    }

    to_play = 1-to_play;
}
//legal_moves
void Board::generate_piece_moves(int position) {
    Piece &piece = piece_list[position];
    piece.set_pins({});
    int type_for_pointer; // set type as used in possible_moves
    if (piece.get_type() == 0) {type_for_pointer = (int)(!(bool)piece.get_color());}
    else {type_for_pointer = piece.get_type()+1;}

    vector<int> * piece_moves = possible_moves[type_for_pointer][position];// used possible moves

    //piece.set_view(&pointer_piece_moves);
    vector<int> legal, pins, defend, update;
    int same_kind;
    if (piece.get_type() != 0) {
        // stuff for kings
        vector<int> * pointer_other_king;
        bool piece_is_king = true;
        if (position == king_positions[0]) {pointer_other_king = possible_moves[6][king_positions[1]];}
        else if (position == king_positions[1]) {pointer_other_king = possible_moves[6][king_positions[0]];}
        else {piece_is_king = false;}


        bool pin_stopped, other_stopped, king_found = false, no_king_add;
        for (int l = 0; l < 8; l++) {
            //vector<int>& line = (pointer_piece_moves+l);
            pin_stopped = false;
            other_stopped = false;
            pins = {};


            for (int move:*(piece_moves+l)) { // go through every line
                if (pin_stopped && other_stopped) {break;}
                if (piece_exists[move] && piece_list[move].get_color() == piece.get_color()) {same_kind = 1;} // set same kind
                else if (piece_exists[move]) {same_kind = 0;}
                else {same_kind = -1;}
                no_king_add = false;
                if (piece_is_king) {
                    for (int r = 0; r < 8; r++) {
                        int opponent_move;
                        if (!(r+pointer_other_king)->empty()) {opponent_move = (r+pointer_other_king)->at(0);} else {continue;}
                        if (opponent_move == move) {no_king_add = true; break;}
                    }
                }
                if (!other_stopped) { // all except pins
                    update.push_back(move); // add always
                    if (same_kind == 1) {
                        defend.push_back(move);
                        other_stopped = true; // cuz piece is hit
                    } else if (same_kind == 0) {
                        other_stopped = true;
                        if (!no_king_add) { // so kings don't get next
                            legal.push_back(move);
                        }
                    } else {
                        if (!no_king_add) {
                            legal.push_back(move);
                        }
                        defend.push_back(move);
                    }
                }
                if (!pin_stopped) {
                    if (same_kind == 0) {
                        pins.push_back(move);
                        if (move == king_positions[abs(piece.get_color()-1)]) {
                            king_found = true;
                            pin_stopped = true;
                        }
                    } else if (same_kind == 1) {
                        pin_stopped = true;
                    }
                }
            }
            if (king_found) {
                piece.set_pins(pins);
                king_found = false;
            }
        }
    } else {
        for (int l = 0; l < 3; l++) {
            vector<int>& line = piece_moves[l];
            if (line.empty()) {continue;}
            int move = line[0];
            if (piece_exists[move] && piece_list[move].get_color() == piece.get_color()) {same_kind = 1;} // set same kind
            else if (piece_exists[move]) {same_kind = 0;}
            else {same_kind = -1;}
            if (line.size() == 1) {
                update.push_back(move);
                if (position % 8 == move % 8) {
                    if (same_kind == -1) {legal.push_back(move);}
                }
                else {
                    defend.push_back(move);
                    if (same_kind == 0) {legal.push_back(move);}
                    if (move == king_positions[1-piece.get_color()]) {piece.set_pins({move});} // only set pins if opponent king in it
                }
            } else {
                bool stop = false;
                update.push_back(move);
                if (same_kind == -1) {legal.push_back(move);}
                else {stop = true;}
                if (!stop) {
                    move = line[1];
                    update.push_back(move);
                    if (!piece_exists[move]) {legal.push_back(move);}
                }
            }
        }
    }
    piece.set_view(piece_moves);
    piece.set_legal_moves(legal);
    piece.set_defend(defend);
    piece.set_update(update);

}

//all_moves
int Board::update_moves() {
    if (mr_count > 99) {return 4;}
    if (!move_list.empty()) {en_passant = -1;} // preserve enpassant state from fen
    int udc = (int)move_list.size()-undo_count;
    bool heavy_piece = false, local_en_passant = false, long_possible = false, short_possible = false, check = false, any_moves = false;
    vector<Move> last_moves = {};
    vector<int> updated_positions, updated_pieces, update_by_pin, update_by_king;
    vector <tuple<int, int>> all_on_board;

    Piece & king = piece_list[king_positions[to_play]], other_king = piece_list[king_positions[abs(to_play-1)]];
    int king_pos = king_positions[to_play], other_king_pos = king_positions[abs(to_play-1)];


    // get pieces to update
    if (udc > 0) {
        if (undo_count == 1) {last_moves.push_back(move_list.at(move_list.size()-1)); last_moves.push_back(move_list.back());}
        else if (undo_count > 1) {last_moves.push_back(move_list.at(move_list.size()-undo_count+1));last_moves.push_back(move_list.at(move_list.size()-undo_count));}
        else {last_moves.push_back(move_list.back());}

        for (const Move& last_move:last_moves) {
            updated_positions.push_back(last_move.pos);
            updated_positions.push_back(last_move.tar);
            updated_pieces.push_back(last_move.tar);
            if (last_move.type == 3) {
                updated_positions.push_back(last_move.rook_pos);
                updated_positions.push_back(last_move.rook_tar);
                updated_pieces.push_back(last_move.rook_tar);
            } else if (last_move.type == 4) {
                updated_positions.push_back(last_move.pos/8*8+last_move.tar%8);
            }
        }
    }
    for (int i = 0; i <64; i++) {
        if (!piece_exists[i]) {continue;}
        Piece & piece = piece_list[i];
        if (!heavy_piece) {
            if (piece.get_type() != 5) {
                all_on_board.emplace_back(piece.get_type(), i);
                if (piece.get_type() == 0 || piece.get_type() == 3 || piece.get_type() == 4) {heavy_piece = true;}
            }
        } if (udc > 0) {
            bool see_other_king = get<0>(piece.pos_in_view(i,king_positions[!to_play]));
            if (piece.get_color() == to_play && see_other_king) {update_by_king.push_back(i);}
            else if (!updated_positions.empty()) {
                tuple<bool, vector<int>> see_king = piece.pos_in_view(i,king_pos), see_org_pos= piece.pos_in_view(i,updated_positions[0]), see_new_pos = piece.pos_in_view(i,updated_positions[1]);
                if (get<0>(see_king) && get<0>(see_org_pos) && get<1>(see_king) == get<1>(see_org_pos)) {update_by_pin.push_back(i);}
                else if (get<0>(see_king) && get<0>(see_new_pos) && get<1>(see_king) == get<1>(see_new_pos)) {update_by_pin.push_back(i);}
            }
            bool compare_positions = false;
            for (int p:piece.get_update()) {
                for (int p1:updated_positions) {
                    if (p == p1) {compare_positions = true; break;}
                }
                if (compare_positions) {break;}
            }
            if (find(updated_pieces.begin(), updated_pieces.end(), i) != updated_pieces.end() ||
            find(update_by_pin.begin(), update_by_pin.end(), i) != update_by_pin.end() ||
            find(update_by_king.begin(), update_by_king.end(), i) != update_by_king.end() ||
            compare_positions || i == king_pos) {
                generate_piece_moves(i);
            }
        } else { generate_piece_moves(i);}
        piece.set_real_moves(piece.get_legal_moves());
    }
    if (!heavy_piece) {
        if (all_on_board.size()<2) {return 6;}
        else if (all_on_board.size() == 2) {
            if (get<0>(all_on_board.at(0)) == 2 && get<0>(all_on_board.at(1)) == 2 &&
                    (get<1>(all_on_board.at(0))/8 + get<1>(all_on_board.at(0))%8 + get<1>(all_on_board.at(1))/8 +
                     get<1>(all_on_board.at(1))%8)%2 == 0) {return 6;}
        }
    }

    // end updates
    if (udc > 0 && move_list.at(move_list.size()-undo_count-1).obj.get_type() == 0) {
        Move last_move = move_list.at(move_list.size()-undo_count-1);
        int old_pos = last_move.pos, old_rank = old_pos/8, file = old_pos%8, new_rank = last_move.tar/8;
        if (old_rank == 1+5*to_play && new_rank == 3+to_play) {
            for (int i: {-1, 1}) {
                if (0 < file +i && file +i < 8) {
                    int pawn = new_rank*8+file+i;
                    if (piece_exists[pawn]) {
                        if (piece_list[pawn].get_type() == 0) {
                            if (piece_list[new_rank*8+file].get_color() != piece_list[pawn].get_color()) {
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
            en_passant = 8 * (2 + 3 * to_play) + file;
        }
    } else if (move_list.empty() && en_passant > -1) {
        for (int pawn = 0; pawn < 2; pawn++) {
            int pos = en_passant+8-(16*to_play) -1+(2*pawn);
            if (piece_exists[pos] && piece_list[pos].get_color() == to_play && piece_list[pos].get_type() == 0) {
                piece_list[pos].add_real_moves({en_passant});
                piece_list[pos].add_defend({en_passant});
            }
        }
    }
    // pre-castle-stuff
    int long_c[2] = {59-to_play*56, 58-to_play*56};
    if (castles[(1-to_play)*2+1]) {
        if (piece_exists[(7-to_play*7)*8]){
            Piece & obj = piece_list[(7-to_play*7)*8];
            if (obj.get_type() == 3 && obj.get_color() == to_play) { // assumable, just in case castles is wrong in fen
                long_possible = true;
                for (int tile:{(7-to_play*7)*8+1, (7-to_play*7)*8+2, (7-to_play*7)*8+3}) {
                    if (piece_exists[tile]) {long_possible = false; break;}
                }
            }
        }
        if (long_possible) {king.add_real_moves({long_c[1]});}
    }

    int short_c[2] = {(7-to_play*7)*8+5, (7-to_play*7)*8+6};
    if (castles[(1-to_play)*2]) {
        if (piece_exists[(7-to_play*7)*8+7]){
            Piece & obj = piece_list[(7-to_play*7)*8+7];
            if (obj.get_type() == 3 && obj.get_color() == to_play) { // assumable, just in case castles is wrong in fen
                short_possible = true;
                for (int tile:short_c) {
                    if (piece_exists[tile]) {short_possible = false; break;}
                }
            }
        }
        if (short_possible) {king.add_real_moves({short_c[1]});}
    }
    vector<int> block_king;
    // main pin loop
    for (int piece_position = 0; piece_position < 64; piece_position++){
        if (!piece_exists[piece_position]) {continue;}
        Piece & piece = piece_list[piece_position];
        Piece & used_king = (find(update_by_king.begin(), update_by_king.end(), piece_position) != update_by_king.end() ? other_king: king);
        int & used_king_pos = (find(update_by_king.begin(), update_by_king.end(), piece_position) != update_by_king.end() ? other_king_pos: king_pos);

        if ((king.get_color() != piece.get_color() || used_king_pos == other_king_pos) && !piece.get_pins().empty() && piece.get_pins().size() < 3) {

            const vector<int> & pin_line = piece.get_pins(), view_line = get<1>(piece.pos_in_view(piece_position, pin_line.at(0)));

            int range_begin = 0, range_end = 0, len = (int) pin_line.size();
            bool is_king;
            vector<int> all_to_remove, new_all_moves;
            if (len == 1) {range_begin = 0; range_end = 64; check = true;}
            else {range_begin = pin_line.at(0); range_end = range_begin+1;}


            for (int pin_pos = range_begin; pin_pos < range_end; pin_pos++) {
                if (!piece_exists[pin_pos] || piece_list[pin_pos].get_color() != used_king.get_color()) {continue;}
                Piece & pinned_piece = piece_list[pin_pos];
                is_king = pin_pos == king_positions[0] || pin_pos == king_positions[1];
                vector<int> new_moves= {};
                bool move_in, block_behind, take_attacker;
                for (int move:pinned_piece.get_real_moves()) {
                    move_in = find(view_line.begin(), view_line.end(), move) != view_line.end();
                    take_attacker = move == piece_position;
                    block_behind = move <= max(piece_position, used_king_pos) && move >= min(piece_position, used_king_pos);
                    if (!is_king && (move_in || take_attacker) && block_behind || is_king && !move_in) {new_moves.push_back(move);}
                    else {all_to_remove.push_back(move);}
                }
                pinned_piece.set_real_moves(new_moves);
            }

        } if (king.get_color() != piece.get_color()) {
            bool do_next;
            vector<int> to_erase;
            if (long_possible) {
                do_next = false;
                for (int &lo:long_c) {
                    if (piece.pos_in_defend((short) lo) || do_next) {
                        to_erase.push_back(lo);
                        do_next = true;
                    }
                }
            } if (short_possible) {
                do_next = false;
                for (int &sho:short_c) {
                    if (piece.pos_in_defend((short) sho) || do_next) {
                        to_erase.push_back(sho);
                        do_next = true;
                    }
                }
            }
            king.pop_real_moves(to_erase);
            block_king.insert(block_king.end(),piece.get_defend().begin(), piece.get_defend().end());

        }

    }
    vector<int> new_real;
    for (int move:king.get_real_moves()) {
        if (find(block_king.begin(), block_king.end(), move) == block_king.end()) {new_real.push_back(move);}
    }
    king.set_real_moves(new_real);
    if (check) {
        king.pop_real_moves({king_pos+2, king_pos-2});
    }
    for (int pos = 0; pos < 64; pos++) {
        if (!piece_exists[pos] || piece_list[pos].get_color() != to_play) {continue;}
        if (!piece_list[pos].get_real_moves().empty()) {any_moves = true;break;}
    }

    // gameend
    if (!any_moves) {
        if (check) {
            return (int) !to_play;} // checkmate
        else {return 2;} // draw
    } else if (positions.size() > 5) {
        int rep = 1;
        Position compare = positions.at(positions.size()-1-undo_count);
        for (int pos = 0; pos < positions.size()-undo_count-1; pos++) {
            if (positions.at(pos).position == compare.position && positions.at(pos).en_passant == compare.en_passant) {
                rep++;
                if (rep == 3) {return 3;}
            }
        }
        return -1;
    } else {return -1;} // standard return
}

Piece *Board::get_pl() {
    return &piece_list[0];
}
bool *Board::get_pe() {
    return &piece_exists[0];
}
string Board::get_fen() {
    string output;
    int count_empty = 0;
    int i;
    for (int wrong_i = 0; wrong_i < 64; wrong_i++) {
        i = 8*(7-wrong_i/8)+wrong_i%8;
        if (piece_exists[i]) {
            if (count_empty > 0) {
                output.append(to_string(count_empty));
                count_empty = 0;
            }
            output.push_back(convert_piece_type(piece_list[i].get_color(), piece_list[i].get_type()));
        } else {
            if (i%8 == 7) {
                output.append(to_string(count_empty+1));
                count_empty = 0;
            } else {count_empty ++;}
        }
        if (i%8 == 7 && wrong_i != 63) {
            output.append("/");
        }
    }
    // to_play
    output.append((to_play == 0 ? " b " : " w "));
    // castles
    char castles_chars[4] = {'K', 'Q', 'k', 'q'};
    bool any_castles = false;
    for (i = 0; i < 4; i++) {
        if (castles[i]) {
            output.push_back(castles_chars[i]);
            any_castles = true;
        }
    }
    //no castles
    if (!any_castles) {output.append("-");}
    output.append(" ");
    // en passant
    if (en_passant >-1) {output.push_back((char) (en_passant/8+97)); output.append(to_string(en_passant%8+1));}
    else {output.append("-");}
    // full moves
    output.append(" " + to_string(mr_count) + " ");
    // half moves
    output.append(to_string(move_count));

    return output;
}

