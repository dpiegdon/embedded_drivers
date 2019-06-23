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

#include <cstdio>
#include <cstring>
#include <machine/endian.h>

#include "embedded_drivers/mpu9250_spi_sensor.h"

namespace embedded_drivers {

	Mpu9250SpiSensor::Mpu9250SpiSensor(void * spiContext, SpiXferCallback spiXfer)
		: mSpiContext(spiContext)
		, mSpiXfer(spiXfer)
	{
		Reset();
	}

	bool Mpu9250SpiSensor::Access1Reg8(uint8_t addr, bool read, uint8_t *reg1)
	{
		struct Hdr3Reg16 {
			uint8_t header;
			uint8_t reg1;
		} __attribute__((packed));

		union Payload3Reg16 {
			struct Hdr3Reg16 fields;
			uint8_t raw[sizeof(Hdr3Reg16)];

		} __attribute__((packed));

		union Payload3Reg16 rxbuf;
		union Payload3Reg16 txbuf;

		if(!read) {
			txbuf.fields.reg1 = *reg1;
		} else {
			memset(&txbuf, 0, sizeof(txbuf));
		}
		txbuf.fields.header = SpiTransferHeader(read, addr);

		if(mSpiXfer(mSpiContext, txbuf.raw, sizeof(txbuf), rxbuf.raw, sizeof(rxbuf))) {
			if(read) {
				*reg1 = rxbuf.fields.reg1;
			}
			return true;
		} else {
			return false;
		}
	}

	bool Mpu9250SpiSensor::Access1Reg16(uint8_t addr, bool read, uint16_t *reg1)
	{
		struct Hdr3Reg16 {
			uint8_t header;
			uint16_t reg1;
		} __attribute__((packed));

		union Payload3Reg16 {
			struct Hdr3Reg16 fields;
			uint8_t raw[sizeof(Hdr3Reg16)];

		} __attribute__((packed));

		union Payload3Reg16 rxbuf;
		union Payload3Reg16 txbuf;

		if(!read) {
			txbuf.fields.reg1 = __htons(*reg1);
		} else {
			memset(&txbuf, 0, sizeof(txbuf));
		}
		txbuf.fields.header = SpiTransferHeader(read, addr);

		if(mSpiXfer(mSpiContext, txbuf.raw, sizeof(txbuf), rxbuf.raw, sizeof(rxbuf))) {
			if(read) {
				*reg1 = (uint16_t)__ntohs(rxbuf.fields.reg1);
			}
			return true;
		} else {
			return false;
		}
	}

	bool Mpu9250SpiSensor::Access2Reg16(uint8_t addr, bool read, uint16_t *reg1, uint16_t *reg2)
	{
		struct Hdr3Reg16 {
			uint8_t header;
			uint16_t reg1;
			uint16_t reg2;
		} __attribute__((packed));

		union Payload3Reg16 {
			struct Hdr3Reg16 fields;
			uint8_t raw[sizeof(Hdr3Reg16)];

		} __attribute__((packed));

		union Payload3Reg16 rxbuf;
		union Payload3Reg16 txbuf;

		if(!read) {
			txbuf.fields.reg1 = __htons(*reg1);
			txbuf.fields.reg2 = __htons(*reg2);
		} else {
			memset(&txbuf, 0, sizeof(txbuf));
		}
		txbuf.fields.header = SpiTransferHeader(read, addr);

		if(mSpiXfer(mSpiContext, txbuf.raw, sizeof(txbuf), rxbuf.raw, sizeof(rxbuf))) {
			if(read) {
				*reg1 = (uint16_t)__ntohs(rxbuf.fields.reg1);
				*reg2 = (uint16_t)__ntohs(rxbuf.fields.reg2);
			}
			return true;
		} else {
			return false;
		}
	}

