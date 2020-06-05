/*
    This file is part of embedded_drivers.

    embedded_drivers is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    embedded_drivers is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with embedded_drivers.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <cstring>
#include <cstdint>
#include <utility>
#include <machine/endian.h>

namespace embedded_drivers {

	class Mcp9808I2cSensor {
		/* Driver for the Microchip MCP9808 temperature sensor.
		 * (Should also work for MCP9804.)
		 */

	public:
		Mcp9808I2cSensor(void * i2cContext,
				bool(*i2cTx)(void * context, uint8_t address, const uint8_t * buffer, size_t len),
				bool(*i2cRx)(void * context, uint8_t address, uint8_t * buffer, size_t len),
				unsigned a2a1a0,
				unsigned addressRemainder = 0x18);
		~Mcp9808I2cSensor(void);

		enum Register {
			Config = 1,
			AlertBoundUpper = 2,
			AlertBoundLower = 3,
			CriticalTemperature = 4,
			Temperature = 5,
			ManufacturerId = 6,
			DeviceId = 7,
			Resolution = 8,
		};

		bool I2cWrite(const uint8_t reg, uint16_t value);
		bool I2cRead(const uint8_t reg, uint16_t & value);
		bool ReadTemperature(float & temperature_celsius);

	private:
		uint8_t const mAddress;

		void * mI2cContext;
		bool(*mI2cTx)(void * context, uint8_t address, const uint8_t * buffer, size_t len);
		bool(*mI2cRx)(void * context, uint8_t address, uint8_t * buffer, size_t len);
	};

} // end of namespace embedded_drivers

