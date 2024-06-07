# 🌎 GS Inovação Azul

[Projeto no Wokwi](https://wokwi.com/projects/399769764705866753)

## 🤖 Edge Computing & Computer Systems

Entrega da matéria Edge Computing & Computer Systems para GS 2024.

## 👥 Integrantes

- RM: 556506 // Nicolas Caciolato Reis
- RM: 554736 // Rafael Federici de Oliveira

## 📕 Sobre o Projeto

<img src="circuito.png"></img>

Este projeto consiste em um sistema de monitoramento de informações sobre combutíveis limpos utilizados em embarcações marinhas, com o intuito de colher dados como: capacidade, pH e temperatura, para minimizar os gastos de combustíveis e emissões de poluentes nos mares e atmosfera.

## 🔨 Ferramentas

- [Arduino UNO](https://docs.arduino.cc/hardware/uno-rev3/)
- [Linguagem Arduino](https://www.arduino.cc/reference/pt/)
- [C++](https://learn.microsoft.com/pt-br/cpp/?view=msvc-170)

## 🖥️ Requisitos e Componentes

Simulador: [Wokwi](https://wokwi.com/)

Componentes Utilizados:

- [Arduino UNO:](https://docs.wokwi.com/pt-BR/parts/wokwi-arduino-uno) Controlar os demais componentes.
- [LCD_I2C:](https://docs.wokwi.com/pt-BR/parts/wokwi-lcd2004) Mostrar visualmente as informações.
- [Sensor de Distância Ultrassônico HC-SR04:](https://docs.wokwi.com/pt-BR/parts/wokwi-hc-sr04) Medir a capacidade do tanque.
- [Potenciômetro:](https://docs.wokwi.com/pt-BR/parts/wokwi-potentiometer) Simular medição do pH.
- [Sensor de Temperatura NTC:](https://docs.wokwi.com/pt-BR/parts/wokwi-ntc-temperature-sensor) Medir a temperatura do combustível.
- [LEDs:](https://docs.wokwi.com/pt-BR/parts/wokwi-led) Feedback da situação do sistema.
- [Buzzer:](https://docs.wokwi.com/pt-BR/parts/wokwi-buzzer) Feedback auditivo para situações críticas.
- [Servo Motor:](https://docs.wokwi.com/pt-BR/parts/wokwi-servo) Comporta para reabastecimento.

Incluir as seguintes bibliotecas no projeto:

``` c++
#include <Wire.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
```

## 📒 Instruções de Uso

- Abrir o [projeto no Wokwi](https://wokwi.com/projects/399769764705866753)
- Iniciar a simulação no botão 'Start the simulation'
- Alterar os valores de capacidade, pH e temperatura, respectivamente nos componentes: 

    - Sensor de Distância Ultrassônico HC-SR04
    - Potenciômetro
    - Sensor de Temperatura NTC

- Observar os dados apresentados no LCD_I2C

## 🧠 Explicando o Código

- Declarando variáveis para cada compenente utilizado
``` c++
LiquidCrystal_I2C lcd(0x27, 20, 4);

Servo servoMotor;
int servoPin = A1;

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
```

- Declarando variáveis do sistema.
``` c++
int temp = 0;
int porcentagem = 0;
float cm = 0;
float ph = 0;
float celsius = 0;

const float BETA = 3950;
```

- Função para apagar uma linha do LCD.
``` c++
void apagarLinha(int coluna, int linha) {
  lcd.setCursor(coluna, linha);
  for(int i = coluna; i < (20 - coluna); i++)
    lcd.print(" ");
  lcd.setCursor(coluna, linha);
}
```

- void setup() do arduino:

Inicializa e define os pinModes dos componentes.
``` c++
void setup() {
  Serial.begin(9600);

  pinMode(temperatura, INPUT);

  pinMode(potenciometro, INPUT);

  pinMode(ledRedTanque, OUTPUT);
  pinMode(ledYellowTanque, OUTPUT);
  pinMode(ledGreenTanque, OUTPUT);

  pinMode(buzzer, OUTPUT);

  pinMode(echo, INPUT);
  pinMode(trigger, OUTPUT);

  lcd.init();
  lcd.backlight();

  servoMotor.attach(servoPin);
}
```

- void loop() do arduino:

Inicia a captação de dados dos componentes
``` c++
void loop() {
  digitalWrite(trigger, LOW);
  delayMicroseconds(5);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);

  cm = pulseIn(echo, HIGH);

  cm = (cm / 58);
  porcentagem = (100 * cm) / 403.50;
  porcentagem = (porcentagem - 100);
  porcentagem = (porcentagem * -1);

  ph = map(analogRead(potenciometro), 0, 1023, 0.0, 14.0);

  temp = analogRead(temperatura);

  celsius = 1 / (log(1 / (1023. / temp - 1)) / BETA + 1.0 / 298.15) - 273.15;
```

- Exibe no LCD as informações sobre a capacidade.
``` c++
  apagarLinha(0, 0);
  lcd.setCursor(0, 0);
  lcd.print("Capacidade:");
  lcd.setCursor(11, 0);
  lcd.print(porcentagem);
  lcd.print("%");
    
  if(porcentagem > 99) {
    apagarLinha(0, 1);
    lcd.setCursor(0, 1);
    lcd.print("Tanque Cheio");

    digitalWrite(ledRedTanque, LOW);
    digitalWrite(ledYellowTanque, LOW);
    digitalWrite(ledGreenTanque, HIGH);

  } else if(porcentagem > 75) {
    apagarLinha(0, 1);
    lcd.setCursor(0, 1);
    lcd.print("Quase Cheio");

    digitalWrite(ledRedTanque, LOW);
    digitalWrite(ledYellowTanque, LOW);
    digitalWrite(ledGreenTanque, HIGH);
    delay(2000);
    digitalWrite(ledGreenTanque, LOW);

  } else if(porcentagem > 25) {
    apagarLinha(0, 1);
    lcd.setCursor(0, 1);
    lcd.print("Combustivel Ok");

    digitalWrite(ledRedTanque, LOW);
    digitalWrite(ledYellowTanque, HIGH);
    digitalWrite(ledGreenTanque, LOW);

  } else if(porcentagem > 1) {
    apagarLinha(0, 1);
    lcd.setCursor(0, 1);
    lcd.print("Quase Vazio");

    digitalWrite(ledRedTanque, HIGH);
    digitalWrite(ledYellowTanque, LOW);
    digitalWrite(ledGreenTanque, LOW);
    tone(buzzer, 260);
    delay(2000);
    noTone(buzzer);
    digitalWrite(ledRedTanque, LOW);
    delay(1000);

  } else {
    apagarLinha(0, 1);
    lcd.setCursor(0, 1);
    lcd.print("Tanque Vazio");

    digitalWrite(ledRedTanque, HIGH);
    digitalWrite(ledYellowTanque, LOW);
    digitalWrite(ledGreenTanque, LOW);
  }
```

- Exibe no LCD as informações sobre o pH.
``` c++
  apagarLinha(0, 2);
  lcd.setCursor(0, 2);
  lcd.print("Ph:");
  lcd.setCursor(3, 2);
  lcd.print(ph, 0);

  if(ph >= 6 && ph <= 8) {
    apagarLinha(0, 3);
    lcd.setCursor(0, 3);
    lcd.print("Ph Ok");

    digitalWrite(ledRedPh, LOW);
    digitalWrite(ledGreenPh, HIGH);

  } else if(ph < 6) {
    apagarLinha(0, 3);
    lcd.setCursor(0, 3);
    lcd.print("Ph Baixo");

    digitalWrite(ledRedPh, HIGH);
    digitalWrite(ledGreenPh, LOW);

  } else if(ph > 8) {
    apagarLinha(0, 3);
    lcd.setCursor(0, 3);
    lcd.print("Ph Alto");

    digitalWrite(ledRedPh, HIGH);
    digitalWrite(ledGreenPh, LOW);
  }
```


- Exibe no LCD as informações sobre a temperatura.
``` c++
  apagarLinha(10, 2);
  lcd.setCursor(10, 2);
  lcd.print("Temp:");
  lcd.setCursor(15, 2);
  lcd.print(celsius, 0);
  lcd.print("C");
  
  if(celsius >= 20 && celsius <= 60) {
    apagarLinha(10, 3);
    lcd.setCursor(10, 3);
    lcd.print("Temp Ok");

    digitalWrite(ledRedTemp, LOW);
    digitalWrite(ledGreenTemp, HIGH);

  } else if(celsius < 20) {
    apagarLinha(10, 3);
    lcd.setCursor(10, 3);
    lcd.print("Temp Baixa");

    digitalWrite(ledRedTemp, HIGH);
    digitalWrite(ledGreenTemp, LOW);

  } else if(celsius > 60) {
    apagarLinha(10, 3);
    lcd.setCursor(10, 3);
    lcd.print("Temp Alta");

    digitalWrite(ledRedTemp, HIGH);
    digitalWrite(ledGreenTemp, LOW);
  }
```

- Ligando um alerta com buzzer para as situações críticas.
``` c++
  if(porcentagem <= 1 || (ph < 6 || ph > 8) || (celsius < 20 || celsius > 60)) {
    tone(buzzer, 260);
    delay(2000);
    noTone(buzzer);
  }
```

- Simulando uma comporta para reabastecimento com ServoMotor.
``` c++
  if(porcentagem <= 1) {
    servoMotor.write(180);
  	delay(2000);

  } else if(porcentagem > 99) {
    servoMotor.write(90);
  	delay(2000);
  }
```

- Exibindo informações no Serial, para controle.
``` c++
  Serial.print("Distancia: ");
  Serial.print(cm, 0);
  Serial.print("cm");
  Serial.print(" || ");
  Serial.print("Capacidade: ");
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
```