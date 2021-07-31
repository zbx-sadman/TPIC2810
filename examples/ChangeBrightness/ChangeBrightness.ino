#include <TPIC2810.h>

// Create instance
// LED driver #1 used Wire library object, have address 0x66, and output enable pin attached to Arduino pin 4
TPIC2810 ledDriver01(Wire, 0x66, 4);

// ESP32 can be inited as follows
//TPIC2810 ledDriver01(Wire, 0x66, BUILTIN_LED, 2);

// Segments configuration is EDCBAFG
const uint8_t symbol[] = {
  0x7E, // B01111110
  0x18, // B00011000
  0x6D, // B01101101
  0x3D, // B00111101
  0x1B, // B00011011
  0x37, // B00110111
  0x77, // B01110111
  0x1C, // B00011100
  0x7F, // B01111111
  0x3F, // B00111111
//  0x80 // B10000000 // DP segment is enabled by most significant bit
};

// "Brightness pot" connected to pin A0
uint8_t  potPin = A0;

// Maximum number, that can taken from analogRead(): 1023 for 8-bit ADC (Arduino Uno, Nano, etc.), or 4095 for 12-bit ADC (ESP32 boards)
//uint16_t maxPotValue = 1023;
uint16_t maxPotValue = 4095;

// Symbols will be changed every second
uint32_t changSymbolInterval = 1000UL;

uint8_t  symbolCounter;
uint32_t prevChangeTime;

void setup() {
  // Another I2C-bus pins can be used on ESP8266 and ESP32
  // Wire.begin(0, 2);
  // Wire.begin(17, 22);

  // Init instance
  ledDriver01.begin();
}

void loop() {
  uint16_t potValue = analogRead(potPin);
  // TPIC2810's enable pin is active LOW, and will be disabled when pin goes HIGH.
  // As a result, brightness is max on PWM duty = 0% and min on 100%
  uint8_t currentBrightness = map(potValue, 0, maxPotValue, 255, 0);
  ledDriver01.outputPwm(currentBrightness);

  // Every changSymbolInterval send new data to the TPIC2810
  if (millis() - prevChangeTime > changSymbolInterval) {
    prevChangeTime = millis();
    ledDriver01.writeAndTransfer(symbol[symbolCounter]);
    symbolCounter = (symbolCounter + 1) % (sizeof(symbol) / sizeof(symbol[0]));
  }
}
