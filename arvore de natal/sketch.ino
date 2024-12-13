#define led_vermelho_inf 9
#define led_verde 10
#define led_vermelho_sup 11
#define led_estrela 12
 
void setup()
{
  Serial.begin(9600);
  pinMode(led_vermelho_inf, OUTPUT);
  pinMode(led_verde, OUTPUT);
  pinMode(led_vermelho_sup, OUTPUT);
  pinMode(led_estrela, OUTPUT);
}
 
void loop()
{
  //Aciona os 3 conjuntos via pwm
  Serial.println("n1 - Executando rotina pwm sequencial");
  analogWrite(led_estrela, 254);
  liga_desliga_leds_pwm(led_vermelho_inf);
  liga_desliga_leds_pwm(led_vermelho_sup);
  liga_desliga_leds_pwm(led_verde);
 
  //Aciona os 3 conjuntos total alternadamente
  Serial.println("2 - Acendimento total alternado");
  analogWrite(led_vermelho_inf, 254);
  delay(1000);
  analogWrite(led_vermelho_sup, 254);
  delay(1000);
  analogWrite(led_verde, 254);
  delay(1000);
  digitalWrite(led_estrela, 254);
  delay(2000);
  //Desliga os 3 conjuntos total alternadamente
  analogWrite(led_vermelho_inf, 0);
  delay(1000);
  analogWrite(led_vermelho_sup, 0);
  delay(1000);
  analogWrite(led_verde, 0);
  delay(1000);
  digitalWrite(led_estrela, 0);
  delay(1000);
 
  //Rotina aleatoria
  Serial.println("3 - Executando rotina aleatoria");
  for (int x = 0; x <= 35; x++)
  {
    int valor = random(1, 5);
    int tempo = 300;
    switch (valor)
    {
      case 1:
        analogWrite(led_vermelho_inf, 254);
        delay(tempo);
        analogWrite(led_vermelho_inf, 0);
        break;
      case 2:
        analogWrite(led_vermelho_sup, 254);
        delay(tempo);
        analogWrite(led_vermelho_sup, 0);
        break;
      case 3:
        analogWrite(led_verde, 254);
        delay(tempo);
        analogWrite(led_verde, 0);
        break;
      case 4:
        digitalWrite(led_estrela, 1);
        delay(tempo);
        digitalWrite(led_estrela, 0);
        break;
    }
  }
   
  //Pisca os 4 conjuntos
  Serial.println("4 - Pisca conjunto");
  Serial.println();
  int tempo = 20;
  for (int y = 0; y <= 20; y++)
  {
    delay(tempo);
    analogWrite(led_vermelho_inf, 254);
    delay(tempo);
    analogWrite(led_vermelho_sup, 254);
    delay(tempo);
    analogWrite(led_verde, 254);
    delay(tempo);
    digitalWrite(led_estrela, 254);
    delay(tempo);
    analogWrite(led_vermelho_inf, 0);
    analogWrite(led_vermelho_sup, 0);
    analogWrite(led_verde, 0);
    digitalWrite(led_estrela, 0);
  }
}
 
void liga_desliga_leds_pwm(int led)
{
  for (int i = 0; i <= 254; i += 5)
  {
    analogWrite(led, i);
    delay(20);
  }
  delay(500);
  //Rotina desligar led vermelho esquerdo pwm
  for (int i = 254; i >= 0 ; i -= 5)
  {
    analogWrite(led, i);
    delay(20);
  }
  analogWrite(led, 0);
}