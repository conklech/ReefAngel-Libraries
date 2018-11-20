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
 /*
  * Updated/refactored, 2018, by Christian W. Conkle (cwc)
  * Updates copyright 2018 Christian W. Conkle and released under Apache License, Version 2.0
  */

#ifndef __RA_TFT_H__
#define __RA_TFT_H__

#include <Arduino.h>
#include <avr/io.h>

#if defined RA_TOUCH || defined RA_TOUCHDISPLAY || defined RA_STAR

#define BACKLIGHT_PIN 2

#define CS0 bitClear(PORTE,5)
#define CS1 bitSet(PORTE,5)
#define RS0 bitClear(PORTE,3)
#define RS1 bitSet(PORTE,3)
#define WR0 bitClear(PORTE,6)
#define WR1 bitSet(PORTE,6)
#define RST0 bitClear(PORTH,3)
#define RST1 bitSet(PORTH,3)
#define BL0 bitClear(PORTE,4)
#define BL1 bitSet(PORTE,4)
#define LED0 bitClear(PORTH, 4)
#define LED1 bitSet(PORTH, 4)

#elif defined(__SAM3X8E__)

#define BACKLIGHT_PIN 3

#define RST0  PIOC->PIO_CODR |= 1<<20;
#define RST1  PIOC->PIO_SODR |= 1<<20;
#define CS0  PIOC->PIO_CODR |= 1<<19;
#define CS1  PIOC->PIO_SODR |= 1<<19;
#define RS0  PIOC->PIO_CODR |= 1<<18;
#define RS1  PIOC->PIO_SODR |= 1<<18;
#define WR0  PIOC->PIO_CODR |= 1<<17;
#define WR1  PIOC->PIO_SODR |= 1<<17;

#endif // defined RA_TOUCH || defined RA_TOUCHDISPLAY

// This class provides direct control over the Reef Angel display hardware (not including
// touch functionality).
//
// It differs from RA_TouchLCD in that it does not depend on any of the other Reef Angel
// infrastructure.
//
// Nominally this supports several Reef Angel models, but it has not been tested on
// anything but the Star since it was significantly refactored.
//
// There are two sets of compile-time conditionals in this file: for the Reef Angel system
// (eg RA_TOUCH) and for the display chipset/protocol (eg ILI9341). During the refactor it
// was not clear to me (cwc) which systems had which chipset, so I preserved mostly the
// status quo.
class RA_TFT
{
public:
	static void Init();
	static void SendComData(int Com,int Data);
	static void SendCom(byte H,byte L);
	static void SendData(byte H,byte L);
	static void WriteBus(byte H,byte L);
	static void Clear(int color, int x1, int y1, int x2, int y2);
	static void SetBox(int x1, int y1, int x2, int y2);
	static void Pixels(int count, const byte* d);
	static void DrawPixel(int color, int x, int y);
	static void Scroll(int offset);
	static void SetBacklight(byte bl);
	static void SetOrientation(byte orientation);
	static void DrawLine(int color, int x1, int y1, int x2, int y2);
	static void DrawRectangle(int color, int x1, int y1, int x2, int y2, boolean fill);
	static void DrawCircle(int color, int x, int y, int radius, boolean fill);
	static void DrawRoundRect(int color, int x1, int y1, int x2, int y2, int radius, boolean fill);
	static void DrawBMP(int ix, int iy, const unsigned char *iPtr);
	static void DrawBMP(int ix, int iy, const unsigned char *iPtr, byte overridecolor_msb, byte overridecolor_lsb);
};
#endif // __RA_TFT_H__
