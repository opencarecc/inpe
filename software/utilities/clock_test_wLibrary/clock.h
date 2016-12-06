#ifndef clock_h
#define clock_h

extern unsigned int year;
extern unsigned int month;
extern unsigned int day;
extern unsigned int hour;
extern unsigned int minute;
extern unsigned int sec;

class cclass{
  public:
    cclass();
    void clock();
};

extern cclass clock;

#endif
