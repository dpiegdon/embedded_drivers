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
#include <cerrno>
#include <cassert>

#include "embedded_drivers/ssd1306_i2c_display.h"

namespace embedded_drivers {

	Ssd1306I2cDisplay::Ssd1306I2cDisplay(void * sleepMsecsContext,
				void(*sleepMsecs)(void * context, unsigned msecs),
				void * i2cContext,
				bool(*i2cTx)(void * context, uint8_t address, uint8_t const * buffer, size_t len),
				bool(*i2cRx)(void * context, uint8_t address, uint8_t * buffer, size_t len),
				const uint8_t * font_data,
				unsigned font_width,
				unsigned font_height,
				bool flipLongEdge)
		: mDisplayWidth(128)
		, mDisplayHeight(64)
		, mFontData(font_data)
		, mFontWidth(font_width)
		, mFontHeight(font_height)
		, mFontFaceSize((mFontWidth * mFontHeight) / 8)
		, mColumnCount(mDisplayWidth/mFontWidth)
		, mLineCount(mDisplayHeight/mFontHeight)
		, mFlipLongEdge(flipLongEdge)
		, mAddress(0x3c)
		, mClearCommand{ 0x40,    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, }
		, mSleepMsecsContext(sleepMsecsContext)
		, mSleepMsecs(sleepMsecs)
		, mI2cContext(i2cContext)
		, mI2cTx(i2cTx)
		, mI2cRx(i2cRx)
	{
		assert(8 == mFontHeight);
		Init();
		printf("fontsize %d\r\n", mFontFaceSize);
	}

	Ssd1306I2cDisplay::~Ssd1306I2cDisplay(void)
	{
	}

	void Ssd1306I2cDisplay::Clear(void)
	{

		SSD1306DisplayCommand(0x22, 0, uint8_t(mLineCount-1));
		SSD1306DisplayCommand(0x21, 0, uint8_t(mDisplayWidth-1));
		for(unsigned i = 0; i < mLineCount*mDisplayWidth / (sizeof(mClearCommand)-1); ++i)
			I2cTx(mClearCommand);

		SetViewPortY(0);
		CursorSetPosition(0, 0);
	}

	void Ssd1306I2cDisplay::ClearColumnsAfterCursor(bool fixCursorPosition)
	{
		for(unsigned i = 0; i < mColumnCount-mCursorX; ++i)
			DrawFont(0);
		if(fixCursorPosition)
			CursorSetPosition(mCursorX, mCursorY);
	}

	void Ssd1306I2cDisplay::ClearLinesAfterCursor(bool fixCursorPosition)
	{
		int lines = mLineCount-mCursorY-1;
		if(lines > 0) {
			SSD1306DisplayCommand(0x22, uint8_t(mCursorY+1), uint8_t(mLineCount-1));
			SSD1306DisplayCommand(0x21, 0, uint8_t(mDisplayWidth-1));

			for(unsigned i = 0; i < lines * mDisplayWidth / (sizeof(mClearCommand)-1); ++i)
				I2cTx(mClearCommand);

			if(fixCursorPosition)
				CursorSetPosition(mCursorX, mCursorY);
		}
	}

	void Ssd1306I2cDisplay::ClearAfterCursor(bool resetCursorToNull)
	{
		ClearColumnsAfterCursor(false);
		ClearLinesAfterCursor(!resetCursorToNull);
		if(resetCursorToNull)
			CursorSetPosition(0, 0);
	}

	bool Ssd1306I2cDisplay::PutChar(char const c)
	{
		unsigned l = c;

		switch(l) {
			case '\n': // new line
				CursorSetPosition(mCursorX, mCursorY+1);
				return true;
			case '\r': // carriage return
				CursorSetPosition(0, mCursorY);
				return true;
			case '\b': // bell
				ColorInvert();
				SleepMsecs(60);
				ColorNormal();
				return true;
			case '\f': // form feed
				Clear();
				return true;
			case '\0':
				return true;
			case '\x01': // move cursor to the right
				mCursorX += 1;
				CursorApplyPosition();
				return true;
			case '\x02':
				ClearColumnsAfterCursor();
				return true;
			case '\x03':
				ClearLinesAfterCursor();
				return true;
			case '\x04':
				ClearAfterCursor();
				return true;
			default:
				break;
		}

		if((l < 0x20) || (l > 0x7f)) {
			return false;
		} else {
			l -= ' ';

			bool ret = DrawFont(l);
			mCursorX += 1;
			if(mCursorX >= mColumnCount)
				CursorApplyPosition();

			return ret;
		}
	}

	bool Ssd1306I2cDisplay::Puts(char const *str)
	{
		int len = strlen(str);
		return len == Write(str, len);
	}

	int Ssd1306I2cDisplay::Write(char const *data, int const len)
	{
		for(int i=0; i<len; ++i)
			if(!PutChar(data[i]))
				return -EINVAL;
		return len;
	}

	void Ssd1306I2cDisplay::CursorSetPosition(unsigned col, unsigned page)
	{
		mCursorX = col;
		mCursorY = page;
		CursorApplyPosition();
	}

