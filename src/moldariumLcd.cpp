#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#define botaoM 2

LiquidCrystal_I2C lcdm(0x27, 16, 2); // Altere para seu endereço I2C (0x3F comum)

unsigned long tempoInicialM,
tempoAgoraM,
tempoPausadoM;
bool rodandoM = false,
pressionadoM;
void contadorM(),exibeTempoM(unsigned long ms),logoM();
//Logo
void logoM(){
  byte caracter[4][8] = {
    {B00000, B00001, B00011, B10001, B11000, B10100, B10010, B10101},  // caractere 0
    {B00000, B10000, B11000, B10001, B00011, B00101, B01001, B10101},  // caractere 1
    {B10110, B10101, B10100, B01000, B00100, B00010, B00001, B00000},  // caractere 2
    {B01101, B10101, B00101, B01010, B10100, B01000, B10000, B00000}   // caractere 3
  };
  for (int i = 0; i < 4; i++) {
    lcdm.createChar(i, caracter[i]);
  }
  for(int i=0;i<2;i++){
    lcdm.setCursor(0,i);
    lcdm.write(byte(i*2));
    lcdm.write(byte(i*2+1));
    lcdm.setCursor(14,i);
    lcdm.write(byte(i*2));
    lcdm.write(byte(i*2+1));
  }
}

void setupM3D() {
  Serial.begin(9600);
  pinMode(botaoM, INPUT_PULLUP);

  lcdm.init();
  lcdm.backlight();
  logoM();
  lcdm.setCursor(2,0);
  lcdm.print("Moldarium 3D");
  lcdm.setCursor(2,1);
  lcdm.print("00:00:00.000");
}

void loopM3D() {
    contadorM();
}

void exibeTempoM(unsigned long ms){
  // Calcula horas, minutos, segundos e milissegundos
  unsigned long st=ms/1000,
  h= st/3600,
  m= (st % 3600)/60,
  s= st % 60,
  mi= ms % 1000;
  char bf[17]; // Buffer para 16 caracteres + terminador nulo
  lcdm.setCursor(2, 1);
  sprintf(bf,"%02lu:%02lu:%02lu.%03lu",h,m,s,mi);
  lcdm.print(bf);
  Serial.println(bf);
}

void contadorM(){
  // bool estadoBotao = digitalRead(botaoM) == LOW; // pressionado
  pressionadoM=digitalRead(botaoM);

  static unsigned long inicioPressao = 0;
  static bool jaTratado = false,
  ultimoEstado = false; // evita repetir ação
  // começou a pressionar
  if (!pressionadoM && ultimoEstado) {
    inicioPressao = millis();
    jaTratado = false;
  }
  // enquanto está pressionado
  if (!pressionadoM && !jaTratado) {
    if (millis() - inicioPressao >= 10000) {
      // pressionou por 1 segundo → PARA
      tempoPausadoM = millis() - tempoInicialM;
      rodandoM = false;
      jaTratado = true;
    }
  }
  // se está solto e não está rodando → inicia
  if (pressionadoM && !rodandoM) {
    tempoInicialM = millis();
    rodandoM = true;
  }
  // atualiza contador
  if (rodandoM) {
    tempoAgoraM = (millis() - tempoInicialM) * 1.0068;
    exibeTempoM(tempoAgoraM);
  }

  ultimoEstado = pressionadoM;
}