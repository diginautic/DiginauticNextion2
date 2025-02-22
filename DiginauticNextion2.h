/*
  DigitalSailing.h - Library for general tools for developing
  boat NMEA2000 equipment.

  Niklas Östblom
  2019-10-23
*/
#ifndef DiginauticNextion_h
#define DiginauticNextion_h

#include "Diginautic.h"
#include "N2kMessages.h"
//#include "DiginauticN2k.h"
//#include "Nextion.h"

enum pPageObjectType
{
  PAGE_text=0,
  PAGE_variable=1,
  PAGE_gauge=2,
  PAGE_button=3
};

#define NMEA_MAX_AGE_SECCONDS 60

inline String getButtonName(int iBtn)
{
  String _srtName;
  switch (iBtn)
  {
    case 1 :
      _srtName =  "vaBTGasol";
    break;

    case 2 :
      _srtName =  "vaBTLightning";
    break;
    
    case 3 : 
      _srtName =  "vaBTFMRadio";
    break;
    
    case 4 : 
      _srtName =  "vaBTSaltWater";
    break;
    
    case 5 : 
      _srtName =  "vaBTSweetWater";
    break;
    
    case 6 : 
      _srtName =  "vaBTUSBOutside";
    break;
    
    case 7 : 
      _srtName =  "vaBTUSBInside";
    break;
    
    case 8 : 
      _srtName =  "vaBTAnchorLant";
    break;
    
    case 9 : 
      _srtName =  "vaBTEngineLan";
    break;
    
    case 10 : 
      _srtName =  "vaBTLanterns";
    break;
    
    case 21 : 
      _srtName =  "vaBTSpotlight";
    break;

  }
  return _srtName;
}

//===================================================================
/*
    Class Time:
      Class to store time and date
*/
class Time
{
  public :
    Time(double dSecondsSinceMidnight, uint16_t iDaysSince1970);
    Time(void);
    //Get
    double getSecondsSinceMidnight(void);
    int getMinutesSinceMidnight(void);
    uint16_t getDaysSince1970(void);
    int getHour(bool bCorrected);
    int getMinute(void);
    int getHour(double dSecondsSinceMidnight, bool bCorrected);
    int getMinute(double dSecondsSinceMidnight);
    String getTime(bool bCorrected);
    String getTime(double dSecondsSinceMidnight, bool bCorrected);
    int getCorrection(void);
    //Set
    void setSecondsSinceMidnight(double dSecondsSinceMidnight);
    void setDaysSince1970(uint16_t iDaysSince1970);
    void setTime(double dSecondsSinceMidnight, uint16_t iDaysSince1970);

  private :
    double _dSecondsSinceMidnight;
    int _iMinutesSinceMidnight;
    uint16_t _iDaysSince1970;
    int _iCorrection;
    int calcCorrection(void);
};

//===================================================================
/*
    Class PageObject:
      Class to store and handle page objects
*/

class PageObject
{
  public :
    PageObject(Stream *streamObject);
    PageObject(Stream *streamObject, String strName, String strValue, pPageObjectType tObjetctType);

    void setValue(String strValue);
    void setName(String strName);
    void setSerial(Stream *streamObject);
    void setObjectType(pPageObjectType tObjetctType);

    void serialPrint(String strName, String strVal, pPageObjectType tObjetctType);
    void serialPrint(void);

  private :
    pPageObjectType _tObjetctType;
    String _strName;
    String _strValue;
    Stream *_streamRef;
};

//===================================================================
/*
    Class AnchorageAlarm:
      Class to store and handle AnchorageAlarms
*/
class AnchorageAlarm
{
  public :
    AnchorageAlarm(unsigned int iMinRadius, unsigned int iMaxRadius, unsigned int RadiusStep);
    AnchorageAlarm(void);
    //Set
    void setAnchoragePosition(double dLatitude, double dLongitude); //Radians
    void setAnchoragePosition(double dLatitude, double dLongitude, double dDistance, double dBearing); //All in Radians
    void setActualPosition(double dLatitude, double dLongitude); //Radians
    void setRadius(unsigned int iRadius); //Meters
    void setActive(bool bIsActive);
    void setTriggered(bool bAlarmIsActive);
    //Get
    double getAnchorageLatitude(void);
    double getAnchorageLongitude(void);
    double getActualLatitude(void);
    double getActualLongitude(void);
    double getDistance(void);
    double getBearing(void);
    double getBearing_DEG(void);
    unsigned int getRadius(void);
    unsigned int getMinRadius(void);
    unsigned int getMaxRadius(void);
    unsigned int getRadiusStep(void);
    bool isActive(void);
    bool isTriggered(void);

  private :
    double _dAnchorageLatitude;
    double _dAnchorageLongitude;
    double _dActualLatitude;
    double _dActualLongitude;
    unsigned int _iRadius;
    unsigned int _iMinRadius;
    unsigned int _iMaxRadius;
    unsigned int _iRadiusStep;
    bool _bIsActive;
    bool _bIsTriggered;
};

//===================================================================

#endif