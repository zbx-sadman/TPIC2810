#include "TPIC2810.h"
//
//    FILE: TPIC2810.cpp
//  AUTHOR: Grigory Prigodin (https://github.com/zbx-sadman)
// VERSION: 1.0.0
// PURPOSE: Texas Instrument TPIC2810 8-bit LED driver with I2C interface
//
// HISTORY:
//   0.1.0  2021-07-25 initial version
//

#define TWI_NO_ERROR                                             (0x00)
#define TWI_BUFFER_FULL                                          (0x01)
#define TWI_ADDRESS_NACK                                         (0x02)
#define TWI_DATA_NACK                                            (0x03)
#define TWI_OTHER                                                (0x04)

//
// Send command to TPIC2810
//
// Params:   * _command   - executing command (write to shift register only, transfer to output storage register only, or write to shift register & transfer to output storage register)
//           * _variable  - variable which will be used to store recieved data or contain data that will be written to TPIC2810 shift register;
//           * _operation - opRead for reading and opWrite for other.
// 
// Returns:  * true on success
//           * false on fail
//
uint8_t TPIC2810::dataExchange(command_t _command, uint8_t& _variable, uint8_t _operation) {
  uint8_t rc = false;

  // Starts communication
  ptrTwoWire->beginTransmission(i2cAddress);
  // Send command
  ptrTwoWire->write((uint8_t)_command);
  // Send value if we want write something
  if (cmdTransferOnly != _command && opWrite == _operation) {
    ptrTwoWire->write(_variable);
  }

  // Close connection (with true flag) on "write" operation
  rc = ptrTwoWire->endTransmission((opWrite == _operation) ? true : false);

  // Otherwise - it's "read" operation and need make request to device
  if (TWI_NO_ERROR == rc && opRead == _operation) {
    // Need only one byte
    if (0x01 == ptrTwoWire->requestFrom((uint8_t)i2cAddress, (uint8_t)0x01, (uint8_t)true)) {
      _variable = ptrTwoWire->read();
   } else {
      rc = TWI_OTHER;
    }
  }

  // Is communication finished OK?
  return (TWI_NO_ERROR == rc);
}

//
// Init instance
//
// Params:   * nothing
//           
// Returns:  * true if device online;
//           * false on any I2C error rised.
//
uint8_t TPIC2810::begin() {
  ptrTwoWire->begin();
  ptrTwoWire->beginTransmission(i2cAddress);

  if (TPIC2810_OE_PIN_NOT_DEFINED != oePin) {
#if (defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_ESP8266))
     digitalWrite(oePin, LOW); 
     pinMode(oePin, OUTPUT);
#elif defined(ARDUINO_ARCH_ESP32)
     ledcSetup(pwmChannel, TPIC2810_ESP32_PWM_FREQUENCY, TPIC2810_ESP32_PWM_RESOLUTUON);
     ledcAttachPin(oePin, pwmChannel);
     ledcWrite(pwmChannel, TPIC2810_ESP32_PWM_MIN);
#endif
  }
  return (TWI_NO_ERROR == ptrTwoWire->endTransmission(true));
}

//
// Read shift register data from the TPIC2810
//
// Params:   * nothing
//
// Returns:  * 8-bit of shift register value
//           * -1 on fail
//
int16_t TPIC2810::read() {
  uint8_t rc, result = 0x00;
  rc = dataExchange(cmdReadWrite, result, opRead);
  return rc ? result : -0x01;
}

//
// Enable driver outputs
//
// Params:   * nothing
//
// Returns   * nothing
//
void TPIC2810::outputEnable() { 
  if (TPIC2810_OE_PIN_NOT_DEFINED == oePin) { return; } 
#if (defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_ESP8266))
  digitalWrite(oePin, LOW); 
#elif defined(ARDUINO_ARCH_ESP32)
  ledcWrite(pwmChannel, TPIC2810_ESP32_PWM_MIN);
#endif
}

//
// Disable driver outputs
//
// Params:   * nothing
//
// Returns   * nothing
//
void TPIC2810::outputDisable() { 
  if (TPIC2810_OE_PIN_NOT_DEFINED == oePin) { return; } 
#if (defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_ESP8266))
  digitalWrite(oePin, HIGH); 
#elif defined(ARDUINO_ARCH_ESP32)
  ledcWrite(pwmChannel, TPIC2810_ESP32_PWM_MAX);
#endif
}

//
// Activate PWM on driver's outputs
//
// Params:   * nothing
//
// Returns   * nothing
//
void TPIC2810::outputPwm(uint8_t _pwmLevel) { 
  if (TPIC2810_OE_PIN_NOT_DEFINED == oePin) { return; } 
#if (defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_ESP8266))
  analogWrite(oePin, _pwmLevel); 
#elif defined(ARDUINO_ARCH_ESP32)
  ledcWrite(pwmChannel, _pwmLevel);
#endif
}
