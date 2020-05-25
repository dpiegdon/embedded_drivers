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

#include <cstddef>
#include <cstdint>
#include <cassert>
#include <cstring>
#include <vector>

namespace embedded_drivers {

	class Si5351I2cClockgenerator {
		/* Driver for the Silicon Labs Si5351A/B/C-B CMOS clock generator. */

	public:
		Si5351I2cClockgenerator(
				void * i2cContext,
				bool(*i2cTx)(void * context, uint8_t address, uint8_t const * buffer, size_t len),
				bool(*i2cRx)(void * context, uint8_t address, uint8_t * buffer, size_t len));

		~Si5351I2cClockgenerator(void);

		enum Pll {
			PLL_A = 0,
			PLL_B = 1
		};

		enum PllSource {
			PLLSRC_XTAL = 0,
			PLLSRC_CLKIN = 1
		};

		enum Clock {
			CLOCK_0 = 0,
			CLOCK_1 = 1,
			CLOCK_2 = 2,
			CLOCK_3 = 3,
			CLOCK_4 = 4,
			CLOCK_5 = 5,
			CLOCK_6 = 6,
			CLOCK_7 = 7
		};

		enum ClockSource {
			CLKSRC_XTAL = 0,
			CLKSRC_CLKIN = 1,
			CLKSRC_OTHER = 2,
			CLKSRC_MULTISYNTH_N = 3
		};

		bool SysInitCompleted(void);
		bool PowerDown(void);
		bool EnableInterrupts(bool sysInit, bool pllALoss, bool pllBLoss, bool clkinLoss);
		bool EnableFanout(bool clkin, bool xtal, bool multisynt0_4);
		bool PllSetInputSource(enum PllSource aSrc, enum PllSource bSrc, uint8_t clockDivider);
		bool PllSetMultisynth(enum Pll pll, unsigned a, unsigned b, unsigned c);
		bool ResetPll(bool pllA, bool pllB);
		bool PllALocked(void);
		bool PllBLocked(void);
		bool ClockSetControl(enum Clock clk, bool powerOn, bool integerMode, enum Pll pll,
				bool invert, enum ClockSource clkSource, uint8_t driveStrength);
		bool ClockSetMultisynth(enum Clock clk, unsigned a, unsigned b, unsigned c,
				unsigned r, uint8_t phase);
		bool OutputEnable(uint8_t bitmask);
		bool OebPinEnable(uint8_t bitmask);

		/*
		OutputSetSpreadSpectrum(output, ...)		(only if output uses PLLA)
		*/

		uint8_t EncodeDivider(unsigned divider, uint8_t & decoded, bool allowLong);
		bool VcxoSetParameters(uint8_t const params[3]);

		template<class T>
		bool I2cWrite(const uint8_t startreg, const T & value)
		{
			uint8_t buf[1 + sizeof(T)];
			buf[0] = startreg;
			memcpy(buf+1, &value, sizeof(value));
			return mI2cTx(mI2cContext, mAddress, buf, sizeof(buf));
		}

		template<class T>
		bool I2cRead(const uint8_t startreg, T & value)
		{
			if(!mI2cTx(mI2cContext, mAddress, &startreg, sizeof(startreg)))
				return false;
			return mI2cRx(mI2cContext, mAddress, &value, sizeof(value));
		}

