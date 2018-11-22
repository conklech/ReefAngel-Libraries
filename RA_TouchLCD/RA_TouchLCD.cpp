/*
 * Copyright 2010 Reef Angel / Roberto Imai
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

 /*
  * Updated by:  Curt Binder
  * Updates Released under Apache License, Version 2.0
  */

#include "RA_TouchLCD.h"
#include "RA_Icons.h"

#if defined __SAM3X8E__ || defined RA_TOUCH || defined RA_TOUCHDISPLAY || defined RA_EVOLUTION || defined RA_STAR

void RA_TouchLCD::Init()
{
	RA_TFT::Init();
	SetOrientation(1);
}

void RA_TouchLCD::SetBacklight(byte bl)
{
	RA_TFT::SetBacklight(bl);
}

void RA_TouchLCD::SetOrientation(byte O)
{
	if (orientation!=O)
	{

		RA_TFT::SetOrientation(O);
		switch(O)
		{
		case 1:
		case 3:
			w=239;
			h=319;
			break;
		case 2:
		case 4:
			w=319;
			h=239;
			break;
		}
		FullClear(BKCOLOR);
	}
}


byte RA_TouchLCD::GetOrientation()
{
	return orientation;
}

int RA_TouchLCD::GetWidth()
{
	return w;
}

int RA_TouchLCD::GetHeight()
{
	return h;
}

void RA_TouchLCD::Scroll(int offset)
{
	RA_TFT::Scroll(offset);
}

void RA_TouchLCD::FullClear(int color)
{
	RA_TFT::Clear(color,0,0,GetWidth(),GetHeight());
}
void RA_TouchLCD::Clear(int color, int x1, int y1, int x2, int y2)
{
	RA_TFT::Clear(color,x1,y1,x2,y2);
}

void RA_TouchLCD::DrawPixel(int color, int x, int y)
{
	RA_TFT::DrawPixel(color,x,y);
}

void RA_TouchLCD::DrawLine(int color, int x1, int y1, int x2, int y2)
{
	RA_TFT::DrawLine(color, x1, y1, x2, y2);
}

void RA_TouchLCD::DrawRectangle(int color, int x1, int y1, int x2, int y2, boolean fill)
{
	RA_TFT::DrawRectangle(color, x1, y1, x2, y2, fill);
}

void RA_TouchLCD::DrawCircle(int color, int x, int y, int radius, boolean fill)
{
	RA_TFT::DrawCircle(color, x, y, radius, fill);
}

void RA_TouchLCD::DrawRoundRect(int color, int x1, int y1, int x2, int y2, int radius, boolean fill)
{
	RA_TFT::DrawRoundRect(color, x1, y1, x2, y2, radius, fill);
}

void RA_TouchLCD::DrawBMP(int ix, int iy, const unsigned char *iPtr)
{
	RA_TFT::DrawBMP(ix, iy, iPtr);
}

void RA_TouchLCD::DrawBMP(int ix, int iy, const unsigned char *iPtr, byte overridecolor_msb, byte overridecolor_lsb)
{
	RA_TFT::DrawBMP(ix, iy, iPtr, overridecolor_msb, overridecolor_lsb);
}

void RA_TouchLCD::DrawSDImage(char *bmp, int x, int y)
{
	int bmpWidth, bmpHeight;
	uint8_t bmpImageoffset;

	boolean bmpGood=true;
	File dataFile = SD.open(bmp);
	if (dataFile) {
		if (read16(dataFile) == 0x4D42) {
			  // read and ignore file size & creator bytes
			  read32(dataFile);
			  read32(dataFile);
			  //read data offset
			  bmpImageoffset=read32(dataFile);
			  // read and ignore DIB header
			  read32(dataFile);
			  // read width & height
			  bmpWidth = read32(dataFile);
			  bmpHeight = read32(dataFile);
			  bmpHeight = abs(bmpHeight);

			  if (read16(dataFile) != 1)
				  bmpGood=false;
			  // read bmp depth. Needs to be 24bit
			  if (read16(dataFile) != 24)
				  bmpGood=false;
			  // read compression. We can't read compressed bmp
			  if (read32(dataFile) != 0) {
				  bmpGood=false;
			  }
			  if (bmpGood)
			  {
				  uint16_t p;
				  uint8_t g, b;
				  int i, j;

				  uint8_t sdbuffer[3 * BUFFPIXEL];  // 3 * pixels to buffer
				  uint8_t buffidx = 3*BUFFPIXEL;

				  dataFile.seek(bmpImageoffset);

				  RA_TFT::SetBox(x, y,x+bmpWidth-1, y+bmpHeight-1);
#ifdef ILI9341
				  RA_TFT::SendCom(0x00,0x2C);   /* Write RAM Memory */
#endif
				  for (i=0; i< bmpHeight; i++)
				  {

#if defined RA_TOUCH || defined RA_TOUCHDISPLAY || defined RA_STAR
					  wdt_reset();
#endif // defined RA_TOUCH || defined RA_TOUCHDISPLAY
					  for (j=0; j<bmpWidth; j++)
					  {
						  // read more pixels
						  if (buffidx >= 3*BUFFPIXEL)
						  {
							  dataFile.read(sdbuffer, 3*BUFFPIXEL);
							  buffidx = 0;
						  }

						  // convert pixel from 888 to 565
						  b = sdbuffer[buffidx++];     // blue
						  g = sdbuffer[buffidx++];     // green
						  p = sdbuffer[buffidx++];     // red

						  p >>= 3;
						  p <<= 6;

						  g >>= 2;
						  p |= g;
						  p <<= 5;

						  b >>= 3;
						  p |= b;

						  // write out the 16 bits of color
						  RA_TFT::SendData(p>>8,p);
					  }
				  }
			  }
		}
		dataFile.close();
	}
}

