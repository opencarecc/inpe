
#include "LDisplay.h"
#include "LGTouch.h"
#include "LSleep.h"
#include "Sleep.h"


char EVENT = 0;
int X = 0;
int Y = 0;

unsigned long previousMillis = 0;

// set here (in milliseconds) the time interval to wait before going to sleep
const long sleepInterval = 1000*10; 

// needed to check what sleep status the system is in and print on the log file
boolean enterSleepIn = FALSE;
boolean enterSleepOut = FALSE;

void sleepClass::sleep(void)
{
    // count time
    unsigned long currentMillis = millis();

    // if current time count is equal or bigger than the interval of time to wait before going to sleep
    if(currentMillis - previousMillis >= sleepInterval)
    {
        // set backlight low
        Lcd.back_light_level(0);
        remoteCall(sleepIn, NULL);
        enterSleepIn = TRUE;
        enterSleepOut = FALSE;

        // save current time count to check for the time interval again later
        previousMillis = currentMillis;
    }
    
    // if there is a touch event
    if(Tp.Event_available())
    {
        Tp.Get_event_xy(&EVENT, &X, &Y);
        //if(EVENT>0 && EVENT!=6 && X>=0 && Y>=0)
        if(EVENT == 2)
        {
            // set backlight high
            Lcd.back_light_level(50);
            remoteCall(sleepOut, NULL);
            enterSleepOut = TRUE;
            enterSleepIn = FALSE;

            // save current count to wait for another whole time interval before going to sleep
            previousMillis = currentMillis;
        }
    }
}

sleepClass lsleep;
