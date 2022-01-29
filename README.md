# Implementierung eines Schachprogramms
## Anleitung
### Wichtig
Das Programm enthält weder einen Compiler noch eine ausführbare Datei. 
Alle Versuche müssen mit eigener Software durchgeführt werden.

### Wie funktioniert das Programm?
Bei Simulationen wird eine Ausgabedatei kreiert. 
Diese enthält 6 Werte. 
Gelesen von links nach rechts handelt es sich um Siege von Schwarz, Siege von Weiß, Patt, Stellungswiederholung, 50-Zug-Regel und Spielende durch Materialmangel. 
Für Simulationen mit verschiedenen Oberflächen ist je der erste Wert in der Initialisierung der Interface-structure in main.cpp zu verändern. 
0 steht für einen menschlichen Spieler (bedienbare Ui), 1 für zufällige Züge und 2 für einen Minmax-Bot. 
Der Wahrheitswert in der Initialisierung von Board (ebenfalls main.cpp) gibt an, ob das Brett nach jedem Zug gezeichnet werden soll.

### CMakeLists
```
cmake_minimum_required(VERSION 3.21)
project(Chesscpp-main)

set(CMAKE_CXX_STANDARD 17)

add_executable(Chesscpp-main Pieces.cpp Board.cpp Headers/Board.h Headers/Pieces.h main.cpp Headers/main.h ui.cpp Headers/ui.h)
```

Ordnername muss, falls notwendig, angepasst werden. Der Ordner sollte immer ***Chesscpp-main*** heißen.
