#include <Arduino.h>

class LEDControl {
  private:
    int pin;
  public:
    LEDControl(int p) : pin(p) {
      pinMode(pin, OUTPUT);
    }
    void turnOn() {
      digitalWrite(pin, HIGH);
    }
    void turnOff() {
      digitalWrite(pin, LOW);
    }
    void toggle(bool state) {
      digitalWrite(pin, state ? HIGH : LOW);
    }
};

LEDControl* led;

void setup() {
  led = new LEDControl(2); // LED no GPIO 2
}

void loop() {
  led->turnOn();
  delay(1000);
  led->turnOff();
  delay(1000);
}
