#pragma once

#include <core_cm4.h>

/*
 * Documentation on debugging components and their registers:
 *
 *
 * ARM TPI / TPIU (Trace Port Interface Unit)
 * @0xE0040000
 * see:
 *	ARM-Cortex-M4 Technical Reference Manual
 *	ARM-v7M Architecture Reference Manual
 *	CoreSight Architecture Specification
 *
 *
 * ARM ITM (Instrumentation Trace Macrocell)
 * @0xE0000000
 * see:
 *	ARM-Cortex-M4 Technical Reference Manual
 *	ARM-v7M Architecture Reference Manual
 *	CoreSight Architecture Specification
 *
 *
 * ARM ETM (Embedded Trace Macrocell)
 * @0xE0041000
 * see:
 *	https://developer.arm.com/documentation/ihi0014/q/programmers--model/the-etm-registers
 *	Embedded Trace Macrocell Architecture Specification
 *	v1.0-v3.5
 *	(Cortex-M4 has v3.5)
 *	-> Embedded Trace Macrocell Architecture Specification
 *	-> Programmer's Model
 *	-> The ETM registers
 *
 *
 * ARM DWT (Data Watchpoint and Trace)
 * @0xE0001000
 * see:
 *	ARM-Cortex-M4 Technical Reference Manual
 *	ARM-v7M Architecture Reference Manual
 *
 *
 * ARM ETB (Embedded Trace Buffer)
 * @0xE0042000
 * NOT AVAILABLE ON CORTEX-M4.
 * see:
 *	ARM-v7M Architecture Reference Manual
 *	CoreSight Architecture Specification
 *
 */



/*
 * Enable ITM tracing on SWO pin.
 *
 * if @uart, uart mode is used; otherwise manchester encoding.
 *
 * @outputPrescaler changes the SWO output clock (effective baud rate):
 *   output clock = async reference clock / (1+outputPrescaler)
 * the async ref clock may somehow be related to the cpu core clock.
 * e.g. for the nRF52840, the async ref clock is the TRACEPORT speed
 * as configured in NRF_CLOCK->TRACECONFIG.
 * e.g. 32MHz effectively yields baudrates of: 32M/(1+outputPrescaler)
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
static inline void arm_cm4_enable_swo_itm_tracing(bool uart, uint32_t outputPrescaler,
					bool globalTs, bool localTs, uint32_t localTsPrescaler,
					bool useTpiuFormatter,
					uint32_t channels)
{
	/* XXX this is work in progress XXX */

	// enable all needed power domains
	// FIXME somehow set CSYSPWRUPREQ and CDBGPWRUPREQ in CTRL/STAT register of SW-DP

	// enable DWT and ITM blocks
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

	// set SWO output outputPrescaler
	TPI->ACPR = outputPrescaler;

	// output type for SWO
	TPI->SPPR = uart ? 2 : 1;

	// en/disable TPIU formatter and flush
	if(useTpiuFormatter)
		TPI->FFCR |= TPI_FFCR_EnFCont_Msk;
	else
		TPI->FFCR &= ~TPI_FFCR_EnFCont_Msk;

	// unlock ITM registers
	ITM->LAR = 0xC5ACCE55;

	uint32_t new_tcr = 0;

	if(globalTs) {
		// generate global timestamp after every packet if the output FIFO is empty
		new_tcr |= (0x3 << ITM_TCR_GTSFREQ_Pos);
	}

	if(localTs) {
		// set timestamp prescaler
		new_tcr |= (localTsPrescaler << ITM_TCR_TSPrescale_Pos) & ITM_TCR_TSPrescale_Msk;

		// enable timestamps in trace
		new_tcr |= ITM_TCR_TSENA_Msk;
	}

	ITM->TCR = new_tcr;

	// enable ITM subsystem
	ITM->TCR |= ITM_TCR_ITMENA_Msk;

	// enable all ITM trace ports
	ITM->TER = channels;

	// allow unprivileged access to all traceports
	ITM->TPR = 0x0;
}

static inline void arm_cm4_enable_traceport_etm_tracing(bool globalTs, bool localTs, uint32_t localTsPrescaler)
{
	/* XXX this is work in progress XXX */

	// enable DWT and ITM blocks
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

	// enable 4-pin tracing in TPIU
	TPI->CSPSR = (1 << 3);
	// XXX this does not seem to work

	// unlock ETM registers
	//ETM->LAR = 0xC5ACCE55;

	// lock ETM registers again
	//ETM->LAR = 0;


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

	uint32_t new_tcr = 0;

	// set ATB ID (traceport) for CoreSight system (?)
	new_tcr |= (0x1 << ITM_TCR_TraceBusID_Pos);

	if(globalTs) {
		// generate global timestamp after every packet if the output FIFO is empty
		new_tcr |= (0x3 << ITM_TCR_GTSFREQ_Pos);
	}

	if(localTs) {
		// set timestamp prescaler
		new_tcr |= (localTsPrescaler << ITM_TCR_TSPrescale_Pos) & ITM_TCR_TSPrescale_Msk;

		// enable timestamps in trace
		new_tcr |= ITM_TCR_TSENA_Msk;
	}


	// enable forwarding of hardware event packets from the DWT to the TPIU
	new_tcr |= ITM_TCR_DWTENA_Msk; // (recent specs renamed this bit to TXENA)

	// enable syncchronization packets for a synchronous TPIU
	new_tcr |= ITM_TCR_SYNCENA_Msk;

	ITM->TCR = new_tcr;

	// enable ITM subsystem
	ITM->TCR |= ITM_TCR_ITMENA_Msk;

	// enable all ITM trace ports
	ITM->TER = 0xffffffff;

	// allow unprivileged access to all traceports
	ITM->TPR = 0x0;

	// lock ITM registers again
	ITM->LAR = 0;
}