		enum Register {
			DeviceStatus = 0,
			InterruptStatusSticky = 1,
			InterruptStatusMask = 2,
			OutputEnableControl = 3,
			OebPinEnableControlMask = 9,
			PllInputSource = 15,
			Clk0Control = 16,
			Clk1Control = 17,
			Clk2Control = 18,
			Clk3Control = 19,
			Clk4Control = 20,
			Clk5Control = 21,
			Clk6Control = 22,
			Clk7Control = 23,
			Clk3_0_DisableState = 24,
			Clk7_4_DisableState = 25,
			MultisynthNAParameters0 = 26,
			MultisynthNAParameters1 = 27,
			MultisynthNAParameters2 = 28,
			MultisynthNAParameters3 = 29,
			MultisynthNAParameters4 = 30,
			MultisynthNAParameters5 = 31,
			MultisynthNAParameters6 = 32,
			MultisynthNAParameters7 = 33,
			MultisynthNBParameters0 = 34,
			MultisynthNBParameters1 = 35,
			MultisynthNBParameters2 = 36,
			MultisynthNBParameters3 = 37,
			MultisynthNBParameters4 = 38,
			MultisynthNBParameters5 = 39,
			MultisynthNBParameters6 = 40,
			MultisynthNBParameters7 = 41,
			Multisynth0Parameters0 = 42,
			Multisynth0Parameters1 = 43,
			Multisynth0Parameters2 = 44,
			Multisynth0Parameters3 = 45,
			Multisynth0Parameters4 = 46,
			Multisynth0Parameters5 = 47,
			Multisynth0Parameters6 = 48,
			Multisynth0Parameters7 = 49,
			Multisynth1Parameters0 = 50,
			Multisynth1Parameters1 = 51,
			Multisynth1Parameters2 = 52,
			Multisynth1Parameters3 = 53,
			Multisynth1Parameters4 = 54,
			Multisynth1Parameters5 = 55,
			Multisynth1Parameters6 = 56,
			Multisynth1Parameters7 = 57,
			Multisynth2Parameters0 = 58,
			Multisynth2Parameters1 = 59,
			Multisynth2Parameters2 = 60,
			Multisynth2Parameters3 = 61,
			Multisynth2Parameters4 = 62,
			Multisynth2Parameters5 = 63,
			Multisynth2Parameters6 = 64,
			Multisynth2Parameters7 = 65,
			Multisynth3Parameters0 = 66,
			Multisynth3Parameters1 = 67,
			Multisynth3Parameters2 = 68,
			Multisynth3Parameters3 = 69,
			Multisynth3Parameters4 = 70,
			Multisynth3Parameters5 = 71,
			Multisynth3Parameters6 = 72,
			Multisynth3Parameters7 = 73,
			Multisynth4Parameters0 = 74,
			Multisynth4Parameters1 = 75,
			Multisynth4Parameters2 = 76,
			Multisynth4Parameters3 = 77,
			Multisynth4Parameters4 = 78,
			Multisynth4Parameters5 = 79,
			Multisynth4Parameters6 = 80,
			Multisynth4Parameters7 = 81,
			Multisynth5Parameters0 = 82,
			Multisynth5Parameters1 = 83,
			Multisynth5Parameters2 = 84,
			Multisynth5Parameters3 = 85,
			Multisynth5Parameters4 = 86,
			Multisynth5Parameters5 = 87,
			Multisynth5Parameters6 = 88,
			Multisynth5Parameters7 = 89,
			Multisynth6Parameters = 90,
			Multisynth7Parameters = 91,
			Clock6_7_OutputDivider = 92,
			SpreadSpectrumParameters0 = 149,
			SpreadSpectrumParameters1 = 150,
			SpreadSpectrumParameters2 = 151,
			SpreadSpectrumParameters3 = 152,
			SpreadSpectrumParameters4 = 153,
			SpreadSpectrumParameters5 = 154,
			SpreadSpectrumParameters6 = 155,
			SpreadSpectrumParameters7 = 156,
			SpreadSpectrumParameters8 = 157,
			SpreadSpectrumParameters9 = 158,
			SpreadSpectrumParametersA = 159,
			SpreadSpectrumParametersB = 160,
			SpreadSpectrumParametersC = 161,
			VcxoParameter0 = 162,
			VcxoParameter1 = 163,
			VcxoParameter2 = 164,
			Clk0InitialPhaseOffset = 165,
			Clk1InitialPhaseOffset = 166,
			Clk2InitialPhaseOffset = 167,
			Clk3InitialPhaseOffset = 168,
			Clk4InitialPhaseOffset = 169,
			Clk5InitialPhaseOffset = 170,
			PllReset = 177,
			CrystalInternalLoadCapacitance = 183,
			FanoutEnable = 187
		};

		// DeviceStatus
		const unsigned SYS_INIT = (1 << 7);
		const unsigned LOL_B = (1 << 6);
		const unsigned LOL_A = (1 << 5);
		const unsigned LOS = (1 << 4);
		const unsigned LOX = (1 << 3); // undocumented "loss of xtal": high if no signal on XA
		const unsigned REV_MASK = (0b11);
		// InterruptStatusMask
		const unsigned SYS_INIT_MASK = (1 << 7);
		const unsigned LOL_B_MASK = (1 << 6);
		const unsigned LOL_A_MASK = (1 << 5);
		const unsigned LOS_MASK = (1 << 4);
		// PllInputSource
		const unsigned PLLB_SRC = (1 << 3);
		const unsigned PLLA_SRC = (1 << 2);
		const unsigned CLKIN_DIV_SHIFT = 6;
		// ClkXControl
		const unsigned CLK_PDN = (1 << 7);
		const unsigned MS_INT = (1 << 6);
		const unsigned MS_SRC = (1 << 5);
		const unsigned CLK_INV = (1 << 4);
		const unsigned CLK_SRC_SHIFT = 2;
		const unsigned CLK_IDRV_SHIFT = 0;
		// PllReset
		const unsigned PLLA_RST = (1 << 5);
		const unsigned PLLB_RST = (1 << 7);
		// FanoutEnable
		const unsigned CLKIN_FANOUT_EN = (1 << 7);
		const unsigned XO_FANOUT_EN = (1 << 6);
		const unsigned MS_FANOUT_EN = (1 << 4);


	private:
		void * mI2cContext;
		bool(*mI2cTx)(void * context, uint8_t address, uint8_t const * buffer, size_t len);
		bool(*mI2cRx)(void * context, uint8_t address, uint8_t * buffer, size_t len);
		uint8_t const mAddress;

		bool GetMultisynthParameter(unsigned a, unsigned b, unsigned c,
				uint32_t & P1, uint32_t & P2, uint32_t & P3);
	};

} // end of namespace embedded_drivers

