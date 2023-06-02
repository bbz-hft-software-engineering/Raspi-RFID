#include <wiringPi.h> // Bibliothek für die GPIO-Steuerung
#include <iostream> // Standardbibliothek für Ein- und Ausgabe
#include <string> // Bibliothek für Zeichenkettenverarbeitung
#include <fstream> // Bibliothek für Datei-Ein- und Ausgabe
#include <vector> // Bibliothek für Vektoren (dynamische Arrays)
#include <unistd.h> // Bibliothek für Systemaufrufe (z. B. sleep)

using namespace std;

class StringStorage {
private:
    vector<string> strings; // Vektor zur Speicherung von Zeichenketten
    string filename; // Dateiname zur Speicherung der Zeichenketten

public:
    StringStorage(const string& file) : filename(file) {
        loadStrings(); // Zeichenketten aus der Datei laden, wenn das Objekt erstellt wird
    }

    void addString(const string& str) {
        strings.push_back(str); // Eine Zeichenkette zum Vektor hinzufügen
        saveStrings(); // Zeichenketten in die Datei speichern
    }

    bool compareWithString(const string& str) {
        for (const string& storedStr : strings) {
            if (storedStr == str) {
                return true; // Überprüfen, ob eine Zeichenkette im Vektor enthalten ist
            }
        }
        return false;
    }

    void loadStrings() {
        ifstream file(filename); // Datei zum Lesen öffnen
        if (file.is_open()) {
            string str;
            while (getline(file, str)) {
                strings.push_back(str); // Zeichenketten aus der Datei in den Vektor laden
            }
            file.close(); // Datei schließen
        }
    }

    void saveStrings() {
        ofstream file(filename); // Datei zum Schreiben öffnen
        if (file.is_open()) {
            for (const string& str : strings) {
                file << str << endl; // Zeichenketten aus dem Vektor in die Datei schreiben
            }
            file.close(); // Datei schließen
        }
    }
};

string get_UID_Input() {
    system("python Read.py"); // Aufruf eines externen Python-Skripts
    ifstream newfile("UID.txt", ios::in); // Öffnen einer Datei zum Lesen
    string UID_Input;

    if (newfile.is_open()) {
        while (getline(newfile, UID_Input)) {
            cout << UID_Input << "\n"; // Ausgabe der Daten
        }
        newfile.close(); // Schließen der Datei
        remove("UID.txt"); // Löschen der Datei
    }

    return UID_Input; // Rückgabe der gelesenen Zeichenkette
}

int main() {




    wiringPiSetup(); // Initialisierung der GPIO-Steuerung
    const int led_red = 7; // GPIO-Nummer für die rote LED
    pinMode(led_red, OUTPUT); // Festlegen des GPIO-Pins als Ausgang
    const int led_blue = 29; // GPIO-Nummer für die blaue LED
    pinMode(led_blue, OUTPUT); // Festlegen des GPIO-Pins als Ausgang
    const int Add_Input = 24; // GPIO-Nummer für den Eingang
    pinMode(Add_Input, INPUT); // Festlegen des GPIO-Pins als Eingang
    const int Start_UIDread = 25; // GPIO-Nummer für den Eingang
    pinMode(Start_UIDread, INPUT); // Festlegen des GPIO-Pins als Eingang
    pullUpDnControl(Add_Input, PUD_DOWN); // Interner Widerstand, um den Eingang auf 0V zu ziehen
    pullUpDnControl(Start_UIDread, PUD_DOWN); // Interner Widerstand, um den Eingang auf 0V zu ziehen
    string UID_Input; // UID in Textformat
    int loopcounter = 0; // Schleifenzähler für Reset der UID


    StringStorage storage("UIDsave.txt"); // Instanz von StringStorage mit Dateinamen "UIDsave.txt" erstellen

    while (true) {
        if (digitalRead(Add_Input) == HIGH && digitalRead(Start_UIDread) == HIGH)
            return 0; // Das Programm beenden, wenn beide Eingänge HIGH sind
        // Start des Leseprozesses
        if (digitalRead(Start_UIDread) == HIGH) {
            digitalWrite(led_blue, HIGH); // Einschalten der blauen LED
            digitalWrite(led_red, HIGH); // Einschalten der roten LED

            UID_Input = get_UID_Input(); // RFID-UID vom Benutzer eingeben
            digitalWrite(led_blue, LOW); // Ausschalten der blauen LED
            digitalWrite(led_red, LOW); // Ausschalten der roten LED

            if (storage.compareWithString(UID_Input)) {
                cout << "RFID Karte erfolgreich erkannt" << endl;
                digitalWrite(led_red, HIGH); // Einschalten der roten LED
                sleep(2); // Warten für 1 Sekunde
                digitalWrite(led_red, LOW); // Ausschalten der roten LED
            }
        }
        // Neue Karte wird nur Hinzugefügt wenn zuvor eine Bekannte Erkannt wurde
        if (digitalRead(Add_Input) == HIGH && storage.compareWithString(UID_Input)) {
            digitalWrite(led_blue, HIGH); // Einschalten der blauen LED
            // Lernmodus aktivieren.
            cout << "Lernmodus: Neue Karte auflegen..." << endl;
            sleep(1); // Warten für 1 Sekunde
            UID_Input = get_UID_Input(); // RFID-UID vom Benutzer eingeben
            storage.addString(UID_Input); // RFID-UID zur Speicherung hinzufügen
            cout << "Lernmodus: Neue Karte registriert" << endl;
            digitalWrite(led_blue, LOW); // Ausschalten der blauen LED
        }
        // Wird Verwendet um Sicherzustellen das die UID nach einer Verzögerung zurückgesetzt wird.
        if (loopcounter > 100000) {
            UID_Input = "0";
            loopcounter = 0;
        }


        loopcounter++; // Schleifenzähler




    }

    return 0;
}
