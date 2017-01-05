#ifndef sleep_h
#define sleep_h

#include "LTask.h"
#include "Arduino.h"


class sleepClass : public _LTaskClass{
  public:
	sleepClass() {};
    
  public:
    void sleep(void);
    
};

extern sleepClass lsleep;
extern boolean enterSleepIn;
extern boolean enterSleepOut;

#endif
