// Definições de pinos
#define RELAY_PIN 16   // Pino conectado ao relé
#define LED_PIN 2     // Pino conectado ao LED (no ESP32, geralmente o pino 2 é o LED onboard)
#define BUTTON_PIN 4  // Pino conectado ao botão

// Classe para controlar o sistema de relé e LED
class RelayLedController {
private:
    int relayPin;
    int ledPin;
    int buttonPin;
    bool relayState;
    unsigned long lastDebounceTime;
    unsigned long debounceDelay;

public:
    RelayLedController(int rPin, int lPin, int bPin, unsigned long dDelay = 200) {
        relayPin = rPin;
        ledPin = lPin;
        buttonPin = bPin;
        relayState = false;
        lastDebounceTime = 0;
        debounceDelay = dDelay;
    }

    void begin() {
        pinMode(relayPin, OUTPUT);
        pinMode(ledPin, OUTPUT);
        pinMode(buttonPin, INPUT_PULLUP);

        digitalWrite(relayPin, LOW); // Relé desligado inicialmente
        digitalWrite(ledPin, LOW);   // LED desligado inicialmente
    }

    void update() {
        unsigned long currentTime = millis();

        if (digitalRead(buttonPin) == LOW) { // Botão pressionado
            if (currentTime - lastDebounceTime > debounceDelay) {
                toggleRelay();
                lastDebounceTime = currentTime;
            }
        }
    }

    void toggleRelay() {
        relayState = !relayState; // Alterna o estado do relé

        if (relayState) {
            digitalWrite(relayPin, HIGH); // Liga o relé
            digitalWrite(ledPin, HIGH);   // Liga o LED
            Serial.println("Relé e LED LIGADOS");
        } else {
            digitalWrite(relayPin, LOW);  // Desliga o relé
            digitalWrite(ledPin, LOW);    // Desliga o LED
            Serial.println("Relé e LED DESLIGADOS");
        }
    }
};

// Instancia o controlador
RelayLedController relayLedController(RELAY_PIN, LED_PIN, BUTTON_PIN);

void setup() {
    Serial.begin(115200);
    relayLedController.begin();
}

void loop() {
    relayLedController.update();
}