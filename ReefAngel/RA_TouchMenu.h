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
  * Updated by:  Christian Conkle
  * Updates Released under Apache License, Version 2.0
  */

#ifndef __RA_TOUCHMENU_H__
#define __RA_TOUCHMENU_H__

#include <Arduino.h>
#include <Time.h>
#include <Font.h>

#include <Globals.h>
#include <ReefAngel.h>
#include <RA_TouchLCD.h>
#include <RA_TS.h>
#include <RA_TFT.h>
#include <InternalEEPROM.h>

class ButtonClass
{
	public:
		ButtonClass();
		void Create(int color, int textcolor, const char *str);
		void Create(int color, int textcolor, const char *str, const unsigned char *Ptr);
		void inline SetPosition(int ix1, int iy1) { x1=ix1; y1=iy1; };
		void Show();
		void Hide(int bkcolor);
		boolean IsPressed();
	private:
		int color, x1, x2, y1, textcolor;
		const char *str;
		const unsigned char *Ptr;
		boolean visible;
};

class ProgressBarClass
{
	public:
		ProgressBarClass();
		void Create(int color, int bkcolor, int textcolor, char *str);
		void inline SetPosition(int ix1, int iy1) { x1=ix1; y1=iy1; };
		void SetCurrent(int value);
		int inline GetCurrent() { return current; };
		void inline SetLabel(char *value) { str=value; };
		void inline SetLabel(String value) { char buff[value.length()+2]; value.toCharArray(buff, value.length()+1); strcpy(str,buff); }; 
		void inline SetLabelP(const char *value) { char buff[16]; byte index=0; while( buff[index++] = pgm_read_byte(value++)); strcpy(str,buff); };
 		void inline SetColor(int value) { color=value; };
		void inline SetBackColor(int value) { bkcolor=value; };
		void inline SetTextColor(int value) { textcolor=value; };
		void Show();
		void Hide();
		boolean IsPressed();
		boolean NeedsRedraw;
	private:
		int color, bkcolor, x1, y1, textcolor;
		int min,max,current;
		char *str;
		boolean visible;
};

class SliderClass
{
	public:
		SliderClass();
		void Create(int color, int textcolor, char *str);
		void inline SetPosition(int ix1, int iy1) { x1=ix1; y1=iy1; };
		void inline SetMin(int value) { min=value; };
		void inline SetMax(int value) { max=value; };
		void inline SetCurrent(int value) { current=value; };
		void inline SetOverrideID(int value) { overrideid=value; };
		int inline GetMin() { return min; };
		int inline GetMax() { return max; };
		int inline GetCurrent() { return current; };
		int inline GetOverrideID() { return overrideid; };
		void inline SetColor(int value) { color=value; };
		void inline SetLabel(char *value) { str=value; };
		void inline SetLabel(String value) { char buff[value.length()+2]; value.toCharArray(buff, value.length()+1); strcpy(str,buff); };
		void inline SetLabelP(const char *value) { char buff[16]; byte index=0; while( buff[index++] = pgm_read_byte(value++)); strcpy(str,buff); };
		void DrawMarker();
		void Show();
		void Hide();
		void Refresh();
		boolean IsPlusPressed();
		boolean IsMinusPressed();
	private:
		int color, x1, y1, textcolor;
		int min,max,current;
		byte overrideid;
		char *str;
		boolean visible;
		boolean NeedsRedraw;
	
};

class RATouchMenu {
public:
    // Init() must be called before ReefAngel->Init() but before doing anything else
    void Init();

    // Show the main touch interface. This should be called in every loop
    // iteration.
    //
    // As of library version 1.1.x, this function also handled all of the actual
    // ReefAngel functionality, such as updating the output pins, uploading data
    // to the cloud, etc. This functionality is being moved into separate
    // functions.
    //
    // Calls ReefAngel->Refresh().
    void ShowTouchInterface();	

    // Process calibration screen mode changes. This used to live in
    // ReefAngelClass. If/when the codebase is refactored so that activities do
    // not depend on the interface state, much of the functionality here should
    // go back to ReefAngelClass.
    void HandleCalibrationChangeMode(); 

    // This code used to be in ShowTouchInterface().
    //
    // TODO: Document or refactor ShowTouchInterface().
    void DrawTouchInterface();

    void ChangeOrientation();
    void SetOrientation(byte o);
    void CalibrateTouchScreen();
    void ChangeDisplayedScreen(signed char index);
    void MainScreen();
    void ResetScreenSaver();
    void SetupTouchDateTime();
    void CheckMenuTimeout();
    void ShowTouchMenu(byte index);
    
