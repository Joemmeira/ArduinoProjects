#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <RtcDS1302.h>
// ===== RTC DS1302 =====
#define CLK_PIN 2
#define DAT_PIN 3
#define RST_PIN 4
ThreeWire myWire(DAT_PIN, CLK_PIN, RST_PIN);
RtcDS1302<ThreeWire> rtcLcd(myWire);
// ===== LCD I2C =====
#define LCD_ADDR 0x27
#define LCD_COLS 16
#define LCD_ROWS 2
LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);
// ===== Variáveis =====
String entradaSerial;
String msgSerial = "Para ajustar o RTC:\nSET DD/MM/YYYY HH:MM:SS";
unsigned long millisBase = 0;
int ultimoSegundo = -1;
// ===== Protótipos =====
void esperaAjusteRtc();
void ajustarRTC(String cmd);
void exibirDataHora();

// Abreviações
const char* diasSemana[] = {
  "DOM", "SEG", "TER", "QUA", "QUI", "SEX", "SAB"
};

void setupRtcLcd() {
  Serial.begin(9600);
  rtcLcd.Begin();

  if (!rtcLcd.GetIsRunning()) rtcLcd.SetIsRunning(true);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("***RTC DS1302***");
  lcd.setCursor(0, 1);
  lcd.print("Inicializando...");

  Serial.println("RTC DS1302 pronto.");
  Serial.println(msgSerial);

  delay(1500);
  lcd.clear();
}

void loopRtcLcd() {
  esperaAjusteRtc();
  exibirDataHora();
  delay(50);
}

void esperaAjusteRtc() {
  while (Serial.available()) {
    char ch = Serial.read();

    if (ch == '\n' || ch == '\r') {
      if (entradaSerial.length() > 0) {
        ajustarRTC(entradaSerial);
        entradaSerial = "";
      }
    } else {
      entradaSerial += ch;
    }
  }
}

void ajustarRTC(String cmd) {
  cmd.trim();
  if (!cmd.startsWith("SET ")) {
    Serial.println("Comando invalido.");
    Serial.println(msgSerial);
    return;
  }

  int dia, mes, ano, hora, minuto, segundo,
  parsed = sscanf(
    cmd.c_str(),
    "SET %d/%d/%d %d:%d:%d",
    &dia, &mes, &ano,
    &hora, &minuto, &segundo
  );
  if (parsed != 6) {
    Serial.println("Formato invalido.");
    return;
  }
  if (ano < 2000 || mes < 1 || mes > 12 ||
      dia < 1 || dia > 31 ||
      hora > 23 || minuto > 59 || segundo > 59) {
    Serial.println("Valores fora do intervalo.");
    return;
  }
  rtcLcd.SetDateTime(RtcDateTime(ano, mes, dia, hora, minuto, segundo));
  Serial.println("Hora ajustada com sucesso.");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("*Hora  Ajustada*");
  lcd.setCursor(0, 1);
  lcd.print("**Com  Sucesso**");

  delay(2000);
  lcd.clear();
}

void exibirDataHora() {
  RtcDateTime now = rtcLcd.GetDateTime();
  unsigned long ms = millis() - millisBase;

  if (!now.IsValid()) return;
  if (now.Second() != ultimoSegundo) {
    ultimoSegundo = now.Second();
    millisBase = millis();
  }
  if (ms > 999) ms = 999;

  char linha1[17], linha2[17];
  snprintf(
    linha1, sizeof(linha1),
    "**%02u:%02u:%02u.%03lu**",
    now.Hour(), now.Minute(), now.Second(), ms
  );
  snprintf(
    linha2, sizeof(linha2),
    "*%s %02u/%02u/%04u*",
    diasSemana[now.DayOfWeek()],
    now.Day(), now.Month(), now.Year()
  );

  Serial.println(String(linha2) + " " + linha1);
  lcd.setCursor(0, 0);
  lcd.print(linha1);
  lcd.setCursor(0, 1);
  lcd.print(linha2);
}
