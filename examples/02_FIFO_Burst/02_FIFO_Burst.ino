// Example: FIFO Burst Read for I2Connect: ICP-20100
// Last Update: August 11, 2026
// Support: http://ptsolns.com/contact-us
//
// WHAT THIS EXAMPLE DOES
// Demonstrates how to utilize the hardware FIFO buffer to collect multiple
// pressure and temperature readings in the background, reading them out in a burst.
//
// SKILLS LEARNED
// flushFIFO()
// getFIFOLevel()
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
    
    pressureSensor.setOperationMode(0); 
    
    // Clear out any old data from the buffer before we start
    pressureSensor.flushFIFO();

    Serial.println("ICP-20100 FIFO Flushed and ready for burst reading.");
    Serial.println("-----------------------------------");
}

void loop() {
    // Check how many pressure/temp pairs are currently stored in the buffer (max 16)
    uint8_t level = pressureSensor.getFIFOLevel();
    
    if (level > 0) {
        Serial.print("Current FIFO Fill Level: ");
        Serial.println(level);
        
        // Read out all available pairs sequentially
        for (uint8_t i = 0; i < level; i++) {
            float pressure, temperature;
            if (pressureSensor.readSensor(&pressure, &temperature)) {
                Serial.print("  [Sample "); Serial.print(i + 1); Serial.print("] ");
                Serial.print("P: "); Serial.print(pressure, 3);
                Serial.print(" kPa, T: "); Serial.print(temperature, 2);
                Serial.println(" C");
            }
        }
        Serial.println("-----------------------------------");
    }
    
    // Wait 2 seconds to allow the background sampling to fill the FIFO
    delay(2000); 
}
