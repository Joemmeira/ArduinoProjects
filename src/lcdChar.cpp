#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcdChar(0x27, 16, 2);
byte customChar[8] = {
  0b00100,
  0b01010,
  0b01110,
  0b10001,
  0b10001,
  0b10001,
  0b01110,
  0b00000
};

void setupLcdChar(){
    lcdChar.init();
    lcdChar.backlight();
    lcdChar.createChar(0, (uint8_t*)customChar);
    lcdChar.clear();
    lcdChar.setCursor(0,0);
    lcdChar.print("Custom Char:");
    lcdChar.write(byte(0));
    lcdChar.setCursor(0,1);
    lcdChar.print(" Char o^:");
    lcdChar.write(byte(0));
}
void loopLcdChar(){}