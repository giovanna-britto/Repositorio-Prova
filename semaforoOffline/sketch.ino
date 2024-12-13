#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Classe Semaforo
class Semaforo {
private:
  int *pinoVermelho, *pinoAmarelo, *pinoVerde; // Ponteiros para os pinos dos LEDs
  LiquidCrystal_I2C *lcd;                      // Ponteiro para o objeto LCD

public:
  // Construtor da classe
  Semaforo(int *vermelho, int *amarelo, int *verde, LiquidCrystal_I2C *lcdDisplay) 
    : pinoVermelho(vermelho), pinoAmarelo(amarelo), pinoVerde(verde), lcd(lcdDisplay) {}

  // Inicialização dos LEDs e do LCD
  void iniciar() {
    pinMode(*pinoVermelho, OUTPUT);
    pinMode(*pinoAmarelo, OUTPUT);
    pinMode(*pinoVerde, OUTPUT);

    lcd->init(); // Inicializa o LCD
    lcd->backlight(); // Liga a luz de fundo do LCD
    lcd->setCursor(0, 0);
    lcd->print("Semaforo Online");
    delay(2000);
  }

  // Método para ativar a fase vermelha
  void faseVermelho(int tempo) {
    ligar(*pinoVermelho);
    desligar(*pinoAmarelo);
    desligar(*pinoVerde);
    mostrarMensagem("Parado", "Aguarde...");
    delay(tempo);
  }

  // Método para ativar a fase amarela
  void faseAmarelo(int tempo, const char *mensagem) {
    desligar(*pinoVermelho);
    ligar(*pinoAmarelo);
    desligar(*pinoVerde);
    mostrarMensagem("Atenção", mensagem);
    delay(tempo);
  }

  // Método para ativar a fase verde
  void faseVerde(int tempo, const char *mensagem) {
    desligar(*pinoVermelho);
    desligar(*pinoAmarelo);
    ligar(*pinoVerde);
    mostrarMensagem("Avance", mensagem);
    delay(tempo);
  }

private:
  // Liga o LED
  void ligar(int pino) {
    digitalWrite(pino, HIGH);
  }

  // Desliga o LED
  void desligar(int pino) {
    digitalWrite(pino, LOW);
  }

  // Mostra mensagem no LCD
  void mostrarMensagem(const char *linha1, const char *linha2) {
    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->print(linha1);
    lcd->setCursor(0, 1);
    lcd->print(linha2);
  }
};

// Pinos dos LEDs
int pinoVermelho = 26;
int pinoAmarelo = 27;
int pinoVerde = 14;

// Endereço I2C do LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Instanciação do semáforo com ponteiros para os pinos e LCD
Semaforo semaforo(&pinoVermelho, &pinoAmarelo, &pinoVerde, &lcd);

void setup() {
  // Inicializa o semáforo
  semaforo.iniciar();
}

void loop() {
  // Ciclo do semáforo
  semaforo.faseVermelho(6000);       // 6 segundos no vermelho
  semaforo.faseAmarelo(2000, "Prepare-se"); // 2 segundos no amarelo
  semaforo.faseVerde(2000, "2s restantes"); // 2 segundos no verde
  semaforo.faseVerde(2000, "Pedestres 2s"); // 2 segundos adicionais no verde
  semaforo.faseAmarelo(2000, "Fim da travessia"); // 2 segundos no amarelo
}
