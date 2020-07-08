#pragma once

#include <core_cm4.h>

/*
 * Enable ITM tracing on SWO pin.
 *
 * if @uart, uart mode is used; otherwise manchester encoding.
 *
 * @prescaler changes the SWO output clock (effective baud rate):
 *   output clock = async reference clock / (1+prescaler)
 * the async ref clock may somehow be related to the cpu core clock.
 * e.g. for the nRF52840, the async ref clock is running at 32MHz, i.e core clock/2
 *   which effectively yields baudrates of: 64M/2/(1+prescaler)
 *
 * if @timestamps, timestamps will be added to the trace.
 * @ts_prescaler controls the timestamp frequency (0b00=/1, 0b01=/4, 0b10=/16, 0b11=/64).
 *
 * The SWO pin is muxed and setup correctly for ITM tracing, tracing is enabled
 * and can be used immediately after calling this function.
 *
 * To write into trace N, use
 *	ITM->PORT[N].{u8,u16,u32} = output;
	ITM->PORT[0].u8 = 'X';
 * Or use ITM_SendChar() from CMSIS for printf-like ouput on trace 0.
 */
static inline void arm_cm4_enable_swo_itm_tracing(bool uart, uint32_t prescaler,
					bool timestamps, uint32_t ts_prescaler)
{
	/* XXX this is work in progress XXX */

	// enable DWT and ITM blocks
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;


	/* ARM documentation on TPI / TPIU (Trace Port Interface Unit)
	 *	https://developer.arm.com/documentation/ddi0314/h/trace-port-interface-unit/tpiu-programmers-model
	 *	CoreSight Components Technical Reference Manual
	 *	v1.0
	 *	-> Trace Port Interface Unit
	 *	-> TPIU programmers model
	 * @0xE0040000
	 */

	// set SWO output prescaler
	TPI->ACPR = prescaler;

	// output type for SWO
	TPI->SPPR = uart ? 2 : 1;


	/* ARM documentation on ITM (Instrumentation Trace Macrocell)
	 *	see:	ARM-Cortex-M4 Technical Reference Manual
	 *		ARM-v7M Architecture Reference Manual
	 *		CoreSight Architecture Specification
	 * @0xE0000000
	 */


	// unlock ITM registers
	ITM->LAR = 0xC5ACCE55;

	// generate global timestamp after every packet if the output FIFO is empty
//	ITM->TCR &= ~ITM_TCR_GTSFREQ_Msk;
//	ITM->TCR |= (0x3 << ITM_TCR_GTSFREQ_Pos);

	if(timestamps) {
		// set timestamp prescaler
		ITM->TCR &= ~ITM_TCR_TSPrescale_Msk;
		ITM->TCR |= (ts_prescaler << ITM_TCR_TSPrescale_Pos) & ITM_TCR_TSPrescale_Msk;

		// enable timestamps in trace
		ITM->TCR |= ITM_TCR_TSENA_Msk;
	}

	// enable ITM subsystem
	ITM->TCR = ITM_TCR_ITMENA_Msk;

	// enable all ITM trace ports
	ITM->TER = 0xffffffff;

	// allow unprivileged access to all traceports
	ITM->TPR = 0x0;

	// lock ITM registers again
	ITM->LAR = 0;
}

