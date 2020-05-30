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

	class Ad5761rSpiDac {
		/*
		 * Driver for the Analog Devices AD5761[R] and AD5721[R].
		 * SPI MODE 2 is required for communication.
		 * Take care to setup the bus accordingly.
		 */

	public:
		typedef bool(*SpiXferCallback)(void * spi_context, uint8_t const * tx_buf, size_t tx_size, uint8_t * rx_buf, size_t rx_size);

		Ad5761rSpiDac(void * spiContext, SpiXferCallback spiXfer);

		int32_t WriteControlRegister(uint8_t CV, bool OVR, bool B2C, bool ETS, bool IRO, uint8_t PV, uint8_t RA)
		{
			uint16_t ctrlreg = 0;
			ctrlreg |= (CV & 0b11) << 9;
			ctrlreg |= (OVR) ? (1 << 8) : 0;
			ctrlreg |= (B2C) ? (1 << 7) : 0;
			ctrlreg |= (ETS) ? (1 << 6) : 0;
			ctrlreg |= (IRO) ? (1 << 5) : 0;
			ctrlreg |= (PV & 0b11) << 3;
			ctrlreg |= (RA & 0b111) << 0;
			return Xfer(CmdWriteControlReg, ctrlreg);
		}

		int32_t ReadControlReg(void)
		{ return Xfer(CmdReadControlReg, 0); }

		int32_t ReadDacReg(void)
		{ return Xfer(CmdReadDacReg, 0); }

		int32_t ReadInputReg(void)
		{ return Xfer(CmdReadInputReg, 0); }

		int32_t WriteInputReg(uint16_t value)
		{ return Xfer(CmdWriteInputReg, value); }

		int32_t WriteInputRegAndUpdate(uint16_t value)
		{ return Xfer(CmdWriteUpdateDacReg, value); }

		int32_t DisableDaisyChain(bool DDC)
		{ return Xfer(CmdDisableDaisyChain, DDC ? (1<<0) : 0); }

		int32_t SoftwareDataReset(void)
		{ return Xfer(CmdSoftwareDataReset, 0); }

		int32_t SoftwareFullReset(void)
		{ return Xfer(CmdSoftwareFullReset, 0); }


	private:
		enum SpiCommand {
			CmdNoOp = 0b0000,
			CmdWriteInputReg = 0b0001,
			CmdUpdateDacReg = 0b0010,
			CmdWriteUpdateDacReg = 0b0011,
			CmdWriteControlReg = 0b0100,
			CmdSoftwareDataReset = 0b0111,
			CmdReserved = 0b1000,
			CmdDisableDaisyChain = 0b1001,
			CmdReadInputReg = 0b1010,
			CmdReadDacReg = 0b1011,
			CmdReadControlReg = 0b1100,
			CmdSoftwareFullReset = 0b1111,
		};

		int32_t Xfer(enum SpiCommand command, uint16_t payload);
		int32_t XferReadback(enum SpiCommand command, uint16_t payload);

		void * mSpiContext;
		SpiXferCallback mSpiXfer;

	};

} // end of namespace embedded_drivers

