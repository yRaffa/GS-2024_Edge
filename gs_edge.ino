// Inclusão de bibliotecas
#include <EEPROM.h>
#include <LiquidCrystal.h>

// Declaração de variaves das portas de cada componente
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

int ledRed = 8;
int ledYellow = 9;
int ledGreen = 10;

int buzzer = 11;

int echo = 12;
int trigger = 13;

// Declaração de variaveis
int porcentagem = 0;
float cm = 0;

// Função para apagar linha do lcd
void apagarLinha(int linha) {
  lcd.setCursor(0, linha);
  for(int i = 0; i < 16; i++)
    lcd.print(" ");
  lcd.setCursor(0, linha);
}

// Iniciando do arduido
void setup() {

  // Iniciando o serial
  Serial.begin(9600);

  // Declaração dos pinModes de cada componente
  pinMode(ledRed, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledGreen, OUTPUT);

  pinMode(buzzer, OUTPUT);

  pinMode(echo, INPUT);
  pinMode(trigger, OUTPUT);

  // Iniciando lcd
  lcd.begin(16, 2);

  // Printando informacao no lcd
  lcd.setCursor(0, 0);
  lcd.print("Tanque:");
}

// Loop do arduino
void loop() {

  // Iniciando o sensor de distancia ultrasonico
  digitalWrite(trigger, LOW);
  delayMicroseconds(5);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);

  // Captando dados do sensor de distancia ultrasonico
  cm = pulseIn(echo, HIGH);

  // Calculando e tranformando unidades de medida
  cm = (cm / 58);
  porcentagem = (100 * cm) / 403.50; //337
  porcentagem = (porcentagem - 100);

  if(porcentagem < 0)
    porcentagem = (porcentagem * -1);

  // Printando a porcentagem no lcd 
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

  // Printando distancia e porcentagem no serial
  Serial.print("Distancia: ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.print(" || ");
  Serial.print("Porcentagem: ");
  Serial.print(porcentagem);
  Serial.print("%");
  Serial.print("\n");
  delay(1000);

  // Bloco de comando para 'Tanque Cheio'
  if(porcentagem > 99) {

    // Apagando a linha e printando 'Tanque Cheio'
    apagarLinha(1);
    lcd.setCursor(0, 1);
    lcd.print("Tanque Cheio");

    // Gerenciamento dos leds
    digitalWrite(ledRed, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledGreen, HIGH);
  }
  
  // Bloco de comando para 'Quase Cheio'
  else if(porcentagem > 75) {

    // Apagando a linha e printando 'Quase Cheio'
    apagarLinha(1);
    lcd.setCursor(0, 1);
    lcd.print("Quase Cheio");

    // Gerenciamento dos leds
    digitalWrite(ledRed, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledGreen, HIGH);
    delay(2000);
    digitalWrite(ledGreen, LOW);
  }

  // Bloco de comando para 'Combustivel OK'
  else if(porcentagem > 25) {

    // Apagando a linha e printando 'Combustivel OK'
    apagarLinha(1);
    lcd.setCursor(0, 1);
    lcd.print("Combustivel OK");

    // Gerenciamento dos leds
    digitalWrite(ledRed, LOW);
    digitalWrite(ledYellow, HIGH);
    digitalWrite(ledGreen, LOW);
  }

  // Bloco de comando para 'Quase Vazio'
  else if(porcentagem > 1) {

    // Apagando a linha e printando 'Quase Vazio'
    apagarLinha(1);
    lcd.setCursor(0, 1);
    lcd.print("Quase Vazio");

    // Gerenciamento dos leds e buzzer
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledGreen, LOW);
    tone(buzzer, 260);
    delay(2000);
    noTone(buzzer);
    digitalWrite(ledRed, LOW);
    delay(1000);
  } 

  // Bloco de comando para 'Tanque Vazio'
  else {

    // Apagando a linha e printando 'Tanque Vazio'
    apagarLinha(1);
    lcd.setCursor(0, 1);
    lcd.print("Tanque Vazio");

    // Gerenciamento dos leds
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledGreen, LOW);
  }
}