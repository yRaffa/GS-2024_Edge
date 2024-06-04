// Incluindo bibliotecas
#include <Wire.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

// Declarando variaves das portas de cada componente
LiquidCrystal_I2C lcd(0x27, 20, 4);

Servo servoMotor;
int servoPin = A1;

const float BETA = 3950;
int temperatura = A2;

int potenciometro = A3;

int ledRedTemp = 4;
int ledGreenTemp = 5;

int ledRedPh = 6;
int ledGreenPh = 7;

int ledRedTanque = 8;
int ledYellowTanque = 9;
int ledGreenTanque = 10;

int buzzer = 11;

int echo = 12;
int trigger = 13;

// Declarando variaveis
int temp = 0;
int porcentagem = 0;
float cm = 0;
float ph = 0;
float celsius = 0;

// Função para apagar linha do lcd
void apagarLinha(int coluna, int linha) {
  lcd.setCursor(coluna, linha);
  for(int i = coluna; i < (20 - coluna); i++)
    lcd.print(" ");
  lcd.setCursor(coluna, linha);
}

// Iniciando o arduido
void setup() {

  // Iniciando o serial
  Serial.begin(9600);

  // Declarando pinModes de cada componente
  pinMode(temperatura, INPUT);

  pinMode(potenciometro, INPUT);

  pinMode(ledRedTanque, OUTPUT);
  pinMode(ledYellowTanque, OUTPUT);
  pinMode(ledGreenTanque, OUTPUT);

  pinMode(buzzer, OUTPUT);

  pinMode(echo, INPUT);
  pinMode(trigger, OUTPUT);

  // Iniciando lcd
  lcd.init();
  lcd.backlight();

  // Linkando objeto e pin do servo
  servoMotor.attach(servoPin);
}

// Loop do arduino
void loop() {

  // Iniciando o sensor de distancia ultrasonico
  digitalWrite(trigger, LOW);
  delayMicroseconds(5);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);

// --- Captacoes de dados --- //

  // Captando dados do sensor de distancia ultrasonico
  cm = pulseIn(echo, HIGH);

  // Calculando e tranformando unidades de medida
  cm = (cm / 58);
  porcentagem = (100 * cm) / 403.50;
  porcentagem = (porcentagem - 100);
  porcentagem = (porcentagem * -1);

  // Simulando ph no potenciometro
  ph = map(analogRead(potenciometro), 0, 1023, 0.0, 14.0);

  // Captando dados do sensor de temperatura
  temp = analogRead(temperatura);

  // Transformando valor para °C
  celsius = 1 / (log(1 / (1023. / temp - 1)) / BETA + 1.0 / 298.15) - 273.15;  

// --- Saidas (Tanque) --- //

  // Printando a porcentagem no lcd
  lcd.setCursor(0, 0);
  lcd.print("Tanque:");
  apagarLinha(7, 0);
  lcd.setCursor(7, 0);
  lcd.print(porcentagem);
  lcd.print("%");
    
  // Bloco de comando para estado 'Tanque Cheio'
  if(porcentagem > 99) {

    // Apagando a linha e printando 'Tanque Cheio'
    apagarLinha(0, 1);
    lcd.setCursor(0, 1);
    lcd.print("Tanque Cheio");

    // Gerenciando leds
    digitalWrite(ledRedTanque, LOW);
    digitalWrite(ledYellowTanque, LOW);
    digitalWrite(ledGreenTanque, HIGH);
  }
    
  // Bloco de comando para estado 'Quase Cheio'
  else if(porcentagem > 75) {

    // Apagando a linha e printando 'Quase Cheio'
    apagarLinha(0, 1);
    lcd.setCursor(0, 1);
    lcd.print("Quase Cheio");

    // Gerenciando leds
    digitalWrite(ledRedTanque, LOW);
    digitalWrite(ledYellowTanque, LOW);
    digitalWrite(ledGreenTanque, HIGH);
    delay(2000);
    digitalWrite(ledGreenTanque, LOW);
  }

  // Bloco de comando para estado 'Combustivel OK'
  else if(porcentagem > 25) {

    // Apagando a linha e printando 'Combustivel OK'
    apagarLinha(0, 1);
    lcd.setCursor(0, 1);
    lcd.print("Combustivel Ok");

    // Gerenciando leds
    digitalWrite(ledRedTanque, LOW);
    digitalWrite(ledYellowTanque, HIGH);
    digitalWrite(ledGreenTanque, LOW);
  }

  // Bloco de comando para estado 'Quase Vazio'
  else if(porcentagem > 1) {

    // Apagando a linha e printando 'Quase Vazio'
    apagarLinha(0, 1);
    lcd.setCursor(0, 1);
    lcd.print("Quase Vazio");

    // Gerenciando leds e buzzer
    digitalWrite(ledRedTanque, HIGH);
    digitalWrite(ledYellowTanque, LOW);
    digitalWrite(ledGreenTanque, LOW);
    tone(buzzer, 260);
    delay(2000);
    noTone(buzzer);
    digitalWrite(ledRedTanque, LOW);
    delay(1000);
  } 

  // Bloco de comando para estado 'Tanque Vazio'
  else {

    // Apagando a linha e printando 'Tanque Vazio'
    apagarLinha(0, 1);
    lcd.setCursor(0, 1);
    lcd.print("Tanque Vazio");

    // Gerenciando leds
    digitalWrite(ledRedTanque, HIGH);
    digitalWrite(ledYellowTanque, LOW);
    digitalWrite(ledGreenTanque, LOW);
  }

