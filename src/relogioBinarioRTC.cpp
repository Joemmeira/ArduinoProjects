#include <Arduino.h>
#include <RTClib.h>

RTC_DS3231 rtcBin;

// ===== Pinos dos LEDs =====
const byte secU[4] = {0, 1, 2, 3},// Unidade de segundo (4)
secD[3] = {4, 5, 6},// Dezena de segundo (3)
minU[4] = {7, 8, 9, 10},// Unidade de minuto (4)
minD[3] = {11, 12, 13},// Dezena de minuto (3)
hourL[4] = {A0, A1, A2, A3};// Hora (12h) (4)
String serialBuffer="";
// ===== Protótipos =====
void escreveBinario(byte valor, const byte* pinos, byte bits),
processaSerial(),
ajustRtc(String dataHora),
BinRTC();

void setupBinRtc() {
  // Serial.begin(9600);
  rtcBin.begin();

  if (rtcBin.lostPower()){
    rtcBin.adjust(DateTime(F(__DATE__), F(__TIME__)));
    Serial.println("RTC ajustado para a data e hora de compilacao.");
  }
  
  // Inicializa LEDs
  for (byte i = 0; i < 4; i++) {
    pinMode(secU[i], OUTPUT);
    pinMode(minU[i], OUTPUT);
    pinMode(hourL[i], OUTPUT);
  }
  for (byte i = 0; i < 3; i++) {
    pinMode(secD[i], OUTPUT);
    pinMode(minD[i], OUTPUT);
  }
}

void loopBinRtc() {
  BinRTC();
  delay(200);
}
// ===== Funções =====
void escreveBinario(byte valor, const byte* pinos, byte bits) {
  for (byte i = 0; i < bits; i++) {
    digitalWrite(pinos[i], (valor >> i) & 1);
  }
}
void processaSerial() {
  while (Serial.available()){
    char c=Serial.read();
    if (c=='\n'||c=='\r'){
      if (serialBuffer.length()>0){
        ajustRtc(serialBuffer);
        serialBuffer="";
      }
    }else{
      serialBuffer+=c;
    }
  }
}
void ajustRtc(String dataHora) {
  dataHora.trim();
  String myMsg="Formato invalido. Use: SET DD/MM/YYYY HH:MM:SS";
  if (!dataHora.startsWith("SET ")) {
    Serial.println(myMsg);
    return;
  }
  // Formato esperado: "DD/MM/YYYY HH:MM:SS"
  int dia, mes, ano, hora, minuto, segundo,
  ok=sscanf(dataHora.c_str(), "SET %d/%d/%d %d:%d:%d", &dia, &mes, &ano, &hora, &minuto, &segundo);
  if (ok != 6) {
    Serial.println(myMsg);
    return;
  }
  if (ano<2000||mes<1||mes>12||dia<1||dia>31||hora>23||minuto>59||segundo>59){
    Serial.println(myMsg);
    return;
  }
  
  rtcBin.adjust(DateTime(ano, mes, dia, hora, minuto, segundo));
  Serial.println("RTC ajustado para: " + dataHora);
}
void BinRTC() {
  processaSerial();
  DateTime now = rtcBin.now();

  byte segundo = now.second(),
  minuto  = now.minute(),
  hora    = now.hour();
  // Converte para 12h
  if (hora == 0) hora = 12;
  else if (hora > 12) hora -= 12;

  escreveBinario(segundo % 10, secU, 4);
  escreveBinario(segundo / 10, secD, 3);
  escreveBinario(minuto % 10, minU, 4);
  escreveBinario(minuto / 10, minD, 3);
  escreveBinario(hora, hourL, 4);
}