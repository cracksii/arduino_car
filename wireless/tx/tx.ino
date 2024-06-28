#include "TinyRF_TX.h"

int packetId;

void setup(){
    Serial.begin(115200);
    setupTransmitter();
    pinMode(13, OUTPUT);
    packetId = 0;
}


void txMsg(String id, String msg="")
{
    msg = String(packetId) + "|" + id + "|" + msg;
    const char* c_msg = msg.c_str();
    // Serial.println(msg);
    sendMulti((byte*)c_msg, strlen(c_msg), 1);
    packetId++;
}

void loop(){
    if(Serial.available() > 0)
    {
        String s =  Serial.readStringUntil('\n');
        String id = s.substring(0, 1);
        String msg = s.substring(1);
    

        if(id == "f")
        {
            digitalWrite(13, HIGH);
            txMsg("f", msg);
        }
        else if(id == "b")
        {
            digitalWrite(13, LOW);
            txMsg("b", msg);
        }
        else if(id == "s")
        {
            digitalWrite(13, LOW);
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
