#pragma once

#error this file is work in progress.

#include <core_cm4.h>

#ifdef __cplusplus
 extern "C" {
#endif


/*
 * ETM registers are not defined as part of CMSIS.
 * This definition is for version 3.5 of the ETM.
 * v3.5 is used in Cortex-M4
 */

typedef struct
{
	__IOM uint32_t CR;			/* Offset: 0x000 (R/W) Main Control Register */
	__IO  uint32_t CCR;			/* Offset: 0x004 (R/ ) Configuration Code Register */
	__OM uint32_t TRIGGER;			/* Offset: 0x008 ( /W) Trigger Event Register */
	__OM uint32_t ASICCR;			/* Offset: 0x00C ( /W) ASIC Control Register */

	__IOM uint32_t SR;			/* Offset: 0x010 (R/W) Status Register */
	__IM  uint32_t SCR;			/* Offset: 0x014 (R/ ) System Configuration Register */
	__OM  uint32_t TSSCR;			/* Offset: 0x018 ( /W) TraceEnable Start/Stop Control Register */
	__OM  uint32_t TECR2;			/* Offset: 0x01C ( /W) TraceEnable Control 2 Register */

	__OM  uint32_t TEEVR;			/* Offset: 0x020 ( /W) TraceEnable Event Register */
	__OM  uint32_t TECR1;			/* Offset: 0x024 ( /W) TraceEnable Control 1 Register */
	__OM  uint32_t FFRR;			/* Offset: 0x028 ( /W) FIFOFULL Region Register */
	__IOM uint32_t FFLR;			/* Offset: 0x02C (R/W) FIFOFULL Level Register */

	__OM  uint32_t VDEVR;			/* Offset: 0x030 ( /W) ViewData Event Register */
	__OM  uint32_t VDCR[3U];		/* Offset: 0x034 ( /W) ViewData Control Registers */

	__OM  uint32_t ACVR[16U];		/* Offset: 0x040 ( /W) Address Comparator Value Registers */
	__OM  uint32_t ACTR[16U];		/* Offset: 0x080 ( /W) Address Comparator Access Type Registers */

	__OM  uint32_t DCVR[16U];		/* Offset: 0x0C0 ( /W) Data Comparator Value Registers */
	__OM  uint32_t DCMR[16U];		/* Offset: 0x100 ( /W) Data Comparator Mask Registers */

	__OM  uint32_t CNTRLDVR[4U];		/* Offset: 0x140 ( /W) Counter Reload Value Registers */
	__OM  uint32_t CNTENR[4U];		/* Offset: 0x150 ( /W) Counter Enable Registers */
	__OM  uint32_t CNTRLDEVR[4U];		/* Offset: 0x160 ( /W) Counter Reload Event Registers */
	__IOM uint32_t CNTVR[4U];		/* Offset: 0x170 ( /W) Counter Value Registers */

	__OM  uint32_t SQabEVR[6U];		/* Offset: 0x180 ( /W) Sequencer State Transition Event Registers */
	      uint32_t RESERVED0[1U];
	__IOM uint32_t SQR;			/* Offset: 0x19C (R/W) Current Sequencer State Register */

...
page 92 of ETM Architecture Specification (v3.5)
page 93 of ETM Architecture Specification (v3.5)
page 94 of ETM Architecture Specification (v3.5)
...

	__OM  uint32_t LAR;			/* Offset: 0xFB0 ( /W) Lock Access Register */
	__IM  uint32_t LSR;			/* Offset: 0xFB4 (R/ ) Lock Status Register */
	      uint32_t RESERVED8[6U];
	__IM  uint32_t PID4;			/* Offset: 0xFD0 (R/ ) Peripheral Identification Register #4 */
	__IM  uint32_t PID5;			/* Offset: 0xFD4 (R/ ) Peripheral Identification Register #5 */
	__IM  uint32_t PID6;			/* Offset: 0xFD8 (R/ ) Peripheral Identification Register #6 */
	__IM  uint32_t PID7;			/* Offset: 0xFDC (R/ ) Peripheral Identification Register #7 */
	__IM  uint32_t PID0;			/* Offset: 0xFE0 (R/ ) Peripheral Identification Register #0 */
	__IM  uint32_t PID1;			/* Offset: 0xFE4 (R/ ) Peripheral Identification Register #1 */
	__IM  uint32_t PID2;			/* Offset: 0xFE8 (R/ ) Peripheral Identification Register #2 */
	__IM  uint32_t PID3;			/* Offset: 0xFEC (R/ ) Peripheral Identification Register #3 */
	__IM  uint32_t CID0;			/* Offset: 0xFF0 (R/ ) Component Identification Register #0 */
	__IM  uint32_t CID1;			/* Offset: 0xFF4 (R/ ) Component Identification Register #1 */
	__IM  uint32_t CID2;			/* Offset: 0xFF8 (R/ ) Component Identification Register #2 */
	__IM  uint32_t CID3;			/* Offset: 0xFFC (R/ ) Component Identification Register #3 */
} ETM_Type;

#define ETM_BASE (0xE0041000)
#define ETM ((ETM_Type *) ETM_BASE ) /* ETM configuration struct */

#ifdef __cplusplus
}
#endif
