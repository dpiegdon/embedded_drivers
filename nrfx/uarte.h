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

	// write data to uart
	uint32_t Write(char const * data, int const len)
	{
		return nrfx_uarte_tx(&mUarteInstance, (unsigned char*)data, len);
	}

	// read data from uart
	uint32_t Read(char * data, int const len)
	{
		return nrfx_uarte_rx(&mUarteInstance, (unsigned char*)data, len);
	}

	// check if at least one byte can be read
	bool RxReady(void)
	{
		return nrfx_uarte_rx_ready(&mUarteInstance);
	}

	// get mask of errors set by uart device
	uint32_t GetErrors(void)
	{
		return nrfx_uarte_errorsrc_get(&mUarteInstance);
	}
private:
	nrfx_uarte_t mUarteInstance;
	nrfx_uarte_config_t mUarteConfig;
};

