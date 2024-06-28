#include <ELECHOUSE_CC1101_SRC_DRV.h>
#define PMOS_1 8
#define PMOS_2 7
#define NMOS_1 6
#define NMOS_2 5
#define A 9
#define B 3

#define SCK 13
#define MISO 12
#define MOSI 11
#define CSN 10
#define GDO0 4

int lastPacketId;

void setup() {
    lastPacketId = -1;

    Serial.begin(115200);

    // Setup pins for motor controller
    pinMode(NMOS_1, OUTPUT);
    pinMode(NMOS_2, OUTPUT);
    pinMode(PMOS_1, OUTPUT);
    pinMode(PMOS_2, OUTPUT);
    pinMode(A, OUTPUT);
    pinMode(B, OUTPUT);

    // Set idle voltages
    reset();

    // Initialize connection with radio module via spi
    ELECHOUSE_cc1101.setSpiPin(SCK, MISO, MOSI, CSN);
    if (ELECHOUSE_cc1101.getCC1101()){ // Check the CC1101 Spi connection.
        Serial.println("Connection OK");
    }else{
        Serial.println("Connection Error");
        return;
    }

    // Configure radio module
    ELECHOUSE_cc1101.Init(); // must be set to initialize the cc1101!
    ELECHOUSE_cc1101.setGDO0(GDO0); // set lib internal gdo pin (gdo0). Gdo2 not use for this example.
    ELECHOUSE_cc1101.setCCMode(1); // set config for internal transmission mode.
    ELECHOUSE_cc1101.setModulation(0); // set modulation mode. 0 = 2-FSK, 1 = GFSK, 2 = ASK/OOK, 3 = 4-FSK, 4 = MSK.
    ELECHOUSE_cc1101.setMHZ(433.92); // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300–348 MHZ, 387–464MHZ and 779–928MHZ. Read More info from datasheet.
    ELECHOUSE_cc1101.setSyncMode(2); // Combined sync-word qualifier mode. 0 = No preamble/sync. 1 = 16 sync word bits detected. 2 = 16/16 sync word bits detected. 3 = 30/32 sync word bits detected. 4 = No preamble/sync, carrier-sense above threshold. 5 = 15/16 + carrier-sense above threshold. 6 = 16/16 + carrier-sense above threshold. 7 = 30/32 + carrier-sense above threshold.
    ELECHOUSE_cc1101.setCrc(1); // 1 = CRC calculation in TX and CRC check in RX enabled. 0 = CRC disabled for TX and RX.
    Serial.println("Rx Mode");
}

byte buffer[61] = {0};
void loop() {
    if (ELECHOUSE_cc1101.CheckReceiveFlag()){
        if (ELECHOUSE_cc1101.CheckCRC()){
            int len = ELECHOUSE_cc1101.ReceiveData(buffer);
            if(len == 0)
                return;
            buffer[len] = '\0';
            String data = String((char *)buffer);
            int packetId = data.substring(0, data.indexOf('|')).toInt();

            if(lastPacketId == -1 || packetId > lastPacketId || packetId < 5)
                lastPacketId = packetId;
            else
                return;

            String id = data.substring(data.indexOf('|') + 1, data.lastIndexOf('|'));
            data = data.substring(data.lastIndexOf('|') + 1);
            
            Serial.print(id);
            Serial.print(" ");
            Serial.println(data);

            if(id == "b")
            {
                int speed = data.toInt();
                digitalWrite(PMOS_1, LOW);
                digitalWrite(PMOS_2, HIGH);

                analogWrite(NMOS_1, speed);
                digitalWrite(NMOS_2, LOW);
            }
            else if(id == "f")
            {
                int speed = data.toInt();
                digitalWrite(PMOS_1, HIGH);
                digitalWrite(PMOS_2, LOW);

                digitalWrite(NMOS_1, LOW);
                analogWrite(NMOS_2, speed);
            }
            else if(id == "s")
            {
                reset();
            }
            else if(id == "r")
            {
                analogWrite(A, data.toInt());
                digitalWrite(B, LOW);
            }
            else if(id == "l")
            {
                analogWrite(B, data.toInt());
                digitalWrite(A, LOW);
            }
            else if(id == "c")
            {
                digitalWrite(A, LOW);
                digitalWrite(B, LOW);
            }
        }
    }
}

void reset()
{
    // P closed == HIGH
    digitalWrite(PMOS_1, HIGH);
    digitalWrite(PMOS_2, HIGH);

    // N closed == LOW
    digitalWrite(NMOS_1, LOW);
    digitalWrite(NMOS_2, LOW);
    //Serial.println("reset");
}