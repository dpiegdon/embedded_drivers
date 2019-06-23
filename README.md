
Collection of different hardware drivers for embedded systems
=============================================================

Contains
--------

This module consists of drivers for:

* Invensense MPU-9250 -- Nine-Axis (Gyro + Accelerometer + Compass) MEMS MotionTracking Device
* Solomon Systech SSD1306 -- 128x64 Dot Matrix OLED Display + Controller
* Silicon Labs Si7020 -- I2C Humidity and Temperature Sensor

It also contains:

* NRFX glue logic -- tools and helper functions to easily use these drivers with the Noric Semi nrfx drivers. See https://github.com/NordicSemiconductor/nrfx


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