	bool Mpu9250SpiSensor::Access3Reg16(uint8_t addr, bool read, uint16_t *reg1, uint16_t *reg2, uint16_t *reg3)
	{
		struct Hdr3Reg16 {
			uint8_t header;
			uint16_t reg1;
			uint16_t reg2;
			uint16_t reg3;
		} __attribute__((packed));

		union Payload3Reg16 {
			struct Hdr3Reg16 fields;
			uint8_t raw[sizeof(Hdr3Reg16)];

		} __attribute__((packed));

		union Payload3Reg16 rxbuf;
		union Payload3Reg16 txbuf;

		if(!read) {
			txbuf.fields.reg1 = __htons(*reg1);
			txbuf.fields.reg2 = __htons(*reg2);
			txbuf.fields.reg3 = __htons(*reg3);
		} else {
			memset(&txbuf, 0, sizeof(txbuf));
		}
		txbuf.fields.header = SpiTransferHeader(read, addr);

		if(mSpiXfer(mSpiContext, txbuf.raw, sizeof(txbuf), rxbuf.raw, sizeof(rxbuf))) {
			if(read) {
				*reg1 = (uint16_t)__ntohs(rxbuf.fields.reg1);
				*reg2 = (uint16_t)__ntohs(rxbuf.fields.reg2);
				*reg3 = (uint16_t)__ntohs(rxbuf.fields.reg3);
			}
			return true;
		} else {
			return false;
		}
	}

	bool Mpu9250SpiSensor::ModeWakeOnMotion(float gThreshold)
	{
		/*
		 * enable Wake-on-Motion interrupt according to
		 * MPU9250 Product Specification v1.1,
		 * Section 6.1: Wake-on-Motion Interrupt
		 */
		if(!ChangeReg16(regPwrMgmt,
				~  ( regPwrMgmt_Cycle
				   | regPwrMgmt_Sleep
				   | regPwrMgmt_Gyro_StandBy
				   | regPwrMgmt_Dis_XAccel
				   | regPwrMgmt_Dis_YAccel
				   | regPwrMgmt_Dis_ZAccel ),
				(    regPwrMgmt_Dis_XGyro
				   | regPwrMgmt_Dis_YGyro
				   | regPwrMgmt_Dis_ZGyro ) ) )
			return false;

		if(!ChangeReg16(regAccelConfig,
				~  ( regAccelConfig_FChoic_B(0xff)
				   | regAccelConfig_A_Dlpf_Cfg(0xff) ),
				(    regAccelConfig_FChoic_B(1)
				   | regAccelConfig_A_Dlpf_Cfg(1) ) ) )
			return false;

		if(!SetReg8(regIntPinCfg,
				  regIntPinCfg_ActiveLow
				| regIntPinCfg_Latch_Int_En
				| regIntPinCfg_Bypass_En ) )
			return false;

		if(!SetReg8(regIntEnable, regIntEnable_WOM))
			return false;

		if(!SetReg8(regMotDetectCtrl,
				(   regMotDetectCtrl_Accel_Intel_En
				  | regMotDetectCtrl_Accel_Intel_Mode ) ) )
			return false;

		if(!SetReg8(regWomThr, G2WomThr(gThreshold)))
			return false;

#if 0
		if(!SetReg8(regLpAccelOdr,
				regLpAccelOdr_Lposc_Clksel(regLpAccelOdr_Lposc_Clksel_250Hz)))
			return false;

		// XXX this does not work:
		if(!ChangeReg16(regPwrMgmt,
				0xffff,
				regPwrMgmt_Cycle ) )
			return false;
#endif

		return true;
	}

	void Mpu9250SpiSensor::PrintAllRegisters(void)
	{
		for(uint8_t reg=0; reg<=0x7e; ++reg) {
			uint8_t val;
			if(GetReg8(reg, &val))
				printf("0x%02x=0x%02x\r\n", reg, val);
			else
				printf("0x%02x: unreadable\r\n", reg);
		}
	}

} // end of namespace embedded_drivers

