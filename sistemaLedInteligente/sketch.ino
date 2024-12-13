#include <Arduino.h>

// Classe para lidar com sensores e atuadores
class SistemaIluminacaoInteligente {
  private:
    int *pinoLDR, *pinoTrig, *pinoEcho, *pinoRele, *pinoBotao, *pinoLED;
    const int *limiarLDR;
    const int *limiarDistancia;
    bool sistemaAtivo;
    unsigned long ultimaAtualizacao;

  public:
    // Construtor
    SistemaIluminacaoInteligente(int *ldr, int *trig, int *echo, int *rele, int *botao, int *led, const int *limiarLDR, const int *limiarDistancia) :
      pinoLDR(ldr), pinoTrig(trig), pinoEcho(echo), pinoRele(rele), pinoBotao(botao), pinoLED(led),
      limiarLDR(limiarLDR), limiarDistancia(limiarDistancia), sistemaAtivo(true), ultimaAtualizacao(0) {}

    // Configuração inicial dos pinos
    void iniciar() {
      pinMode(*pinoLDR, INPUT);
      pinMode(*pinoTrig, OUTPUT);
      pinMode(*pinoEcho, INPUT);
      pinMode(*pinoRele, OUTPUT);
      pinMode(*pinoBotao, INPUT_PULLUP);
      pinMode(*pinoLED, OUTPUT);
      Serial.begin(115200);
      Serial.println("Sistema iniciado com POO e Ponteiros!");
    }

    // Função para calcular distância com o sensor ultrassônico
    long obterDistancia() {
      digitalWrite(*pinoTrig, LOW);
      delayMicroseconds(2);
      digitalWrite(*pinoTrig, HIGH);
      delayMicroseconds(10);
      digitalWrite(*pinoTrig, LOW);
      long duracao = pulseIn(*pinoEcho, HIGH);
      return duracao * 0.034 / 2; // Converte para cm
    }

    // Verifica se o botão foi pressionado
    void verificarBotao() {
      if (digitalRead(*pinoBotao) == LOW) {
        sistemaAtivo = false;
        digitalWrite(*pinoRele, LOW);
        digitalWrite(*pinoLED, LOW);
        Serial.println("Sistema desativado manualmente!");
        delay(500); // Evitar múltiplas leituras rápidas
      }
    }

    // Atualiza o estado do sistema com base no LDR e na distância
    void atualizar() {
      unsigned long tempoAtual = millis();
      if (tempoAtual - ultimaAtualizacao >= 500) { // Atualiza a cada 500 ms
        ultimaAtualizacao = tempoAtual;

        if (sistemaAtivo) {
          int valorLDR = analogRead(*pinoLDR);
          long distancia = obterDistancia();

          Serial.print("Valor do LDR: ");
          Serial.println(valorLDR);
          Serial.print("Distância: ");
          Serial.print(distancia);
          Serial.println(" cm");

          if (valorLDR < *limiarLDR && distancia < *limiarDistancia) {
            digitalWrite(*pinoLED, HIGH);
            digitalWrite(*pinoRele, HIGH);
            Serial.println("LEDs e Relé ativados!");
          } else {
            digitalWrite(*pinoLED, LOW);
            digitalWrite(*pinoRele, LOW);
            Serial.println("LEDs e Relé desativados!");
          }
        }
      }
    }
};

// Definição dos pinos e limites como variáveis globais
int pinoLDR = 34, pinoTrig = 5, pinoEcho = 18, pinoRele = 4, pinoBotao = 15, pinoLED = 2;
const int LIMIAR_LDR = 800; // Limite para luz ambiente
const int LIMIAR_DISTANCIA = 20; // Limite de distância em cm

// Instanciação do sistema de iluminação inteligente com ponteiros
SistemaIluminacaoInteligente sistemaInteligente(&pinoLDR, &pinoTrig, &pinoEcho, &pinoRele, &pinoBotao, &pinoLED, &LIMIAR_LDR, &LIMIAR_DISTANCIA);

void setup() {
  sistemaInteligente.iniciar();
}

void loop() {
  sistemaInteligente.verificarBotao();
  sistemaInteligente.atualizar();
}
