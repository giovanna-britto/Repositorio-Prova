#include <Arduino.h>

// Definição dos pinos
#define BUTTON_PIN 15
#define RELAY_PIN 4
#define LED_PIN 2

// Variáveis para debounce
unsigned long lastDebounceTime = 0; // Último tempo de mudança
unsigned long debounceDelay = 50;  // Delay para debounce (50 ms)
bool buttonState = false;          // Estado atual do botão
bool lastButtonState = false;      // Estado anterior do botão

void setup() {
  // Configuração dos pinos
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Ativa o resistor de pull-up interno
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  // Inicialização do Serial Monitor
  Serial.begin(115200);
  Serial.println("Sistema de Automação Iniciado!");
}

void loop() {
  // Leitura do estado do botão
  bool reading = digitalRead(BUTTON_PIN);

  // Verifica se o botão mudou de estado (para debounce)
  if (reading != lastButtonState) {
    lastDebounceTime = millis(); // Atualiza o tempo de debounce
  }

  // Verifica se o tempo de debounce passou
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Se o estado do botão mudou
    if (reading != buttonState) {
      buttonState = reading;

      // Verifica se o botão está pressionado
      if (buttonState == LOW) {
        digitalWrite(RELAY_PIN, HIGH); // Ativa o relé
        digitalWrite(LED_PIN, HIGH);  // Acende o LED
        Serial.println("Relé e LED ativados!");
      } else {
        digitalWrite(RELAY_PIN, LOW); // Desativa o relé
        digitalWrite(LED_PIN, LOW);  // Apaga o LED
        Serial.println("Relé e LED desativados!");
      }
    }
  }

  // Salva o estado atual do botão como o estado anterior
  lastButtonState = reading;
}
