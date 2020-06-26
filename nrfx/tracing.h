
# include <core_cm4.h>

/*
 * Enable ITM tracing on SWO pin.
 * if @uart, uart mode is used; otherwise manchester encoding.
 * @prescaler changes the baud rate. for UART, 0x40 = 500KBaud, 0x10 = 2MBaud.
 * @enable_channels is a bitmask of the channels to be enabled.
 *
 * The SWO pin is muxed and setup correctly for ITM tracing, tracing is enabled
 * and can be used immediately after calling this function.
 *
 * To write into trace N, use
 *	ITM->PORT[N].{u8,u16,u32} = output;
	ITM->PORT[0].u8 = 'X';
 * Or use ITM_SendChar() from CMSIS for printf-like ouput on trace 0.
 */
__attribute__((no_instrument_function))
static inline void enable_swo_etm_tracing(bool uart, uint32_t prescaler,
					bool timestamps, uint32_t ts_prescaler, uint32_t enable_channels)
{
	// mux SWO pin for use
	NRF_CLOCK->TRACECONFIG |= CLOCK_TRACECONFIG_TRACEMUX_Serial << CLOCK_TRACECONFIG_TRACEMUX_Pos;
	NRF_P1->PIN_CNF[0] = (GPIO_PIN_CNF_DRIVE_H0H1 << GPIO_PIN_CNF_DRIVE_Pos)
			   | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
			   | (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos);

	// enable tracing
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

	// unlock tracing
	ITM->LAR = 0xC5ACCE55;

	// output type for SWO
	TPI->SPPR = uart ? 2 : 1;

	// set tracing output prescaler (0x40 = 500KBaud for UART, 0x10=2MBaud)
	TPI->ACPR = prescaler;

	// enable ITM trace and timestamps
	ITM->TCR = ITM_TCR_ITMENA_Msk;
	if(timestamps)
		ITM->TCR |= ITM_TCR_TSENA_Msk | (ts_prescaler << ITM_TCR_SWOENA_Pos);

	// enable trace channels
	ITM->TER = enable_channels;
}

