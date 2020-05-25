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

#include "embedded_drivers/nrfx/glue.h"

namespace embedded_drivers {

	// TWIM glue logic

	void nrfx_init_twim(nrfx_twim_t *twim_instance,
			nrfx_gpiote_pin_t sck,
			nrfx_gpiote_pin_t sda,
			nrf_twim_frequency_t frequency,
			uint8_t irq_priority,
			bool hold_bus_uninit)
	{
		nrfx_twim_config_t twimConfig;

		twimConfig.scl = sck;
		twimConfig.sda = sda;
		twimConfig.frequency = frequency;
		twimConfig.interrupt_priority = irq_priority;
		twimConfig.hold_bus_uninit = hold_bus_uninit;

		nrfx_err_t err;
		err = nrfx_twim_init(twim_instance, &twimConfig, NULL, NULL);
		assert(NRFX_SUCCESS == err);

		nrfx_twim_enable(twim_instance);
	}

	void nrfx_deinit_twim(nrfx_twim_t *twim_instance)
	{
		nrfx_twim_disable(twim_instance);
	}

	bool nrfx_twim_tx_implementation(void * context,
			uint8_t address,
			uint8_t const * buffer,
			size_t len)
	{
		nrfx_twim_t * twim_instance = (nrfx_twim_t*)context;
		return (NRFX_SUCCESS == nrfx_twim_tx(twim_instance, address, buffer, len, false));
	}

	bool nrfx_twim_rx_implementation(void * context,
			uint8_t address,
			uint8_t * buffer,
			size_t len)
	{
		nrfx_twim_t * twim_instance = (nrfx_twim_t*)context;
		return (NRFX_SUCCESS == nrfx_twim_rx(twim_instance, address, buffer, len));
	}

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
			nrf_spim_bit_order_t bit_order)
	{
		nrfx_spim_config_t spim_config;

		spim_config.sck_pin = sck;
		spim_config.mosi_pin = mosi;
		spim_config.miso_pin = miso;
		spim_config.ss_pin = ss;

		spim_config.ss_active_high = active_high;
		spim_config.irq_priority = irq_priority;
		spim_config.orc = orc;
		spim_config.frequency = frequency;
		spim_config.mode = mode;
		spim_config.bit_order = bit_order;

		nrfx_err_t err;
		err = nrfx_spim_init(spim_instance, &spim_config, NULL, NULL);
		assert(NRFX_SUCCESS == err);
	}

	bool nrfx_spim_xfer_implementation(void * spi_context,
			uint8_t const * tx_buf,
			size_t tx_size,
			uint8_t * rx_buf,
			size_t rx_size)
	{
		nrfx_spim_t * spim_instance = (nrfx_spim_t *)spi_context;

		nrfx_spim_xfer_desc_t xfer;

		xfer.p_tx_buffer = tx_buf;
		xfer.tx_length = tx_size;
		xfer.p_rx_buffer = rx_buf;
		xfer.rx_length = rx_size;

		return (NRFX_SUCCESS == nrfx_spim_xfer(spim_instance, &xfer, 0));
	}

	bool nrfx_spim_xfer_manual_cs_implementation(void * spi_context_with_cs,
			uint8_t const * tx_buf,
			size_t tx_size,
			uint8_t * rx_buf,
			size_t rx_size)
	{
		struct spi_context_with_cs * ctx = (struct spi_context_with_cs *)spi_context_with_cs;
		if(ctx->cs_active_low)
			nrf_gpio_pin_clear(ctx->cs_pin);
		else
			nrf_gpio_pin_set(ctx->cs_pin);
		bool ret = nrfx_spim_xfer_implementation(ctx->spim_instance, tx_buf, tx_size, rx_buf, rx_size);
		if(ctx->cs_active_low)
			nrf_gpio_pin_set(ctx->cs_pin);
		else
			nrf_gpio_pin_clear(ctx->cs_pin);
		return ret;
	}

	// glue logic for MPU9250 interrupts

	bool nrfx_setup_mpu9250_motion_interrupt(Mpu9250SpiSensor & motionSensor,
			nrfx_gpiote_pin_t pin,
			float gThreshold,
			nrfx_gpiote_evt_handler_t eventHandler)
	{
		nrfx_gpiote_in_config_t interruptPinCfg;
		interruptPinCfg.sense = NRF_GPIOTE_POLARITY_HITOLO;
		interruptPinCfg.pull = NRF_GPIO_PIN_PULLUP;
		interruptPinCfg.is_watcher = false;
		interruptPinCfg.hi_accuracy = true;
		interruptPinCfg.skip_gpio_setup = false;

		if(NRFX_SUCCESS != nrfx_gpiote_in_init(	pin,
							&interruptPinCfg,
							eventHandler )) {
			return false;
		}

		nrfx_gpiote_in_event_enable(pin, true);

		if(!motionSensor.ModeWakeOnMotion(gThreshold)) {
			return false;
		};

		return true;
	}

} // end of namespace embedded_drivers

