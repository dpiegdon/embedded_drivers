
Collection of different hardware drivers for embedded systems
=============================================================

Contains
--------

This module consists of drivers for:

* Invensense MPU-9250 -- Nine-Axis (Gyro + Accelerometer + Compass) MEMS MotionTracking Device
* Solomon Systech SSD1306 -- 128x64 Dot Matrix OLED Display + Controller
* Silicon Labs Si7020 -- I2C Humidity and Temperature Sensor

In the subdiretory @nrfx/@, it also contains glue logic, ports and drivers specific to NRFX,
a driver suite specific to microcontroller of Nordic Semi (e.g. the NRF52840).
See https://github.com/NordicSemiconductor/nrfx

* nrfx/glue.cpp -- Glue logic to nrfx.


Authors
-------

David R. Piegdon <dgit@piegdon.de>


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

