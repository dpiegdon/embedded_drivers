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

	namespace font_tama_mini02 {

		extern uint8_t const * dataptr;
		extern unsigned const width;
		extern unsigned const height;
		extern unsigned const first_character;
		extern unsigned const character_count;

	} // end of namespace font_tama_mini02

} // end of namespace embedded_drivers

