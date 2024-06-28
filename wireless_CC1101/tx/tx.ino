#include <ELECHOUSE_CC1101_SRC_DRV.h>
byte packetId;

void setup() {
    byte sck = 13;
    byte miso = 12;
    byte mosi = 11;
    byte csn = 10;    // also called ss-pin
    int gdo0 = 6;

    packetId = 0;

    // initializing library with custom pins selected

    Serial.begin(115200);
    ELECHOUSE_cc1101.setSpiPin(sck, miso, mosi, csn);
    if (ELECHOUSE_cc1101.getCC1101()){ // Check the CC1101 Spi connection.
        Serial.println("Connection OK");
    }else{
        Serial.println("Connection Error");
        return;
    }

    ELECHOUSE_cc1101.Init(); // must be set to initialize the cc1101!
    ELECHOUSE_cc1101.setGDO0(gdo0); // set lib internal gdo pin (gdo0). Gdo2 not use for this example.
    ELECHOUSE_cc1101.setCCMode(1); // set config for internal transmission mode.
    ELECHOUSE_cc1101.setModulation(0); // set modulation mode. 0 = 2-FSK, 1 = GFSK, 2 = ASK/OOK, 3 = 4-FSK, 4 = MSK.
    ELECHOUSE_cc1101.setMHZ(433.92); // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300–348 MHZ, 387–464MHZ and 779–928MHZ. Read More info from datasheet.
    ELECHOUSE_cc1101.setSyncMode(2); // Combined sync-word qualifier mode. 0 = No preamble/sync. 1 = 16 sync word bits detected. 2 = 16/16 sync word bits detected. 3 = 30/32 sync word bits detected. 4 = No preamble/sync, carrier-sense above threshold. 5 = 15/16 + carrier-sense above threshold. 6 = 16/16 + carrier-sense above threshold. 7 = 30/32 + carrier-sense above threshold.
    // ELECHOUSE_cc1101.setPA(10); // set TxPower. The following settings are possible depending on the frequency band. (-30 -20 -15 -10 -6 0 5 7 10 11 12) Default is max!
    ELECHOUSE_cc1101.setCrc(1); // 1 = CRC calculation in TX and CRC check in RX enabled. 0 = CRC disabled for TX and RX.
    Serial.println("Tx Mode");
}

void txMsg(String id, String msg="") {
    msg = String(packetId) + "|" + id + "|" + msg;
    char* c_msg = (char *)malloc(sizeof(char) * msg.length());
    strcpy(c_msg, msg.c_str());
    Serial.println(c_msg);
    ELECHOUSE_cc1101.SendData(c_msg);
    free(c_msg);
    packetId++;
}

void loop() {
    if(Serial.available() > 0)
    {
        String s =  Serial.readStringUntil('\n');
        String id = s.substring(0, 1);
        String msg = s.substring(1);
    

        if(id == "f")
        {
            txMsg("f", msg);
        }
        else if(id == "b")
        {
            txMsg("b", msg);
        }
        else if(id == "s")
        {
            txMsg("s");
        }
        else if(id == "r")
        {
            txMsg("r", msg);
        }
        else if(id == "l")
        {
            txMsg("l", msg);
        }
        else if(id == "c")
        {
            txMsg("c");
        }
    }    
}