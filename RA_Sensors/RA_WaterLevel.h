#ifndef __RA_WATERLEVEL_H__
#define __RA_WATERLEVEL_H__

/*
 * Interface to the several ways of determining where the water level is:
 * - One float switch (like former SingleATO system)
 * - Two float switches (like former StandardATO system)
 * - Water Level expansions
 */

/* 
 * Generalized reresentation of where the water level is relative to either the physical
 * float switches or the preset analog water level. The water can be:
 * - just too low, e.g. below the bottom float switch (need to topoff)
 * - just too high, e.g. above the top float switch (need to stop adding water)
 * - somewhere between (keep doing what you're doing, e.g. nothing) 
 * - error state (e.g. below bottom float but above top float; something is wrong)
 */
enum WaterLevelState { wlLow, wlMiddle, wlHigh, wlError };

/*
 * Abstract base class for water level sensors.
 * 
 * The derived classes should always be declared in a constexpr block, e.g.:
 * 
 * constexpr auto sensor = SingleFloatWaterLevelSensor(switchPin);
 *
 * Virtual dispatch costs SRAM on AVR, because the vtable has to be stored in SRAM.
 * Sprinking in "constexpr" can help, but won't fully eliminate the vtable. See
 * https://gcc.godbolt.org/z/XH1Uny. Worse, if you're not careful you pay the vtable
 * cost even if you use templating. See https://gcc.godbolt.org/z/POco5l. In the second
 * example, nothing about the base class is ever used, but it dramatically increases both
 * the code and SRAM usage.
 * 
 */
class WaterLevelSensor 
{
public:
    virtual WaterLevelState GetWaterLevelState() const = 0;
};

class StandardTwoFloatWaterLevelSensor : public WaterLevelSensor
{
public:
    constexpr StandardTwoFloatWaterLevelSensor
        ( uint8_t lowPin
        , uint8_t highPin
        , int lowSignalWhenSubmerged=LOW
        , int highSignalWhenSubmerged=HIGH)
        : lowPin(lowPin)
        , highPin(highPin)
        , lowSignalWhenSubmerged(lowSignalWhenSubmerged)
        , highSignalWhenSubmerged(highSignalWhenSubmerged) {};
    inline WaterLevelState GetWaterLevelState() const override;
private:
    const uint8_t lowPin, highPin;
    const int lowSignalWhenSubmerged, highSignalWhenSubmerged;
};

WaterLevelState StandardTwoFloatWaterLevelSensor::GetWaterLevelState() const
{
    const bool aboveLow  =  lowSignalWhenSubmerged == digitalRead(lowPin);
    const bool aboveHigh = highSignalWhenSubmerged == digitalRead(highPin);
    
    if      ( aboveLow &&  aboveHigh)  return wlHigh;
    else if ( aboveLow && !aboveHigh)  return wlMiddle;
    else if (!aboveLow && !aboveHigh)  return wlLow;
    else /* (!aboveLow &&  aboveHigh)*/return wlError;       
}

class SingleFloatWaterLevelSensor : public WaterLevelSensor 
{    
    constexpr SingleFloatWaterLevelSensor
        ( uint8_t switchPin
        , int switchSignalWhenSubmerged=LOW)
        : switchPin(switchPin)
        , switchSignalWhenSubmerged(switchSignalWhenSubmerged)
        {};
        
    inline WaterLevelState GetWaterLevelState() const override;
private:
    const uint8_t switchPin;
    const int switchSignalWhenSubmerged;
};

WaterLevelState SingleFloatWaterLevelSensor::GetWaterLevelState() const
{
    const bool aboveSwitch = switchSignalWhenSubmerged == digitalRead(switchPin);
    if (aboveSwitch) return wlHigh;
    else return wlLow;
}

#endif // __RA_WATERLEVEL_H__