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

#include "nrfx_gpiote.h"
#include "nrfx_spim.h"
#include "nrfx_twim.h"

#include "embedded_drivers/mpu9250_spi_sensor.h"

#include <cstdint>

namespace embedded_drivers {

	// TWIM glue logic
	void nrfx_init_twim(nrfx_twim_t *twim_instance,
			nrfx_gpiote_pin_t sck,
			nrfx_gpiote_pin_t sda,
			nrf_twim_frequency_t frequency,
			uint8_t irq_priority,
			bool hold_bus_uninit,
                        nrfx_twim_evt_handler_t event_handler = NULL,
                        void *p_context = NULL);
	void nrfx_deinit_twim(nrfx_twim_t *twim_instance);
	bool nrfx_twim_tx_implementation(void * context,
			uint8_t address,
			uint8_t const * buffer,
			size_t len);
	bool nrfx_twim_rx_implementation(void * context,
			uint8_t address,
			uint8_t * buffer,
			size_t len);

	// SPIM glue logic
	void nrfx_init_spim(nrfx_spim_t * spim_instance,
			nrfx_gpiote_pin_t sck,
			nrfx_gpiote_pin_t mosi,
			nrfx_gpiote_pin_t miso,
			nrfx_gpiote_pin_t ss,
			bool active_high,
			uint8_t irq_priority,
			uint8_t orc,
			nrf_spim_frequency_t frequency,
			nrf_spim_mode_t mode,
			nrf_spim_bit_order_t bit_order,
			nrfx_spim_evt_handler_t handler = NULL,
			void *p_context = NULL);
	bool nrfx_spim_xfer_implementation(void * spi_context,
			uint8_t const * tx_buf,
			size_t tx_size,
			uint8_t * rx_buf,
			size_t rx_size);
	struct spi_context_with_cs {
		void * spim_instance;
		bool cs_active_low;
		nrfx_gpiote_pin_t cs_pin;
	};
	bool nrfx_spim_xfer_manual_cs_implementation(void * spi_context_with_cs,
			uint8_t const * tx_buf,
			size_t tx_size,
			uint8_t * rx_buf,
			size_t rx_size);


	// glue logic for MPU9250 interrupts
	bool nrfx_setup_mpu9250_motion_interrupt(Mpu9250SpiSensor & motionSensor,
			nrfx_gpiote_pin_t pin,
			float gThreshold,
			nrfx_gpiote_evt_handler_t eventHandler);

} // end of namespace embedded_drivers

