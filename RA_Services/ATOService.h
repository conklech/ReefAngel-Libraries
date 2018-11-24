#ifndef __ATOSERVICE_H__
#define __ATOSERVICE_H__

#include <Arduino.h>
#include "RelayService.h"

class ATOServiceVariables 
{
private:
    bool isTopping;
    unsigned long startTime;
    byte Status;
public:
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
class ATOService 
{
public:
    constexpr ATOService(ATOServiceVariables *const variables, RelayServiceInterface *const relay, const unsigned long maxDuration)
        : variables(variables), relay(relay), maxDuration(maxDuration) {};
    // Service interface
    void Setup() {};
    void Run();

    // Returns true if the system state is such that automatic topoff can continue or
    // begin, based on the configured float switches or waterlevel sensor (or both). For
    // example, if the water level is below the configured setpoint (but not so low that
    // an error is detected).
    //
    // Returns false otherwise. For example, returns false if the water level is at or
    // above the setpoint, or if there is an error condition.
    bool IsToppable();
    
    // Returns true if the timer has not yet expired.
    inline bool IsWithinTimeLimit() 
        { return (millis() - variables->startTime) < maxDuration; };
private:
    ATOServiceVariables *const variables;
    RelayServiceInterface *const relay;
    const unsigned long maxDuration;
};

void ATOService::Run()
{
    if(variables->isTopping)
    {
        if(IsWithinTimeLimit())
        {
            if (IsToppable())
            {
                relay->On();
            }
            else
            {
                // The set water level has been reached; stop.
                relay->Off();
                variables->isTopping = false;
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