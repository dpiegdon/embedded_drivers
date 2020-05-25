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

#include <cstring>

#include <machine/endian.h>

#include "embedded_drivers/ad5761r_spi_dac.h"

namespace embedded_drivers {

	Ad5761rSpiDac::Ad5761rSpiDac(void * spiContext, SpiXferCallback spiXfer)
		: mSpiContext(spiContext)
		, mSpiXfer(spiXfer)
	{
	}

	int32_t Ad5761rSpiDac::Xfer(enum SpiCommand command, uint16_t payload)
	{
		struct TxCommand {
			uint8_t command;
			uint16_t payload;
		} __attribute__((packed));

		struct TxCommand txBuf;
		uint8_t rxBuf[3];

		txBuf.command = command;
		txBuf.payload = __htons(payload);

		if(mSpiXfer(mSpiContext, (uint8_t const*)&txBuf, sizeof(txBuf), (uint8_t*)&rxBuf, 3)) {
			uint16_t ret;
			memcpy(&ret, rxBuf+1, sizeof(ret));
			return __ntohs(ret);
		} else {
			return -1;
		}
	}

} // end of namespace embedded_drivers

