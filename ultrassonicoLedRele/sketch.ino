// Definição de pinos para ESP32
#define RELAY_PIN 14   // Pino conectado ao relé
#define LED_PIN 2      // Pino conectado ao LED
#define BUTTON_PIN 4   // Pino conectado ao botão
#define LDR_PIN 34     // Pino conectado ao LDR (pinos ADC no ESP32)
#define TRIG_PIN 5     // Pino TRIG do sensor ultrassônico
#define ECHO_PIN 18    // Pino ECHO do sensor ultrassônico

class RelayLedController {
private:
    int relayPin;
    int ledPin;
    int buttonPin;
    int ldrPin;
    int trigPin;
    int echoPin;
    bool relayState;
    unsigned long lastDebounceTime;
    unsigned long debounceDelay;
    unsigned long lastButtonPressTime; // Armazena o tempo da última interação do botão
    unsigned long sensorControlDelay; // Tempo para reativar controle dos sensores

public:
    RelayLedController(int rPin, int lPin, int bPin, int ldr, int trig, int echo, unsigned long dDelay = 200, unsigned long sDelay = 5000) {
        relayPin = rPin;
        ledPin = lPin;
        buttonPin = bPin;
        ldrPin = ldr;
        trigPin = trig;
        echoPin = echo;
        relayState = false;
        lastDebounceTime = 0;
        lastButtonPressTime = 0;
        debounceDelay = dDelay;
        sensorControlDelay = sDelay; // 5 segundos de prioridade do botão
    }

    void begin() {
        pinMode(relayPin, OUTPUT);
        pinMode(ledPin, OUTPUT);
        pinMode(buttonPin, INPUT_PULLUP);
        pinMode(trigPin, OUTPUT);
        pinMode(echoPin, INPUT);

        digitalWrite(relayPin, LOW); // Relé desligado inicialmente
        digitalWrite(ledPin, LOW);   // LED desligado inicialmente
    }

    void update() {
        unsigned long currentTime = millis();
        int buttonState = digitalRead(buttonPin);

        // Controle do botão
        if (buttonState == LOW) { // Botão pressionado
            if (currentTime - lastDebounceTime > debounceDelay) {
                toggleRelay();
                lastDebounceTime = currentTime;
                lastButtonPressTime = currentTime; // Atualiza o tempo da última interação do botão
            }
        }

        // Controle pelos sensores, somente se o botão não foi usado recentemente
        if (currentTime - lastButtonPressTime > sensorControlDelay) {
            int ldrValue = analogRead(ldrPin);
            Serial.print("LDR Value: ");
            Serial.println(ldrValue);

            float distance = measureDistance();
            Serial.print("Distance: ");
            Serial.print(distance);
            Serial.println(" cm");

            // Condição para controle pelos sensores
            if (ldrValue < 500 && distance < 50) {
                digitalWrite(relayPin, HIGH); // Liga o relé
                digitalWrite(ledPin, HIGH);   // Liga o LED
                Serial.println("Relé e LED LIGADOS devido aos sensores");
            } else {
                digitalWrite(relayPin, LOW);  // Desliga o relé
                digitalWrite(ledPin, LOW);    // Desliga o LED
                Serial.println("Relé e LED DESLIGADOS devido aos sensores");
            }
        }
    }

    void toggleRelay() {
        relayState = !relayState; // Alterna o estado do relé

        if (relayState) {
            digitalWrite(relayPin, HIGH); // Liga o relé
            digitalWrite(ledPin, HIGH);   // Liga o LED
            Serial.println("Relé e LED LIGADOS pelo botão");
        } else {
            digitalWrite(relayPin, LOW);  // Desliga o relé
            digitalWrite(ledPin, LOW);    // Desliga o LED
            Serial.println("Relé e LED DESLIGADOS pelo botão");
        }
    }

    float measureDistance() {
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        long duration = pulseIn(echoPin, HIGH);
        float distance = (duration * 0.034) / 2; // Calcula a distância em cm
        return distance;
    }
};

// Instancia a classe com os pinos do ESP32
RelayLedController relayLedController(RELAY_PIN, LED_PIN, BUTTON_PIN, LDR_PIN, TRIG_PIN, ECHO_PIN);

void setup() {
    Serial.begin(115200);
    relayLedController.begin();
}

void loop() {
    relayLedController.update();
}
