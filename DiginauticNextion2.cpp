/*
  Diginautic.cpp - Library for general tools for developing
  boat NMEA2000 equipment.

  Niklas Östblom
  2019-10-23
*/

#include "Arduino.h"
#include "DiginauticNextion2.h"

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
  double _dPartOfYear = (_iDaysSince1970 / 365.24) - (int) (_iDaysSince1970 / 365.24);

  if ( _dPartOfYear > 0.25 && _dPartOfYear < 0.83 )
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

PageObject::PageObject(Stream *streamObject)
{
  _streamRef = streamObject;
}

PageObject::PageObject(Stream *streamObject, String strName, String strValue, pPageObjectType tObjetctType)
{
  _streamRef = streamObject;
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
    _streamRef->print(strName + "=" + strValue.trim());
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
/*
    Class AnchorageAlarm:
      Class to store and handle AnchorageAlarm
*/
AnchorageAlarm::AnchorageAlarm(unsigned int iMinRadius, unsigned int iMaxRadius, unsigned int iRadiusStep)
{
  _iMinRadius = iMinRadius;
  _iMaxRadius = iMaxRadius;
  _iRadiusStep = iRadiusStep;
  _iRadius = _iMinRadius;
}

AnchorageAlarm::AnchorageAlarm(void)
{

}

//Set
void AnchorageAlarm::setAnchoragePosition(double dLatitude, double dLongitude)
{
  _dAnchorageLatitude = dLatitude;
  _dAnchorageLongitude = dLongitude;
}

void AnchorageAlarm::setActualPosition(double dLatitude, double dLongitude)
{
  _dActualLatitude = dLatitude;
  _dActualLongitude = dLongitude;
}

void AnchorageAlarm::setAnchoragePosition(double dLatitude, double dLongitude, double dDistance, double dBearing)
{
  double dLat = dLatitude;
  double dLong = dLongitude;
  double R = 6371000.0;

  _dAnchorageLatitude = asin((sin(dLat) * cos(dDistance/R)) + (cos(dLat) * sin(dDistance/R) * cos(dBearing)));
  _dAnchorageLongitude = dLong + atan2(sin(dBearing) * sin(dDistance/R) * cos(dLat), cos(dDistance/R) - (sin(dLat) * sin(_dAnchorageLatitude)));
}

void AnchorageAlarm::setRadius(unsigned int iRadius)
{
  _iRadius = iRadius;
}

void AnchorageAlarm::setActive(bool bIsActive)
{
  _bIsActive = bIsActive;
}

void AnchorageAlarm::setTriggered(bool bIsTriggered)
{
  _bIsTriggered = bIsTriggered;
}


//Get
double AnchorageAlarm::getAnchorageLatitude(void)
{
  return _dAnchorageLatitude;
}

double AnchorageAlarm::getAnchorageLongitude(void)
{
  return _dAnchorageLongitude;
}

double AnchorageAlarm::getActualLatitude(void)
{
  return _dActualLatitude;
}

double AnchorageAlarm::getActualLongitude(void)
{
  return _dActualLongitude;
}

double AnchorageAlarm::getBearing_DEG(void)
{
  double _Vinkel = RadToDeg(getBearing());
  return fmod((450.0 - _Vinkel), 360.0);
}

unsigned int AnchorageAlarm::getRadius(void)
{
  return _iRadius;
}

unsigned int AnchorageAlarm::getMinRadius(void)
{
    return _iMinRadius;
}

unsigned int AnchorageAlarm::getMaxRadius(void)
{
    return _iMaxRadius;
}

unsigned int AnchorageAlarm::getRadiusStep(void)
{
    return _iRadiusStep;
}

bool AnchorageAlarm::isActive(void)
{
  return _bIsActive;
}

bool AnchorageAlarm::isTriggered(void)
{
  return _bIsTriggered;
}

double AnchorageAlarm::getBearing(void)
{
  //Formula:	θ = atan2( sin Δλ ⋅ cos φ2 , cos φ1 ⋅ sin φ2 − sin φ1 ⋅ cos φ2 ⋅ cos Δλ )
  //where	φ1,λ1 is the start point, φ2,λ2 the end point (Δλ is the difference in longitude)
  //OBS!!! φ and λ is in radians!!!
  double y = sin(_dAnchorageLongitude - _dActualLongitude) * cos(_dAnchorageLatitude);
  double x = (cos(_dActualLatitude) * sin(_dAnchorageLatitude)) - (sin(_dActualLatitude) * cos(_dAnchorageLatitude) * cos(_dAnchorageLongitude - _dActualLongitude));
  return atan2(x, y);
}

double AnchorageAlarm::getDistance(void)
{
  long earthRadiusm = 6371000;
  double dLat = DegToRad(RadToDeg(_dAnchorageLatitude) - RadToDeg(_dActualLatitude));
  double dLon = DegToRad(RadToDeg(_dAnchorageLongitude) - RadToDeg(_dActualLongitude)); //_dAnchorageLongitude - _dActualLongitude;

  double a = sq(sin(dLat / 2)) +
             (sq(sin(dLon / 2)) * cos(_dActualLatitude) * cos(_dAnchorageLatitude));
  double c = 2 * atan2(sqrt(a), sqrt(1 - a));

  return earthRadiusm * c;
}

/*
double AnchorageAlarm::getDistance(void)
{
  long earthRadiusm = 6371000;
  double dLat = _dAnchorageLatitude - _dActualLatitude;
  double dLon = _dAnchorageLongitude - _dActualLongitude;

  double a = sq(sin(dLat / 2)) +
             (sq(sin(dLon / 2)) * cos(_dActualLatitude) * cos(_dAnchorageLatitude));
  double c = 2.0 * earthRadiusm * asin(sqrt(a));
  
  //2 * atan2(sqrt(a), sqrt(1 - a));

  return c;
}
*/

//*******************************************************************
