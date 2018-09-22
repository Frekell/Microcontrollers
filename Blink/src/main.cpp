#include <Arduino.h>

void setup() {
    pinMode(12, OUTPUT);
    pinMode(14, OUTPUT);
    pinMode(16, OUTPUT);
    // put your setup code here, to run once:
}

void loop() {
    delay(100);
        digitalWrite(12, HIGH); 
        digitalWrite(14, HIGH);
        digitalWrite(16, HIGH);
        delay(100);
        digitalWrite(12, LOW);
        digitalWrite(14, LOW);
        digitalWrite(16, LOW);
}