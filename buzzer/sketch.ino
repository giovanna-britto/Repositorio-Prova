// Exemplo simples de funcionamento de um Buzzer com ESP32 usando millis

#define BUZZER_PIN 25 // Definir o pino conectado ao buzzer

unsigned long previousMillis = 0;
const long interval = 1000; // Intervalo de 1 segundo
bool buzzerState = false;

void setup() {
  pinMode(BUZZER_PIN, OUTPUT); // Configurar o pino do buzzer como saída
  Serial.begin(115200);
  Serial.println("Iniciando o exemplo do Buzzer com millis!");
}

void loop() {
  unsigned long currentMillis = millis();

  // Verifica se o intervalo de tempo passou
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // Atualiza o tempo anterior

    // Alterna o estado do buzzer
    buzzerState = !buzzerState;
    digitalWrite(BUZZER_PIN, buzzerState ? HIGH : LOW);

    // Exibe no monitor serial o estado do buzzer
    if (buzzerState) {
      Serial.println("Buzzer ligado!");
    } else {
      Serial.println("Buzzer desligado!");
    }
  }
}
