
Collection of different hardware drivers for embedded systems
=============================================================

Contains
--------

This module consists of drivers for:

* AD5761[R] + AD5721[R] -- Analog Devices, SPI, DAC
* ARM tracing support routines (WIP)
* LFSR -- Abstract linear feedback shift register
* MCP9804 + MCP9808 -- Microchip, I2C, temperature sensor
* MPU9250 -- Invensense, I2C, Nine-Axis (Gyro + Accelerometer + Compass) MEMS MotionTracking Device
* SI5351 -- Silicon Labs, I2C, Programmable Clock Generator + VCXO
* SI7020 -- Silicon Labs, I2C, Humidity and Temperature Sensor
* SSD1306 -- Solomon Systech, I2C, 128x64 Dot Matrix OLED Display + Controller

In the subdiretory `nrfx/`, it also contains glue logic, ports and drivers specific to NRFX,
a driver suite specific to microcontroller of Nordic Semi (e.g. the NRF52840).
See https://github.com/NordicSemiconductor/nrfx

* nrfx/glue -- Glue logic to nrfx
* nrfx/lfsr_rng -- True RNG-supported LFSR for fast random number generation
* nrfx/tracing_nrf52840 -- Tracing support routines (WIP)
* nrfx/uarte -- UARTE abstraction


Coding Style
------------

To keep some consistency, the following guidelines hold,
unless the readability is impaired.

Naming:

* Classnames should be `CamelCase`, with the first letter upper.
* Types should be `CamelCase`, with the first letter upper.
* Variables should be `snake_case`.

Indentation:

* Indentation and bracing should roughly follow linux-kernel-style (`indent -kr -i8`).
* Each `case` statement is indented inside a switch-brace.
* Code belonging to a `case` again is indented further.
* Single-line blocks may omit curly braces.
* `} else {` on a single line.

Where-to-do-what:

* Documentation of function use/API belongs in the header. (Unless C++ modules are used.)
* Everything should be inside `namespace embedded_drivers`.
* Constants should be const member variables, not defines.
* Global variables should not exist, except in monoid-style hardware abstractions (like `nrfx_lfsr`) or constant definitions (like fonts).

Target:

* C++17 and C11 are assumed. Less *may* work.
* Driver implementations should be MCU-agnostic.
* MCU-specific interfaces + implementations are collected in a subfolder, e.g. `nrfx` for nordic's MCUs.


Authors
-------

`font_tama_mini02.cpp` -- Authors unknown, see file itself.

Everything else:

* David R. Piegdon <dgit@piegdon.de>


Licensing
---------

License exceptions:

`font_tama_mini02.cpp` -- unknown license, probably public domain.

All OTHER contained files are licensed under the LGPL v3.0, see LICENSE.txt .
That means that you may use the provided drivers/files in a proprietary
software without publishing your proprietary code.
But you must publish any changes that you did to the provided drivers/files.
I suggest that you include this repository as a submodule.
That way you can easily publish any changes separately from your own code.

