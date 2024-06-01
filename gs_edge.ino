#include <EEPROM.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

int ledRed = 8;
int ledYellow = 9;
int ledGreen = 10;

int buzzer = 11;

int echo = 12;
int trigger = 13;

int porcentagem = 0;
float cm = 0;

void clearLine(int line) {
  lcd.setCursor(0, line);
  for(int i = 0; i < 16; i++)
    lcd.print(" ");
  lcd.setCursor(0, line);
}

void setup() {
  Serial.begin(9600);

  pinMode(ledRed, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledGreen, OUTPUT);

  pinMode(buzzer, OUTPUT);

  pinMode(echo, INPUT);
  pinMode(trigger, OUTPUT);

  lcd.begin(16, 2);

  lcd.setCursor(0, 0);
  lcd.print("Tanque:");
}

void loop() {
  digitalWrite(trigger, LOW);
  delayMicroseconds(5);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);

  cm = pulseIn(echo, HIGH);
  cm = (cm / 58);

  porcentagem = (100 * cm) / 403.50; //337
  porcentagem = (porcentagem - 100);

  if(porcentagem < 0)
    porcentagem = (porcentagem * -1);

  String str_porcentagem = String(porcentagem);

  lcd.setCursor(7, 0);
  lcd.print(porcentagem);
  lcd.print("%");

  if(str_porcentagem.length() < 3) {
    lcd.setCursor(10, 0);
    lcd.print(' ');
  } 

  if(str_porcentagem.length() < 2) {
    lcd.setCursor(9, 0);
    lcd.print(' ');
    lcd.setCursor(10, 0);
    lcd.print(' ');
  }

  Serial.print("Distancia: ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.print(" || ");
  Serial.print("Porcentagem: ");
  Serial.print(porcentagem);
  Serial.print("%");
  Serial.print("\n");
  delay(1000);

  if(porcentagem > 99) {
    clearLine(1);
    lcd.setCursor(0, 1);
    lcd.print("Tanque Cheio");

    digitalWrite(ledRed, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledGreen, HIGH);
  }
  
  else if(porcentagem > 75) {
    clearLine(1);
    lcd.setCursor(0, 1);
    lcd.print("Quase Cheio");

    digitalWrite(ledRed, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledGreen, HIGH);
    delay(2000);
    digitalWrite(ledGreen, LOW);
  }

  else if(porcentagem > 25) {
    clearLine(1);
    lcd.setCursor(0, 1);
    lcd.print("Combustivel OK");

    digitalWrite(ledRed, LOW);
    digitalWrite(ledYellow, HIGH);
    digitalWrite(ledGreen, LOW);
  }

  else if(porcentagem > 1) {
    clearLine(1);
    lcd.setCursor(0, 1);
    lcd.print("Quase Vazio");

    digitalWrite(ledRed, HIGH);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledGreen, LOW);
    tone(buzzer, 260);
    delay(2000);
    noTone(buzzer);
    digitalWrite(ledRed, LOW);
    delay(1000);
  } 

  else {
    clearLine(1);
    lcd.setCursor(0, 1);
    lcd.print("Tanque Vazio");

    digitalWrite(ledRed, HIGH);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledGreen, LOW);
  }
}