#include "Arduino.h"
#include "LSensorHub.h"

extern "C" {
#include "vmtimer.h"
}

volatile long accData[128][3];
volatile int lastRead = 0;

extern "C" void sensorLoop(VM_TIMER_ID_PRECISE tid, void* user_data)
{
  // vm_timer_delete_precise(tid);
  long x, y, z;
  //unsigned long start = micros();
  LSensorHub.GetAccData(&x, &y, &z);
  //unsigned long end = micros();
  //Serial.println(end-start);
  accData[lastRead][0] = x;
  accData[lastRead][1] = y;
  accData[lastRead][2] = z;
  lastRead++;
  if (lastRead == 128) lastRead = 0;
}

extern "C" unsigned char startSensorLoop(void *)
{
  VM_TIMER_ID_PRECISE t = vm_timer_create_precise(30, sensorLoop, NULL);
  if (t < 0)
  {
    Serial.print("ERROR: ");
    Serial.println(t);
  }
}

