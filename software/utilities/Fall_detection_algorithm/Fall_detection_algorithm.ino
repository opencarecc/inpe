#include <LSensorHub.h>
#include <LDisplay.h>

// initialize variables for accelerometer data
long accData1 = 0;  // will be used for X axis values
long accData2 = 0;  // will be used for Y axis values
long accData3 = 0;  // will be used for Z axis values
long prevData1 = 0;  // will be used for X axis values
long prevData2 = 0;  // will be used for Y axis values
long prevData3 = 0;  // will be used for Z axis values
long runningDelta = 0;

const long THRESH_FF = 250; // constant to check if free-fall is happening
const long FALLING_MINIMUM_TIME = 5;
long freeFallCounter = 0;
bool falling;

const long THRESH_ACT = 2000;  // constant to check if impact is happening
bool impact;

const long THRESH_INACT = 187;  // constant to check if impact is happening
bool inactivity;

int status = 0;
int fallingTime = 0;
int impactTime = 0;
int inactivityTime = 0;
bool fallDetected;

void setup() {
  Lcd.init();
  Lcd.font_init();
  Lcd.back_light_level(50);
  Lcd.screen_set(0x000000);
  Lcd.draw_font(0, 0, "1", 0x000000, 0xFFFFFF);
}

void loop() {

  accelLoop();

  switch (status) {
    case 0:
      if (falling) {
        Lcd.draw_font(90, 0, "S1", 0x000000, 0xffffff);
        status = 1;
        fallingTime = 0;
      }
      break;
    case 1:
      if (!impact) {
        fallingTime++;
      } else {
        if (fallingTime > 200) {
          status = 0;
        } else {
          Lcd.draw_font(90, 30, "S2", 0x000000, 0xffffff);
          status = 2;
        }
      }
      break;
    case 2:
      if (!inactivity) {
        impactTime++;
      } else {
        if (impactTime < 3500) {
          status = 3;
        }
      }
      break;
    case 3:
      inactivityTime++;
      if (inactivityTime > 2000) {
        fallDetected = TRUE;
      }
      break;
  }

  if (fallDetected) {
    Lcd.draw_font(10, 10, "Fall Detected!", 0x000000, 0xffffff);
  }

  Lcd.draw_updata();
}

/* ==== FALL DETECTION ==== */
int y = 0, y2 = 0;
void accelLoop() {
  // get data from sensors
  LSensorHub.GetAccData(&accData1, &accData2, &accData3);
  // check if Free-fall (needs better algorithm)
  if (accData1 >= -THRESH_FF && accData1 <= THRESH_FF && accData2 >= -THRESH_FF && accData2 <= THRESH_FF && accData3 >= -THRESH_FF && accData3 <= THRESH_FF) {
    freeFallCounter++;
    Lcd.draw_number(10, y, freeFallCounter, 0x000000, 0xffffff);
    if (freeFallCounter >= FALLING_MINIMUM_TIME) {
      falling = TRUE;
    }
  } else {
    if (freeFallCounter > 0)
      y += 10;
    freeFallCounter = 0;
    falling = FALSE;
  }

  if (accData1 <= -THRESH_ACT || accData1 >= THRESH_ACT || accData2 <= -THRESH_ACT || accData2 >= THRESH_ACT || accData3 <= -THRESH_ACT || accData3 >= THRESH_ACT) {
    Lcd.draw_font(30, y2, "IM", 0x000000, 0xffffff);
    y2 += 10;
    impact = TRUE;
  } else {
    if (impact) y += 10;
    impact = FALSE;
  }

  long delta = abs(accData1 - prevData1) + abs(accData2 - prevData2) + abs(accData3 - prevData3);
  runningDelta += -(runningDelta / 8) + delta / 8;
  Lcd.draw_font(70, 10, "     ", 0x000000, 0xffffff);
  Lcd.draw_number(70, 10, runningDelta, 0x000000, 0xffffff);
  if (runningDelta < THRESH_INACT) {
    inactivity = TRUE;
    Lcd.draw_font(70, 30, "   ", 0x000000, 0xffffff);
  } else {
    inactivity = FALSE;
    Lcd.draw_font(70, 30, "AC", 0x000000, 0xffffff);
  }

  prevData1 = accData1;
  prevData2 = accData2;
  prevData3 = accData3;
}
