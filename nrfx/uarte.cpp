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


#include "embedded_drivers/nrfx/uarte.h"

Uarte::Uarte(uint32_t tx, uint32_t rx, uint32_t cts, uint32_t rts)
{
	mUarteInstance.p_reg = NRFX_CONCAT_2(NRF_UARTE, 0);
	mUarteInstance.drv_inst_idx = NRFX_CONCAT_3(NRFX_UARTE, 0, _INST_IDX);

	mUarteConfig.hwfc     = (nrf_uarte_hwfc_t) NRFX_UART_DEFAULT_CONFIG_HWFC;
	mUarteConfig.parity   = (nrf_uarte_parity_t) NRFX_UART_DEFAULT_CONFIG_PARITY;
	mUarteConfig.baudrate = (nrf_uarte_baudrate_t) NRFX_UART_DEFAULT_CONFIG_BAUDRATE;
	mUarteConfig.interrupt_priority = NRFX_UART_DEFAULT_CONFIG_IRQ_PRIORITY;

	mUarteConfig.pseltxd = tx;
	mUarteConfig.pselrxd = rx;
	mUarteConfig.pselcts = cts;
	mUarteConfig.pselrts = rts;

	if(NRFX_SUCCESS != nrfx_uarte_init(&mUarteInstance, &mUarteConfig, NULL))
		while(1) { /* endless */ };
}

