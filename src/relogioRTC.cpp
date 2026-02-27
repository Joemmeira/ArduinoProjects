#include <Arduino.h>
#include <RtcDS1302.h>

// DS1302: RST, CLK, DAT
#define RST_PIN 2
#define DAT_PIN 3
#define CLK_PIN 4
ThreeWire myWirePins(DAT_PIN, CLK_PIN, RST_PIN);
RtcDS1302<ThreeWire> rtcSerial(myWirePins);
String entrada = "",msg=" | Para ajustar o RTC, envie o comando:\nSET DD/MM/YYYY HH:MM:SS\n";
void esperaAjuste(),
ajustaRTC(String cmd),
exibeDataHora();

const char* diasDaSemana[] = {
    "Domingo",
    "Segunda",
    "Terca",
    "Quarta",
    "Quinta",
    "Sexta",
    "Sabado"
};

void setupRTC() {
    Serial.begin(9600);
    while (!Serial);
    rtcSerial.Begin();
    Serial.println("RTC DS1302 pronto.");
    Serial.println(msg);
}
void loopRTC() {
    esperaAjuste();
    exibeDataHora();
    delay(1000);
}

void esperaAjuste() {
    // Le entrada serial
    while (Serial.available()) {
        char ch = Serial.read();
        if (ch == '\n' || ch == '\r') {
            if (entrada.length() > 0) {
                ajustaRTC(entrada);
                entrada = "";
            }
        } else {
            entrada += ch;
        }
    }
}
void ajustaRTC(String cmd) {
    cmd.trim();
    if (!cmd.startsWith("SET ")) {
        Serial.println("Comando invalido."+msg);
        return;
    }
    int ano, mes, dia, hora, minuto, segundo,
    parsed = sscanf(
        cmd.c_str(),
        "SET %d/%d/%d %d:%d:%d",
        &dia, &mes, &ano,
        &hora, &minuto, &segundo
    );

    if (parsed != 6) {
        Serial.print("Formato invalido. Use: ");
        Serial.println("SET DD/MM/YYYY HH:MM:SS");
        return;
    }
    if (ano < 2000 || mes < 1 || mes > 12 ||
        dia < 1 || dia > 31 ||
        hora > 23 || minuto > 59 || segundo > 59) {
        Serial.println("Valores fora do intervalo.");
        return;
    }

    rtcSerial.SetDateTime(RtcDateTime(ano, mes, dia, hora, minuto, segundo));
    Serial.println("Hora ajustada com sucesso.");
}
void exibeDataHora() {
    RtcDateTime now = rtcSerial.GetDateTime();
    char buffer[40];
    snprintf(
        buffer, sizeof(buffer),
        "%s, %02d/%02d/%04d %02d:%02d:%02d",
        diasDaSemana[now.DayOfWeek()],
        now.Day(), now.Month(), now.Year(),
        now.Hour(), now.Minute(), now.Second()
    );
    if (now.Second() % 2 == 0) {
        Serial.print(msg);
    }    
    Serial.println(buffer);
}