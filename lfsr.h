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

//#pragma once

#include <cstdint>

namespace embedded_drivers {

	// Implements a generic linear feedback shift register that allows to shift
	// additional random bits into the front to improve its randomness.
	// The default parameters match to a fibonacci LFSR.
	template <typename basetype_t=uint16_t, unsigned int WIDTH=16, basetype_t INIT_VALUE=0b1010110011100001, basetype_t FEEDBACK=0b0000000000101101>
	class lfsr {
		basetype_t shiftreg;

	public:
		lfsr(basetype_t initial_state=INIT_VALUE)
		{
			static_assert(sizeof(basetype_t)*8 >= WIDTH,
				"basetype_t is not large enough to represent full LFSR state.");
			this->shiftreg = initial_state;
		}

		template<unsigned int COUNT=1>
		basetype_t iterate(void)
		{
			static_assert(COUNT <= WIDTH);
			basetype_t out = this->shiftreg & ((1 << COUNT)-1);

			for (unsigned int i = 0; i < COUNT; ++i) {
				this->iterate(false);
			}
			return out;
		}

		bool iterate(bool input)
		{
			bool out = this->shiftreg & 1;

			basetype_t feedback = this->shiftreg & FEEDBACK;
			if (input)
				feedback = feedback ^ 1;
			while (feedback > 1)
				feedback = (feedback >> 1) ^ (feedback & 1);

			this->shiftreg >>= 1;
			this->shiftreg |= feedback << (WIDTH-1);

			return out;
		}

		template <typename intype_t>
		bool iterate(intype_t input, unsigned bits)
		{
			bool out;
			while (bits) {
				out = this->iterate(input & 1);
				input >>= 1;
				--bits;
			}
			return out;
		}
	};

}; // end of namespace embedded_drivers
