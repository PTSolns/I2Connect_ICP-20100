# PTSolns *I2Connect: ICP-20100* Library

![Arduino Library](https://img.shields.io/badge/Arduino-Library-blue)
![License](https://img.shields.io/badge/License-MIT-green)
![Sensor](https://img.shields.io/badge/Sensor-Barometric_Pressure_&_Temperature-orange)

Library for the PTSolns *I2Connect: ICP-20100* module. Available through both the Arduino Library Manager and the PTSolns IDE Library Manager, this library provides a streamlined interface to read high-accuracy barometric pressure and temperature data, utilize onboard FIFO buffers, and configure hardware interrupts while automatically handling I2C communication.

---

## Overview

The *I2Connect: ICP-20100* is a high-accuracy, low-power barometric pressure and temperature sensor solution. Designed as part of the I2Connect Series, this module utilizes a capacitive pressure sensor for monitoring pressure changes in the range of 30 to 110 kPa.

The sensor integrates a DSP module for on-chip calibration, an Analog-to-Digital converter (ADC) delivering up to 20-bits of resolution, and programmable digital filtering. It features a 96-byte FIFO buffer that enables the application to read up to 16 pressure-temperature pairs in a burst. The module features an onboard voltage regulator, making it fully compatible with both 3.3V and 5V systems out of the box. This allows it to be used with a wide range of popular microcontroller platforms without the need for external level shifters or power step-downs.

Like all I2Connect modules, it connects using dual Qwiic® compatible connectors to easily integrate into existing I2C systems without soldering. The preinstalled right-angle header provides direct access to the I2C bus and the hardware interrupt (`INT`) pin for breadboard-based prototyping.

---

## *I2Connect* Series

The PTSolns *I2Connect* Series is a family of plug and play expansion modules designed to simplify the integration of sensors, displays, user interfaces, and other peripherals into embedded systems. Built around the industry standard I2C communication protocol and featuring convenient Qwiic® compatible connectors (4 pin, 1 mm JST SH), *I2Connect* modules can be quickly connected without soldering or complex wiring.

Each module is fully assembled and includes a preinstalled male header that is compatible with standard solderless breadboards, enabling immediate use and rapid prototyping. Designed to work seamlessly with a wide range of microcontroller platforms, I2Connect modules combine compact hardware, comprehensive documentation, and easy to use software libraries. The _I2Connect_ Series enables makers, students, educators, and professionals to rapidly prototype, evaluate, and develop reliable electronic systems.

The following modules are part of the *I2Connect* Series:

* [**AHT20**](https://ptsolns.com/products/i2connect-aht20) — Temperature & Humidity
* [**VEML7700**](https://ptsolns.com/products/i2connect-veml7700) — Light & Lux
* [**EEPROM**](https://ptsolns.com/products/i2connect-eeprom) — 2 Mbit Memory
* **Buttons** — User Input via 4×4 Matrix of Buttons
* **Digit Display** — Dual 7 Segment Display
* **ICP-20100** — Pressure & Temperature
* **KXTJ3-1057** — 3 Axis Accelerometer
* **MUX** — 4 Channel I2C Multiplexer
* **OLED** — 0.96" 128×64 OLED Display
* **I2C Doctor** — I2C Bus Recovery and Diagnostics
* **...and more!**

---

## Key Features

* High-accuracy capacitive barometric pressure and temperature sensor.
* Functional pressure range of 30 to 110 kPa.
* Operating temperature range of -40°C to 85°C.
* Up to 20-bit output data resolution.
* 96-byte FIFO buffer enabling the storage of up to 16 pressure-temperature measurement pairs.
* Programmable digital filtering, including Finite Impulse Response (FIR) for improved noise performance and Infinite Impulse Response (IIR) for filtering sudden pressure glitches.
* Hardware `INT` pin configured for reliable active-low interrupt signaling to trigger custom host alerts.
* Default I2C address of 0x63. Alternative address of 0x64 accessible via jumper pad.
* Onboard voltage regulator** ensuring safe 3.3V and 5V logic and power compatibility.
* Dual Qwiic® compatible 4-pin JST SH connectors for daisy-chaining.
* Breadboard-friendly 5-pin 2.54mm angled male header (GND, 3V3, SDA, SCL, INT).

---

## I2Connect Ecosystem

The *I2Connect: ICP-20100* is part of the PTSolns I2Connect ecosystem.

All *I2Connect* modules utilize a standardized 4 pin JST SH 1.0 mm connector carrying:

| Pin | Function |
| --- | -------- |
| 1   | GND      |
| 2   | VCC      |
| 3   | SDA      |
| 4   | SCL      |

This connector follows the widely adopted Qwiic® standard, allowing modules and compatible boards to be connected without soldering.

For maximum compatibility, the module also includes a preinstalled angled male header, allowing use with virtually any 3.3 V or 5 V microcontroller platform. The male header on this module also breaks out the hardware Interrupt (`INT`) pin.

---

## Sensor Specifications

| Parameter               | Value                                      |
| ----------------------- | ------------------------------------------ |
| Sensor IC               | TDK InvenSense ICP-20100                   |
| Pressure Range          | 30 to 110 kPa                              |
| Temperature Range       | -40°C to 85°C                              |
| Interface               | I2C, I3CSM, SPI                            |
| I2C Address             | 0x63 (Default) / 0x64 (Alt)                |
| Operating Voltage       | 3.3 V / 5 V Rated (Onboard Regulator)      |
| Communication Speed     | Up to 1 MHz (I2C)                          |
| Resolutions             | Up to 20-bit                               |
| Header Pinout           | INT, SCL, SDA, 3V3, GND                    |
| Connectors              | 2x 4-pin 1mm JST SH                        |

---

## I2C Addressing & Multiplexing

The *I2Connect: ICP-20100* module uses a default I2C address of:

```text
0x63
```

This address can be modified to 0x64 by adjusting the ADDR solder pad on the rear of the module PCB. If more than two pressure modules are required in the same system, an I2C multiplexer must be used.
The PTSolns I2Connect: MUX module provides four independent I2C channels, allowing multiple identically-addressed modules to be connected to a single microcontroller.

## *PTSolns IDE*

The [*PTSolns IDE*](https://ptsolns.com/ide) is a free desktop application for programming PTSolns microcontroller development boards and compatible Arduino platforms.

This library is available directly through both the PTSolns IDE Library Manager and the Arduino Library Manager.

To install:

1. Open the PTSolns IDE (or Arduino IDE).
2. Open the Library Manager.
3. Search for:

```text
I2Connect_ICP_20100
```

4. Click Install.

Features of the [*PTSolns IDE*](https://ptsolns.com/ide) include:

* Integrated board management
* Built in serial monitor
* Library management
* Example project browser
* Support for over 8,500 third party libraries
* Built in CH340 USB driver support
* Open source and community driven

The [*PTSolns IDE*](https://ptsolns.com/ide) can be used alongside the Arduino IDE and supports many PTSolns development boards out of the box.

---
