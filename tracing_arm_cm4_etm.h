#pragma once

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
	__OM  uint32_t TRIGGER;			/* Offset: 0x008 ( /W) Trigger Event Register */
	__OM  uint32_t ASICCR;			/* Offset: 0x00C ( /W) ASIC Control Register */

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
	__OM  uint32_t EXTOUTEVR[4U];		/* Offset: 0x1A0 ( /W) External Output Event Register */

	__OM  uint32_t CIDCVR[3U];		/* Offset: 0x1B0 ( /W) Context ID Comparator Value Registers */
	__OM  uint32_t CIDCMR;			/* Offset: 0x1BC ( /W) Context ID Comparator Mask Register */

	__OM  uint32_t ISR[8U];			/* Offset: 0x1C0 ( /W) Implementation Specific Registers */
	__IOM uint32_t SYNCFR;			/* Offset: 0x1E0 (R/W) Synchronization Frequency Register */
	__IM  uint32_t IDR;			/* Offset: 0x1E4 (R/ ) ID Register */
	__IM  uint32_t CCER;			/* Offset: 0x1E8 (R/ ) Configuration Code Extension Register */
	__OM  uint32_t EXTINSELR;		/* Offset: 0x1EC ( /W) Extended External Input Selection Register */
	__OM  uint32_t TESSEICR;		/* Offset: 0x1F0 ( /W) TraceEnable Start/Stop EmbeddedICE Control Register */
	__OM  uint32_t EIBCR;			/* Offset: 0x1F4 ( /W) EmbeddedICE Behavior Control Register */
	__IOM uint32_t TSEVR;			/* Offset: 0x1F8 (R/W) Timestamp Event Register */
	__IOM uint32_t AUXCR;			/* Offset: 0x1FC (R/W) Auxiliary Control Register */
	__IOM uint32_t TRACEIDR;		/* Offset: 0x200 (R/W) CoreSight Trace ID Register */
	      uint32_t RESERVED1[1U];
	__IM  uint32_t IDR2;			/* Offset: 0x208 (R/ ) ETM ID Register 2 */
	      uint32_t RESERVED2[13U];
	__IOM uint32_t VMIDCVR;			/* Offset: 0x240 (R/W) VMID Comparator Value Register */
	      uint32_t RESERVED3[47U];

	__OM  uint32_t OSLAR;			/* Offset: 0x300 ( /W) OS Lock Access Register */
	__IM  uint32_t OSLSR;			/* Offset: 0x304 (R/ ) OS Lock Status Register */
	__IOM uint32_t OSSRR;			/* Offset: 0x308 (R/W) OS Save and Restore Register */
	      uint32_t RESERVED4[1U];

	__IOM uint32_t PDCR;			/* Offset: 0x310 (R/W) Power Down Control Register */
	__IOM uint32_t PDSR;			/* Offset: 0x314 (R/W) Power Down Status Register */

	      uint32_t RESERVED5[698U];
	__IOM uint32_t IDTIR[64U];		/* Offset: 0xE00 (???) Implementation Defined Topology And Integration Registers */

	__IOM uint32_t ITCTRL;			/* Offset: 0xF00 (R/W) Integration Mode Control Register */
	      uint32_t RESERVED6[39U];
	__IOM uint32_t CLAIMSET;		/* Offset: 0xFA0 (R/W) Claim Tag Set Register */
	__IOM uint32_t CLAIMCLR;		/* Offset: 0xFA4 (R/W) Claim Tag Clear Register */
	      uint32_t RESERVED7[2U];
	__OM  uint32_t LAR;			/* Offset: 0xFB0 ( /W) Lock Access Register */
	__IM  uint32_t LSR;			/* Offset: 0xFB4 (R/ ) Lock Status Register */
	__IM  uint32_t AUTHSTATUS;		/* Offset: 0xFB8 (R/ ) Authentication Status Registers */
	      uint32_t RESERVED8[3U];
	__IM  uint32_t DEVID;			/* Offset: 0xFC8 (R/ ) Device Configuration Register */
	__IM  uint32_t DEVTYPE;			/* Offset: 0xFCC (R/ ) Device Type Register */
	__IM  uint32_t PIDR4;			/* Offset: 0xFD0 (R/ ) Peripheral Identification Register #4 */
	__IM  uint32_t PIDR5;			/* Offset: 0xFD4 (R/ ) Peripheral Identification Register #5 */
	__IM  uint32_t PIDR6;			/* Offset: 0xFD8 (R/ ) Peripheral Identification Register #6 */
	__IM  uint32_t PIDR7;			/* Offset: 0xFDC (R/ ) Peripheral Identification Register #7 */
	__IM  uint32_t PIDR0;			/* Offset: 0xFE0 (R/ ) Peripheral Identification Register #0 */
	__IM  uint32_t PIDR1;			/* Offset: 0xFE4 (R/ ) Peripheral Identification Register #1 */
	__IM  uint32_t PIDR2;			/* Offset: 0xFE8 (R/ ) Peripheral Identification Register #2 */
	__IM  uint32_t PIDR3;			/* Offset: 0xFEC (R/ ) Peripheral Identification Register #3 */
	__IM  uint32_t CIDR0;			/* Offset: 0xFF0 (R/ ) Component Identification Register #0 */
	__IM  uint32_t CIDR1;			/* Offset: 0xFF4 (R/ ) Component Identification Register #1 */
	__IM  uint32_t CIDR2;			/* Offset: 0xFF8 (R/ ) Component Identification Register #2 */
	__IM  uint32_t CIDR3;			/* Offset: 0xFFC (R/ ) Component Identification Register #3 */
} ETM_Type;

#define ETM_BASE (0xE0041000)
#define ETM ((ETM_Type *) ETM_BASE ) /* ETM configuration struct */

#ifdef __cplusplus
}
#endif
