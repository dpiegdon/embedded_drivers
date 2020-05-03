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

#include "nrfx_uarte.h"
#include <cerrno>
#include <cstddef>

/* very simple C++ bindings for UARTE driver of NRFX */
class Uarte {
public:
	Uarte(uint32_t tx, uint32_t rx, uint32_t cts, uint32_t rts);

	int Write(char const * data, int const len)
	{
		if(NRFX_SUCCESS != nrfx_uarte_tx(&mUarteInstance, (unsigned char*)data, len))
			return -EINVAL;
		return len;
	}

	int Read(char * data, int const len)
	{
		if(NRFX_SUCCESS != nrfx_uarte_rx(&mUarteInstance, (unsigned char*)data, len))
			return -EINVAL;
		return len;
	}
private:
	nrfx_uarte_t mUarteInstance;
	nrfx_uarte_config_t mUarteConfig;
};

