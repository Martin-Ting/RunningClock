#ifndef MODULE_EEPROMADDRESSES_H
#define MODULE_EEPROMADDRESSES_H


// Alarm Memory bounds definitions based on EEPROMAddresses
#define NUMALARMELEMENTS 4
#define NUMALARMS EEPROM_ALARM_TOPADDRESS/NUMALARMELEMENTS

enum EEPROMAddresses { _addrAlarm1Hour, _addrAlarm1Minute, _addr1IsAM, _addr1IsActive,
	_addrAlarm2Hour, _addrAlarm2Minute, _addr2IsAM, _addr2IsActive,
	_addrAlarm3Hour, _addrAlarm3Minute, _addr3IsAM, _addr3IsActive,
	_addrAlarm4Hour, _addrAlarm4Minute, _addr4IsAM, _addr4IsActive,
	_addrAlarm5Hour, _addrAlarm5Minute, _addr5IsAM, _addr5IsActive,
	EEPROM_ALARM_TOPADDRESS,
EEPROM_TOPADDRESS };

#endif