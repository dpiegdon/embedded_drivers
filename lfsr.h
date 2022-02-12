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

#ifdef LFSR_USE_LEGACY_C_HEADER
# include <stdint.h>
#else
# include <cstdint>
#endif

namespace embedded_drivers {

	// Implements a generic linear feedback shift register that allows to shift
	// additional random bits into the front to improve its randomness.
	template <class T, unsigned int WIDTH, T INIT_VALUE, T FEEDBACK>
	class Lfsr {
	public:
		typedef T BaseType;
		static const unsigned int cWidth = WIDTH;

		Lfsr(BaseType initial_state=INIT_VALUE)
		{
			static_assert(sizeof(BaseType)*8 >= cWidth,
				"BaseType is not large enough to represent full LFSR state.");
			static_assert(cWidth > 0,
				"Width must be larger than zero.");
			mShiftReg = initial_state;
		}

		BaseType Iterate(unsigned count=1, BaseType input=0)
		{
			assert(count <= (8 * sizeof(BaseType)));

			BaseType output = 0;
			mShiftReg ^= input;
			for (unsigned i = 0; i < count; ++i) {
				bool feedback = mShiftReg & 1;

				mShiftReg >>= 1;
				output >>= 1;
				if (feedback) {
					mShiftReg ^= FEEDBACK;
					output |= (1ULL << (8 * sizeof(BaseType) - 1));
				}
			}
			output >>= (8 * sizeof(BaseType)) - count;
			return output;
		}

		BaseType GetRegisterState(void)
		{
			return mShiftReg;
		}

	private:
		BaseType mShiftReg;
	};

	// Several default LFSR implementations with maximal period, as per http://users.ece.cmu.edu/~koopman/lfsr/
	typedef Lfsr<uint8_t,   4, 0b1,    0xC>           LfsrDefault4;
	typedef Lfsr<uint8_t,   8, 0b1,    0x8E>          LfsrDefault8;
	typedef Lfsr<uint16_t,  9, 0b1,    0x108>         LfsrDefault9;
	typedef Lfsr<uint16_t, 10, 0b1,    0x204>         LfsrDefault10;
	typedef Lfsr<uint16_t, 15, 0b1,    0x4001>        LfsrDefault15;
	typedef Lfsr<uint16_t, 16, 0xACE1, 0xB400>        LfsrFibonacci;
	typedef Lfsr<uint16_t, 16, 0b1,    0x8016>        LfsrDefault16;
	typedef Lfsr<uint32_t, 17, 0b1,    0x10004>       LfsrDefault17;
	typedef Lfsr<uint32_t, 18, 0b1,    0x2001F>       LfsrDefault18;
	typedef Lfsr<uint32_t, 24, 0b1,    0x80000D>      LfsrDefault24;
	typedef Lfsr<uint32_t, 32, 0b1,    0x80000057LLU> LfsrDefault32;

} // end of namespace embedded_drivers
