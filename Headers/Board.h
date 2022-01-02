#include "ui.h"
static string fancy_symbols[12] = {"\u265F", "\u265E", "\u265D", "\u265C", "\u265B", "\u265A", "\u2659", "\u2658", "\u2657", "\u2656", "\u2655", "\u2654"};
static string fancier_symbols[12] = {"p" ,"n" ,"b" ,"r" ,"q" ,"k" ,"P" ,"N" ,"B" ,"R" ,"Q" ,"K" };
class Board {
public:
    explicit Board(const string &startpos, Random_ui *ui_white, Random_ui *ui_black);
    vector<int> possible_moves[7][64][8] = {{{{8, 16}, {}, {9}, {}, {}, {}, {}, {}}, {{9, 17}, {8}, {10}, {}, {}, {}, {}, {}}, {{10, 18}, {9}, {11}, {}, {}, {}, {}, {}}, {{11, 19}, {10}, {12}, {}, {}, {}, {}, {}}, {{12, 20}, {11}, {13}, {}, {}, {}, {}, {}}, {{13, 21}, {12}, {14}, {}, {}, {}, {}, {}}, {{14, 22}, {13}, {15}, {}, {}, {}, {}, {}}, {{15, 23}, {14}, {}, {}, {}, {}, {}, {}}, {{16, 24}, {}, {17}, {}, {}, {}, {}, {}}, {{17, 25}, {16}, {18}, {}, {}, {}, {}, {}}, {{18, 26}, {17}, {19}, {}, {}, {}, {}, {}}, {{19, 27}, {18}, {20}, {}, {}, {}, {}, {}}, {{20, 28}, {19}, {21}, {}, {}, {}, {}, {}}, {{21, 29}, {20}, {22}, {}, {}, {}, {}, {}}, {{22, 30}, {21}, {23}, {}, {}, {}, {}, {}}, {{23, 31}, {22}, {}, {}, {}, {}, {}, {}}, {{24}, {}, {25}, {}, {}, {}, {}, {}}, {{25}, {24}, {26}, {}, {}, {}, {}, {}}, {{26}, {25}, {27}, {}, {}, {}, {}, {}}, {{27}, {26}, {28}, {}, {}, {}, {}, {}}, {{28}, {27}, {29}, {}, {}, {}, {}, {}}, {{29}, {28}, {30}, {}, {}, {}, {}, {}}, {{30}, {29}, {31}, {}, {}, {}, {}, {}}, {{31}, {30}, {}, {}, {}, {}, {}, {}}, {{32}, {}, {33}, {}, {}, {}, {}, {}}, {{33}, {32}, {34}, {}, {}, {}, {}, {}}, {{34}, {33}, {35}, {}, {}, {}, {}, {}}, {{35}, {34}, {36}, {}, {}, {}, {}, {}}, {{36}, {35}, {37}, {}, {}, {}, {}, {}}, {{37}, {36}, {38}, {}, {}, {}, {}, {}}, {{38}, {37}, {39}, {}, {}, {}, {}, {}}, {{39}, {38}, {}, {}, {}, {}, {}, {}}, {{40}, {}, {41}, {}, {}, {}, {}, {}}, {{41}, {40}, {42}, {}, {}, {}, {}, {}}, {{42}, {41}, {43}, {}, {}, {}, {}, {}}, {{43}, {42}, {44}, {}, {}, {}, {}, {}}, {{44}, {43}, {45}, {}, {}, {}, {}, {}}, {{45}, {44}, {46}, {}, {}, {}, {}, {}}, {{46}, {45}, {47}, {}, {}, {}, {}, {}}, {{47}, {46}, {}, {}, {}, {}, {}, {}}, {{48}, {}, {49}, {}, {}, {}, {}, {}}, {{49}, {48}, {50}, {}, {}, {}, {}, {}}, {{50}, {49}, {51}, {}, {}, {}, {}, {}}, {{51}, {50}, {52}, {}, {}, {}, {}, {}}, {{52}, {51}, {53}, {}, {}, {}, {}, {}}, {{53}, {52}, {54}, {}, {}, {}, {}, {}}, {{54}, {53}, {55}, {}, {}, {}, {}, {}}, {{55}, {54}, {}, {}, {}, {}, {}, {}}, {{56}, {}, {57}, {}, {}, {}, {}, {}}, {{57}, {56}, {58}, {}, {}, {}, {}, {}}, {{58}, {57}, {59}, {}, {}, {}, {}, {}}, {{59}, {58}, {60}, {}, {}, {}, {}, {}}, {{60}, {59}, {61}, {}, {}, {}, {}, {}}, {{61}, {60}, {62}, {}, {}, {}, {}, {}}, {{62}, {61}, {63}, {}, {}, {}, {}, {}}, {{63}, {62}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}}}, {{{}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}}, {{}, {}, {}, {}, {}, {}, {}, {}}, {{0}, {}, {1}, {}, {}, {}, {}, {}}, {{1}, {0}, {2}, {}, {}, {}, {}, {}}, {{2}, {1}, {3}, {}, {}, {}, {}, {}}, {{3}, {2}, {4}, {}, {}, {}, {}, {}}, {{4}, {3}, {5}, {}, {}, {}, {}, {}}, {{5}, {4}, {6}, {}, {}, {}, {}, {}}, {{6}, {5}, {7}, {}, {}, {}, {}, {}}, {{7}, {6}, {}, {}, {}, {}, {}, {}}, {{8}, {}, {9}, {}, {}, {}, {}, {}}, {{9}, {8}, {10}, {}, {}, {}, {}, {}}, {{10}, {9}, {11}, {}, {}, {}, {}, {}}, {{11}, {10}, {12}, {}, {}, {}, {}, {}}, {{12}, {11}, {13}, {}, {}, {}, {}, {}}, {{13}, {12}, {14}, {}, {}, {}, {}, {}}, {{14}, {13}, {15}, {}, {}, {}, {}, {}}, {{15}, {14}, {}, {}, {}, {}, {}, {}}, {{16}, {}, {17}, {}, {}, {}, {}, {}}, {{17}, {16}, {18}, {}, {}, {}, {}, {}}, {{18}, {17}, {19}, {}, {}, {}, {}, {}}, {{19}, {18}, {20}, {}, {}, {}, {}, {}}, {{20}, {19}, {21}, {}, {}, {}, {}, {}}, {{21}, {20}, {22}, {}, {}, {}, {}, {}}, {{22}, {21}, {23}, {}, {}, {}, {}, {}}, {{23}, {22}, {}, {}, {}, {}, {}, {}}, {{24}, {}, {25}, {}, {}, {}, {}, {}}, {{25}, {24}, {26}, {}, {}, {}, {}, {}}, {{26}, {25}, {27}, {}, {}, {}, {}, {}}, {{27}, {26}, {28}, {}, {}, {}, {}, {}}, {{28}, {27}, {29}, {}, {}, {}, {}, {}}, {{29}, {28}, {30}, {}, {}, {}, {}, {}}, {{30}, {29}, {31}, {}, {}, {}, {}, {}}, {{31}, {30}, {}, {}, {}, {}, {}, {}}, {{32}, {}, {33}, {}, {}, {}, {}, {}}, {{33}, {32}, {34}, {}, {}, {}, {}, {}}, {{34}, {33}, {35}, {}, {}, {}, {}, {}}, {{35}, {34}, {36}, {}, {}, {}, {}, {}}, {{36}, {35}, {37}, {}, {}, {}, {}, {}}, {{37}, {36}, {38}, {}, {}, {}, {}, {}}, {{38}, {37}, {39}, {}, {}, {}, {}, {}}, {{39}, {38}, {}, {}, {}, {}, {}, {}}, {{40, 32}, {}, {41}, {}, {}, {}, {}, {}}, {{41, 33}, {40}, {42}, {}, {}, {}, {}, {}}, {{42, 34}, {41}, {43}, {}, {}, {}, {}, {}}, {{43, 35}, {42}, {44}, {}, {}, {}, {}, {}}, {{44, 36}, {43}, {45}, {}, {}, {}, {}, {}}, {{45, 37}, {44}, {46}, {}, {}, {}, {}, {}}, {{46, 38}, {45}, {47}, {}, {}, {}, {}, {}}, {{47, 39}, {46}, {}, {}, {}, {}, {}, {}}, {{48, 40}, {}, {49}, {}, {}, {}, {}, {}}, {{49, 41}, {48}, {50}, {}, {}, {}, {}, {}}, {{50, 42}, {49}, {51}, {}, {}, {}, {}, {}}, {{51, 43}, {50}, {52}, {}, {}, {}, {}, {}}, {{52, 44}, {51}, {53}, {}, {}, {}, {}, {}}, {{53, 45}, {52}, {54}, {}, {}, {}, {}, {}}, {{54, 46}, {53}, {55}, {}, {}, {}, {}, {}}, {{55, 47}, {54}, {}, {}, {}, {}, {}, {}}}, {{{}, {}, {}, {}, {}, {10}, {}, {17}}, {{}, {}, {}, {}, {}, {11}, {16}, {18}}, {{}, {}, {}, {}, {8}, {12}, {17}, {19}}, {{}, {}, {}, {}, {9}, {13}, {18}, {20}}, {{}, {}, {}, {}, {10}, {14}, {19}, {21}}, {{}, {}, {}, {}, {11}, {15}, {20}, {22}}, {{}, {}, {}, {}, {12}, {}, {21}, {23}}, {{}, {}, {}, {}, {13}, {}, {22}, {}}, {{}, {}, {}, {2}, {}, {18}, {}, {25}}, {{}, {}, {}, {3}, {}, {19}, {24}, {26}}, {{}, {}, {0}, {4}, {16}, {20}, {25}, {27}}, {{}, {}, {1}, {5}, {17}, {21}, {26}, {28}}, {{}, {}, {2}, {6}, {18}, {22}, {27}, {29}}, {{}, {}, {3}, {7}, {19}, {23}, {28}, {30}}, {{}, {}, {4}, {}, {20}, {}, {29}, {31}}, {{}, {}, {5}, {}, {21}, {}, {30}, {}}, {{}, {1}, {}, {10}, {}, {26}, {}, {33}}, {{0}, {2}, {}, {11}, {}, {27}, {32}, {34}}, {{1}, {3}, {8}, {12}, {24}, {28}, {33}, {35}}, {{2}, {4}, {9}, {13}, {25}, {29}, {34}, {36}}, {{3}, {5}, {10}, {14}, {26}, {30}, {35}, {37}}, {{4}, {6}, {11}, {15}, {27}, {31}, {36}, {38}}, {{5}, {7}, {12}, {}, {28}, {}, {37}, {39}}, {{6}, {}, {13}, {}, {29}, {}, {38}, {}}, {{}, {9}, {}, {18}, {}, {34}, {}, {41}}, {{8}, {10}, {}, {19}, {}, {35}, {40}, {42}}, {{9}, {11}, {16}, {20}, {32}, {36}, {41}, {43}}, {{10}, {12}, {17}, {21}, {33}, {37}, {42}, {44}}, {{11}, {13}, {18}, {22}, {34}, {38}, {43}, {45}}, {{12}, {14}, {19}, {23}, {35}, {39}, {44}, {46}}, {{13}, {15}, {20}, {}, {36}, {}, {45}, {47}}, {{14}, {}, {21}, {}, {37}, {}, {46}, {}}, {{}, {17}, {}, {26}, {}, {42}, {}, {49}}, {{16}, {18}, {}, {27}, {}, {43}, {48}, {50}}, {{17}, {19}, {24}, {28}, {40}, {44}, {49}, {51}}, {{18}, {20}, {25}, {29}, {41}, {45}, {50}, {52}}, {{19}, {21}, {26}, {30}, {42}, {46}, {51}, {53}}, {{20}, {22}, {27}, {31}, {43}, {47}, {52}, {54}}, {{21}, {23}, {28}, {}, {44}, {}, {53}, {55}}, {{22}, {}, {29}, {}, {45}, {}, {54}, {}}, {{}, {25}, {}, {34}, {}, {50}, {}, {57}}, {{24}, {26}, {}, {35}, {}, {51}, {56}, {58}}, {{25}, {27}, {32}, {36}, {48}, {52}, {57}, {59}}, {{26}, {28}, {33}, {37}, {49}, {53}, {58}, {60}}, {{27}, {29}, {34}, {38}, {50}, {54}, {59}, {61}}, {{28}, {30}, {35}, {39}, {51}, {55}, {60}, {62}}, {{29}, {31}, {36}, {}, {52}, {}, {61}, {63}}, {{30}, {}, {37}, {}, {53}, {}, {62}, {}}, {{}, {33}, {}, {42}, {}, {58}, {}, {}}, {{32}, {34}, {}, {43}, {}, {59}, {}, {}}, {{33}, {35}, {40}, {44}, {56}, {60}, {}, {}}, {{34}, {36}, {41}, {45}, {57}, {61}, {}, {}}, {{35}, {37}, {42}, {46}, {58}, {62}, {}, {}}, {{36}, {38}, {43}, {47}, {59}, {63}, {}, {}}, {{37}, {39}, {44}, {}, {60}, {}, {}, {}}, {{38}, {}, {45}, {}, {61}, {}, {}, {}}, {{}, {41}, {}, {50}, {}, {}, {}, {}}, {{40}, {42}, {}, {51}, {}, {}, {}, {}}, {{41}, {43}, {48}, {52}, {}, {}, {}, {}}, {{42}, {44}, {49}, {53}, {}, {}, {}, {}}, {{43}, {45}, {50}, {54}, {}, {}, {}, {}}, {{44}, {46}, {51}, {55}, {}, {}, {}, {}}, {{45}, {47}, {52}, {}, {}, {}, {}, {}}, {{46}, {}, {53}, {}, {}, {}, {}, {}}}, {{{}, {}, {}, {}, {}, {}, {}, {9, 18, 27, 36, 45, 54, 63}}, {{}, {}, {}, {}, {}, {8}, {}, {10, 19, 28, 37, 46, 55}}, {{}, {}, {}, {}, {}, {9, 16}, {}, {11, 20, 29, 38, 47}}, {{}, {}, {}, {}, {}, {10, 17, 24}, {}, {12, 21, 30, 39}}, {{}, {}, {}, {}, {}, {11, 18, 25, 32}, {}, {13, 22, 31}}, {{}, {}, {}, {}, {}, {12, 19, 26, 33, 40}, {}, {14, 23}}, {{}, {}, {}, {}, {}, {13, 20, 27, 34, 41, 48}, {}, {15}}, {{}, {}, {}, {}, {}, {14, 21, 28, 35, 42, 49, 56}, {}, {}}, {{}, {}, {1}, {}, {}, {}, {}, {17, 26, 35, 44, 53, 62}}, {{0}, {}, {2}, {}, {}, {16}, {}, {18, 27, 36, 45, 54, 63}}, {{1}, {}, {3}, {}, {}, {17, 24}, {}, {19, 28, 37, 46, 55}}, {{2}, {}, {4}, {}, {}, {18, 25, 32}, {}, {20, 29, 38, 47}}, {{3}, {}, {5}, {}, {}, {19, 26, 33, 40}, {}, {21, 30, 39}}, {{4}, {}, {6}, {}, {}, {20, 27, 34, 41, 48}, {}, {22, 31}}, {{5}, {}, {7}, {}, {}, {21, 28, 35, 42, 49, 56}, {}, {23}}, {{6}, {}, {}, {}, {}, {22, 29, 36, 43, 50, 57}, {}, {}}, {{}, {}, {9, 2}, {}, {}, {}, {}, {25, 34, 43, 52, 61}}, {{8}, {}, {10, 3}, {}, {}, {24}, {}, {26, 35, 44, 53, 62}}, {{9, 0}, {}, {11, 4}, {}, {}, {25, 32}, {}, {27, 36, 45, 54, 63}}, {{10, 1}, {}, {12, 5}, {}, {}, {26, 33, 40}, {}, {28, 37, 46, 55}}, {{11, 2}, {}, {13, 6}, {}, {}, {27, 34, 41, 48}, {}, {29, 38, 47}}, {{12, 3}, {}, {14, 7}, {}, {}, {28, 35, 42, 49, 56}, {}, {30, 39}}, {{13, 4}, {}, {15}, {}, {}, {29, 36, 43, 50, 57}, {}, {31}}, {{14, 5}, {}, {}, {}, {}, {30, 37, 44, 51, 58}, {}, {}}, {{}, {}, {17, 10, 3}, {}, {}, {}, {}, {33, 42, 51, 60}}, {{16}, {}, {18, 11, 4}, {}, {}, {32}, {}, {34, 43, 52, 61}}, {{17, 8}, {}, {19, 12, 5}, {}, {}, {33, 40}, {}, {35, 44, 53, 62}}, {{18, 9, 0}, {}, {20, 13, 6}, {}, {}, {34, 41, 48}, {}, {36, 45, 54, 63}}, {{19, 10, 1}, {}, {21, 14, 7}, {}, {}, {35, 42, 49, 56}, {}, {37, 46, 55}}, {{20, 11, 2}, {}, {22, 15}, {}, {}, {36, 43, 50, 57}, {}, {38, 47}}, {{21, 12, 3}, {}, {23}, {}, {}, {37, 44, 51, 58}, {}, {39}}, {{22, 13, 4}, {}, {}, {}, {}, {38, 45, 52, 59}, {}, {}}, {{}, {}, {25, 18, 11, 4}, {}, {}, {}, {}, {41, 50, 59}}, {{24}, {}, {26, 19, 12, 5}, {}, {}, {40}, {}, {42, 51, 60}}, {{25, 16}, {}, {27, 20, 13, 6}, {}, {}, {41, 48}, {}, {43, 52, 61}}, {{26, 17, 8}, {}, {28, 21, 14, 7}, {}, {}, {42, 49, 56}, {}, {44, 53, 62}}, {{27, 18, 9, 0}, {}, {29, 22, 15}, {}, {}, {43, 50, 57}, {}, {45, 54, 63}}, {{28, 19, 10, 1}, {}, {30, 23}, {}, {}, {44, 51, 58}, {}, {46, 55}}, {{29, 20, 11, 2}, {}, {31}, {}, {}, {45, 52, 59}, {}, {47}}, {{30, 21, 12, 3}, {}, {}, {}, {}, {46, 53, 60}, {}, {}}, {{}, {}, {33, 26, 19, 12, 5}, {}, {}, {}, {}, {49, 58}}, {{32}, {}, {34, 27, 20, 13, 6}, {}, {}, {48}, {}, {50, 59}}, {{33, 24}, {}, {35, 28, 21, 14, 7}, {}, {}, {49, 56}, {}, {51, 60}}, {{34, 25, 16}, {}, {36, 29, 22, 15}, {}, {}, {50, 57}, {}, {52, 61}}, {{35, 26, 17, 8}, {}, {37, 30, 23}, {}, {}, {51, 58}, {}, {53, 62}}, {{36, 27, 18, 9, 0}, {}, {38, 31}, {}, {}, {52, 59}, {}, {54, 63}}, {{37, 28, 19, 10, 1}, {}, {39}, {}, {}, {53, 60}, {}, {55}}, {{38, 29, 20, 11, 2}, {}, {}, {}, {}, {54, 61}, {}, {}}, {{}, {}, {41, 34, 27, 20, 13, 6}, {}, {}, {}, {}, {57}}, {{40}, {}, {42, 35, 28, 21, 14, 7}, {}, {}, {56}, {}, {58}}, {{41, 32}, {}, {43, 36, 29, 22, 15}, {}, {}, {57}, {}, {59}}, {{42, 33, 24}, {}, {44, 37, 30, 23}, {}, {}, {58}, {}, {60}}, {{43, 34, 25, 16}, {}, {45, 38, 31}, {}, {}, {59}, {}, {61}}, {{44, 35, 26, 17, 8}, {}, {46, 39}, {}, {}, {60}, {}, {62}}, {{45, 36, 27, 18, 9, 0}, {}, {47}, {}, {}, {61}, {}, {63}}, {{46, 37, 28, 19, 10, 1}, {}, {}, {}, {}, {62}, {}, {}}, {{}, {}, {49, 42, 35, 28, 21, 14, 7}, {}, {}, {}, {}, {}}, {{48}, {}, {50, 43, 36, 29, 22, 15}, {}, {}, {}, {}, {}}, {{49, 40}, {}, {51, 44, 37, 30, 23}, {}, {}, {}, {}, {}}, {{50, 41, 32}, {}, {52, 45, 38, 31}, {}, {}, {}, {}, {}}, {{51, 42, 33, 24}, {}, {53, 46, 39}, {}, {}, {}, {}, {}}, {{52, 43, 34, 25, 16}, {}, {54, 47}, {}, {}, {}, {}, {}}, {{53, 44, 35, 26, 17, 8}, {}, {55}, {}, {}, {}, {}, {}}, {{54, 45, 36, 27, 18, 9, 0}, {}, {}, {}, {}, {}, {}, {}}}, {{{}, {}, {}, {}, {1, 2, 3, 4, 5, 6, 7}, {}, {8, 16, 24, 32, 40, 48, 56}, {}}, {{}, {}, {}, {0}, {2, 3, 4, 5, 6, 7}, {}, {9, 17, 25, 33, 41, 49, 57}, {}}, {{}, {}, {}, {1, 0}, {3, 4, 5, 6, 7}, {}, {10, 18, 26, 34, 42, 50, 58}, {}}, {{}, {}, {}, {2, 1, 0}, {4, 5, 6, 7}, {}, {11, 19, 27, 35, 43, 51, 59}, {}}, {{}, {}, {}, {3, 2, 1, 0}, {5, 6, 7}, {}, {12, 20, 28, 36, 44, 52, 60}, {}}, {{}, {}, {}, {4, 3, 2, 1, 0}, {6, 7}, {}, {13, 21, 29, 37, 45, 53, 61}, {}}, {{}, {}, {}, {5, 4, 3, 2, 1, 0}, {7}, {}, {14, 22, 30, 38, 46, 54, 62}, {}}, {{}, {}, {}, {6, 5, 4, 3, 2, 1, 0}, {}, {}, {15, 23, 31, 39, 47, 55, 63}, {}}, {{}, {0}, {}, {}, {9, 10, 11, 12, 13, 14, 15}, {}, {16, 24, 32, 40, 48, 56}, {}}, {{}, {1}, {}, {8}, {10, 11, 12, 13, 14, 15}, {}, {17, 25, 33, 41, 49, 57}, {}}, {{}, {2}, {}, {9, 8}, {11, 12, 13, 14, 15}, {}, {18, 26, 34, 42, 50, 58}, {}}, {{}, {3}, {}, {10, 9, 8}, {12, 13, 14, 15}, {}, {19, 27, 35, 43, 51, 59}, {}}, {{}, {4}, {}, {11, 10, 9, 8}, {13, 14, 15}, {}, {20, 28, 36, 44, 52, 60}, {}}, {{}, {5}, {}, {12, 11, 10, 9, 8}, {14, 15}, {}, {21, 29, 37, 45, 53, 61}, {}}, {{}, {6}, {}, {13, 12, 11, 10, 9, 8}, {15}, {}, {22, 30, 38, 46, 54, 62}, {}}, {{}, {7}, {}, {14, 13, 12, 11, 10, 9, 8}, {}, {}, {23, 31, 39, 47, 55, 63}, {}}, {{}, {8, 0}, {}, {}, {17, 18, 19, 20, 21, 22, 23}, {}, {24, 32, 40, 48, 56}, {}}, {{}, {9, 1}, {}, {16}, {18, 19, 20, 21, 22, 23}, {}, {25, 33, 41, 49, 57}, {}}, {{}, {10, 2}, {}, {17, 16}, {19, 20, 21, 22, 23}, {}, {26, 34, 42, 50, 58}, {}}, {{}, {11, 3}, {}, {18, 17, 16}, {20, 21, 22, 23}, {}, {27, 35, 43, 51, 59}, {}}, {{}, {12, 4}, {}, {19, 18, 17, 16}, {21, 22, 23}, {}, {28, 36, 44, 52, 60}, {}}, {{}, {13, 5}, {}, {20, 19, 18, 17, 16}, {22, 23}, {}, {29, 37, 45, 53, 61}, {}}, {{}, {14, 6}, {}, {21, 20, 19, 18, 17, 16}, {23}, {}, {30, 38, 46, 54, 62}, {}}, {{}, {15, 7}, {}, {22, 21, 20, 19, 18, 17, 16}, {}, {}, {31, 39, 47, 55, 63}, {}}, {{}, {16, 8, 0}, {}, {}, {25, 26, 27, 28, 29, 30, 31}, {}, {32, 40, 48, 56}, {}}, {{}, {17, 9, 1}, {}, {24}, {26, 27, 28, 29, 30, 31}, {}, {33, 41, 49, 57}, {}}, {{}, {18, 10, 2}, {}, {25, 24}, {27, 28, 29, 30, 31}, {}, {34, 42, 50, 58}, {}}, {{}, {19, 11, 3}, {}, {26, 25, 24}, {28, 29, 30, 31}, {}, {35, 43, 51, 59}, {}}, {{}, {20, 12, 4}, {}, {27, 26, 25, 24}, {29, 30, 31}, {}, {36, 44, 52, 60}, {}}, {{}, {21, 13, 5}, {}, {28, 27, 26, 25, 24}, {30, 31}, {}, {37, 45, 53, 61}, {}}, {{}, {22, 14, 6}, {}, {29, 28, 27, 26, 25, 24}, {31}, {}, {38, 46, 54, 62}, {}}, {{}, {23, 15, 7}, {}, {30, 29, 28, 27, 26, 25, 24}, {}, {}, {39, 47, 55, 63}, {}}, {{}, {24, 16, 8, 0}, {}, {}, {33, 34, 35, 36, 37, 38, 39}, {}, {40, 48, 56}, {}}, {{}, {25, 17, 9, 1}, {}, {32}, {34, 35, 36, 37, 38, 39}, {}, {41, 49, 57}, {}}, {{}, {26, 18, 10, 2}, {}, {33, 32}, {35, 36, 37, 38, 39}, {}, {42, 50, 58}, {}}, {{}, {27, 19, 11, 3}, {}, {34, 33, 32}, {36, 37, 38, 39}, {}, {43, 51, 59}, {}}, {{}, {28, 20, 12, 4}, {}, {35, 34, 33, 32}, {37, 38, 39}, {}, {44, 52, 60}, {}}, {{}, {29, 21, 13, 5}, {}, {36, 35, 34, 33, 32}, {38, 39}, {}, {45, 53, 61}, {}}, {{}, {30, 22, 14, 6}, {}, {37, 36, 35, 34, 33, 32}, {39}, {}, {46, 54, 62}, {}}, {{}, {31, 23, 15, 7}, {}, {38, 37, 36, 35, 34, 33, 32}, {}, {}, {47, 55, 63}, {}}, {{}, {32, 24, 16, 8, 0}, {}, {}, {41, 42, 43, 44, 45, 46, 47}, {}, {48, 56}, {}}, {{}, {33, 25, 17, 9, 1}, {}, {40}, {42, 43, 44, 45, 46, 47}, {}, {49, 57}, {}}, {{}, {34, 26, 18, 10, 2}, {}, {41, 40}, {43, 44, 45, 46, 47}, {}, {50, 58}, {}}, {{}, {35, 27, 19, 11, 3}, {}, {42, 41, 40}, {44, 45, 46, 47}, {}, {51, 59}, {}}, {{}, {36, 28, 20, 12, 4}, {}, {43, 42, 41, 40}, {45, 46, 47}, {}, {52, 60}, {}}, {{}, {37, 29, 21, 13, 5}, {}, {44, 43, 42, 41, 40}, {46, 47}, {}, {53, 61}, {}}, {{}, {38, 30, 22, 14, 6}, {}, {45, 44, 43, 42, 41, 40}, {47}, {}, {54, 62}, {}}, {{}, {39, 31, 23, 15, 7}, {}, {46, 45, 44, 43, 42, 41, 40}, {}, {}, {55, 63}, {}}, {{}, {40, 32, 24, 16, 8, 0}, {}, {}, {49, 50, 51, 52, 53, 54, 55}, {}, {56}, {}}, {{}, {41, 33, 25, 17, 9, 1}, {}, {48}, {50, 51, 52, 53, 54, 55}, {}, {57}, {}}, {{}, {42, 34, 26, 18, 10, 2}, {}, {49, 48}, {51, 52, 53, 54, 55}, {}, {58}, {}}, {{}, {43, 35, 27, 19, 11, 3}, {}, {50, 49, 48}, {52, 53, 54, 55}, {}, {59}, {}}, {{}, {44, 36, 28, 20, 12, 4}, {}, {51, 50, 49, 48}, {53, 54, 55}, {}, {60}, {}}, {{}, {45, 37, 29, 21, 13, 5}, {}, {52, 51, 50, 49, 48}, {54, 55}, {}, {61}, {}}, {{}, {46, 38, 30, 22, 14, 6}, {}, {53, 52, 51, 50, 49, 48}, {55}, {}, {62}, {}}, {{}, {47, 39, 31, 23, 15, 7}, {}, {54, 53, 52, 51, 50, 49, 48}, {}, {}, {63}, {}}, {{}, {48, 40, 32, 24, 16, 8, 0}, {}, {}, {57, 58, 59, 60, 61, 62, 63}, {}, {}, {}}, {{}, {49, 41, 33, 25, 17, 9, 1}, {}, {56}, {58, 59, 60, 61, 62, 63}, {}, {}, {}}, {{}, {50, 42, 34, 26, 18, 10, 2}, {}, {57, 56}, {59, 60, 61, 62, 63}, {}, {}, {}}, {{}, {51, 43, 35, 27, 19, 11, 3}, {}, {58, 57, 56}, {60, 61, 62, 63}, {}, {}, {}}, {{}, {52, 44, 36, 28, 20, 12, 4}, {}, {59, 58, 57, 56}, {61, 62, 63}, {}, {}, {}}, {{}, {53, 45, 37, 29, 21, 13, 5}, {}, {60, 59, 58, 57, 56}, {62, 63}, {}, {}, {}}, {{}, {54, 46, 38, 30, 22, 14, 6}, {}, {61, 60, 59, 58, 57, 56}, {63}, {}, {}, {}}, {{}, {55, 47, 39, 31, 23, 15, 7}, {}, {62, 61, 60, 59, 58, 57, 56}, {}, {}, {}, {}}}, {{{}, {}, {}, {}, {1, 2, 3, 4, 5, 6, 7}, {}, {8, 16, 24, 32, 40, 48, 56}, {9, 18, 27, 36, 45, 54, 63}}, {{}, {}, {}, {0}, {2, 3, 4, 5, 6, 7}, {8}, {9, 17, 25, 33, 41, 49, 57}, {10, 19, 28, 37, 46, 55}}, {{}, {}, {}, {1, 0}, {3, 4, 5, 6, 7}, {9, 16}, {10, 18, 26, 34, 42, 50, 58}, {11, 20, 29, 38, 47}}, {{}, {}, {}, {2, 1, 0}, {4, 5, 6, 7}, {10, 17, 24}, {11, 19, 27, 35, 43, 51, 59}, {12, 21, 30, 39}}, {{}, {}, {}, {3, 2, 1, 0}, {5, 6, 7}, {11, 18, 25, 32}, {12, 20, 28, 36, 44, 52, 60}, {13, 22, 31}}, {{}, {}, {}, {4, 3, 2, 1, 0}, {6, 7}, {12, 19, 26, 33, 40}, {13, 21, 29, 37, 45, 53, 61}, {14, 23}}, {{}, {}, {}, {5, 4, 3, 2, 1, 0}, {7}, {13, 20, 27, 34, 41, 48}, {14, 22, 30, 38, 46, 54, 62}, {15}}, {{}, {}, {}, {6, 5, 4, 3, 2, 1, 0}, {}, {14, 21, 28, 35, 42, 49, 56}, {15, 23, 31, 39, 47, 55, 63}, {}}, {{}, {0}, {1}, {}, {9, 10, 11, 12, 13, 14, 15}, {}, {16, 24, 32, 40, 48, 56}, {17, 26, 35, 44, 53, 62}}, {{0}, {1}, {2}, {8}, {10, 11, 12, 13, 14, 15}, {16}, {17, 25, 33, 41, 49, 57}, {18, 27, 36, 45, 54, 63}}, {{1}, {2}, {3}, {9, 8}, {11, 12, 13, 14, 15}, {17, 24}, {18, 26, 34, 42, 50, 58}, {19, 28, 37, 46, 55}}, {{2}, {3}, {4}, {10, 9, 8}, {12, 13, 14, 15}, {18, 25, 32}, {19, 27, 35, 43, 51, 59}, {20, 29, 38, 47}}, {{3}, {4}, {5}, {11, 10, 9, 8}, {13, 14, 15}, {19, 26, 33, 40}, {20, 28, 36, 44, 52, 60}, {21, 30, 39}}, {{4}, {5}, {6}, {12, 11, 10, 9, 8}, {14, 15}, {20, 27, 34, 41, 48}, {21, 29, 37, 45, 53, 61}, {22, 31}}, {{5}, {6}, {7}, {13, 12, 11, 10, 9, 8}, {15}, {21, 28, 35, 42, 49, 56}, {22, 30, 38, 46, 54, 62}, {23}}, {{6}, {7}, {}, {14, 13, 12, 11, 10, 9, 8}, {}, {22, 29, 36, 43, 50, 57}, {23, 31, 39, 47, 55, 63}, {}}, {{}, {8, 0}, {9, 2}, {}, {17, 18, 19, 20, 21, 22, 23}, {}, {24, 32, 40, 48, 56}, {25, 34, 43, 52, 61}}, {{8}, {9, 1}, {10, 3}, {16}, {18, 19, 20, 21, 22, 23}, {24}, {25, 33, 41, 49, 57}, {26, 35, 44, 53, 62}}, {{9, 0}, {10, 2}, {11, 4}, {17, 16}, {19, 20, 21, 22, 23}, {25, 32}, {26, 34, 42, 50, 58}, {27, 36, 45, 54, 63}}, {{10, 1}, {11, 3}, {12, 5}, {18, 17, 16}, {20, 21, 22, 23}, {26, 33, 40}, {27, 35, 43, 51, 59}, {28, 37, 46, 55}}, {{11, 2}, {12, 4}, {13, 6}, {19, 18, 17, 16}, {21, 22, 23}, {27, 34, 41, 48}, {28, 36, 44, 52, 60}, {29, 38, 47}}, {{12, 3}, {13, 5}, {14, 7}, {20, 19, 18, 17, 16}, {22, 23}, {28, 35, 42, 49, 56}, {29, 37, 45, 53, 61}, {30, 39}}, {{13, 4}, {14, 6}, {15}, {21, 20, 19, 18, 17, 16}, {23}, {29, 36, 43, 50, 57}, {30, 38, 46, 54, 62}, {31}}, {{14, 5}, {15, 7}, {}, {22, 21, 20, 19, 18, 17, 16}, {}, {30, 37, 44, 51, 58}, {31, 39, 47, 55, 63}, {}}, {{}, {16, 8, 0}, {17, 10, 3}, {}, {25, 26, 27, 28, 29, 30, 31}, {}, {32, 40, 48, 56}, {33, 42, 51, 60}}, {{16}, {17, 9, 1}, {18, 11, 4}, {24}, {26, 27, 28, 29, 30, 31}, {32}, {33, 41, 49, 57}, {34, 43, 52, 61}}, {{17, 8}, {18, 10, 2}, {19, 12, 5}, {25, 24}, {27, 28, 29, 30, 31}, {33, 40}, {34, 42, 50, 58}, {35, 44, 53, 62}}, {{18, 9, 0}, {19, 11, 3}, {20, 13, 6}, {26, 25, 24}, {28, 29, 30, 31}, {34, 41, 48}, {35, 43, 51, 59}, {36, 45, 54, 63}}, {{19, 10, 1}, {20, 12, 4}, {21, 14, 7}, {27, 26, 25, 24}, {29, 30, 31}, {35, 42, 49, 56}, {36, 44, 52, 60}, {37, 46, 55}}, {{20, 11, 2}, {21, 13, 5}, {22, 15}, {28, 27, 26, 25, 24}, {30, 31}, {36, 43, 50, 57}, {37, 45, 53, 61}, {38, 47}}, {{21, 12, 3}, {22, 14, 6}, {23}, {29, 28, 27, 26, 25, 24}, {31}, {37, 44, 51, 58}, {38, 46, 54, 62}, {39}}, {{22, 13, 4}, {23, 15, 7}, {}, {30, 29, 28, 27, 26, 25, 24}, {}, {38, 45, 52, 59}, {39, 47, 55, 63}, {}}, {{}, {24, 16, 8, 0}, {25, 18, 11, 4}, {}, {33, 34, 35, 36, 37, 38, 39}, {}, {40, 48, 56}, {41, 50, 59}}, {{24}, {25, 17, 9, 1}, {26, 19, 12, 5}, {32}, {34, 35, 36, 37, 38, 39}, {40}, {41, 49, 57}, {42, 51, 60}}, {{25, 16}, {26, 18, 10, 2}, {27, 20, 13, 6}, {33, 32}, {35, 36, 37, 38, 39}, {41, 48}, {42, 50, 58}, {43, 52, 61}}, {{26, 17, 8}, {27, 19, 11, 3}, {28, 21, 14, 7}, {34, 33, 32}, {36, 37, 38, 39}, {42, 49, 56}, {43, 51, 59}, {44, 53, 62}}, {{27, 18, 9, 0}, {28, 20, 12, 4}, {29, 22, 15}, {35, 34, 33, 32}, {37, 38, 39}, {43, 50, 57}, {44, 52, 60}, {45, 54, 63}}, {{28, 19, 10, 1}, {29, 21, 13, 5}, {30, 23}, {36, 35, 34, 33, 32}, {38, 39}, {44, 51, 58}, {45, 53, 61}, {46, 55}}, {{29, 20, 11, 2}, {30, 22, 14, 6}, {31}, {37, 36, 35, 34, 33, 32}, {39}, {45, 52, 59}, {46, 54, 62}, {47}}, {{30, 21, 12, 3}, {31, 23, 15, 7}, {}, {38, 37, 36, 35, 34, 33, 32}, {}, {46, 53, 60}, {47, 55, 63}, {}}, {{}, {32, 24, 16, 8, 0}, {33, 26, 19, 12, 5}, {}, {41, 42, 43, 44, 45, 46, 47}, {}, {48, 56}, {49, 58}}, {{32}, {33, 25, 17, 9, 1}, {34, 27, 20, 13, 6}, {40}, {42, 43, 44, 45, 46, 47}, {48}, {49, 57}, {50, 59}}, {{33, 24}, {34, 26, 18, 10, 2}, {35, 28, 21, 14, 7}, {41, 40}, {43, 44, 45, 46, 47}, {49, 56}, {50, 58}, {51, 60}}, {{34, 25, 16}, {35, 27, 19, 11, 3}, {36, 29, 22, 15}, {42, 41, 40}, {44, 45, 46, 47}, {50, 57}, {51, 59}, {52, 61}}, {{35, 26, 17, 8}, {36, 28, 20, 12, 4}, {37, 30, 23}, {43, 42, 41, 40}, {45, 46, 47}, {51, 58}, {52, 60}, {53, 62}}, {{36, 27, 18, 9, 0}, {37, 29, 21, 13, 5}, {38, 31}, {44, 43, 42, 41, 40}, {46, 47}, {52, 59}, {53, 61}, {54, 63}}, {{37, 28, 19, 10, 1}, {38, 30, 22, 14, 6}, {39}, {45, 44, 43, 42, 41, 40}, {47}, {53, 60}, {54, 62}, {55}}, {{38, 29, 20, 11, 2}, {39, 31, 23, 15, 7}, {}, {46, 45, 44, 43, 42, 41, 40}, {}, {54, 61}, {55, 63}, {}}, {{}, {40, 32, 24, 16, 8, 0}, {41, 34, 27, 20, 13, 6}, {}, {49, 50, 51, 52, 53, 54, 55}, {}, {56}, {57}}, {{40}, {41, 33, 25, 17, 9, 1}, {42, 35, 28, 21, 14, 7}, {48}, {50, 51, 52, 53, 54, 55}, {56}, {57}, {58}}, {{41, 32}, {42, 34, 26, 18, 10, 2}, {43, 36, 29, 22, 15}, {49, 48}, {51, 52, 53, 54, 55}, {57}, {58}, {59}}, {{42, 33, 24}, {43, 35, 27, 19, 11, 3}, {44, 37, 30, 23}, {50, 49, 48}, {52, 53, 54, 55}, {58}, {59}, {60}}, {{43, 34, 25, 16}, {44, 36, 28, 20, 12, 4}, {45, 38, 31}, {51, 50, 49, 48}, {53, 54, 55}, {59}, {60}, {61}}, {{44, 35, 26, 17, 8}, {45, 37, 29, 21, 13, 5}, {46, 39}, {52, 51, 50, 49, 48}, {54, 55}, {60}, {61}, {62}}, {{45, 36, 27, 18, 9, 0}, {46, 38, 30, 22, 14, 6}, {47}, {53, 52, 51, 50, 49, 48}, {55}, {61}, {62}, {63}}, {{46, 37, 28, 19, 10, 1}, {47, 39, 31, 23, 15, 7}, {}, {54, 53, 52, 51, 50, 49, 48}, {}, {62}, {63}, {}}, {{}, {48, 40, 32, 24, 16, 8, 0}, {49, 42, 35, 28, 21, 14, 7}, {}, {57, 58, 59, 60, 61, 62, 63}, {}, {}, {}}, {{48}, {49, 41, 33, 25, 17, 9, 1}, {50, 43, 36, 29, 22, 15}, {56}, {58, 59, 60, 61, 62, 63}, {}, {}, {}}, {{49, 40}, {50, 42, 34, 26, 18, 10, 2}, {51, 44, 37, 30, 23}, {57, 56}, {59, 60, 61, 62, 63}, {}, {}, {}}, {{50, 41, 32}, {51, 43, 35, 27, 19, 11, 3}, {52, 45, 38, 31}, {58, 57, 56}, {60, 61, 62, 63}, {}, {}, {}}, {{51, 42, 33, 24}, {52, 44, 36, 28, 20, 12, 4}, {53, 46, 39}, {59, 58, 57, 56}, {61, 62, 63}, {}, {}, {}}, {{52, 43, 34, 25, 16}, {53, 45, 37, 29, 21, 13, 5}, {54, 47}, {60, 59, 58, 57, 56}, {62, 63}, {}, {}, {}}, {{53, 44, 35, 26, 17, 8}, {54, 46, 38, 30, 22, 14, 6}, {55}, {61, 60, 59, 58, 57, 56}, {63}, {}, {}, {}}, {{54, 45, 36, 27, 18, 9, 0}, {55, 47, 39, 31, 23, 15, 7}, {}, {62, 61, 60, 59, 58, 57, 56}, {}, {}, {}, {}}}, {{{}, {}, {}, {}, {1}, {}, {8}, {9}}, {{}, {}, {}, {0}, {2}, {8}, {9}, {10}}, {{}, {}, {}, {1}, {3}, {9}, {10}, {11}}, {{}, {}, {}, {2}, {4}, {10}, {11}, {12}}, {{}, {}, {}, {3}, {5}, {11}, {12}, {13}}, {{}, {}, {}, {4}, {6}, {12}, {13}, {14}}, {{}, {}, {}, {5}, {7}, {13}, {14}, {15}}, {{}, {}, {}, {6}, {}, {14}, {15}, {}}, {{}, {0}, {1}, {}, {9}, {}, {16}, {17}}, {{0}, {1}, {2}, {8}, {10}, {16}, {17}, {18}}, {{1}, {2}, {3}, {9}, {11}, {17}, {18}, {19}}, {{2}, {3}, {4}, {10}, {12}, {18}, {19}, {20}}, {{3}, {4}, {5}, {11}, {13}, {19}, {20}, {21}}, {{4}, {5}, {6}, {12}, {14}, {20}, {21}, {22}}, {{5}, {6}, {7}, {13}, {15}, {21}, {22}, {23}}, {{6}, {7}, {}, {14}, {}, {22}, {23}, {}}, {{}, {8}, {9}, {}, {17}, {}, {24}, {25}}, {{8}, {9}, {10}, {16}, {18}, {24}, {25}, {26}}, {{9}, {10}, {11}, {17}, {19}, {25}, {26}, {27}}, {{10}, {11}, {12}, {18}, {20}, {26}, {27}, {28}}, {{11}, {12}, {13}, {19}, {21}, {27}, {28}, {29}}, {{12}, {13}, {14}, {20}, {22}, {28}, {29}, {30}}, {{13}, {14}, {15}, {21}, {23}, {29}, {30}, {31}}, {{14}, {15}, {}, {22}, {}, {30}, {31}, {}}, {{}, {16}, {17}, {}, {25}, {}, {32}, {33}}, {{16}, {17}, {18}, {24}, {26}, {32}, {33}, {34}}, {{17}, {18}, {19}, {25}, {27}, {33}, {34}, {35}}, {{18}, {19}, {20}, {26}, {28}, {34}, {35}, {36}}, {{19}, {20}, {21}, {27}, {29}, {35}, {36}, {37}}, {{20}, {21}, {22}, {28}, {30}, {36}, {37}, {38}}, {{21}, {22}, {23}, {29}, {31}, {37}, {38}, {39}}, {{22}, {23}, {}, {30}, {}, {38}, {39}, {}}, {{}, {24}, {25}, {}, {33}, {}, {40}, {41}}, {{24}, {25}, {26}, {32}, {34}, {40}, {41}, {42}}, {{25}, {26}, {27}, {33}, {35}, {41}, {42}, {43}}, {{26}, {27}, {28}, {34}, {36}, {42}, {43}, {44}}, {{27}, {28}, {29}, {35}, {37}, {43}, {44}, {45}}, {{28}, {29}, {30}, {36}, {38}, {44}, {45}, {46}}, {{29}, {30}, {31}, {37}, {39}, {45}, {46}, {47}}, {{30}, {31}, {}, {38}, {}, {46}, {47}, {}}, {{}, {32}, {33}, {}, {41}, {}, {48}, {49}}, {{32}, {33}, {34}, {40}, {42}, {48}, {49}, {50}}, {{33}, {34}, {35}, {41}, {43}, {49}, {50}, {51}}, {{34}, {35}, {36}, {42}, {44}, {50}, {51}, {52}}, {{35}, {36}, {37}, {43}, {45}, {51}, {52}, {53}}, {{36}, {37}, {38}, {44}, {46}, {52}, {53}, {54}}, {{37}, {38}, {39}, {45}, {47}, {53}, {54}, {55}}, {{38}, {39}, {}, {46}, {}, {54}, {55}, {}}, {{}, {40}, {41}, {}, {49}, {}, {56}, {57}}, {{40}, {41}, {42}, {48}, {50}, {56}, {57}, {58}}, {{41}, {42}, {43}, {49}, {51}, {57}, {58}, {59}}, {{42}, {43}, {44}, {50}, {52}, {58}, {59}, {60}}, {{43}, {44}, {45}, {51}, {53}, {59}, {60}, {61}}, {{44}, {45}, {46}, {52}, {54}, {60}, {61}, {62}}, {{45}, {46}, {47}, {53}, {55}, {61}, {62}, {63}}, {{46}, {47}, {}, {54}, {}, {62}, {63}, {}}, {{}, {48}, {49}, {}, {57}, {}, {}, {}}, {{48}, {49}, {50}, {56}, {58}, {}, {}, {}}, {{49}, {50}, {51}, {57}, {59}, {}, {}, {}}, {{50}, {51}, {52}, {58}, {60}, {}, {}, {}}, {{51}, {52}, {53}, {59}, {61}, {}, {}, {}}, {{52}, {53}, {54}, {60}, {62}, {}, {}, {}}, {{53}, {54}, {55}, {61}, {63}, {}, {}, {}}, {{54}, {55}, {}, {62}, {}, {}, {}, {}}}};
    //, const int * (*)(), const int * (*)()
    int run();
    void generate_piece_moves(int);
    int update_moves();
    void make_move(int, int, int = -1);
    void update(int, int, Piece, int);
    void draw(int);
    Piece * get_pl();
    bool * get_pe();
    string get_fen();
    int get_debug() {return move_count;};
    const vector<Move> & get_ml() {return move_list;};
    *Board() {vector<Move>().swap(move_list);move_list = {};move_list.shrink_to_fit();move_list.clear(); en_passant = -1, mr_count = 0, to_play = 1;}
private:
    Random_ui *ui_white = new Random_ui(1), *ui_black = new Random_ui(0);
    Piece piece_list[64] = {};
    int king_positions[2] = {}, en_passant, mr_count, undo_count = 0, move_count = 0;
    bool piece_exists[64] = {}, castles[4] = {false, false, false, false}, to_play;
    vector<Move> move_list = {};
    vector<Position> positions = {};
};

