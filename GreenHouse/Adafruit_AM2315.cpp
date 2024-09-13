#include "Adafruit_AM2315.h"
#if defined(__AVR__)
#include <util/delay.h>
#endif

Adafruit_AM2315::Adafruit_AM2315(TwoWire *theI2C) {
  i2c_dev = new Adafruit_I2CDevice(AM2315_I2CADDR, theI2C);
  humidity = -9999;  // Default invalid value
  temp = -9999;      // Default invalid value
  lastreading = 0;
}

boolean Adafruit_AM2315::begin(void) {
  // Attempt to initialize the I2C device
  if (!i2c_dev->begin()) {
    Serial.println("Failed to find AM2315C sensor!");
    return false;
  }
  Serial.println("AM2315C sensor initialized successfullyadafruit_AM2315.cpp.");
  return true;
}

boolean Adafruit_AM2315::readData(void) {
    uint8_t reply[8] = {0}; // Initialize reply buffer

    // Check if it's been less than 2000 milliseconds since the last successful reading
    if (millis() - lastreading < 2000) {
        Serial.println("Too soon to read again.");
        return false; // Prevent too frequent reads
    }
    lastreading = millis(); // Update last reading time

    // Step 1: Wake up the sensor
    i2c_dev->write(reply, 1);  // Dummy write to wake up
    delay(2000);  // A longer delay might be necessary to ensure the sensor is awake

    // Step 2: Send the read command
    reply[0] = AM2315_READREG; // Function code for reading registers
    reply[1] = 0x00;           // Start address of data to read
    reply[2] = 0x04;           // Number of bytes to read (humidity and temperature)

    if (!i2c_dev->write(reply, 3)) {
        Serial.println("Failed to send read command to AM2315C!");
        return false;
    }

    delay(50); // Short delay to allow sensor to process the command

    // Step 3: Read the response data
    if (!i2c_dev->read(reply, 8)) {
        Serial.println("Failed to read data from AM2315C!");
        return false;
    }

    // Validate response
    if (reply[0] != AM2315_READREG || reply[1] != 0x04) {
        Serial.println("Invalid response header from AM2315C!");
        return false;
    }

    // Parse humidity
    humidity = (reply[2] << 8 | reply[3]) / 10.0;

    // Parse temperature
    temp = (reply[4] & 0x7F) * 256 + reply[5];
    temp /= 10.0;
    if (reply[4] & 0x80) {
        temp = -temp; // Handle negative temperatures
    }

    // Debug output for verifying values
    Serial.print("Temperature: ");
    Serial.println(temp);
    Serial.print("Humidity: ");
    Serial.println(humidity);

    return true; // Data read successfully
}



float Adafruit_AM2315::readTemperature(void) {
  if (!readData()) {
    return NAN;
  }
  return temp;
}

float Adafruit_AM2315::readHumidity(void) {
  if (!readData()) {
    return NAN;
  }
  return humidity;
}

bool Adafruit_AM2315::readTemperatureAndHumidity(float *t, float *h) {
  if (!readData()) {
    return false;
  }
  *t = temp;
  *h = humidity;
  return true;
}
