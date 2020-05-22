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

#include "embedded_drivers/si5351_i2c_clockgen.h"

namespace embedded_drivers {

	Si5351I2cClockgenerator::Si5351I2cClockgenerator(

				void * i2cContext,
				bool(*i2cTx)(void * context, uint8_t address, uint8_t const * buffer, size_t len),
				bool(*i2cRx)(void * context, uint8_t address, uint8_t * buffer, size_t len)
			)
		: mI2cContext(i2cContext)
		, mI2cTx(i2cTx)
		, mI2cRx(i2cRx)
		, mAddress(0x60)
	{
	}

	Si5351I2cClockgenerator::~Si5351I2cClockgenerator(void)
	{
	}

	bool Si5351I2cClockgenerator::SysInitCompleted(void)
	{
		bool ret;
		uint8_t value;
		ret = I2cRead(DeviceStatus, value);
		return (ret) ? (0 == (value & SYS_INIT)) : false;
	}

	bool Si5351I2cClockgenerator::PowerDown(void)
	{
		if(!OutputEnable(0xff))
			return false;

		uint8_t clockOff = 0x80;
		for(uint8_t i = Clk0Control; i <= Clk7Control; ++i)
			if(!I2cWrite(i, clockOff))
				return false;

		return true;
	}

	bool Si5351I2cClockgenerator::EnableInterrupts(bool sysInit, bool pllALoss, bool pllBLoss, bool clkinLoss)
	{
		uint8_t val =     (sysInit ? 0 : SYS_INIT_MASK)
				| (pllALoss ? 0 : LOL_A_MASK)
				| (pllBLoss ? 0 : LOL_B_MASK)
				| (clkinLoss ? 0 : LOS_MASK);

		return I2cWrite(InterruptStatusMask, val);
	}

	bool Si5351I2cClockgenerator::PllALocked(void)
	{
		bool ret;
		uint8_t value;
		ret = I2cRead(DeviceStatus, value);
		return (ret) ? !(value & SYS_INIT) : false;
	}

	bool Si5351I2cClockgenerator::PllBLocked(void)
	{
		bool ret;
		uint8_t value;
		ret = I2cRead(DeviceStatus, value);
		return (ret) ? !(value & SYS_INIT) : false;
	}

	uint8_t Si5351I2cClockgenerator::EncodeDivider(unsigned divider, uint8_t & decoded, bool allowLong)
	{
		switch(divider) {
			case 1:
				decoded = 0;
				return true;
			case 2:
				decoded = 1;
				return true;
			case 4:
				decoded = 2;
				return true;
			case 8:
				decoded = 3;
				return true;
			case 16:
				decoded = 4;
				return allowLong;
			case 32:
				decoded = 5;
				return allowLong;
			case 64:
				decoded = 6;
				return allowLong;
			case 128:
				decoded = 7;
				return allowLong;

			default:
				return false;
		};
	}

	bool Si5351I2cClockgenerator::PllSetInputSource(enum PllSource aSrc, enum PllSource bSrc, uint8_t clockDivider)
	{
		uint8_t value;
		if(!EncodeDivider(clockDivider, value, false))
			return false;
		value <<= CLKIN_DIV_SHIFT;
		value |= ((PLLSRC_CLKIN == bSrc) ? PLLB_SRC : 0) | ((PLLSRC_CLKIN == aSrc) ? PLLA_SRC : 0);
		return I2cWrite(PllInputSource, value);
	}

	bool Si5351I2cClockgenerator::OutputEnable(uint8_t bitmask)
	{
		return I2cWrite(OutputEnableControl, bitmask);
	}

	bool Si5351I2cClockgenerator::OebPinEnable(uint8_t bitmask)
	{
		return I2cWrite(OebPinEnableControlMask, bitmask);
	}

	bool Si5351I2cClockgenerator::ClockSetControl(
				enum Clock clk, bool powerOn, bool integerMode, enum Pll pll,
				bool invert, enum ClockSource clkSource, uint8_t driveStrength)
	{
		uint8_t value;

		uint8_t reg = Clk0Control + clk;
		value =   (powerOn ? 0 : CLK_PDN)
			| (integerMode ? MS_INT : 0)
			| ((pll == PLL_B) ? MS_SRC : 0)
			| (invert ? CLK_INV : 0)
			| (clkSource & 0b11) << CLK_SRC_SHIFT
			| (driveStrength & 0b11) << CLK_IDRV_SHIFT;
		return I2cWrite(reg, value);
	}