void RA_TouchLCD::DrawSDRawImage(char *bmp, int x, int y, int w, int h)
{
	File dataFile = SD.open(bmp);

	if (dataFile) {
		unsigned long i, j;
		uint8_t sdbuffer[2 * BUFFPIXEL];  // 2 * pixels to buffer
		uint8_t buffidx = 2*BUFFPIXEL;

		RA_TFT::SetBox(x, y,x+w-1, y+h-1);
#ifdef ILI9341
		RA_TFT::SendCom(0x00,0x2C);   /* Write RAM Memory */
#endif
		RS1;
		CS0;
		j=h;
		j*=w;
		for (i=0; i<j; i++)
		{
			// read more pixels
			if (buffidx >= 2*BUFFPIXEL)
			{
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY || defined RA_STAR
					  wdt_reset();
#endif // defined RA_TOUCH || defined RA_TOUCHDISPLAY
				dataFile.read(sdbuffer, 2*BUFFPIXEL);
				buffidx = 0;
			}
			// write out the 16 bits of color
			// RA_TFT::SendData(sdbuffer[buffidx++],sdbuffer[buffidx++]);
#if defined RA_TOUCH || defined RA_TOUCHDISPLAY || defined RA_STAR
			PORTA=sdbuffer[buffidx++];
			PORTC=sdbuffer[buffidx++];
#elif defined(__SAM3X8E__)
			PIOC->PIO_ODSR = sdbuffer[buffidx++]<<9 | sdbuffer[buffidx++]<<1;
#endif // defined RA_TOUCH || defined RA_TOUCHDISPLAY
			WR0;
			WR1;
		}
		CS1;
		dataFile.close();
	}
}


void RA_TouchLCD::DrawDateTime(unsigned long t, int x, int y, boolean militarytime, FontClass Font)
{
	char text[15];

	//Font.SetColor(TOPBAR_FC,TOPBAR_BC,false);
	sprintf(text,"%02d/%02d/%02d ",month(t),day(t),year(t)-2000);
	Font.DrawText(x,y,text);
	if (militarytime)
	{
		sprintf(text,"%02d:%02d:%02d ",hour(t),minute(t),second(t));
	}
	else
	{
		int h = hour(t);
		if(h > 12) {
			h = h - 12;
		} else if ( h == 0 ) {
			h = 12;
		}
		sprintf(text, "%02d:%02d:%02d %s", h, minute(t), second(t), (hour(t)>=12)?"PM":"AM");
	}
	Font.DrawText(text);
}

void RA_TouchLCD::DrawSetupDateTime(int x, int y, char *value, FontClass Font)
{
	DrawBMP(x-12,y-20,ARROWUP);
	Font.DrawCenterText(x,y,value);
	DrawBMP(x-12,y+34,ARROWDOWN);
}

void RA_TouchLCD::DrawSetupDateTime(int x, int y, byte value, FontClass Font)
{
	char sdate[5];
	sprintf(sdate," %02d ",value);
	DrawSetupDateTime(x,y,sdate,Font);
}

void RA_TouchLCD::DrawRelayStatus(int x, int y, boolean status)
{
	int color;
	if (status) color=RELAYGREEN; else color=COLOR_RED;
	for (int c=0;c<=6;c++) DrawLine(alphaBlend(color,(10-c)*10),x+c+4,y,x+c+4,y+17);
	for (int c=4;c<=10;c++) DrawLine(alphaBlend(color,c*10),x+c+14,y,x+c+14,y+17);
	Clear(alphaBlend(color,40),x+11,y,x+17,y+17);
	DrawLine(COLOR_SILVER,x+1,y-1,x+27,y-1);
	DrawLine(COLOR_SILVER,x+1,y+18,x+27,y+18);
//	DrawLine(COLOR_SILVER,x+34,y,x+34,y+17);
	for (int c=0;c<=3;c++) DrawLine(alphaBlend(COLOR_SILVER,80-(c*10)),x+c,y,x+c,y+17);
	for (int c=0;c<=3;c++) DrawLine(alphaBlend(COLOR_SILVER,(c*10)+50),x+25+c,y,x+25+c,y+17);
//	Clear(color,x+10,y,x+15,y+17);

}

#endif // defined RA_TOUCH || defined RA_TOUCHDISPLAY || defined RA_EVOLUTION || defined RA_STAR)
