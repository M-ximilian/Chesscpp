#include "Headers/Board.h"
#include <ctime>


int main() {
    int used_seed = time(0); // um zufällige Spiele zu gewährleisten
    int bot_depth = 3; // Suchtiefe, auf schwächeren Laptops eher auf 3 belassen, sonst auch 4 realistisch
    int scores[6] = {0, 0, 0, 0, 0, 0}; // speichern des Punktestandes, für Bedeutung siehe Readme
    srand(used_seed);
    string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"; //Startpositon in FEN-Notation

    for (int i = 0; i < 100000; i++) { // Nie erreichbar hohe Zahl gewünschter Durchläufe
        // Initialisierung der Interfaces, siehe Readme
        auto *ui_white = new Interface{0, new Ascii_ui(1), new Random_ui(1), new Bot_ui(1, bot_depth)},
                *ui_black = new Interface{2, new Ascii_ui(0), new Random_ui(0), new Bot_ui(0, bot_depth)};

        // Initialisierung des Brettes, letzter Parameter entscheidet, ob gezeichnet werden soll
        Board b = Board(fen, ui_white, ui_black, true);

        // setzen zusätzlicher Parameter für das Brett
        if (ui_white->type == 2) { ui_white->bui->set_board(&b); }
        if (ui_black->type == 2) { ui_black->bui->set_board(&b); }
        int result = b.run(); // Simulation des Spiels und speichern des Ergebnisses
        scores[result == 6 ? 5 : result]++; // ändern in Scores-Array

        // Schreiben der Erbebnisse in die Ausgabedatei
        ofstream Output("../scores/scores.txt");
        Output << scores[0] << " " << scores[1] << " " << scores[2] << " " << scores[3] << " " << scores[4] << " "
               << scores[5];
        Output.close();

        // Angabe der Spiele pro Sekunde bei jedem 100. Spiel
        if (i % 100 == 0) {
            cout << "total " << i << endl << ((float) i) / ((float) (time(0) - used_seed)) << " games/s" << endl
                 << endl;
        }
    }
    return 0;
}


