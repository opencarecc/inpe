/*
  UI to start and hang calls for RePhone Kit
  
  Hello, this sketch will show a UI on the Xadow 1.54" Touchscreen v1.0 module
  Interacting with the button you can make a call, or end a call to a number.
  
  HARDWARE:
  - Xadow GSM+BLE
  - Xadow 1.54" Touchscreen v1.0
  
  TO-DO:
  - add counter while talking (in the form of hh:mm:ss)
  - add UI for receiving a call
  - add a keyboard to type number to call
  
  Alessandro Contini for http://opencare.cc/
  August 2016
*/

#include <LDisplay.h>
#include <LGTouch.h>
#include <LCheckSIM.h>
#include <LGSM.h>

// initialize variables for GSM
char *charbuffer = "00393402686996";
char num[20] = {0};

int xInit = 0;
int yInit = 0;
int xFinal = 240;
int yFinal = 240;

void draw_rect(int x, int y, int width, int height, uint32_t color){
  Lcd.draw_fill_rectangle(x,y,(x+width),(y+height),color);
}

char EVENT = 0;
int X = 0;
int Y = 0;

//int elapsedTime = 0;

void setup() {
  Serial.begin(115200);
  
  Tp.Init();
  Lcd.init();
  Lcd.font_init();
  Lcd.back_light_level(50);
  Lcd.screen_set(0xffffff);
}

void loop() {
  
  
  
  Lcd.draw_font((xFinal/2)-62, (yInit)+20, "Number to call:", 0xffffff, 0);
  Lcd.draw_font((xFinal/2)-72, (yInit)+40, charbuffer, 0xffffff, 0);  
  
  // check if SIM is ready
  if(LCheckSIM.isCheck() == 1){ 
 Serial.println("SIM is ready");   
    Lcd.draw_font((xFinal/2)-65, (yInit)+80, " SIM is ready          ", 0xffffff, 0);
    
    // START CALL button
    draw_rect((xFinal/2)-90,(yInit)+130,80,60,0xace600);
    Lcd.draw_font((xFinal/2)-75, (yInit)+150, "CALL", 0xace600, 0xffffff);

    
    // STOP CALL button
    draw_rect((xFinal/2)+10,(yInit)+130,80,60,0xff3300);
    Lcd.draw_font((xFinal/2)+30, (yInit)+150, "END", 0xff3300, 0xffffff);

    
    if(Tp.Event_available())
      {
          Tp.Get_event_xy(&EVENT, &X, &Y);
          // fix touch library (or hardware?) bug
          int invX = map(X,239,0,0,239);
          
          // if START CALL is clicked
          // EVENT>0 touche events values are between 1 and 6
          // EVENT!=6 a bug forces the system to get value 6 after first touch
          if (EVENT>0 && EVENT!=6 && invX>=30 && invX<=110 && Y>=130 && Y<=190){
            Serial.println("event CALL");
              draw_rect((xFinal/2)-90,(yInit)+130,80,60,0x000000);
              Lcd.draw_font((xFinal/2)-75, (yInit)+150, "CALL", 0x000000, 0xffffff);

              
              // start call
              LVoiceCall.voiceCall(charbuffer);              
              if (LVoiceCall.getVoiceCallStatus()==CALLING){
                Serial.println("calling");
                Lcd.draw_font((xFinal/2)-65, (yInit)+80, "   CALLING...  ", 0xffffff, 0);
              }
              

              while(1){
                while(LVoiceCall.getVoiceCallStatus()==TALKING){
                  Serial.println("talking");
                  Lcd.draw_font((xFinal/2)-65, (yInit)+80, "   TALKING...  ", 0xffffff, 0);
                  if(LVoiceCall.getVoiceCallStatus() == IDLE_CALL){
                    Lcd.draw_font((xFinal/2)-65, (yInit)+80, "   CALL ENDED  ", 0xffffff, 0);
                    Serial.println("call ended");
                    delay(1000);
                    break;
                  }
                }
                if(Tp.Event_available())
                  {
                      Tp.Get_event_xy(&EVENT, &X, &Y);
                      // fix touch library (or hardware?) bug
                      int invX = map(X,239,0,0,239);
                      
                      // if STOP CALL is clicked
                      if (EVENT>0 && EVENT!=6 && invX>=130 && invX<=210 && Y>=130 && Y<=190){
                        Serial.println("event END");
                          draw_rect((xFinal/2)+10,(yInit)+130,80,60,0x000000);
                          Lcd.draw_font((xFinal/2)+30, (yInit)+150, "END", 0x000000, 0xffffff);
                          
                          // end call
                          LVoiceCall.hangCall(); 
                          Lcd.draw_font((xFinal/2)-65, (yInit)+80, "ENDING CALL", 0xffffff, 0);
                          Serial.println("ending call");
                          delay(1000);
                          break;            
                      }
                }              
                if(LVoiceCall.getVoiceCallStatus() == IDLE_CALL)break;
                delay(50);
              }  
          }        
      }
  } 
  
  // SIM not ready
  if(LCheckSIM.isCheck() == 0) {
    Lcd.draw_font((xFinal/2)-70, (yInit)+80, "SIM is not ready", 0xffffff, 0);
    // START CALL button
    draw_rect((xFinal/2)-90,(yInit)+130,80,60,0xf2f3f2);
    Lcd.draw_font((xFinal/2)-75, (yInit)+150, "CALL", 0xf2f3f2, 0xffffff);
    
    // STOP CALL button
    draw_rect((xFinal/2)+10,(yInit)+130,80,60,0xf2f3f2);
    Lcd.draw_font((xFinal/2)+30, (yInit)+150, "END", 0xf2f3f2, 0xffffff);
  }
  
  
  
  Lcd.draw_updata();
}
