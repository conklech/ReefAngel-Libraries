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

#ifndef __RA_TFT_H__
#define __RA_TFT_H__

#include <Arduino.h>
#include <avr/io.h>

#if defined RA_TOUCH || defined RA_TOUCHDISPLAY || defined RA_STAR

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

#define RST0  PIOC->PIO_CODR |= 1<<20;
#define RST1  PIOC->PIO_SODR |= 1<<20;
#define CS0  PIOC->PIO_CODR |= 1<<19;
#define CS1  PIOC->PIO_SODR |= 1<<19;
#define RS0  PIOC->PIO_CODR |= 1<<18;
#define RS1  PIOC->PIO_SODR |= 1<<18;
#define WR0  PIOC->PIO_CODR |= 1<<17;
#define WR1  PIOC->PIO_SODR |= 1<<17;

#endif // defined RA_TOUCH || defined RA_TOUCHDISPLAY

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
	static void Scroll(int offset);};

#endif // __RA_TFT_H__
