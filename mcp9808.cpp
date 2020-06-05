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

#include "machine/endian.h"
#include "embedded_drivers/mcp9808.h"

namespace embedded_drivers {

	Mcp9808I2cSensor::Mcp9808I2cSensor(void * i2cContext,
				bool(*i2cTx)(void * context, uint8_t address, const uint8_t * buffer, size_t len),
				bool(*i2cRx)(void * context, uint8_t address, uint8_t * buffer, size_t len),
				unsigned a2a1a0,
				unsigned addressRemainder)
		: mAddress(addressRemainder | (a2a1a0 & 0b111))
		, mI2cContext(i2cContext)
		, mI2cTx(i2cTx)
		, mI2cRx(i2cRx)
	{
	}

	Mcp9808I2cSensor::~Mcp9808I2cSensor(void)
	{
	}


	bool Mcp9808I2cSensor::I2cWrite(const uint8_t reg, uint16_t value)
	{
		uint8_t buf[3];
		value = __htons(value);
		buf[0] = reg;
		memcpy(buf+1, &value, sizeof(value));

		return mI2cTx(mI2cContext, mAddress, buf, sizeof(buf));
	}

	bool Mcp9808I2cSensor::I2cRead(const uint8_t reg, uint16_t & value)
	{
		if(!mI2cTx(mI2cContext, mAddress, &reg, sizeof(reg)))
			return false;
		bool ret = mI2cRx(mI2cContext, mAddress, (uint8_t*)&value, sizeof(value));

		value = __ntohs(value);
		return ret;
	}

	bool Mcp9808I2cSensor::ReadTemperature(float & temperature_celsius)
	{
		uint16_t reg;
		if(!I2cRead(Temperature, reg))
			return false;

		temperature_celsius = float(reg & 0x0fff) / 16.;

		if(reg & 0x1000)
			temperature_celsius -= 256;
		return true;
	}


} // end of namespace embedded_drivers

