#ifndef __ATOSERVICE_H__
#define __ATOSERVICE_H__

#include <Arduino.h>
#include "RelayService.h"
#include <RA_WaterLevel.h>


class ATOServiceVariables 
{
private:
    bool isTopping;
    unsigned long startTime;
    byte Status;
public:
    template <typename T>
    friend class ATOService;
};
/*
 * Under the service architecture, this class is entirely constant. Every instance of the
 * class should be defined using the constexpr keyword, e.g.
 *
 * auto atoVariables = ATOServiceVariables(...);
 *
 * constexpr auto atoService = * ATOService(&atoVariables, ...);
 *
 * With this design, the optimizer will ensure that the ATOService itself takes no space
 * in RAM.
 *
 */
template <typename WaterLevelSensorType>
class ATOService 
{
public:
    constexpr ATOService
        (ATOServiceVariables *const variables
        , RelayServiceInterface *const relay
        , const unsigned long maxDuration
        , const WaterLevelSensorType sensor)
        : variables(variables), relay(relay), maxDuration(maxDuration), sensor(sensor) {};
    // Service interface
    void Setup() {};
    void Run();
    
    // Returns true if the timer has not yet expired.
    inline bool IsWithinTimeLimit() 
        { return (millis() - variables->startTime) < maxDuration; };


private:
    ATOServiceVariables *const variables;
    RelayServiceInterface *const relay;
    const unsigned long maxDuration;
    const WaterLevelSensorType sensor;
};

template <typename WaterLevelSensorType>
void ATOService<WaterLevelSensorType>::Run()
{
    if(variables->isTopping)
    {
        if(IsWithinTimeLimit())
        {
            switch (sensor.GetWaterLevelState())
            {
                case wlLow: 
                    relay->On(); 
                    break;
                case wlMiddle:
                    relay->On();
                    break;
                case wlHigh:
                    relay->Off();
                    variables->isTopping = false;
                    break;
                case wlError:
                    // TODO: Log/report error condition
                    relay->Off();
                    break;
            }
        }   
        else
        {
            // We have been running too long; stop.
            relay->Off();
            // TODO: Log/report that ATOService hit the max duration
            variables->isTopping = false;
        }
    }
}

#endif // __ATOSERVICE_H__