#include <Arduino.h>
// #define CRONOMETRO
// #define RELOGIO
// #define RELOGIORTC
// #define RTCLCD
// #define LCDCHAR
// #define RELOGIOBINRTC
#define LCDKEY

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
#ifdef LCDKEY
void setupMLK();
void loopMLK();
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
    #ifdef LCDKEY
    setupMLK();
    #endif

}

void loop() {
    #ifdef LCDKEY
    loopMLK();
    #endif
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