#include <Arduino.h>
#include <LiquidCrystal.h>
// pinos padrão do shield
LiquidCrystal lcdk(8, 9, 4, 5, 6, 7);
#define start 2
unsigned long tempoInicial, tempoAgora, tempoPausado;
bool rodando = false,pressionado;
int leitura = 0;
void logo(), keypad(),exibeTempo(unsigned long ms),contador();

void setupMLK() {
    Serial.begin(9600);
    pinMode(start, INPUT_PULLUP);
  lcdk.begin(16, 2);
  logo();
  lcdk.setCursor(2, 0);
  lcdk.print("Moldarium 3D");
  lcdk.setCursor(2, 1);
  lcdk.print("00:00:00.000");
}

void loopMLK() {
    contador();
}

void contador(){
  pressionado=digitalRead(start);
  // Detecta borda de descida (botão pressionado)
  if(pressionado){
    if(!rodando){
      tempoPausado=0;
      tempoInicial=millis();
      rodando=true;
    }
    tempoAgora=(millis()-tempoInicial)*1.0068;
    exibeTempo(tempoAgora);
  }else if(rodando){
    tempoPausado=millis()-tempoInicial;
    rodando=false;
  }
}

void exibeTempo(unsigned long ms){
  // Calcula horas, minutos, segundos e milissegundos
  unsigned long st=ms/1000,
  h= st/3600,
  m= (st % 3600)/60,
  s= st % 60,
  mi= ms % 1000;
  char bf[17]; // Buffer para 16 caracteres + terminador nulo
  lcdk.setCursor(2, 1);
  sprintf(bf,"%02lu:%02lu:%02lu.%03lu",h,m,s,mi);
  lcdk.print(bf);
  Serial.println(bf);
}

void logo(){
  byte caracter[4][8] = {
    {B00000, B00001, B00011, B10001, B11000, B10100, B10010, B10101},  // caractere 0
    {B00000, B10000, B11000, B10001, B00011, B00101, B01001, B10101},  // caractere 1
    {B10110, B10101, B10100, B01000, B00100, B00010, B00001, B00000},  // caractere 2
    {B01101, B10101, B00101, B01010, B10100, B01000, B10000, B00000}   // caractere 3
  };
  for (int i = 0; i < 4; i++) {
    lcdk.createChar(i, caracter[i]);
  }
  for(int i=0;i<2;i++){
    lcdk.setCursor(0,i);
    lcdk.write(byte(i*2));
    lcdk.write(byte(i*2+1));
    lcdk.setCursor(14,i);
    lcdk.write(byte(i*2));
    lcdk.write(byte(i*2+1));
  }
}

void keypad(){
  leitura = analogRead(A0);

  lcdk.setCursor(0, 1);

  if (leitura < 50) {
    lcdk.print("RIGHT");
  } 
  else if (leitura < 150) {
    lcdk.print("UP");
  } 
  else if (leitura < 350) {
    lcdk.print("DOWN");
} 
else if (leitura < 500) {
    lcdk.print("LEFT");

  } 
  else if (leitura < 800) {
    lcdk.print("SELECT");
  } 
  else {
    lcdk.print("NONE");
  }

  delay(200);
}