	bool Si5351I2cClockgenerator::ResetPll(bool pllA, bool pllB)
	{
		uint8_t value = (pllA ? PLLA_RST : 0) | (pllB ? PLLB_RST : 0);
		return I2cWrite(PllReset, value);
	}

	bool Si5351I2cClockgenerator::EnableFanout(bool clkin, bool xtal, bool multisynt0_4)
	{
		uint8_t value = (clkin ? CLKIN_FANOUT_EN : 0)
				| (xtal ? XO_FANOUT_EN : 0)
				| (multisynt0_4 ? MS_FANOUT_EN : 0);
		return I2cWrite(FanoutEnable, value);
	}

	bool Si5351I2cClockgenerator::GetMultisynthParameter(unsigned a, unsigned b, unsigned c,
							uint32_t & P1, uint32_t & P2, uint32_t & P3)
	{
		P1 = 128 * a + ((128 * b) / c) - 512;
		P2 = 128 * b - c * ((128 * b) / c);
		P3 = c;

		// bounds-check inputs
		if(P1 & ~((1 << 18)-1))
			return false;
		if(P2 & ~((1 << 20)-1))
			return false;
		if(P3 & ~((1 << 20)-1))
			return false;

		return true;
	}

	bool Si5351I2cClockgenerator::PllSetMultisynth(enum Pll pll, unsigned a, unsigned b, unsigned c)
	{
		if(a < 15 || a > 90)
			return false;

		uint32_t MSNA_P1, MSNA_P2, MSNA_P3;
		if(!GetMultisynthParameter(a, b, c, MSNA_P1, MSNA_P2, MSNA_P3))
			return false;

		uint8_t buf[8];
		buf[0] = (MSNA_P3 >>  8) & 0xff;
		buf[1] = (MSNA_P3 >>  0) & 0xff;
		buf[2] = (MSNA_P1 >> 16) & 0x02;
		buf[3] = (MSNA_P1 >>  8) & 0xff;
		buf[4] = (MSNA_P1 >>  0) & 0xff;
		buf[5] = (((MSNA_P3 >> 16) & 0x0f) << 8) | (((MSNA_P2 >> 16) & 0x0f) << 0);
		buf[6] = (MSNA_P2 >>  8) & 0xff;
		buf[7] = (MSNA_P2 >>  0) & 0xff;

		unsigned reg = (pll == PLL_A) ? MultisynthNAParameters0 : MultisynthNBParameters0;
		return I2cWrite(reg, buf);
	}

	bool Si5351I2cClockgenerator::ClockSetMultisynth(enum Clock clk, unsigned a, unsigned b, unsigned c,
				unsigned r, uint8_t phase)
	{
		if(clk <= CLOCK_5) {
			if(a < 8 || a > 900)
				return false;

			uint32_t MSNA_P1, MSNA_P2, MSNA_P3;
			if(!GetMultisynthParameter(a, b, c, MSNA_P1, MSNA_P2, MSNA_P3))
				return false;

			uint8_t div;
			if(!EncodeDivider(r, div, true))
				return false;

			uint8_t buf[8];
			buf[0] = (MSNA_P3 >>  8) & 0xff;
			buf[1] = (MSNA_P3 >>  0) & 0xff;
			buf[2] = ((MSNA_P1 >> 16) & 0x02) | (div << 4);
			buf[3] = (MSNA_P1 >>  8) & 0xff;
			buf[4] = (MSNA_P1 >>  0) & 0xff;
			buf[5] = (((MSNA_P3 >> 16) & 0x0f) << 8) | (((MSNA_P2 >> 16) & 0x0f) << 0);
			buf[6] = (MSNA_P2 >>  8) & 0xff;
			buf[7] = (MSNA_P2 >>  0) & 0xff;

			unsigned reg = Multisynth0Parameters0 + 8 * clk;
			if(!I2cWrite(reg, buf))
				return false;

			return I2cWrite(Clk0InitialPhaseOffset + clk, phase);
		} else {
			// out6 and out7 dividers and phase are restricted
			// FIXME
			return false;
		}
	}

	bool Si5351I2cClockgenerator::VcxoSetParameters(uint8_t const params[3])
	{
		return I2cWrite(VcxoParameter0, params);
	}

} // end of namespace embedded_drivers

