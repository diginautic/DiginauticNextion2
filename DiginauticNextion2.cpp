/*
  Diginautic.cpp - Library for general tools for developing
  boat NMEA2000 equipment.

  Niklas Östblom
  2019-10-23
*/

#include "Arduino.h"
#include "DiginauticNextion.h"

//*******************************************************************

String ACTIVE_COLOR = "64512";
String STANDBY_COLOR = "65535";

//*******************************************************************
/*
    Class Time:
      Class to store date and time
*/
Time::Time(double dSecondsSinceMidnight, uint16_t iDaysSince1970)
{
  _dSecondsSinceMidnight = dSecondsSinceMidnight;
  _iDaysSince1970 = iDaysSince1970;
  _iCorrection = calcCorrection();
}

Time::Time(void)
{
  _dSecondsSinceMidnight = 0;
  _iDaysSince1970 = 0;
  _iCorrection = 1;
}

int Time::calcCorrection(void)
{
  float _fPartOfYear = (_iDaysSince1970 / 365.24) - (int) (_iDaysSince1970 / 365.24);

  if ( _fPartOfYear > 0.25 && _fPartOfYear < 0.83 )
  {
    return 2;
  }
  else
  {
    return 1;
  }
}

double Time::getSecondsSinceMidnight(void)
{
  return _dSecondsSinceMidnight;
}

int Time::getMinutesSinceMidnight(void)
{
  return _dSecondsSinceMidnight / 60;
}


uint16_t Time::getDaysSince1970(void)
{
  return _iDaysSince1970;
}

int Time::getHour(bool bCorrected)
{
  return getHour(_dSecondsSinceMidnight, bCorrected);
}

int Time::getHour(double dSecondsSinceMidnight, bool bCorrected)
{
  // Timme + tidszon - 24*0/1 1=slår över midnatt 0=klarar midnatt
  int _iHour = dSecondsSinceMidnight / 3600;
  return _iHour + (_iCorrection * (int) bCorrected) - ((int)((_iHour + (_iCorrection * (int) bCorrected)) > 23) * 24);
}

int Time::getMinute (void)
{
  return getMinute(_dSecondsSinceMidnight);
}

int Time::getMinute (double dSecondsSinceMidnight)
{
  return ((dSecondsSinceMidnight / 3600.0) - (int)(dSecondsSinceMidnight / 3600)) * 60;
}

String Time::getTime(bool bCorrected)
{
  return getTime(_dSecondsSinceMidnight, bCorrected);
}

String Time::getTime(double dSecondsSinceMidnight, bool bCorrected)
{
  int _iHour = getHour(dSecondsSinceMidnight, bCorrected);
  int _iMinute = getMinute();
  String _strHPrefix = "", _strMPrefix = "";

  if ( _iHour < 10 )
  {
    _strHPrefix = "0";
  }
  
  if (_iMinute < 10)
  {
    _strMPrefix = "0";
  }

  return _strHPrefix + String(_iHour) + ":" + _strMPrefix + String(_iMinute);
}

int Time::getCorrection(void)
{
  return _iCorrection;
}

//Set
void Time::setSecondsSinceMidnight(double dSecondsSinceMidnight)
{
  _dSecondsSinceMidnight = dSecondsSinceMidnight;
  _iCorrection = calcCorrection();
}

void Time::setDaysSince1970(uint16_t iDaysSince1970)
{
  _iDaysSince1970 = iDaysSince1970;
  _iCorrection = calcCorrection();
}

void Time::setTime(double dSecondsSinceMidnight, uint16_t iDaysSince1970)
{
  _dSecondsSinceMidnight = dSecondsSinceMidnight;
  _iDaysSince1970 = iDaysSince1970;
  _iCorrection = calcCorrection();
}


//*******************************************************************
/*
    Class PageObject:
      Base Class to handle page objects
*/

/*

    PageObject(Stream *streamObject);
    PageObject(Stream *streamObject, String strName, String strValue, pPageObjectType tObjetctType);

    void setValue(String strValue);
    void setName(String strName);
    void setSerial(Stream *streamObject);

    void serialPrint(String strName, String strVal);
    void serialPrint(String strValue);
    void serialPrint(void);


*/

PageObject::PageObject(Stream *streamObject)
{
  _streamRef=streamObject;
}

PageObject::PageObject(Stream *streamObject, String strName, String strValue, pPageObjectType tObjetctType)
{
  _streamRef=streamObject;
  _strName = strName;
  _strValue = strValue;
  _tObjetctType = tObjetctType;
}

void PageObject::serialPrint(void)
{
  if (_tObjetctType == PAGE_text)
  {
    _streamRef->print(_strName + "=\"" + _strValue + "\"");
  }
  else
  {
    _streamRef->print(_strName + "=" + _strValue);
  }
  _streamRef->write(0xff);
  _streamRef->write(0xff);
  _streamRef->write(0xff);
  _streamRef->flush();
}

void PageObject::serialPrint(String strName, String strValue, pPageObjectType tObjetctType)
{
  if (tObjetctType == PAGE_text)
  {
    _streamRef->print(strName + "=\"" + strValue + "\"");
  }
  else
  {
    _streamRef->print(strName + "=" + strValue);
  }
  _streamRef->write(0xff);
  _streamRef->write(0xff);
  _streamRef->write(0xff);
  _streamRef->flush();
}

void PageObject::setSerial(Stream *streamObject)
{
  _streamRef=streamObject;
}

void PageObject::setValue(String strValue)
{
  _strValue = strValue;
}

void PageObject::setName(String strName)
{
  _strName = strName;
}

void PageObject::setObjectType(pPageObjectType tObjetctType)
{
  _tObjetctType = tObjetctType;
}
//*******************************************************************