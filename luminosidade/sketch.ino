// Definições de pinos
#define RELAY_PIN 14   // Pino conectado ao relé
#define LED_PIN 2      // Pino conectado ao LED (pode ser o LED onboard)
#define LDR_PIN 34     // Pino ADC conectado ao LDR

// Limite para detecção de baixa luminosidade
#define LDR_THRESHOLD 800

void setup() {
    // Configura os pinos
    pinMode(RELAY_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);

    // Inicializa os estados
    digitalWrite(RELAY_PIN, LOW); // Relé desligado inicialmente
    digitalWrite(LED_PIN, LOW);   // LED desligado inicialmente

    Serial.begin(115200);
}

void loop() {
    int ldrValue = analogRead(LDR_PIN); // Lê o valor do LDR
    Serial.print("LDR Value: ");
    Serial.println(ldrValue);

    if (ldrValue < LDR_THRESHOLD) { // Detecta baixa luminosidade
        digitalWrite(RELAY_PIN, LOW); // Liga o relé
        digitalWrite(LED_PIN, LOW);   // Liga o LED
        Serial.println("Baixa luminosidade detectada: LED e relé LIGADOS");
    } else {
        digitalWrite(RELAY_PIN, HIGH);  // Desliga o relé
        digitalWrite(LED_PIN, HIGH);    // Desliga o LED
        Serial.println("Luminosidade adequada: LED e relé DESLIGADOS");
    }

    delay(500); // Aguarda 500ms antes de ler novamente
}
