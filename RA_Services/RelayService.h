#ifndef __RELAYSERVICE_H__
#define __RELAYSERVICE_H__

#include <Arduino.h>

class RelayService
{
public:
    // Notes: 
    // Calling On() with a relay port that is already on should be a no-op.
    void On(byte ID);
    void Off(byte ID);
    boolean Status(byte ID) const;
};

class RelayServiceInterface
{
public:
    constexpr RelayServiceInterface(RelayService *relay, byte ID) : relay(relay), ID(ID) {};
    inline void On() { relay->On(ID); };
    inline void Off() { relay->Off(ID); };
    inline boolean Status() { relay->Status(ID); };
private:
    RelayService *const relay;
    const byte ID;
};

#endif // __RELAYSERVICE_H__