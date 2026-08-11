#include "PTSolns_I2Connect_ICP_20100.h"

PTSolns_I2Connect_ICP_20100::PTSolns_I2Connect_ICP_20100() {
    _address = ICP_20100_DEFAULT_ADDR;
    _i2cPort = &Wire;
}

bool PTSolns_I2Connect_ICP_20100::begin(uint8_t address, TwoWire &wirePort) {
    _address = address;
    _i2cPort = &wirePort;
    
    // Dummy read to wake up the I2C interface.(recommended)
    readRegister(ICP_20100_REG_DEVICE_ID);
    delay(10);

    // Verify Device ID (Expected: 0x63)
    uint8_t deviceId = readRegister(ICP_20100_REG_DEVICE_ID);
    if (deviceId != 0x63) {
        return false;
    }
    
    // Flush any residual data in FIFO
    flushFIFO();
    
    return true;
}

bool PTSolns_I2Connect_ICP_20100::readSensor(float* pressure, float* temperature) {
    uint8_t buffer[6];
    
    // Read 6 bytes starting from PRESS_DATA_0 (0xFA) to TEMP_DATA_2 (0xFF)
    if (!readRegisters(ICP_20100_REG_PRESS_DATA_0, buffer, 6)) {
        return false;
    }

    // Parse 20-bit Pressure Data (buffer[0] = LSB, buffer[2] = MSB)
    int32_t rawPress = ((uint32_t)(buffer[2] & 0x0F) << 16) | ((uint32_t)buffer[1] << 8) | buffer[0];
    
    // Parse 20-bit Temperature Data (buffer[3] = LSB, buffer[5] = MSB)
    int32_t rawTemp = ((uint32_t)(buffer[5] & 0x0F) << 16) | ((uint32_t)buffer[4] << 8) | buffer[3];

    // Sign extend 20-bit two's complement values to 32-bit integers
    if (rawPress & 0x080000) rawPress |= 0xFFF00000;
    if (rawTemp & 0x080000) rawTemp |= 0xFFF00000;

    *pressure = convertPressure(rawPress);
    *temperature = convertTemperature(rawTemp);

    return true;
}

void PTSolns_I2Connect_ICP_20100::setOperationMode(uint8_t mode) {
    if (mode > 4) mode = 4; // Cap at Mode 4

    // Register 0xC0 (MODE_SELECT)
    // Bits 7:5 = MEAS_CONFIG (Mode)
    // Bit 3 = MEAS_MODE (1 = Continuous)
    // Bit 2 = POWER_MODE (0 = Normal/Standby)
    uint8_t modeSelect = (mode << 5) | (1 << 3); 
    
    writeRegister(ICP_20100_REG_MODE_SELECT, modeSelect);
}

void PTSolns_I2Connect_ICP_20100::configFilter(bool enable) {
    // Placeholder for IIR filter setup (requires specific timing and register writes
    // outlined in the User Configurable Operation Mode application note).
    // Can be expanded as needed for specific glitch rejection profiles.
}

void PTSolns_I2Connect_ICP_20100::flushFIFO() {
    // Write 0x80 to FIFO_FILL to flush
    writeRegister(ICP_20100_REG_FIFO_FILL, 0x80);
}

uint8_t PTSolns_I2Connect_ICP_20100::getFIFOLevel() {
    uint8_t fillStatus = readRegister(ICP_20100_REG_FIFO_FILL);
    return (fillStatus & 0x1F); // Bits 4:0 represent FIFO fill level (0 to 16)
}

void PTSolns_I2Connect_ICP_20100::configInterrupt(bool watermarkHigh, bool watermarkLow, bool dataReady) {
    uint8_t mask = 0xFF; // Start with all interrupts masked (1 = masked)
    
    if (watermarkHigh) mask &= ~(1 << 2); // Unmask FIFO_WMK_HIGH
    if (watermarkLow) mask &= ~(1 << 3);  // Unmask FIFO_WMK_LOW
    
    writeRegister(ICP_20100_REG_INTERRUPT_MASK, mask);
}

void PTSolns_I2Connect_ICP_20100::setFIFOWatermark(uint8_t highWatermark, uint8_t lowWatermark) {
    // Bits 7:4 are High Watermark, Bits 3:0 are Low Watermark
    uint8_t config = ((highWatermark & 0x0F) << 4) | (lowWatermark & 0x0F);
    writeRegister(ICP_20100_REG_FIFO_CONFIG, config);
}

void PTSolns_I2Connect_ICP_20100::clearInterrupts() {
    // The ICP-20100 uses Write-1-to-Clear (W1C) for its interrupt status register
    writeRegister(ICP_20100_REG_INTERRUPT_STATUS, 0xFF); 
}

// Conversion Helpers

float PTSolns_I2Connect_ICP_20100::convertPressure(int32_t rawPressure) {
    // Formula: P = (P_out / 2^17) * 40 kPa + 70 kPa
    return ((float)rawPressure / 131072.0f) * 40.0f + 70.0f;
}

float PTSolns_I2Connect_ICP_20100::convertTemperature(int32_t rawTemperature) {
    // Formula: T = (T_out / 2^18) * 65 C + 25 C
    return ((float)rawTemperature / 262144.0f) * 65.0f + 25.0f;
}

// Low-Level I2C Functions

bool PTSolns_I2Connect_ICP_20100::writeRegister(uint8_t reg, uint8_t data) {
    _i2cPort->beginTransmission(_address);
    _i2cPort->write(reg);
    _i2cPort->write(data);
    return (_i2cPort->endTransmission() == 0);
}

uint8_t PTSolns_I2Connect_ICP_20100::readRegister(uint8_t reg) {
    _i2cPort->beginTransmission(_address);
    _i2cPort->write(reg);
    _i2cPort->endTransmission(false);
    
    _i2cPort->requestFrom(_address, (uint8_t)1);
    if (_i2cPort->available()) {
        return _i2cPort->read();
    }
    return 0;
}

bool PTSolns_I2Connect_ICP_20100::readRegisters(uint8_t reg, uint8_t* buffer, uint8_t length) {
    _i2cPort->beginTransmission(_address);
    _i2cPort->write(reg);
    if (_i2cPort->endTransmission(false) != 0) {
        return false;
    }
    
    _i2cPort->requestFrom(_address, length);
    for (uint8_t i = 0; i < length; i++) {
        if (_i2cPort->available()) {
            buffer[i] = _i2cPort->read();
        } else {
            return false;
        }
    }
    return true;
}