// --- Saidas (Ph) --- //

  // Printando Ph no lcd
  lcd.setCursor(0, 2);
  lcd.print("Ph:");
  apagarLinha(3, 2);
  lcd.setCursor(3, 2);
  lcd.print(ph, 0);

  // Bloco de comando para estado 'Ph Ok'
  if(ph >= 6 && ph <= 8) {

    // Apagando a linha e printando estado ph 'PH OK'
    apagarLinha(0, 3);
    lcd.setCursor(0, 3);
    lcd.print("Ph Ok");

    // Gerenciando leds
    digitalWrite(ledRedPh, LOW);
    digitalWrite(ledGreenPh, HIGH);
  } 
  
  // Bloco de comando para estado 'Ph Baixo'
  else if(ph < 6) {

    // Apagando a linha e printando estado ph 'PH BAIXO'
    apagarLinha(0, 3);
    lcd.setCursor(0, 3);
    lcd.print("Ph Baixo");

    // Gerenciando leds
    digitalWrite(ledRedPh, HIGH);
    digitalWrite(ledGreenPh, LOW);
  }

  // Bloco de comando para estado 'Ph Alto'
  else if(ph > 8) {

    // Apagando a linha e printando estado ph 'PH ALTO'
    apagarLinha(0, 3);
    lcd.setCursor(0, 3);
    lcd.print("Ph Alto");

    // Gerenciando leds
    digitalWrite(ledRedPh, HIGH);
    digitalWrite(ledGreenPh, LOW);
  }

// --- Saidas (Temperatura) --- //

  // Printando temperatura no lcd
  lcd.setCursor(10, 2);
  lcd.print("Temp:");
  apagarLinha(15, 2);
  lcd.setCursor(15, 2);
  lcd.print(celsius, 0);
  lcd.print("C");
  
  // VALORES FICTICIOS DE TEMPERATURA PARA TESTES COM SENSOR (SÓ VAI ATÉ 80°C)

  // Bloco de comando para estado 'Temp Ok'
  if(celsius >= 20 && celsius <= 60) {

    // Apagando a linha e printando estado de temperatura 'Temp Ok'
    apagarLinha(10, 3);
    lcd.setCursor(10, 3);
    lcd.print("Temp Ok");

    // Gerenciando leds
    digitalWrite(ledRedTemp, LOW);
    digitalWrite(ledGreenTemp, HIGH);
  }

  // Bloco de comando para estado 'Temp Baixa'
  else if(celsius < 20) {

    // Apagando a linha e printando estado de temperatura 'Temp Baixa'
    apagarLinha(10, 3);
    lcd.setCursor(10, 3);
    lcd.print("Temp Baixa");

    // Gerenciando leds
    digitalWrite(ledRedTemp, HIGH);
    digitalWrite(ledGreenTemp, LOW);
  }

  // Bloco de comando para estado 'Temp Alta'
  else if(celsius > 60) {

    // Apagando a linha e printando estado de temperatura 'Temp Alta'
    apagarLinha(10, 3);
    lcd.setCursor(10, 3);
    lcd.print("Temp Alta");

    // Gerenciando leds
    digitalWrite(ledRedTemp, HIGH);
    digitalWrite(ledGreenTemp, LOW);
  }

  // Ligando buzzer quando em situacao critica
  if(porcentagem <= 1 || (ph < 6 || ph > 8) || (celsius < 20 || celsius > 60)) {
    tone(buzzer, 260);
    delay(2000);
    noTone(buzzer);
  }

// --- ServoMotor --- //

  // Abrindo comporta para reabastecimento quando tanque vazio
  if(porcentagem <= 1) {
    servoMotor.write(180);
  	delay(2000);
  } 
  
  // Fechando comporta para reabastecimento quando tanque cheio
  else if(porcentagem > 99) {
    servoMotor.write(90);
  	delay(2000);
  }

// --- Serial --- //

  // Printando dados no serial
  Serial.print("Distancia: ");
  Serial.print(cm, 0);
  Serial.print("cm");
  Serial.print(" || ");
  Serial.print("Tanque: ");
  Serial.print(porcentagem);
  Serial.print("%");
  Serial.print(" || ");
  Serial.print("Ph: ");
  Serial.print(ph, 0);
  Serial.print(" || ");
  Serial.print("Temperatura: ");
  Serial.print(celsius, 0);
  Serial.print("°C");
  Serial.print("\n");
  delay(1000);
}