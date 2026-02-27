#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcdRelogio(0x27, 16, 2);

// ===== HORA PADRÃO =====
int hora = 14, minuto = 51, segundo = 10,meio = 4;
// =======================
void atualizarRelogio(),lerSerial(),mostrarHora(),processarHora(String horaStr);
unsigned long ultimoMillis = 0;
String bufferSerial = "";

void setupRelogio() {
  lcdRelogio.init();
  lcdRelogio.backlight();
  Serial.begin(9600);

  lcdRelogio.setCursor(0, 0);
  lcdRelogio.print("***Hora Atual***");
}

void loopRelogio() {
  lerSerial();
  atualizarRelogio();
  mostrarHora();
}
void processarHora(String horaStr) {
  int h, m, s;
  if (sscanf(horaStr.c_str(), "%d:%d:%d", &h, &m, &s) == 3) {
    if (h >= 0 && h < 24 && m >= 0 && m < 60 && s >= 0 && s < 60) {
      hora = h;
      minuto = m;
      segundo = s;
      ultimoMillis = millis(); // reseta o segundo certinho
    }
  }
}

void atualizarRelogio() {
  unsigned long agora = millis();
  if (agora - ultimoMillis >= 1000) {
    ultimoMillis = agora;
    segundo++;
    if (segundo >= 60) {
      segundo = 0;
      minuto++;
    }
    if (minuto >= 60) {
      minuto = 0;
      hora++;
    }
    if (hora >= 24) {
      hora = 0;
    }
  }
}

void lerSerial() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') { // fim da mensagem
      processarHora(bufferSerial);
      bufferSerial = "";
    }
    else if (c != '\r') {
      bufferSerial += c;
    }
  }
}

void mostrarHora() {
  static int ultimoSegundo = -1;

  if (segundo != ultimoSegundo) {
    ultimoSegundo = segundo;

    lcdRelogio.setCursor(0, 1);
    lcdRelogio.print("                ");
    lcdRelogio.setCursor(meio, 1);

    if (hora < 10) lcdRelogio.print("0");
    lcdRelogio.print(hora);
    lcdRelogio.print(":");

    if (minuto < 10) lcdRelogio.print("0");
    lcdRelogio.print(minuto);
    lcdRelogio.print(":");

    if (segundo < 10) lcdRelogio.print("0");
    lcdRelogio.print(segundo);
    Serial.print("Hora atual: ");
    Serial.print(hora);
    Serial.print(":");
    Serial.print(minuto);
    Serial.print(":");
    Serial.println(segundo);
  }
}
