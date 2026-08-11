#ifndef PTSOLNS_I2CONNECT_ICP_20100_H
#define PTSOLNS_I2CONNECT_ICP_20100_H

#include <Arduino.h>
#include <Wire.h>

// I2C Addresses
#define ICP_20100_DEFAULT_ADDR 0x63
#define ICP_20100_ALT_ADDR     0x64

// Register Map (from datasheet)
#define ICP_20100_REG_DEVICE_ID        0x0C
#define ICP_20100_REG_MODE_SELECT      0xC0
#define ICP_20100_REG_INTERRUPT_STATUS 0xC1
#define ICP_20100_REG_INTERRUPT_MASK   0xC2
#define ICP_20100_REG_FIFO_CONFIG      0xC3
#define ICP_20100_REG_FIFO_FILL        0xC4
#define ICP_20100_REG_VERSION          0xD3
#define ICP_20100_REG_PRESS_DATA_0     0xFA
#define ICP_20100_REG_PRESS_DATA_1     0xFB
#define ICP_20100_REG_PRESS_DATA_2     0xFC
#define ICP_20100_REG_TEMP_DATA_0      0xFD
#define ICP_20100_REG_TEMP_DATA_1      0xFE
#define ICP_20100_REG_TEMP_DATA_2      0xFF

class PTSolns_I2Connect_ICP_20100 {
public:
    PTSolns_I2Connect_ICP_20100();

    // Initialization
    bool begin(uint8_t address = ICP_20100_DEFAULT_ADDR, TwoWire &wirePort = Wire);
    
    // Core Data Reading
    bool readSensor(float* pressure, float* temperature);
    
    // Configuration
    void setOperationMode(uint8_t mode);
    void configFilter(bool enable); // For FIR/IIR filtering
    
    // FIFO Management
    void flushFIFO();
    uint8_t getFIFOLevel();
    
    // Interrupt Management
    void configInterrupt(bool watermarkHigh, bool watermarkLow, bool dataReady);

    // Sets the high and low watermark levels (0-15)
    void setFIFOWatermark(uint8_t highWatermark, uint8_t lowWatermark);
    
    // Clears the interrupt status flags (Write-1-to-Clear)
    void clearInterrupts();

private:
    uint8_t _address;
    TwoWire *_i2cPort;

    // Low-level I2C helpers
    bool writeRegister(uint8_t reg, uint8_t data);
    uint8_t readRegister(uint8_t reg);
    bool readRegisters(uint8_t reg, uint8_t* buffer, uint8_t length);
    
    // conversion helpers
    float convertPressure(int32_t rawPressure);
    float convertTemperature(int32_t rawTemperature);
};

#endif // PTSOLNS_I2CONNECT_ICP_20100_H