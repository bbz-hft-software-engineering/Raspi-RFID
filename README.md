# Raspi-RFID
Das Projekt besteht aus zwei Programmen. Ein Python-Programm wird verwendet, um die UID einzulesen
und ein C++-Programm wertet die gesamte Information aus.

Benutzerhandbuch

1. Die erste UID einer Karte muss manuell in das Dokument UIDsave.txt eingetragen werden.
2. Mit dem Befehl "get_UID_Input" wird der UID-Leseprozess gestartet. Wenn dieser erfolgreich ist, werden die rote und blaue LED eingeschaltet.
3. Wenn eine Karte erkannt wird, erlischt die blaue LED und die rote LED blinkt einmal.
4. Um eine weitere Karte hinzuzufügen, muss zunächst eine bereits bekannte RFID-Karte eingelesen werden. Nach dem Einlesen muss der Befehl "Add_Input" zeitnah gedrückt werden.
5. Wenn dies erfolgreich ist, leuchtet die blaue LED auf.
6. Nun kann eine neue RFID-Karte registriert werden. Sobald dieser Vorgang erfolgreich abgeschlossen ist, erlischt die blaue LED.
