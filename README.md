# TPIC2810
Arduino library (AVR, ESP8266, ESP32) for Texas Instrument TPIC2810 8-bit LED driver with I2C interface

## Basic Usage

#### TPIC2810 myDriver(Wire, I2C_address, Output_enable_pin, PWM_Channel);
Creates the TPIC2810, using _Wire_ object, specific I2C address, and "output enable pin" (optional), and PWM channel (optional, ESP32 only). You can create multiple TPIC2810 objects, one for each address.

Returns: 
 - TPIC2810 object instance.

#### myDriver.begin()
Inits instance's internal stuff. 

Returns: 
  - true if device detected on the bus;
  - false if there was any I2C error.

#### mydDriver.write(value)
Writes 8-bit of data to shift register of the TPIC2810.

Returns: 
 - true on success; 
 - false on fail (I2C error detected).

#### mydDriver.transfer()
Sends command to tranfer shift register content to output storage register.

Returns: 
 - true on success; 
 - false on fail (I2C error detected).

#### mydDriver.writeAndTransfer(value) 
Writes to shift register of the TPIC2810 8-bit of data which tranferred to output storage register immedately.

Returns: 
 - true on success; 
 - false on fail (I2C error detected).

#### mydDriver.read() 
Reads shift register data from the TPIC2810

Returns (as signed integer 16 bit number): 
 - shift register value;
 - -1 on fail (I2C error detected).

#### mydDriver.outputEnable() 
Enables driver outputs (sets output enable pin to LOW).

Returns: 
 - nothing.

#### mydDriver.outputDisable() 
Disables driver outputs (sets output enable pin to HIGH).

Returns: 
 - nothing.

#### mydDriver.outputPwm(value) 
Activates PWM on driver's outputs. 

Returns: 
 - nothing.

_Note_: TPIC2810's enable pin is active LOW, and will be disabled when pin goes HIGH. As a result, brightness is max on PWM duty = 0% and min on 100%

## Credits
This project initiated and sponsored by Dmitry Pudov.
