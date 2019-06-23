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

#include <cstdint>

namespace embedded_drivers {

	class Mpu9250SpiSensor {

		typedef bool(*SpiXferCallback)(void * spi_context, uint8_t const * tx_buf, size_t tx_size, uint8_t * rx_buf, size_t rx_size);
	public:
		Mpu9250SpiSensor(void * spiContext, SpiXferCallback spiXfer);

		bool Reset(void)
		{ return SetReg16(regPwrMgmt, regPwrMgmt_Reset); }

		bool ReadAccel(int16_t *x, int16_t *y, int16_t *z)
		{ return Access3Reg16(regAccel, true, (uint16_t*)x, (uint16_t*)y, (uint16_t*)z); };

		bool ReadGyro(int16_t *x, int16_t *y, int16_t *z)
		{ return Access3Reg16(regGyro, true, (uint16_t*)x, (uint16_t*)y, (uint16_t*)z); };

		bool ReadTemp(float *temp)
		{
			uint16_t rawTemp;
			if(!Access1Reg16(regTemp, true, &rawTemp))
				return false;
			*temp = rawTemp / 333.87 + 21.0;
			return true;
		}

		/* enable WakeOnMotion interrupt with given threshold in g */
		bool ModeWakeOnMotion(float gThreshold);

		bool AcknowledgeInterrupt(uint8_t *interruptFlags)
		{ return GetReg8(regIntStatus, interruptFlags); }

		static float WomThr2g(uint8_t womThr)
		{ return float(womThr)*(4./1000.); }

		static uint8_t G2WomThr(float g)
		{ return uint8_t(g*(1000/4)); }

		void PrintAllRegisters(void);

		unsigned const regAccelConfig = 0x1c;
		unsigned const regAccelConfig_Ax_St_En = (1<<15);
		unsigned const regAccelConfig_Ay_St_En = (1<<14);
		unsigned const regAccelConfig_Az_St_En = (1<<13);
		static unsigned regAccelConfig_Accel_Fs_Sel(unsigned x) { return ((x&3)<<11); }
		static unsigned regAccelConfig_FChoic_B(unsigned x) { return ((x&3)<<2); }
		static unsigned regAccelConfig_A_Dlpf_Cfg(unsigned x) { return ((x&3)<<0); }

		unsigned const regLpAccelOdr = 0x1e;
		static unsigned regLpAccelOdr_Lposc_Clksel(unsigned x) { return (x&0xf); }
		unsigned const regLpAccelOdr_Lposc_Clksel_0_24Hz  = 0;
		unsigned const regLpAccelOdr_Lposc_Clksel_0_49Hz  = 1;
		unsigned const regLpAccelOdr_Lposc_Clksel_0_98Hz  = 2;
		unsigned const regLpAccelOdr_Lposc_Clksel_1_95Hz  = 3;
		unsigned const regLpAccelOdr_Lposc_Clksel_3_91Hz  = 4;
		unsigned const regLpAccelOdr_Lposc_Clksel_7_81Hz  = 5;
		unsigned const regLpAccelOdr_Lposc_Clksel_15_63Hz = 6;
		unsigned const regLpAccelOdr_Lposc_Clksel_31_25Hz = 7;
		unsigned const regLpAccelOdr_Lposc_Clksel_62_50Hz = 8;
		unsigned const regLpAccelOdr_Lposc_Clksel_125Hz   = 9;
		unsigned const regLpAccelOdr_Lposc_Clksel_250Hz   = 10;
		unsigned const regLpAccelOdr_Lposc_Clksel_500Hz   = 11;

		unsigned const regWomThr = 0x1f;

		unsigned const regIntPinCfg = 0x37;
		unsigned const regIntPinCfg_ActiveLow = (1<<7);
		unsigned const regIntPinCfg_OpenDrain = (1<<6);
		unsigned const regIntPinCfg_Latch_Int_En = (1<<5);
		unsigned const regIntPinCfg_AnyRead2ClearInt = (1<<4);
		unsigned const regIntPinCfg_ActiveLow_Fsync = (1<<3);
		unsigned const regIntPinCfg_Fsync_Int_Mode_En = (1<<2);
		unsigned const regIntPinCfg_Bypass_En = (1<<1);

