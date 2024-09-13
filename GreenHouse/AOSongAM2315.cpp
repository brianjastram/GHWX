/**
 * @file AOSongAM2315.cpp
 * @copyright 2017-2022 Stroud Water Research Center
 * Part of the EnviroDIY ModularSensors library for Arduino
 * @author Sara Geleskie Damiano <sdamiano@stroudcenter.org>
 *
 * @brief Implements the AM2315C class.
 */

// #include <AM2315C.h>
#include <sensors/AOSongAM2315.h>

// Constructor for the AM2315C_Sensor class
AM2315C_Sensor::AM2315C_Sensor(TwoWire* wire, int8_t powerPin, uint8_t i2cAddress)
    : Sensor("AM2315C", 2,  // Sensor name and number of variables
             100, 500, 500, // Warm-up, stabilization, and measurement times in milliseconds
             powerPin, -1, i2cAddress),
      _am2315(wire) {}  // Initialize _am2315 with the I2C interface

// Setup method to initialize the sensor
bool AM2315C_Sensor::setup() {
    // Initialize the I2C bus
    Wire.begin();

    Serial.println("Starting sensor setup...X");
    
    // Attempt to begin communication with the sensor
    if (!_am2315.begin()) {
        Serial.println(F("AM2315C failed to initialize at I2C address 0x38. Check connections."));
        return false;
    }

    Serial.println("AM2315C sensor initialized successfully at I2C address 0x38!");
    return true;  // Add this line

}

// Function to add a single measurement result without 'override' since it doesn't
// match a base class function
bool AM2315C_Sensor::addSingleMeasurementResult(int varNum, float result) {
    float temp, hum;
    
    // Use Adafruit_AM2315's readTemperatureAndHumidity method
    if (!_am2315.readTemperatureAndHumidity(&temp, &hum)) {
        Serial.println("Failed to read temperature and humidity from AM2315C sensor!");
        return false;
    }

    // Verify and add temperature and humidity measurements
    verifyAndAddMeasurementResult(AM2315_TEMP_VAR_NUM, temp);
    verifyAndAddMeasurementResult(AM2315_HUMIDITY_VAR_NUM, hum);

    // Unset the time stamp for the beginning of this measurement
    _millisMeasurementRequested = 0;
    // Unset the status bits for a measurement request (bits 5 & 6)
    _sensorStatus &= 0b10011111;

    return true;
}

// Simplified version if not variable-specific
bool AM2315C_Sensor::addSingleMeasurementResult() {
    return addSingleMeasurementResult(-1, 0.0);
}

// Method to get the temperature from the sensor
float AM2315C_Sensor::getTemperature() {
    return _am2315.readTemperature();  // Call the Adafruit method directly
}

// Method to get the humidity from the sensor
float AM2315C_Sensor::getHumidity() {
    return _am2315.readHumidity();  // Call the Adafruit method directly
}
