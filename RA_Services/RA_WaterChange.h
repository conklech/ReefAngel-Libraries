#ifndef __RA_WATERCHANGE_H__
#define __RA_WATERCHANGE_H__

/*
 * Copyright 2018 Christian W. Conkle
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

#include <Globals.h>
#include <Relay.h>

/*
 * UNFINISHED Automatic water changes using relays.
 */ 
class WaterChange
{
public:
    constexpr WaterChange() : waterEmptyRelay() {};

private:
    // What to control:
    const byte waterEmptyRelay; // Relay to turn on for emptying tank, or 0 for none
    const byte waterFillRelay; // Relay to turn on for filling tank from reservoir
    
    // How to detect water level conditions?
    // Either with float switches (pins)
    const uint8_t lowTankPin; // Pin for sensing low water level
    // If true, a high/on signal from the low tank pin indicates low tank condition.
    // Otherwise, and by default, a low/off signal from the low tank pin indicates low
    // tank condition. 
    const bool lowTankPinReversed; 
    const uint8_t highTankPin;
    const bool highTankPinReversed;
    const uint8_t reservoirEmptyPin;
    const bool reservoirEmptyPinReversed;
    // Or with a water level expansion
    const bool useWaterLevelExpansion;
    const byte waterLevelExpansionChannel;
    // TODO Support second water level expansion channel (for new-water tank)

    // When to trigger a water changes?
};

#endif // __RA_WATERCHANGE_H__