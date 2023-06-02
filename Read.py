#!/usr/bin/env python

import signal
import time
import sys

from pirc522 import RFID

run = True
rdr = RFID()
util = rdr.util()
util.debug = False

def end_read(signal,frame):
    global run
    print("\nCtrl+C captured, ending read.")
    run = False
    rdr.cleanup()
    sys.exit()

signal.signal(signal.SIGINT, end_read)

print("Starting")
while run:
    rdr.wait_for_tag()

    (error, data) = rdr.request()
    if not error:
        print("\nDetected: " + format(data, "02x"))

    (error, uid) = rdr.anticoll()
    if not error:
        # UID Ausgeben in der Konsole
        print("Card read UID: "+str(uid[0])+str(uid[1])+str(uid[2])+str(uid[3]))
        #UID.txt file erstellen und die UID darin Speichern 
        try:
            with open('UID.txt', 'x') as f:
                f.write(str(uid[0])+str(uid[1])+str(uid[2])+str(uid[3]))
        #Abfangen des Errors
        except FileExistsError:
            print("File bereits erstellt")
        util.set_tag(uid)
        util.auth(rdr.auth_b, [0x74, 0x00, 0x52, 0x35, 0x00, 0xFF])
        util.read_out(4)
        util.deauth()
        run = False
        rdr.cleanup()
        sys.exit()
        
        
