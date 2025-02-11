#ifndef AM2315C_H
#define AM2315C_h
#pragma once
//
//    FILE: AM2315C.h
//  AUTHOR: Rob Tillaart
// Edited by Brian Jastram for use with ModularSensors and Mayfly board v1.1 2025
// PURPOSE: Arduino library for AM2315C I2C temperature and humidity sensor.
// VERSION: 0.2.1
//     URL: https://github.com/RobTillaart/AM2315C
//
//
//  Always check datasheet
//
//                    +-----------------+
//    RED    -------- | VDD             |
//    YELLOW -------- | SDA    AM2315C  |
//    BLACK  -------- | GND             |
//    WHITE  -------- | SCL             |
//                    +-----------------+



#include "Arduino.h"
#include "Wire.h"
#include "SensorBase.h"  // Include the header file where the Sensor class is defined

#define AM2315C_LIB_VERSION                    (F("0.2.1"))

#define AM2315C_OK                             0
#define AM2315C_ERROR_CHECKSUM                -10
#define AM2315C_ERROR_CONNECT                 -11
#define AM2315C_MISSING_BYTES                 -12
#define AM2315C_ERROR_BYTES_ALL_ZERO          -13
#define AM2315C_ERROR_READ_TIMEOUT            -14
#define AM2315C_ERROR_LASTREAD                -15


class AM2315C : public Sensor
{
public:
  //  CONSTRUCTOR
  //  fixed address 0x38
AM2315C(int8_t powerPin, TwoWire* wire = &Wire);  //  start the I2C
  bool     begin();
  bool     isConnected();
  uint8_t  getAddress();


  //  ASYNCHRONUOUS CALL
  //  trigger acquisition.
  int      requestData();
  //  read the raw data.
  int      readData();
  //  converts raw data bits to temperature and humidity.
  int      convert();


  //  SYNCHRONOUS CALL
  //  blocking read call to read + convert data
  int      read();
  //  access the converted temperature & humidity
  float    getHumidity();
  float    getTemperature();

  // Implement required pure virtual function from SensorBase.h
  bool addSingleMeasurementResult(void) override;

  //  OFFSET  1st order adjustments
  void     setHumOffset(float offset = 0);
  void     setTempOffset(float offset = 0);
  float    getHumOffset();
  float    getTempOffset();


  //  READ STATUS
  uint8_t  readStatus();
  //  3 wrapper functions around readStatus()
  bool     isCalibrated();
  bool     isMeasuring();
  bool     isIdle();
  //  status from last read()
  int      internalStatus();


  //  TIMING
  uint32_t lastRead();
  uint32_t lastRequest();


  //  RESET  (new since 0.1.4)
  //  use with care
  //  returns number of registers reset => must be 3
  //  3     = OK
  //  0,1,2 = error.
  //  255   = no reset needed.
  //  See datasheet 7.4 Sensor Reading Process, point 1
  //  use with care
  uint8_t  resetSensor();


private:
  float    _humidity;
  float    _temperature;
  float    _humOffset;
  float    _tempOffset;

  uint8_t  _status;
  uint32_t _lastRequest;
  uint32_t _lastRead;
  uint8_t  _bits[7];

  uint8_t  _crc8(uint8_t *ptr, uint8_t len);

  //  use with care
  bool     _resetRegister(uint8_t reg);

  TwoWire* _wire;
};
#ifndef AM2315C_VARIABLES_H
#define AM2315C_VARIABLES_H

#include <ModularSensors.h>
#include "AM2315C.h"

class AM2315C_Humidity_Var : public Variable {
public:
    explicit AM2315C_Humidity_Var(Sensor* parentSense, const char* uuid = "")
        : Variable(parentSense, 0, 2, "Humidity", "percent", "AM2315C_Humidity", uuid) {}
};

class AM2315C_Temp_Var : public Variable {
public:
    explicit AM2315C_Temp_Var(Sensor* parentSense, const char* uuid = "")
        : Variable(parentSense, 1, 2, "Temperature", "degreeCelsius", "AM2315C_Temp", uuid) {}
};


#endif  // AM2315C_VARIABLES_H

#endif  // AM2315C_H

//  -- END OF FILE --