#ifndef __TIMEPROVIDER_H
#define __TIMEPROVIDER_H

class TimeProvider {

public:
    virtual int GetHour() = 0;
    virtual int GetMinute() = 0;
};

#endif
