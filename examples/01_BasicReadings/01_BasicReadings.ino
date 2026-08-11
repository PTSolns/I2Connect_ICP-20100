// Example: Basic Read for I2Connect: ICP-20100
// Last Update: August 11, 2026
// Support: http://ptsolns.com/contact-us
//
// WHAT THIS EXAMPLE DOES
// Initializes the ICP-20100 sensor and reads basic barometric pressure 
// and temperature data to the Serial Monitor.
//
// SKILLS LEARNED
// begin()
// setOperationMode()
// readSensor()
//
// DESCRIPTION
// The I2Connect: ICP-20100 is a high-accuracy barometric pressure and 
// temperature sensing module. It provides a simple and reliable way to add 
// environmental sensing and altitude tracking capabilities to embedded systems.
// The module communicates over the standard I2C interface with a default address 
// of 0x63 (0x64 alternative). If multiple modules are required with the same 
// address, an I2C multiplexer must be used.
// Like all I2Connect modules, it features dual Qwiic compatible connectors for 
// easy daisy-chaining and an angled male header for breadboard use. It is 
// fully compatible with both 3.3V and 5V microcontroller systems.

#include <Wire.h>
#include <PTSolns_I2Connect_ICP_20100.h>

// User Settings
const uint32_t SERIAL_BAUD  = 115200; // Serial baud setting
const uint32_t I2C_CLOCK_HZ = 100000; // I2C clock speed. Keep at 100 kHz most of the time.
const uint8_t  I2C_ADDRESS  = 0x63;   // Default address for the ICP-20100 sensor

PTSolns_I2Connect_ICP_20100 pressureSensor;

void setup() {
    Serial.begin(SERIAL_BAUD);
    while (!Serial) { }

    Wire.begin();
    Wire.setClock(I2C_CLOCK_HZ);
    
    if (!pressureSensor.begin(I2C_ADDRESS, Wire)) {
        Serial.println("ICP-20100 not detected. Please check your wiring!");
        while (1) { } // Halt execution
    }
    
    // Set to Mode 0: Continuous measurement, high accuracy
    pressureSensor.setOperationMode(0); 

    Serial.println("ICP-20100 detected and initialized.");
    Serial.println("-----------------------------------");
}

void loop() {
    float pressure, temperature;
    
    // Poll the sensor for the latest pressure and temperature pair
    if (pressureSensor.readSensor(&pressure, &temperature)) {
        Serial.print("Pressure: ");
        Serial.print(pressure, 2);
        Serial.print(" kPa  |  Temperature: ");
        Serial.print(temperature, 2);
        Serial.println(" C");
    } else {
        Serial.println("Failed to read sensor data.");
    }
    
    delay(500); // Wait 500ms between measurements
}
