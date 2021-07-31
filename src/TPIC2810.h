#pragma once
//
//    FILE: TPIC2810.h
//  AUTHOR: Grigory Prigodin (https://github.com/zbx-sadman)
// VERSION: 1.0.0
// PURPOSE: Texas Instrument TPIC2810 8-bit LED driver with I2C interface
// HISTORY: See TPIC2810.cpp
//     URL: https://github.com/zbx-sadman/TPIC2810
//

#include <Arduino.h>
#include <Wire.h>

#define TPIC2810_I2C_ADDRESS_DEFAULT                             (0x60)
#define TPIC2810_OE_PIN_NOT_DEFINED                              (-0x01)

#define TPIC2810_ESP32_PWM_CHANNEL_DEFAULT                       (0x00)
#define TPIC2810_ESP32_PWM_RESOLUTUON                            (0x08)
#define TPIC2810_ESP32_PWM_FREQUENCY                             (0x03E8)
#define TPIC2810_ESP32_PWM_MIN                                   (0x00)
#define TPIC2810_ESP32_PWM_MAX                                   ((1 << TPIC2810_ESP32_PWM_RESOLUTUON) - 0x01)

typedef enum : uint8_t { 
   cmdReadWrite        = 0x11, 
   cmdTransferOnly     = 0x22, 
   cmdWriteAndTransfer = 0x44 
} command_t;

typedef enum : uint8_t { 
   opRead              = 0x01, 
   opWrite             = 0x02
} operation_t;

class TPIC2810  {
  public:
    //
    // Create instance with default I2C address, and detached output enable pin
    //
    TPIC2810(TwoWire& _twoWire)                                      : ptrTwoWire(&_twoWire), i2cAddress(TPIC2810_I2C_ADDRESS_DEFAULT), oePin(TPIC2810_OE_PIN_NOT_DEFINED) { ; }
    //
    // Create instance with specified I2C address, and detached output enable pin
    //
    TPIC2810(TwoWire& _twoWire, uint8_t _i2cAddress)                 : ptrTwoWire(&_twoWire), i2cAddress(_i2cAddress), oePin(TPIC2810_OE_PIN_NOT_DEFINED) { ; }
    //
    // Create instance with specified I2C address, and attached output enable pin
    //
    TPIC2810(TwoWire& _twoWire, uint8_t _i2cAddress, int8_t _oePin)  : ptrTwoWire(&_twoWire), i2cAddress(_i2cAddress), oePin(_oePin) { ; }

#if defined(ARDUINO_ARCH_ESP32)
    //
    // Create instance with specified I2C address, and attached output enable pin, and PWM channel specified
    //
    TPIC2810(TwoWire& _twoWire, uint8_t _i2cAddress, int8_t _oePin, uint8_t _pwmChannel)  : ptrTwoWire(&_twoWire), i2cAddress(_i2cAddress), oePin(_oePin), pwmChannel(_pwmChannel)  { ; }
#endif
    //
    // Init instance
    //
    // Returns:  * true if device detected on the bus;
    //           * false if there was any I2C error.
    //
    uint8_t begin();
    
    //
    // Read shift register data from the TPIC2810
    //
    // Params:   * nothing
    //
    // Returns:  * 8-bit of shift register value
    //           * -1 on fail
    //
    int16_t read();
    
    //
    // Write 8-bit of data to shift register of the TPIC2810
    //
    // Params:   * Written value
    //
    // Returns   * true on success
    //           * false on fail
    //
    inline uint8_t write(uint8_t _value) { return dataExchange(cmdReadWrite, _value, opWrite); }

    //
    // Write 8-bit of data to shift register and tranfer to output storage register for one operation
    //
    // Params:   * Written value
    //
    // Returns   * true on success
    //           * false on fail
    //
    inline uint8_t writeAndTransfer(uint8_t _value) { return dataExchange(cmdWriteAndTransfer, _value, opWrite); }

    //
    // Tranfer shift register content to output storage register
    //
    // Params:   * nothing
    //
    // Returns   * true on success
    //           * false on fail
    //
    inline uint8_t transfer() { uint8_t stub; return dataExchange(cmdTransferOnly, stub, opWrite); }

    //
    // Enable driver outputs
    //
    // Params:   * nothing
    //
    // Returns   * nothing
    //
    void outputEnable();

    //
    // Disable driver outputs
    //
    // Params:   * nothing
    //
    // Returns   * nothing
    //
    void outputDisable();

    //
    // Do PWM on driver's outputs
    //
    // Params:   * nothing
    //
    // Returns   * nothing
    //
    void outputPwm(uint8_t);

  private:
    uint8_t  dataExchange(command_t, uint8_t&, uint8_t);

    TwoWire* ptrTwoWire;
    uint8_t  i2cAddress;
    int8_t   oePin;
    uint8_t  pwmChannel = TPIC2810_ESP32_PWM_CHANNEL_DEFAULT;
};

