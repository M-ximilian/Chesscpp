Implementierung eines Schachprogramms.
Bei Simulationen wird eine Ausgabedatei kreeiert. Diese enthält 6 Werte. Gelesen von links nach rechts handelt es sich um Siege von Schwarz, Siege von Weiß, Patt, Stellungswiederholung, 50-Zug-Regel und Spielende durch Materialmangel.
für Simulationen mit verschiedenen Oberflächen ist je der erste Wert in der Initialisierung der Interface-structure in main.cpp zu verändern.
0 steht für einen menschlichen Spieler (bedienbare Ui), 1 für zufällige Züge und 2 für einen Minmax-Bot.
Der Wahrheitwert in der Initialisierung von Board (ebenfalls main.cpp) gibt an, ob das Brett nach jedem Zug gezeichnet werden soll.
