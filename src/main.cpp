#include <Arduino.h>
// #define CRONOMETRO
// #define RELOGIO
// #define RELOGIORTC
// #define RTCLCD
// #define LCDCHAR
#define RELOGIOBINRTC

#ifdef CRONOMETRO
void setupCronometro();
void loopCronometro();
#endif
#ifdef RELOGIO
void setupRelogio();
void loopRelogio();
#endif
#ifdef RELOGIORTC
void setupRTC();
void loopRTC();
#endif
#ifdef RTCLCD
void setupRtcLcd();
void loopRtcLcd();
#endif
#ifdef LCDCHAR
void setupLcdChar();
void loopLcdChar();
#endif
#ifdef RELOGIOBINRTC
void setupBinRtc();
void loopBinRtc();
#endif

void setup() {
    #ifdef CRONOMETRO
    setupCronometro();
    #endif
    #ifdef RELOGIO
    setupRelogio();
    #endif
    #ifdef RELOGIORTC
    setupRTC();
    #endif
    #ifdef RTCLCD
    setupRtcLcd();
    #endif
    #ifdef LCDCHAR
    setupLcdChar();
    #endif
    #ifdef RELOGIOBINRTC
    setupBinRtc();
    #endif
}

void loop() {
    #ifdef RELOGIO
    loopRelogio();
    #endif
    #ifdef RELOGIORTC
    loopRTC();
    #endif
    #ifdef RTCLCD
    loopRtcLcd();
    #endif
    #ifdef CRONOMETRO
    loopCronometro();
    #endif
    #ifdef LCDCHAR
    loopLcdChar();
    #endif
    #ifdef RELOGIOBINRTC
    loopBinRtc();
    #endif
}