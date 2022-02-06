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

#include "nrfx_rng.h"

#include "embedded_drivers/nrfx/lfsr_rng.h"


embedded_drivers::LfsrFibonacci lfsr_rng;

static void rng_callback(uint8_t rng_data) {
	lfsr_rng.Iterate(rng_data, 8);
}

void lfsr_rng_init(void) {
	nrfx_rng_config_t cfg;

	cfg.error_correction = NRFX_RNG_CONFIG_ERROR_CORRECTION;
	cfg.interrupt_priority = NRFX_RNG_CONFIG_ERROR_CORRECTION;

	nrfx_rng_init(&cfg, rng_callback);
	nrfx_rng_start();
}

void lfsr_rng_deinit(void) {
	nrfx_rng_stop();
	nrfx_rng_uninit();
}
