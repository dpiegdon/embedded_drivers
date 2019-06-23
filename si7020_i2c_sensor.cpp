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

#include "embedded_drivers/si7020_i2c_sensor.h"

namespace embedded_drivers {

	Si7020I2cSensor::Si7020I2cSensor(void * i2cContext,
				bool(*i2cTx)(void * context, uint8_t address, const uint8_t * buffer, size_t len),
				bool(*i2cRx)(void * context, uint8_t address, uint8_t * buffer, size_t len))
		: mAddress(0x40)
		, mI2cContext(i2cContext)
		, mI2cTx(i2cTx)
		, mI2cRx(i2cRx)
	{
	}

	Si7020I2cSensor::~Si7020I2cSensor(void)
	{
	}

	float Si7020I2cSensor::RHCodeToHumidity(uint16_t rhcode)
	{ return 125. * __ntohs(rhcode) / 65536. - 6.; }

	float Si7020I2cSensor::TempCodeToTemperature(uint16_t tempcode)
	{ return 175.72 * __ntohs(tempcode) / 65536. - 46.85; }

	float Si7020I2cSensor::ReadHumidity(void)
	{
		uint8_t txbuf = Command_MeasureRelativeHumidity_HoldMaster;
		uint16_t rxbuf;

		if(mI2cTx(mI2cContext, mAddress, &txbuf, sizeof(txbuf)))
			if(mI2cRx(mI2cContext, mAddress, (uint8_t*)&rxbuf, sizeof(rxbuf)))
				return RHCodeToHumidity(rxbuf);

		return -1.;
	}

	float Si7020I2cSensor::ReadTemperature(void)
	{
		uint8_t txbuf = Command_MeasureTemperature_HoldMaster;
		uint16_t rxbuf;

		if(mI2cTx(mI2cContext, mAddress, &txbuf, sizeof(txbuf)))
			if(mI2cRx(mI2cContext, mAddress, (uint8_t*)&rxbuf, sizeof(rxbuf)))
				return TempCodeToTemperature(rxbuf);

		return -999.;
	}

	std::pair<float, float> Si7020I2cSensor::ReadHumidityTemperature(void)
	{
		float humidity = -1.;
		float temperature = -999.;
		uint8_t txbuf;
		uint16_t rxbuf;

		txbuf = Command_MeasureRelativeHumidity_HoldMaster;

		if(mI2cTx(mI2cContext, mAddress, &txbuf, sizeof(txbuf)))
			if(mI2cRx(mI2cContext, mAddress, (uint8_t*)&rxbuf, sizeof(rxbuf)))
				humidity = RHCodeToHumidity(rxbuf);

		txbuf = Command_ReadTempFromRHMeasurement;
		if(mI2cTx(mI2cContext, mAddress, &txbuf, sizeof(txbuf)))
			if(mI2cRx(mI2cContext, mAddress, (uint8_t*)&rxbuf, sizeof(rxbuf)))
				temperature = TempCodeToTemperature(rxbuf);

		return std::pair<float,float>(humidity, temperature);
	}

} // end of namespace embedded_drivers

