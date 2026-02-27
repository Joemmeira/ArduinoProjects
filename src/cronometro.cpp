#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#define botao 2
void mostrarTempo(unsigned long ms);
LiquidCrystal_I2C lcdCronometro(0x27, 16, 2);
unsigned long tempoInicio = 0,tempoDecorrido = 0;
bool cronometroAtivo = false,ultimoEstadoBotao = HIGH;
byte oCirconflexo[8] = {
  0b00100,
  0b01010,
  0b01110,
  0b10001,
  0b10001,
  0b10001,
  0b01110,
  0b00000
};

void setupCronometro() {
    lcdCronometro.createChar(0, (uint8_t*)oCirconflexo);
    lcdCronometro.clear();
  pinMode(botao, INPUT_PULLUP);
  Serial.begin(9600);
  lcdCronometro.init();
  lcdCronometro.backlight();
  lcdCronometro.setCursor(0, 0);
  lcdCronometro.print("***Cron");
  lcdCronometro.write(byte(0));
  lcdCronometro.print("metro***");
  lcdCronometro.setCursor(0, 1);
  lcdCronometro.print("  00:00:00.000");
}
void loopCronometro() {
    bool estadoBotao = digitalRead(botao);
    if (ultimoEstadoBotao == HIGH && estadoBotao == LOW) {
        cronometroAtivo = !cronometroAtivo;
        if (cronometroAtivo) {
        tempoInicio = millis() - tempoDecorrido;
        }delay(200); // Debounce
    }
    ultimoEstadoBotao = estadoBotao;
    
    if (cronometroAtivo) {
        tempoDecorrido = millis() - tempoInicio;
        mostrarTempo(tempoDecorrido);
    }
    if(Serial.available()){
        String comando = Serial.readStringUntil('\n');
        comando.trim();
        if(comando == "start"){
            cronometroAtivo = true;
            tempoInicio = millis() - tempoDecorrido;
        }else if(comando == "stop"){
            cronometroAtivo = false;
        }else if(comando == "reset"){
            cronometroAtivo = false;
            tempoDecorrido = 0;
            mostrarTempo(0);
        }
    }
}
void mostrarTempo(unsigned long ms) {
    unsigned long segundos = ms / 1000,
                  minutos = segundos / 60,
                  horas = minutos / 60,
                  milissegundos = ms % 1000;
    segundos %= 60, minutos %= 60;

    lcdCronometro.setCursor(2, 1);
    lcdCronometro.print((horas < 10 ? "0" : "") + String(horas) + ":");
    lcdCronometro.print((minutos < 10 ? "0" : "") + String(minutos) + ":");
    lcdCronometro.print((segundos < 10 ? "0" : "") + String(segundos) + ".");
    if (milissegundos < 10) {
        lcdCronometro.print("00");
    } else if (milissegundos < 100) {
        lcdCronometro.print("0");
    }
    lcdCronometro.print(String(milissegundos));
}