	void Ssd1306I2cDisplay::Init(void)
	{
		SSD1306DisplayCommand(0xd3, 0);		// disable display, start line := COM0
		if(mFlipLongEdge)
			SSD1306DisplayCommand(0xc8);	// COM output scan direction := inverted
		else
			SSD1306DisplayCommand(0xc0);	// COM output scan direction := normal mode
		SSD1306DisplayCommand(0xda, 0x12);	// "alternative" COM pin config, disable COM left/right remap
		SSD1306DisplayCommand(0x81, 127);	// set contrast for to 127
		SSD1306DisplayCommand(0xa4);		// display on
		SSD1306DisplayCommand(0xd5, 0x80);	// set OSC frequency: DIV=1, F=0b1000 (400 KHz?)
		SSD1306DisplayCommand(0x8d, 0x14);	// enable charge pump regulator during display on
		SSD1306DisplayCommand(0x20, 0);		// set horizontal addressing mode
		On();
		Clear();
	}

	void Ssd1306I2cDisplay::CursorApplyPosition(void)
	{
		bool clearLine = false;
		if(mCursorX >= mColumnCount) {
			mCursorX = 0;
			mCursorY += 1;
		}
		if(mCursorY >= mLineCount) {
			mCursorY = mLineCount-1;
			SetViewPortY(mViewPortY + mFontHeight);
			clearLine = true;
		}

		uint8_t col = mCursorX * mFontWidth;
		uint8_t page = (mViewPortY/mFontHeight + mCursorY) % (mDisplayHeight / mFontHeight);
		SSD1306DisplayCommand(0x22, page, page);
		if(clearLine) {
			SSD1306DisplayCommand(0x21, 0, uint8_t(mDisplayWidth-1));
			for(unsigned i = 0; i < (mDisplayWidth / (sizeof(mClearCommand)-1)); ++i)
				I2cTx(mClearCommand);
		}
		SSD1306DisplayCommand(0x21, col, uint8_t(mDisplayWidth-1));
	}

	void Ssd1306I2cDisplay::Test(void)
	{
		unsigned i;

		Clear();

		for(i = 0; i < (2 * mColumnCount * mLineCount); ++i) {
			if(0 == (i % mColumnCount)) {
				PutChar(0x7f);
				SleepMsecs(20);
			} else if(0 == (i / mColumnCount)) {
				PutChar('<');
				SleepMsecs(20);
			} else {
				PutChar('0' + (i%10));
			}
		}

		SleepMsecs(1000);
		Clear();

		for(i=0; i<255; ++i)
			Write("ScrollSpeed <--  ", 17);

		SleepMsecs(1000);
		Clear();

		char buf[] = "Page X ";
		for(unsigned page=0; page<5; ++page) {
			buf[5] = '0'+page;
			for(i=0; i<(mColumnCount * mLineCount / sizeof(buf)); ++i)
				Write(buf, sizeof(buf));
			SleepMsecs(500);
		}

		SleepMsecs(1000);
		Clear();

		for(int x=mColumnCount-3; x>=0; --x) {
			int y = x%mLineCount;
			CursorSetPosition(x,y);
			PutChar('*');
			PutChar('0'+y);
			PutChar('*');
			SleepMsecs(20);
		}
		CursorSetPosition(0, mLineCount-3);
		const char newlineRemovesNextLine[] = "Newline removes next line?";
		const char notThis[] = "\r\nNot this ->";
		const char butThis[] = "\r\nBut this ->";
		Write(newlineRemovesNextLine, sizeof(newlineRemovesNextLine));
		SleepMsecs(1000);
		Write(notThis, sizeof(notThis));
		SleepMsecs(1000);
		Write(butThis, sizeof(butThis));

		SleepMsecs(2000);
		Clear();

		for(i = 0; i < mLineCount*mColumnCount - 1; ++i)
			PutChar('H');
		CursorSetPosition(5, 2);
		PutChar('\x02');
		CursorSetPosition(5, 3);
		Write("^Cleared?", 9);

		SleepMsecs(2000);
		Clear();

		for(i = 0; i < mLineCount*mColumnCount - 1; ++i)
			PutChar('V');
		CursorSetPosition(5, 2);
		PutChar('\x03');
		CursorSetPosition(2, 2);
		Write("__Cleared__?", 12);

		SleepMsecs(2000);
		Clear();

		for(i = 0; i < mLineCount*mColumnCount - 1; ++i)
			PutChar('X');
		CursorSetPosition(16, 1);
		PutChar('\x04');
		CursorSetPosition(1, 1);
		Write("__Cleared_\x01\x01\x01>?", 15);

		SleepMsecs(2000);
		Clear();

		Write("On/Off", 6);
		SleepMsecs(500);
		Off();
		SleepMsecs(500);
		On();
		SleepMsecs(500);
		Off();
		SleepMsecs(500);
		On();

		Clear();

		Write("Invert/Normal", 13);
		SleepMsecs(500);
		ColorInvert();
		SleepMsecs(500);
		ColorNormal();
		SleepMsecs(500);
		ColorInvert();
		SleepMsecs(500);
		ColorNormal();

		Clear();

		Write("Hide/Show", 9);
		SleepMsecs(500);
		HideDisplay();
		SleepMsecs(500);
		ShowDisplay();
		SleepMsecs(500);
		HideDisplay();
		SleepMsecs(500);
		ShowDisplay();

		Clear();

		PutChar('\n');
		for(i=0x20; i<=0x7f; ++i)
			PutChar(i);

		SleepMsecs(10000);
		Clear();
	}

} // end of namespace embedded_drivers