static inline void arm_cm4_enable_traceport_etm_tracing(bool timestamps, uint32_t ts_prescaler)
{
	/* XXX this is work in progress XXX */

	// enable DWT and ITM blocks
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;


	/* ARM documentation on TPI / TPIU (Trace Port Interface Unit)
	 *	https://developer.arm.com/documentation/ddi0314/h/trace-port-interface-unit/tpiu-programmers-model
	 *	CoreSight Components Technical Reference Manual
	 *	v1.0
	 *	-> Trace Port Interface Unit
	 *	-> TPIU programmers model
	 * @0xE0040000
	 */

	// enable 4-pin tracing in TPIU
	TPI->CSPSR = (1 << 3);
	// XXX this does not seem to work


	/* ARM documentation on ETM (Embedded Trace Macrocell)
	 *	https://developer.arm.com/documentation/ihi0014/q/programmers--model/the-etm-registers
	 *	Embedded Trace Macrocell Architecture Specification
	 *	v1.0-v3.5
	 *	(Cortex-M4 has v3.5)
	 *	-> Embedded Trace Macrocell Architecture Specification
	 *	-> Programmer's Model
	 *	-> The ETM registers
	 * @0xE0041000
	 */

	// unlock ETM registers
	//ETM->LAR = 0xC5ACCE55;

	// lock ETM registers again
	//ETM->LAR = 0;


	/* ETB @0xE0042000
	 * not equipped on many chips.
	 */


	/* ARM documentation on DWT (Data Watchpoint and Trace)
	 *	see:	ARM-Cortex-M4 Technical Reference Manual
	 *		ARM-v7M Architecture Reference Manual
	 * @0xE0001000
	 */

	// enable instruction tracing
	DWT->CTRL |= DWT_CTRL_PCSAMPLENA_Msk;

	// set frequency of sync trace packets (also needs ITM_TCR_SYNCENA)
	DWT->CTRL &= ~DWT_CTRL_SYNCTAP_Msk;
	DWT->CTRL |= (0x1 << DWT_CTRL_SYNCTAP_Pos);

	// set frequency of instruction trace packets
	DWT->CTRL &= ~DWT_CTRL_CYCTAP_Msk;
	DWT->CYCCNT = 0;

	// ??? init value for POSTCNT register
	DWT->CTRL &= ~DWT_CTRL_POSTINIT_Msk;
	DWT->CTRL |= (0x1 << DWT_CTRL_POSTINIT_Pos);

	// ??? reload value for POSTCNT register
	DWT->CTRL &= ~DWT_CTRL_POSTPRESET_Msk;
	DWT->CTRL |= (0xf << DWT_CTRL_POSTPRESET_Pos);

	// enable core cycle counter
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;


	/* ARM documentation on ITM (Instrumentation Trace Macrocell)
	 *	see:	ARM-Cortex-M4 Technical Reference Manual
	 *		ARM-v7M Architecture Reference Manual
	 *		CoreSight Architecture Specification
	 * @0xE0000000
	 */

	// unlock ITM registers
	ITM->LAR = 0xC5ACCE55;

	// set ATB ID (traceport) for CoreSight system (?)
	ITM->TCR &= ~ITM_TCR_TraceBusID_Msk;
	ITM->TCR |= (0x1 << ITM_TCR_TraceBusID_Pos);

	// generate global timestamp after every packet if the output FIFO is empty
	ITM->TCR &= ~ITM_TCR_GTSFREQ_Msk;
	ITM->TCR |= (0x3 << ITM_TCR_GTSFREQ_Pos);

	if(timestamps) {
		// set local timestamp prescaler
		ITM->TCR &= ~ITM_TCR_TSPrescale_Msk;
		ITM->TCR |= (ts_prescaler << ITM_TCR_TSPrescale_Pos) & ITM_TCR_TSPrescale_Msk;

		// enable local timestamps generation
		ITM->TCR |= ITM_TCR_TSENA_Msk;
	}

	// enable forwarding of hardware event packets from the DWT to the TPIU
	ITM->TCR |= ITM_TCR_DWTENA_Msk;

	// enable syncchronization packets for a synchronous TPIU
	ITM->TCR |= ITM_TCR_SYNCENA_Msk;

	// enable ITM subsystem
	ITM->TCR |= ITM_TCR_ITMENA_Msk;

	// enable all ITM trace ports
	ITM->TER = 0xffffffff;

	// allow unprivileged access to all traceports
	ITM->TPR = 0x0;

	// lock ITM registers again
	ITM->LAR = 0;
}