    void LightsOn();
    void LightsOff();
    
    // The following need to be accessible to the Slider etc. classes
    FontClass SmallFont;
    FontClass Font;
    FontClass LargeFont;
    ButtonClass OkButton;
    ButtonClass CancelButton;
    ProgressBarClass PB[6];
    SliderClass Slider;
    
    RA_TouchLCD TouchLCD;
    RA_TS TS;
private:

// TODO: Move extension menus into extensions. Note that we use function
// pointers internally though.

    inline void FeedingModeStart() { ReefAngel->FeedingModeStart(); };
    inline void WaterChangeModeStart () { ReefAngel->WaterChangeModeStart(); };
    inline void Reboot() { ReefAngel->Reboot(); };
    inline void ATOClear() { ReefAngel->ATOClear(); };
    inline void OverheatClear() { ReefAngel->OverheatClear(); };
    inline void LeakClear() { ReefAngel->LeakClear(); };
    void SetupTouchCalibratePH(); 
    void ShowTouchCalibratePH();
    void SetupTouchCalibrateSal();
    void ShowTouchCalibrateSal();
    void SetupTouchCalibrateORP();
    void ShowTouchCalibrateORP();
    void SetupTouchCalibratePHExp();
    void ShowTouchCalibratePHExp();
    void SetupTouchCalibrateWL(byte channel);
    void ShowTouchCalibrateWL(byte channel);
    void SetupTouchCalibrateCustom(byte index);
    void ShowTouchCalibrateCustom(byte index);
    inline void SetupTouchCalibrateWL0() { SetupTouchCalibrateWL(0); };
    inline void SetupTouchCalibrateWL1() { SetupTouchCalibrateWL(1); };
    inline void SetupTouchCalibrateWL2() { SetupTouchCalibrateWL(2); };
    inline void SetupTouchCalibrateWL3() { SetupTouchCalibrateWL(3); };
    inline void SetupTouchCalibrateWL4() { SetupTouchCalibrateWL(4); };
    inline void SetupTouchCalibrateCustom1() { SetupTouchCalibrateCustom(0); };
    inline void SetupTouchCalibrateCustom2() { SetupTouchCalibrateCustom(1); };
    inline void SetupTouchCalibrateCustom3() { SetupTouchCalibrateCustom(2); };
    inline void SetupTouchCalibrateCustom4() { SetupTouchCalibrateCustom(3); };
    inline void SetupTouchCalibrateCustom5() { SetupTouchCalibrateCustom(4); };
    inline void SetupTouchCalibrateCustom6() { SetupTouchCalibrateCustom(5); };
    inline void SetupTouchCalibrateCustom7() { SetupTouchCalibrateCustom(6); };
    inline void SetupTouchCalibrateCustom8() { SetupTouchCalibrateCustom(7); };
    void CheckTouch();
    void ProcessTouch();
    void ReDrawScreen();
    void DisplayVersion();
    void ClearScreen(int Color);
	void ExitMenu();
    
    // TODO: Wire up ReefAngelClass::ShowNoInternalMemoryError and RATouchmenu::ShowNoInternalMemoryError
    void ShowNoInternalMemoryError();

    // Menu internal state
    byte orientation = 4;
    byte LastOrientation;
    signed char DisplayedScreen;
    signed char RecallScreen;
    bool TouchEnabled;
    bool Sleeping;
    bool NeedsRedraw;
    unsigned long lastRedraw; 
    unsigned long lastDisplayChange; 
	time_t menutimeout;
    boolean Splash;

    boolean MilitaryTime;
        
    ParamsStruct LastParams;
    
    typedef void (RATouchMenu::* FuncPtr) (); // declare function pointers
    FuncPtr MenuFunctionPtr;
    FuncPtr menu_button_functions1[6];
    FuncPtr menu_button_functions2[6];
    FuncPtr menu_button_functions3[6];
    FuncPtr menu_button_functions4[6];
    FuncPtr menu_button_functions5[6];
    FuncPtr menu_button_functions6[6];
    void inline Touch() {}; // Used for function pointer in 

    
    time_t newnow; // TODO Determine whether RATouchMenu.newnow is necessary

    // Calibration values
    int CalVal1, CalVal2;
    byte CalStep;
};

extern RATouchMenu TouchMenu;  // make an instance for the user

#endif  // __RA_TOUCHMENU_H__
