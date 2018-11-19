// Star
Serial.println(F("RA Init"));
Board=RAStar;
wdt_enable(WDTO_2S);
EM=0;
EM1=0;
REM=0;
SDFound=false;
pinMode(53,OUTPUT);
digitalWrite(53,HIGH); // Pull up resistor on hardware SS SPI
Serial1.begin(57600);
Serial2.begin(57600);
pinMode(SDPin,OUTPUT);
digitalWrite(SDPin,HIGH);
pinMode(BuzzerPin,OUTPUT);
pinMode(actinic2PWMPin,OUTPUT);
pinMode(daylight2PWMPin,OUTPUT);
pinMode(i2cMuxEnable,OUTPUT);
pinMode(i2cEnable1,OUTPUT);
pinMode(i2cEnable2,OUTPUT);
digitalWrite(actinic2PWMPin,LOW); //pull down resistor on actinicPWMPin
digitalWrite(daylight2PWMPin,LOW); //pull down resistor on daylightPWMPin
DDRJ&=(0<<3); //PJ3 as input (SD card detect pin)
PORTJ|=(1<<3); //PJ3 pull up
DDRJ&=(0<<4); //PJ4 as input (Alarm pin)
PORTJ|=(1<<4); //PJ4 pull up
DDRH|=(1<<2); // Port PH2 output (Exp Bus Power)
cbi(PORTH,2); // Turn on exp bus power
digitalWrite(i2cMuxEnable,LOW);
delay(10);
digitalWrite(i2cMuxEnable,HIGH);
digitalWrite(i2cEnable1,HIGH);
digitalWrite(i2cEnable2,LOW);
leakSensitivity=900;
SPI.begin();
Serial.println(F("SPI Init"));
Serial.println(F("LCD Init"));
setSyncProvider(RTC.get);   // the function to get the time from the RTC
setSyncInterval(SECS_PER_HOUR*6);  // Changed to sync every 6 hours.
//EthernetDHCP.begin(NetMac, 1); // Start Ethernet with DHCP polling enabled
//NetServer.begin();
Network.Init();
Serial.println(F("Network Init"));

Serial.println(F("Touch Init"));
#ifdef RANET
RANetSeq=0;
RANetlastmillis=millis();
#endif
#if not defined NOSD
SDFound=(PINJ & (1<<PJ3))==0;
Serial.print(F("SD Found: "));
Serial.println(SDFound);
#endif // NOSD
if (SDFound)
{
	wdt_reset();
	SD.begin(SDPin);
	wdt_reset();
	Serial.println(F("SD Init"));
	if (SD.exists("FIRMWARE.BIN")) {
		Serial.println(F("deleting firmware..."));
		SD.remove("FIRMWARE.BIN");
	}
}
BuzzerOn(0);
delay(100);
BuzzerOff();
for (int a=0;a<8;a++)
{
	CustomExpansionValue[a]=0;
	CustomExpansionLastValue[a]=0;
	
}
if (InternalMemory.IMCheck_read()!=0xCF06A31E)
{
	Serial.println(F("No Internal Memory"));
	char temptext[25];
	while(1)
	{
		BuzzerOn(0);
		delay(50);
		BuzzerOff();
		delay(100);
		BuzzerOn(0);
		delay(50);
		BuzzerOff();
		delay(700);
		wdt_reset();
		ShowNoInternalMemoryError();
		wdt_reset();
	}
}
#ifdef I2CMASTER 
Wire.onReceive(receiveEventMaster);
Wire.onRequest(NULL);
Wire.begin(I2CRA_Master);
I2CCommand=0;
#else
Wire.onReceive(NULL);
Wire.onRequest(NULL);
Wire.begin();
Wire.beginTransmission(i2cMux);
Wire.write(0x6);
Wire.endTransmission();
#endif
