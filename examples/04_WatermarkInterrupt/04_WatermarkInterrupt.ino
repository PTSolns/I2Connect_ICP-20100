// Example: Watermark Interrupt for I2Connect: ICP-20100
// Last Update: August 11, 2026
// Support: http://ptsolns.com/contact-us
//
// WHAT THIS EXAMPLE DOES
// Demonstrates how to use the hardware INT pin to trigger an alert only when 
// the internal FIFO buffer reaches a specific fill level (watermark).
//
// SKILLS LEARNED
// setFIFOWatermark()
// configInterrupt()
// clearInterrupts()
// Arduino hardware interrupts
//
// DESCRIPTION
// The I2Connect: ICP-20100 is a high-accuracy barometric pressure and 
// temperature sensing module. It provides a simple and reliable way to add 
// environmental sensing and altitude tracking capabilities to embedded systems.
// 
// By utilizing the hardware FIFO and the INT pin, your microcontroller (like 
// an Arduino or ESP32) can focus on other tasks—or even enter deep sleep—and 
// only wake up when a batch of sensor data is ready to be read.
//
// The module communicates over the standard I2C interface with a default address 
// of 0x63 (0x64 alternative). Like all I2Connect modules, it features dual Qwiic 
// compatible connectors and an angled male header for breadboard use, which 
// breaks out the necessary INT pin.

#include <Wire.h>
#include <PTSolns_I2Connect_ICP_20100.h>

// User Settings
const uint32_t SERIAL_BAUD  = 115200; // Serial baud setting
const uint32_t I2C_CLOCK_HZ = 100000; // I2C clock speed. Keep at 100 kHz most of the time.
const uint8_t  I2C_ADDRESS  = 0x63;   // Default address for the ICP-20100 sensor
const uint8_t  INT_PIN      = 2;      // Hardware interrupt pin (Pin 2 is standard for Uno/Mega)

PTSolns_I2Connect_ICP_20100 pressureSensor;

// Volatile flag to safely pass the interrupt state to the main loop
volatile bool watermarkTriggered = false;

// Interrupt Service Routine (ISR)
// Keep this as short and fast as possible.
#if defined(ESP8266) || defined(ESP32)
  void IRAM_ATTR sensorISR() {
      watermarkTriggered = true;
  }
#else
  void sensorISR() {
      watermarkTriggered = true;
  }
#endif

void setup() {
    Serial.begin(SERIAL_BAUD);
    while (!Serial) { }

    Wire.begin();
    Wire.setClock(I2C_CLOCK_HZ);
    
    // Configure the Arduino pin to listen for the active-low interrupt
    pinMode(INT_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(INT_PIN), sensorISR, FALLING);
    
    if (!pressureSensor.begin(I2C_ADDRESS, Wire)) {
        Serial.println("ICP-20100 not detected. Please check your wiring!");
        while (1) { } // Halt execution
    }
    
    // Clear out any old data from the buffer before we configure it
    pressureSensor.flushFIFO();
    pressureSensor.clearInterrupts();
    
    // Configure the watermark to trigger when 10 samples are in the FIFO
    // (High watermark = 10, Low watermark = 0)
    pressureSensor.setFIFOWatermark(10, 0);
    
    // Enable the High Watermark interrupt (High, Low, DataReady)
    pressureSensor.configInterrupt(true, false, false);
    
    // Start continuous measurement
    pressureSensor.setOperationMode(0); 

    Serial.println("ICP-20100 Watermark Interrupt Initialized.");
    Serial.println("Waiting for FIFO to fill to 10 samples...");
    Serial.println("-----------------------------------");
}

void loop() {
    // Check if our hardware interrupt has fired
    if (watermarkTriggered) {
        Serial.println(">>> HARDWARE INTERRUPT TRIGGERED: High Watermark Reached!");
        
        uint8_t level = pressureSensor.getFIFOLevel();
        Serial.print("Current FIFO Fill Level: ");
        Serial.println(level);
        
        // Read out all available pairs in a burst
        for (uint8_t i = 0; i < level; i++) {
            float pressure, temperature;
            if (pressureSensor.readSensor(&pressure, &temperature)) {
                Serial.print("  [Sample "); Serial.print(i + 1); Serial.print("] ");
                Serial.print("P: "); Serial.print(pressure, 3);
                Serial.print(" kPa, T: "); Serial.print(temperature, 2);
                Serial.println(" C");
            }
        }
        
        Serial.println("FIFO Emptied. Waiting for next batch...");
        Serial.println("-----------------------------------");
        
        // Reset our software flag
        watermarkTriggered = false;
        
        // Clear the hardware interrupt flag on the sensor so it can trigger again
        pressureSensor.clearInterrupts();
    }
    
    // The microcontroller can freely do other things here without missing data!
}
