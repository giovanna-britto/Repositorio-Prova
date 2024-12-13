long tempoAnterior = 0; 
long tempo = 1000;      

#define RELAY_PIN 16 

void setup() {
  pinMode(RELAY_PIN, OUTPUT); 
}

void loop() {
  if (millis() >= tempoAnterior + tempo) {
    tempoAnterior = millis(); 
    digitalWrite(RELAY_PIN, !digitalRead(RELAY_PIN));
  }
}
