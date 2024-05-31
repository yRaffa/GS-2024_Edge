// #include <Wire.h>
// #include <LiquidCrystal_I2C.h>

int ledRed = 2;
int ledYellow = 3;
int ledGreen = 4;

int buzzer = 11;

int echo = 12;
int trigger = 13;
float dist = 0;

void setup(){
    Serial.begin(9600);

    pinMode(ledRed, OUTPUT);
    pinMode(ledYellow, OUTPUT);
    pinMode(ledGreen, OUTPUT);

    pinMode();

    pinMode(echo, INPUT);
    pinMode(trigger, OUTPUT);
}

void loop(){

}