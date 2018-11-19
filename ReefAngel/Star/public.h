// Star

DS1307RTC RTC;
RA_ATOALARMClass AlarmInput;

//byte LongTouch;
bool SDFound;
int leakSensitivity;

void ApplySalinityCompensation();
inline void LeakSensitivity(int value) { leakSensitivity=value; };
