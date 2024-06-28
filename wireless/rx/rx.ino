#include "TinyRF_RX.h"

#define RX 2
#define NMOS_1 10
#define NMOS_2 11

#define PMOS_1 8
#define PMOS_2 9

#define A 5
#define B 6




int lastPacketId = -1;

void setup() {
    Serial.begin(115200);
    setupReceiver(RX);
    pinMode(NMOS_1, OUTPUT);
    pinMode(NMOS_2, OUTPUT);
    pinMode(PMOS_1, OUTPUT);
    pinMode(PMOS_2, OUTPUT);
    pinMode(A, OUTPUT);
    pinMode(B, OUTPUT);
    pinMode(13, OUTPUT);

    reset();
}

void loop() {
    const uint8_t bufSize = 30;
    byte buf[bufSize];
    uint8_t numLostMsgs = 0;
    uint8_t numRcvdBytes = 0;

    uint8_t err = getReceivedData(buf, bufSize, numRcvdBytes, numLostMsgs);

    if(err == TRF_ERR_NO_DATA) {
        return;
    }

    if(err == TRF_ERR_BUFFER_OVERFLOW) {
        Serial.println("Buffer too small for received data!");
        return;
    }

    if(err == TRF_ERR_CORRUPTED) {
        Serial.println("Received corrupted data.");
        return;
    }

    if(err == TRF_ERR_SUCCESS) {
        String data = "";
        for(int i = 0; i < numRcvdBytes; i++){
            data += (char)buf[i];
        }

        if(numLostMsgs > 0){
            Serial.print(numLostMsgs);
            Serial.println(" messages were lost before this message.");
        }

        int packetId = data.substring(0, data.indexOf('|')).toInt();
        /*Serial.print(packetId);
        Serial.print(" ");
        Serial.println(lastPacketId);*/
        
        if(lastPacketId == -1 || packetId > lastPacketId || packetId == 0)
            lastPacketId = packetId;
        else
            return;
        
        String id = data.substring(data.indexOf('|') + 1, data.lastIndexOf('|'));
        data = data.substring(data.lastIndexOf('|') + 1);
        
        Serial.print(id);
        Serial.print(" ");
        Serial.println(data);
        if(id == "f")
        {
            digitalWrite(13, HIGH);
            int speed = data.toInt();
            digitalWrite(PMOS_1, LOW);
            digitalWrite(PMOS_2, HIGH);

            analogWrite(NMOS_1, speed);
            digitalWrite(NMOS_2, LOW);
        }
        else if(id == "b")
        {
            digitalWrite(13, LOW);
            int speed = data.toInt();
            digitalWrite(PMOS_1, HIGH);
            digitalWrite(PMOS_2, LOW);

            digitalWrite(NMOS_1, LOW);
            analogWrite(NMOS_2, speed);
        }
        else if(id == "s")
        {
            digitalWrite(13, LOW);
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