		unsigned const regIntEnable = 0x38;
		unsigned const regIntEnable_WOM = (1<<6);
		unsigned const regIntEnable_FIFO_Overflow = (1<<4);
		unsigned const regIntEnable_FSyncInt = (1<<3);
		unsigned const regIntEnable_RawDataReady = (1<<0);

		unsigned const regIntStatus = 0x3a;
		unsigned const regIntStatus_WOM = (1<<6);
		unsigned const regIntStatus_FIFO_Overflow = (1<<4);
		unsigned const regIntStatus_FSyncInt = (1<<3);
		unsigned const regIntStatus_RawDataReady = (1<<0);

		unsigned const regAccel = 0x3b;

		unsigned const regTemp = 0x41;

		unsigned const regGyro = 0x43;

		unsigned const regMotDetectCtrl = 0x69;
		unsigned const regMotDetectCtrl_Accel_Intel_En = (1<<7);
		unsigned const regMotDetectCtrl_Accel_Intel_Mode = (1<<6);

		unsigned const regPwrMgmt = 0x6b;
		unsigned const regPwrMgmt_Reset = (1<<15);
		unsigned const regPwrMgmt_Cycle = (1<<14);
		unsigned const regPwrMgmt_Sleep = (1<<13);
		unsigned const regPwrMgmt_Gyro_StandBy = (1<<12);
		unsigned const regPwrMgmt_Pd_Stat = (1<<11);
		static unsigned regPwrMgmt_ClkSel(unsigned x) { return ((x&7)<<8); }
		unsigned const regPwrMgmt_Dis_XAccel = (1<<5);
		unsigned const regPwrMgmt_Dis_YAccel = (1<<4);
		unsigned const regPwrMgmt_Dis_ZAccel = (1<<3);
		unsigned const regPwrMgmt_Dis_XGyro = (1<<2);
		unsigned const regPwrMgmt_Dis_YGyro = (1<<1);
		unsigned const regPwrMgmt_Dis_ZGyro = (1<<0);

	private:
		void * mSpiContext;
		SpiXferCallback mSpiXfer;

		uint8_t SpiTransferHeader(bool read, uint8_t reg)
		{ return (read?1:0) << 7 | reg; }

		bool Access1Reg8(uint8_t addr, bool read, uint8_t *reg1);
		bool Access1Reg16(uint8_t addr, bool read, uint16_t *reg1);
		bool Access2Reg16(uint8_t addr, bool read, uint16_t *reg1, uint16_t *reg2);
		bool Access3Reg16(uint8_t addr, bool read, uint16_t *reg1, uint16_t *reg2, uint16_t *reg3);

		bool ChangeReg8(uint8_t addr, uint8_t andMask, uint8_t orMask)
		{
			uint8_t value;
			if(!Access1Reg8(addr, true, &value))
				return false;
			value = (value & andMask) | orMask;
			return Access1Reg8(addr, false, &value);
		}

		bool ChangeReg16(uint8_t addr, uint16_t andMask, uint16_t orMask)
		{
			uint16_t value;
			if(!Access1Reg16(addr, true, &value))
				return false;
			value = (value & andMask) | orMask;
			return Access1Reg16(addr, false, &value);
		}

		bool SetReg8(uint8_t addr, uint8_t value)
		{ return Access1Reg8(addr, false, &value); }

		bool SetReg16(uint8_t addr, uint16_t value)
		{ return Access1Reg16(addr, false, &value); }

		bool GetReg8(uint8_t addr, uint8_t *value)
		{ return Access1Reg8(addr, true, value); }

		bool GetReg8(uint8_t addr, uint16_t *value)
		{ return Access1Reg16(addr, true, value); }
	};

} // end of namespace embedded_drivers

