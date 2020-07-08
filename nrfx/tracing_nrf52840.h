#pragma once

#include <nrf52840.h>

/* mux SWO pin for use */
static inline void nrf_mux_swo(void)
{
	NRF_CLOCK->TRACECONFIG |= CLOCK_TRACECONFIG_TRACEMUX_Serial << CLOCK_TRACECONFIG_TRACEMUX_Pos;
	NRF_P1->PIN_CNF[0] = (GPIO_PIN_CNF_DRIVE_H0H1 << GPIO_PIN_CNF_DRIVE_Pos)
			   | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
			   | (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos);
}

/* mux full traceport (1 clock + 4 data) for use */
static inline void nrf_mux_traceport(void)
{
	// set internal tracing clock to 32MHz, mux tracing pins for ETM output
        NRF_CLOCK->TRACECONFIG = CLOCK_TRACECONFIG_TRACEMUX_Parallel << CLOCK_TRACECONFIG_TRACEMUX_Pos
				| CLOCK_TRACECONFIG_TRACEPORTSPEED_32MHz << CLOCK_TRACECONFIG_TRACEPORTSPEED_Pos;
	// mux all four trace data pins and the trace clock pin
        NRF_P0->PIN_CNF[7]  = (GPIO_PIN_CNF_DRIVE_H0H1 << GPIO_PIN_CNF_DRIVE_Pos)
			    | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
			    | (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos);
        NRF_P1->PIN_CNF[0]  = (GPIO_PIN_CNF_DRIVE_H0H1 << GPIO_PIN_CNF_DRIVE_Pos)
			    | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
			    | (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos);
        NRF_P0->PIN_CNF[12] = (GPIO_PIN_CNF_DRIVE_H0H1 << GPIO_PIN_CNF_DRIVE_Pos)
			    | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
			    | (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos);
        NRF_P0->PIN_CNF[11] = (GPIO_PIN_CNF_DRIVE_H0H1 << GPIO_PIN_CNF_DRIVE_Pos)
			    | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
			    | (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos);
        NRF_P1->PIN_CNF[9]  = (GPIO_PIN_CNF_DRIVE_H0H1 << GPIO_PIN_CNF_DRIVE_Pos)
			    | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
			    | (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos);
}

