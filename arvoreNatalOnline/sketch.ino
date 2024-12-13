#include <WiFi.h>
#include <PubSubClient.h>

// Pinos do ESP32
#define LDR_PIN 34
#define BLUE_LED_PIN 26
#define RED_LED_PIN 27
#define BUTTON_PIN 15
#define RELAY_PIN 14

// Configurações Wi-Fi e MQTT
const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com";
const char* mqtt_topic = "villa_lobos/tree_status";

WiFiClient espClient;
PubSubClient client(espClient);

// Variáveis globais
unsigned long lastLdrRead = 0;
const unsigned long ldrInterval = 1000; // 1 segundo
int ldrValue = 0;

// Classe para controle do botão e relé
class ButtonRelayControl {
private:
  int buttonPin;
  int relayPin;

public:
  ButtonRelayControl(int buttonPin, int relayPin) {
    this->buttonPin = buttonPin;
    this->relayPin = relayPin;
    pinMode(buttonPin, INPUT_PULLDOWN);
    pinMode(relayPin, OUTPUT);
    digitalWrite(relayPin, LOW);
  }

  void checkButton() {
    if (digitalRead(buttonPin) == HIGH) {
      digitalWrite(relayPin, HIGH);
      Serial.println("Relé ativado!");
    } else {
      digitalWrite(relayPin, LOW);
    }
  }
};

ButtonRelayControl buttonRelay(BUTTON_PIN, RELAY_PIN);

// Conecta ao Wi-Fi
void setupWiFi() {
  delay(10);
  Serial.println("Conectando ao WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi conectado!");
  Serial.println(WiFi.localIP());
}

// Conecta ao broker MQTT
void reconnect() {
  while (!client.connected()) {
    Serial.println("Conectando ao MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("Conectado ao MQTT!");
    } else {
      Serial.print("Falha ao conectar. Estado MQTT: ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void setup() {
  // Configuração inicial
  Serial.begin(115200);

  pinMode(LDR_PIN, INPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);

  setupWiFi();
  client.setServer(mqtt_server, 1883);

  Serial.println("Sistema iniciado!");
}

void loop() {
  // Conexão MQTT
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Leitura do LDR a cada 1 segundo
  unsigned long currentMillis = millis();
  if (currentMillis - lastLdrRead >= ldrInterval) {
    lastLdrRead = currentMillis;
    ldrValue = analogRead(LDR_PIN);
    Serial.print("LDR Value: ");
    Serial.println(ldrValue);

    // Controle dos LEDs baseado no valor do LDR
    if (ldrValue > 2048) { // 50% do range (4096 é o máximo para ESP32 ADC)
      digitalWrite(BLUE_LED_PIN, HIGH);
      digitalWrite(RED_LED_PIN, LOW);
      client.publish(mqtt_topic, "LED Azul ligado");
    } else {
      digitalWrite(BLUE_LED_PIN, LOW);
      digitalWrite(RED_LED_PIN, HIGH);
      client.publish(mqtt_topic, "LED Vermelho ligado");
    }

    // Publica o valor do LDR
    char ldrMessage[50];
    sprintf(ldrMessage, "LDR Value: %d", ldrValue);
    client.publish(mqtt_topic, ldrMessage);
  }

  // Verifica o botão e controla o relé
  buttonRelay.checkButton();
}
