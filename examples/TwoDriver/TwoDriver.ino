#include <TPIC2810.h>

// Create instances

// LED driver #1 Used Wire library object, have address 0x66, and output enable pin connected to Arduino pin 11
TPIC2810 ledDriver01(Wire, 0x66, 11);

// LED driver #1 Used Wire library object, have address 0x66, output enable pin connected to GPIO 11 which attached to PWM channel #2 (ESP only)
//TPIC2810 ledDriver01(Wire, 0x66, 11, 2);
//TPIC2810 ledDriver01(Wire, 0x66, BUILTIN_LED, 2);

// LED driver #2 Used Wire library object, have address 0x67, and output enable pin haven't connected
TPIC2810 ledDriver02(Wire, 0x67);

uint8_t ledDriverCounter = 127;

void setup() {
  Serial.begin(115200);

  // Another I2C-bus pins can be used on ESP8266 and ESP32
  // Wire.begin(0,2);
  // Wire.begin(17,22);

  // Init instances
  ledDriver01.begin();
  ledDriver02.begin();
}

void loop() {

  // Write data to driver #1, and then transfer shift register content to output storage register
  ledDriver01.write(ledDriverCounter);
  ledDriver01.transfer();

  // Write data and transfer to output storage register of driver #2 for one operation
  ledDriver02.writeAndTransfer(255 - ledDriverCounter);

  ledDriverCounter++;

  // We can disable led output for driver #1
  ledDriver01.outputDisable();
  delay(1000);
  // And enable it again
  ledDriver01.outputEnable();
  delay(1000);
  // Or set PWM on it, if output enable pin attached to PWM pin
  ledDriver01.outputPwm(64);
  delay(1000);
}