#define NMOS_1 10
#define NMOS_2 11

#define PMOS_1 8
#define PMOS_2 9

#define A 5
#define B 6

#define LAMP 13


void setup() 
{
    Serial.begin(115200);
    pinMode(NMOS_1, OUTPUT);
    pinMode(NMOS_2, OUTPUT);
    pinMode(PMOS_1, OUTPUT);
    pinMode(PMOS_2, OUTPUT);
    pinMode(A, OUTPUT);
    pinMode(B, OUTPUT);
    pinMode(LAMP, OUTPUT);

    reset();
}

void loop() 
{
    if(Serial.available() > 0)
    {
        String s =  Serial.readStringUntil('\n');
        String id = s.substring(0, 1);
        String msg = s.substring(1);
        
        if(id == "f")
        {
            digitalWrite(13, HIGH);
            int speed = msg.toInt();
            digitalWrite(PMOS_1, LOW);
            digitalWrite(PMOS_2, HIGH);

            analogWrite(NMOS_1, speed);
            digitalWrite(NMOS_2, LOW);
        }
        else if(id == "b")
        {
            digitalWrite(13, LOW);
            int speed = msg.toInt();
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
            analogWrite(A, msg.toInt());
            digitalWrite(B, LOW);
        }
        else if(id == "l")
        {
            analogWrite(B, msg.toInt());
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
