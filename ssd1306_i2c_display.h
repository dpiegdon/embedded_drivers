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

#include <alloca.h>
#include <cstring>
#include <cstdint>
#include <vector>

namespace embedded_drivers {

#define SSD1306DisplayCommand(...) I2cTx({0x00, __VA_ARGS__})

	class Ssd1306I2cDisplay {
	public:
		Ssd1306I2cDisplay(void * sleepMsecsContext,
				void(*sleepMsecs)(void * context, unsigned msecs),
				void * mI2cContext,
				bool(*i2cTx)(void * context, uint8_t address, uint8_t const * buffer, size_t len),
				bool(*i2cRx)(void * context, uint8_t address, uint8_t * buffer, size_t len),
				uint8_t const * font_data,
				unsigned font_width,
				unsigned font_height,
				bool flipLongEdge=false);
		~Ssd1306I2cDisplay(void);

		void Clear(void);
		void ClearColumnsAfterCursor(bool fixCursorPosition = true);
		void ClearLinesAfterCursor(bool fixCursorPosition = true);
		void ClearAfterCursor(bool resetCursorToNull = true);
		bool PutChar(char const c);
		bool Puts(char const *str);
		int Write(char const *data, int const len);

		void Off(void)			{ SSD1306DisplayCommand(0xae); };
		void On(void)			{ SSD1306DisplayCommand(0xaf); };
		void ColorNormal(void)		{ SSD1306DisplayCommand(0xa6); };
		void ColorInvert(void)		{ SSD1306DisplayCommand(0xa7); };
		void HideDisplay(void)		{ SSD1306DisplayCommand(0xa5); };
		void ShowDisplay(void)		{ SSD1306DisplayCommand(0xa4); };

		void CursorSetPosition(unsigned col, unsigned page);

		void Test(void);
	private:
		unsigned const mDisplayWidth;
		unsigned const mDisplayHeight;
		uint8_t const * mFontData;
		unsigned const mFontWidth;
		unsigned const mFontHeight;
		unsigned const mFontFaceSize;
		unsigned const mColumnCount;
		unsigned const mLineCount;
		bool const mFlipLongEdge;
		uint8_t const mAddress;
		std::vector<uint8_t> const mClearCommand;

		unsigned mCursorX;
		unsigned mCursorY;
		unsigned mViewPortY;

		void * mSleepMsecsContext;
		void(*mSleepMsecs)(void * context, unsigned msecs);
		void * mI2cContext;
		bool(*mI2cTx)(void * context, uint8_t address, uint8_t const * buffer, size_t len);
		bool(*mI2cRx)(void * context, uint8_t address, uint8_t * buffer, size_t len);

		void Init(void);

		void CursorApplyPosition(void);

		bool DrawFont(unsigned c)
		{
			unsigned const tx_size = 1+mFontFaceSize;
			uint8_t * buf = static_cast<uint8_t*>(alloca(tx_size));
			buf[0] = 0x40;
			memcpy(buf+1, &mFontData[c*mFontFaceSize], mFontFaceSize);
			return I2cTx(buf, tx_size);
		}

		bool I2cTx(uint8_t const * buf, size_t len)
		{
			return mI2cTx(mI2cContext, mAddress, buf, len);
		}

		bool I2cTx(std::vector<uint8_t> const & payload)
		{
			return mI2cTx(mI2cContext, mAddress, &payload[0], payload.size());
		}

		void SetViewPortY(unsigned y)
		{
			mViewPortY = y % mDisplayHeight;
			SSD1306DisplayCommand(uint8_t(0x40 + mViewPortY));
		}

		void SleepMsecs(unsigned msecs)
		{
			mSleepMsecs(mSleepMsecsContext, msecs);
		}
	};

} // end of namespace embedded_drivers

