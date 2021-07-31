#include <TPIC2810.h>

// Create instance
// LED driver #1 Used Wire library object, have address 0x66
TPIC2810 ledDriver01(Wire, 0x66);

uint8_t ledDriverCounter = 127;

void setup() {
  uint8_t success;
  Serial.begin(115200);

  // Another I2C-bus pins can be used on ESP8266 and ESP32
  // Wire.begin(0, 2);
  // Wire.begin(17, 22);

  // Init instance
  Serial.print(F("\nDriver #1 init "));
  success = ledDriver01.begin();
  if (success) {
    Serial.println(F("OK"));
  } else {
    Serial.println(F("failed"));
  }
  Serial.println();
}

void loop() {
  uint8_t success, sendData;
  int16_t recvData;

  // Send some value to TPIC2810's shift register
  success = ledDriver01.write(ledDriverCounter);
  Serial.print(F("Write: "));
  if (success) {
    Serial.print(F("0x")); Serial.println((uint8_t) ledDriverCounter, HEX);
  } else {
    Serial.println(F("failed"));  
  }

  // Verify by reading 
  Serial.print(F("Read after write: "));
  recvData = ledDriver01.read();
  if (0x00 <= recvData) {
    Serial.print(F("0x")); Serial.println((uint8_t)recvData, HEX);
  } else {
    Serial.println(F("failed"));
  }
  Serial.println();

  // Transfer shift register content to output load register
  ledDriver01.transfer();

  // Send some value to TPIC2810's shift register with tranferring to output load register for one operation
  sendData = 255 - ledDriverCounter;
  success = ledDriver01.writeAndTransfer(sendData);
  Serial.print(F("Write and transfer: "));
  if (success) {
    Serial.print(F("0x")); Serial.println((uint8_t)sendData, HEX);
  } else {
    Serial.println(F("failed"));
  }

  // Verify by reading 
  Serial.print(F("Read after writeAndTransfer: "));
  recvData = ledDriver01.read();
  if (0x00 <= recvData) {
    Serial.print(F("0x")); Serial.println((uint8_t)recvData, HEX);
  } else {
    Serial.println(F("failed"));
  }
  Serial.println();

  ledDriverCounter++;
  delay(1000);
}
