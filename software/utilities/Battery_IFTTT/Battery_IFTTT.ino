#include "L_https.h"
#include "string.h"
#include <LBattery.h>    // Battery library
#include <LDisplay.h>    // LCD library

String action = "POST"; // Edit to build your command - "GET" or "POST"
char server[27] = "https://maker.ifttt.com";
char path[100] = "/trigger/rephone/with/key/"; // Edit Path to include you Variable ID
char token[31] = "dYARPJA3YrFVXftcGJtOhW"; // Edit to insert you API Token
char TEST_URL[256] = {0};
char part1[20] = "{\"value1\": ";
//char bracket[1] = "}";
char HTTP_BODY[100];
//int sensorReading = 25; //Just a random value
int value_index = 0;
String response_body,value_string, value; // We'll use these strings to parse the response from Ubidots
char *buffer;

int currBatteryLevel;
int lastBatteryLevel = 0;

int numRuns = 0;   // execution count, so this sketch doesn't run forever
int maxRuns = 1;  // the max number of times the Twitter Update Choreo should run

void setup()
{
  Serial.begin(115200);
  
  // initialize LCD display
  Lcd.init();
  Lcd.font_init();
  Lcd.back_light_level(50);
  Lcd.screen_set(0xffff00);
}

void loop()
{
  while(LBattery.isCharging()){
    Lcd.draw_font(10,10,"Charging     ", 0xffff00, 0);
    Lcd.draw_font(10,35,"Battery level is: ", 0xffff00, 0);
    Lcd.draw_number(150, 35, LBattery.level(), 0xffff00, 0); 
    Serial.println("Battery is charging");
    delay(100);
  }
  while(!LBattery.isCharging()){
    Lcd.draw_font(10,10,"NOT Charging", 0xffff00, 0);
    Lcd.draw_font(10,35,"Battery level is: ", 0xffff00, 0);
    Lcd.draw_number(150, 35, LBattery.level(), 0xffff00, 0);
    
    currBatteryLevel = LBattery.level();
    
    sprintf(TEST_URL, "%s%s%s",server,path,token);
    sprintf(HTTP_BODY, "%s%i}",part1,currBatteryLevel);
    
    if (LBattery.level()==100){
       if (numRuns < maxRuns){
         https.connect(TEST_URL);
         https.get_handle(print_url);
         numRuns++;
       }
    }
    
    if (currBatteryLevel<lastBatteryLevel){
      numRuns=0;
      if (numRuns < maxRuns){
         https.connect(TEST_URL);
         https.get_handle(print_url);
         numRuns++;
      }
    }
    
//    if (LBattery.level()==66){
//      numRuns=0;
//      if (numRuns < maxRuns){
//         https.connect(TEST_URL);
//         https.get_handle(print_url);
//         Serial.println("Battery 66 - Connecting to:");
//         numRuns++;
//      }
//    }

    lastBatteryLevel = currBatteryLevel;    

  }
}

void print_url(char *content, unsigned long len)
{

//Serial.println("Connecting to...");

Serial.println(content);

response_body = (content);

value_index = response_body.indexOf("\"value1\": ");

// Chop the response from that index, until the end of the response string
value_string = response_body.substring(value_index);

// Get the value that is between the nine (9) characters of "\"-v-a-l-u-e-\"-: " and the next comma
value = value_string.substring(9, value_string.indexOf(","));

Serial.println(value);

Serial.flush();

}
