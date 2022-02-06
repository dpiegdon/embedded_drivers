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

namespace embedded_drivers {

	// Implements a generic linear feedback shift register that allows to shift
	// additional random bits into the front to improve its randomness.
	template <typename basetype_t, unsigned int WIDTH, basetype_t INIT_VALUE, basetype_t FEEDBACK>
	class Lfsr {
	public:
		Lfsr(basetype_t initial_state=INIT_VALUE)
		{
			static_assert(sizeof(basetype_t)*8 >= WIDTH,
				"basetype_t is not large enough to represent full LFSR state.");
			this->mShiftReg = initial_state;
		}

		template<unsigned int COUNT=1>
		basetype_t Iterate(void)
		{
			static_assert(COUNT <= WIDTH);
			basetype_t out = this->mShiftReg & ((1 << COUNT)-1);

			for (unsigned int i = 0; i < COUNT; ++i) {
				this->Iterate(false);
			}
			return out;
		}

		bool Iterate(bool input)
		{
			bool out = this->mShiftReg & 1;

			basetype_t feedback = this->mShiftReg & FEEDBACK;
			if (input)
				feedback = feedback ^ 1;
			while (feedback > 1)
				feedback = (feedback >> 1) ^ (feedback & 1);

			this->mShiftReg >>= 1;
			this->mShiftReg |= feedback << (WIDTH-1);

			return out;
		}

		template <typename intype_t>
		bool Iterate(intype_t input, unsigned bits)
		{
			bool out;
			while (bits) {
				out = this->Iterate(input & 1);
				input >>= 1;
				--bits;
			}
			return out;
		}

	private:
		basetype_t mShiftReg;
	};

	// These parameters match to a fibonacci LFSR:
	typedef Lfsr<uint16_t, 16, 0b1010110011100001, 0b0000000000101101> LfsrFibonacci;

} // end of namespace embedded_drivers
