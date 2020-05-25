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

#include <cstdint>
#include <utility>

namespace embedded_drivers {

	class Si7020I2cSensor {
		/* Driver for the Silicon Labs Si7021-A20 i2c humidity and temperature sensor. */

	public:
		Si7020I2cSensor(void * i2cContext,
				bool(*i2cTx)(void * context, uint8_t address, const uint8_t * buffer, size_t len),
				bool(*i2cRx)(void * context, uint8_t address, uint8_t * buffer, size_t len));
		~Si7020I2cSensor(void);

		float ReadHumidity(void);
		float ReadTemperature(void);
		std::pair<float, float> ReadHumidityTemperature(void);

	private:
		float TempCodeToTemperature(uint16_t tempcode);
		float RHCodeToHumidity(uint16_t rhcode);

		const uint8_t Command_MeasureRelativeHumidity_HoldMaster = 0xe5;
		const uint8_t Command_MeasureRelativeHumidity_NoHoldMaster = 0xf5;
		const uint8_t Command_MeasureTemperature_HoldMaster = 0xe3;
		const uint8_t Command_MeasureTemperature_NoHoldMaster = 0xf3;
		const uint8_t Command_ReadTempFromRHMeasurement = 0xe0;
		const uint8_t Command_Reset = 0xfe;
		const uint8_t Command_WriteUserRegister_High = 0xe6;
		const uint8_t Command_WriteUserRegister_Low = 0xe7;
		const uint8_t Command_WriteHeaterControl = 0x51;
		const uint8_t Command_ReadHeaterControl = 0x11;

		uint8_t const mAddress;

		void * mI2cContext;
		bool(*mI2cTx)(void * context, uint8_t address, const uint8_t * buffer, size_t len);
		bool(*mI2cRx)(void * context, uint8_t address, uint8_t * buffer, size_t len);
	};

} // end of namespace embedded_drivers

