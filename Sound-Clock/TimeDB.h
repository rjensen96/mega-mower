#ifndef __TIMEDB_H
#define __TIMEDB_H

#include "TimeProvider.h"

class TimeDB : public TimeProvider {

public:
    TimeDB();
    bool SetTimezone(String timezone);
    bool UpdateTime();
    virtual int GetHour() {return hour;}
    virtual int GetMinute() {return minute;}

protected:
    static const char *APIKey;
    String timezone;
    unsigned int hour;
    unsigned int minute;
    long last_query_timestamp;
    long last_query_millis;
    bool SendRequest();
};

#endif
