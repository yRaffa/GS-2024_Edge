#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

int ledRed = 8;
int ledYellow = 9;
int ledGreen = 10;

int buzzer = 11;

int echo = 12;
int trigger = 13;

float dist = 0;
float polegadas = 0;

int val = 0; 
int porcentagem = 0;

void setup() {
  Serial.begin(9600);

  pinMode(ledRed, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledGreen, OUTPUT);

  pinMode(buzzer, OUTPUT);

  pinMode(echo, INPUT);
  pinMode(trigger, OUTPUT);

  lcd.begin(16, 2);

  val = EEPROM.read(0);

  if(val > 150) {
    val = 150;
  }

  lcd.setCursor(0, 0);
  lcd.print("Combustivel:");
}

void loop() {
  digitalWrite(trigger, LOW);
  delayMicroseconds(5);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);

  dist = pulseIn(echo, HIGH);
  dist = dist / 58;
  polegadas = microsecondsToInches(dist);

  porcentagem = (val - polegadas) * 100 / val;

  lcd.setCursor(12, 0);
  lcd.print("Comustivel");

  Serial.print("Distancia : " + dist + " cm");
  delay(1000);

  if(dist <= 50) {
    digitalWrite(ledRed, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledGreen, HIGH);

    lcd.setCursor(0, 1);
    lcd.print("Tanque Cheio");
  }
}