/*
  DigitalSailing.h - Library for general tools for developing
  boat NMEA2000 equipment.

  Niklas Östblom
  2019-10-23
*/
#ifndef DiginauticNextion_h
#define DiginauticNextion_h

#include "Diginautic.h"
#include "DiginauticN2k.h"
#include "Nextion.h"
#include "N2kMessages.h"

enum pPageObjectType
{
  PAGE_text=0,
  PAGE_variable=1,
  PAGE_gauge=2,
  PAGE_button=3
};

#define NMEA_MAX_AGE_SECCONDS 60

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


#endif