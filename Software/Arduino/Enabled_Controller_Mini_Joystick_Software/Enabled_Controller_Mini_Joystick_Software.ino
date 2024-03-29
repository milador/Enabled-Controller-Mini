
/***************************************************************************
 * File Name: Enabled_Controller_Mini_Joystick_Software.ino 
 * Title: Enabled Controller Mini Joystick Software
 * Developed by: Milad Hajihassan
 * Version Number: 1.2 (01/6/2023)
 * Github Link: https://github.com/milador/Enabled_Controller_Mini
 ***************************************************************************/

#include "XACGamepad.h"
#include <math.h>
#include <Adafruit_NeoPixel.h>


//Can be changed based on the needs of the users 
#define JOYSTICK_DEADZONE 20                                          //Joystick deadzone
#define SWITCH_REACTION_TIME 50                                       //Minimum time for each switch action
#define SWITCH_MODE 1                                                 //Only one mode
#define JOYSTICK_ENABLED true                                          //Joystick enabled or diabled

#define LED_BRIGHTNESS 100                                             //The mode led color brightness which is always on ( Use a low value to decrease power usage )
#define LED_ACTION_BRIGHTNESS 100                                      //The action led color brightness which can be a higher value than LED_BRIGHTNESS


//Define Switch pins
#define LED_PIN 11


#define SWITCH_A_PIN 6
#define SWITCH_B_PIN 7
#define SWITCH_C_PIN 3
#define SWITCH_D_PIN 2

//Define Joystick pins
#define JOYSTICK_X1_PIN A0
#define JOYSTICK_Y1_PIN A1  


// Variable Declaration

//Declare switch state variables for each switch
int switchAState;
int switchBState;
int switchCState;
int switchDState;

//Previous status of switches
int switchAPrevState = HIGH;          
int switchBPrevState = HIGH;
int switchCPrevState = HIGH;
int switchDPrevState = HIGH;

//Declare joystick variables 
int joystickX;
int joystickY;


//Declare Switch variables for settings 
int switchConfigured;
int switchReactionTime;
int switchReactionLevel;
int switchMode;


//RGB LED Color code structure 

struct rgbColorCode {
    int r;    // red value 0 to 255
    int g;   // green value
    int b;   // blue value
 };

//Color structure 
typedef struct { 
  uint8_t colorNumber;
  String colorName;
  rgbColorCode colorCode;
} colorStruct;

 //Mode structure 
typedef struct { 
  uint8_t modeNumber;
  String modeName;
  uint8_t modeColorNumber;
} modeStruct;


 //Switch structure 
typedef struct { 
  uint8_t switchNumber;
  String switchButtonName;
  uint8_t switchModeButtonNumber;
  uint8_t switchColorNumber;
} switchStruct;


 //Settings Action structure 
typedef struct { 
  uint8_t settingsActionNumber;
  String settingsActionName;
  uint8_t settingsActionColorNumber;
} settingsActionStruct;

//Color properties 
const colorStruct colorProperty[] {
    {1,"Green",{60,0,0}},
    {2,"Pink",{0,50,60}},
    {3,"Yellow",{60,50,0}},    
    {4,"Orange",{20,60,0}},
    {5,"Blue",{0,0,60}},
    {6,"Red",{0,60,0}},
    {7,"Purple",{0,50,128}},
    {8,"Teal",{128,0,128}}       
};

//Switch properties 
const switchStruct switchProperty[] {
    {1,"A",1,5},
    {2,"B",2,3},
    {3,"C",3,1},
    {4,"D",4,6}
};

//Mode properties 
const modeStruct modeProperty[] {
    {1,"Mode 1",8}
};


//Setup NeoPixel LED
Adafruit_NeoPixel ledPixels = Adafruit_NeoPixel(1, LED_PIN, NEO_GRB + NEO_KHZ800);

//Starts an instance of the XAC compatible gamepad object
XACGamepad gamepad;
 

void setup() {
  
  gamepad.begin();
  ledPixels.begin();                                                           //Start NeoPixel
  Serial.begin(115200);                                                        //Start Serial
  switchSetup();
  delay(1000);
  initLedFeedback();                                                          //Led will blink in a color to show the current mode 
  delay(5);
  displayFeatureList();
  
  //Initialize the switch pins as inputs
  pinMode(SWITCH_A_PIN, INPUT_PULLUP);    
  pinMode(SWITCH_B_PIN, INPUT_PULLUP);   
  pinMode(SWITCH_C_PIN, INPUT_PULLUP);    
  pinMode(SWITCH_D_PIN, INPUT_PULLUP);  

  //Initialize the LED pin as an output
  pinMode(LED_PIN, OUTPUT);                                                


};

void loop() {
  //Perform joystick actions based on the mode
  joystickAction(switchMode);
  ledPixels.show(); 
  delay(5);
}

//***DISPLAY FEATURE LIST FUNCTION***//

void displayFeatureList(void) {

  Serial.println(" ");
  Serial.println(" --- ");
  Serial.println("This is the Enabled Controller Mini XAC Compatible Gamepad Software");
  Serial.println(" ");
  Serial.println("VERSION: 1.2 (1 June 2023)");
  Serial.println(" ");
  Serial.println(" --- ");
  Serial.println("Features: XAC Compatible Gamepad");
  Serial.println(" --- ");
  Serial.println(" ");

}

//***RGB LED FUNCTION***//

void setLedBlink(int numBlinks, int delayBlinks, int ledColor,uint8_t ledBrightness) {
  if (numBlinks < 0) numBlinks *= -1;

      for (int i = 0; i < numBlinks; i++) {
        ledPixels.setPixelColor(0, ledPixels.Color(colorProperty[ledColor-1].colorCode.g,colorProperty[ledColor-1].colorCode.r,colorProperty[ledColor-1].colorCode.b));
        ledPixels.setBrightness(ledBrightness);
        ledPixels.show(); 
        delay(delayBlinks);
        ledPixels.setBrightness(ledBrightness);
        ledPixels.setPixelColor(0, ledPixels.Color(0,0,0));
        ledPixels.setBrightness(ledBrightness);
        ledPixels.show(); 
        delay(delayBlinks);
      }
}

//***UPDATE RGB LED COLOR FUNCTION***//

void updateLedColor(int ledColor, uint8_t ledBrightness) {
    ledPixels.setPixelColor(0, ledPixels.Color(colorProperty[ledColor-1].colorCode.g,colorProperty[ledColor-1].colorCode.r,colorProperty[ledColor-1].colorCode.b));
    ledPixels.setBrightness(ledBrightness);
    ledPixels.show();
}

//***GET RGB LED COLOR FUNCTION***//

uint32_t getLedColor(int ledModeNumber) {

  int colorNumber= modeProperty[ledModeNumber-1].modeColorNumber-1;
  
  return (ledPixels.Color(colorProperty[colorNumber].colorCode.g,colorProperty[colorNumber].colorCode.r,colorProperty[colorNumber].colorCode.b));
}

//***GET RGB LED BRIGHTNESS FUNCTION***//

uint8_t getLedBrightness() {
  return (ledPixels.getBrightness());
}

//***SET RGB LED COLOR FUNCTION***//

void setLedColor (uint32_t ledColor, uint8_t ledBrightness){
    
  ledPixels.setPixelColor(0, ledColor);
  ledPixels.setBrightness(ledBrightness);
  ledPixels.show(); 

}

//***SET RGB LED BRIGHTNESS FUNCTION***//

void setLedBrightness(uint8_t ledBrightness) {
  ledPixels.setBrightness(ledBrightness);
  ledPixels.show();
}

//***CLEAR RGB LED FUNCTION***//

void ledClear() {
  ledPixels.setPixelColor(0, ledPixels.Color(0,0,0));
  ledPixels.show(); 
}

//***SWITCH FEEDBACK FUNCTION***//

void switchFeedback(int switchNumber,int modeNumber,int delayTime, int blinkNumber =1)
{
  //Get previous led color and brightness 
  uint32_t previousColor = getLedColor(modeNumber);
  uint8_t previousBrightness = getLedBrightness();
 
  setLedBlink(blinkNumber,delayTime,switchProperty[switchNumber-1].switchColorNumber,LED_ACTION_BRIGHTNESS);
  delay(5);

  //Set previous led color and brightness 
  setLedColor(previousColor,previousBrightness);
  
}

//***MODE FEEDBACK FUNCTION***//

void modeFeedback(int modeNumber,int delayTime, int blinkNumber =1)
{

   //Get new led color and brightness 
  uint32_t newColor = getLedColor(modeNumber);
  uint8_t newBrightness = getLedBrightness();
  
  setLedBlink(blinkNumber,delayTime,modeProperty[modeNumber-1].modeColorNumber,LED_ACTION_BRIGHTNESS);
  delay(5);

  //Set new led color and brightness 
  setLedColor(newColor,newBrightness);
  
}

//***PERFORM JOYSTICK ACTIONS FUNCTION***//

void joystickAction(int mode) {

    //Update status of switch inputs
  switchAState = digitalRead(SWITCH_A_PIN);
  switchBState = digitalRead(SWITCH_B_PIN);
  switchCState = digitalRead(SWITCH_C_PIN);
  switchDState = digitalRead(SWITCH_D_PIN);

  

  //Update joystick values based on available joystick number 
  if(JOYSTICK_ENABLED) {
    joystickX = analogRead(JOYSTICK_X1_PIN);
    joystickY = analogRead(JOYSTICK_Y1_PIN);

    //Perform joystick move Actions
    int xx = map(joystickX, 0, 1023, -127, 127);
    int yy = map(joystickY, 0, 1023, -127, 127);
    

    if (xx<=JOYSTICK_DEADZONE && xx>=-JOYSTICK_DEADZONE){
      gamepad.xAxis(0);  
      gamepad.send();
    }
    else {
      gamepad.xAxis(xx);  
      gamepad.send();  
    }

    if (yy<=JOYSTICK_DEADZONE && yy>=-JOYSTICK_DEADZONE){
      gamepad.yAxis(0);  
      gamepad.send();    
      }
    else {
      gamepad.yAxis(yy);  
      gamepad.send();    
    }
    
  } else {
    joystickX = 0;
    joystickY = 0;
  }


    //Perform button actions
    if(!switchAState) {
      switchFeedback(1,mode,switchReactionTime,1);
      gamepad.press(switchProperty[0].switchModeButtonNumber-1);
      gamepad.send();
    } else if(switchAState && switchAPrevState) {
      gamepad.release(switchProperty[0].switchModeButtonNumber-1);
      gamepad.send();
    }
    
    if(!switchBState) {
      switchFeedback(2,mode,switchReactionTime,1);
      gamepad.press(switchProperty[1].switchModeButtonNumber-1);
      gamepad.send();
    } else if(switchBState && switchBPrevState) {
      gamepad.release(switchProperty[1].switchModeButtonNumber-1);
      gamepad.send();
    }
    
    if(!switchCState) {
      switchFeedback(3,mode,switchReactionTime,1);
      gamepad.press(switchProperty[2].switchModeButtonNumber-1);
      gamepad.send();
    } else if(switchCState && switchDPrevState) {
      gamepad.release(switchProperty[2].switchModeButtonNumber-1);
      gamepad.send();
    }
    
    if(!switchDState) {
      switchFeedback(4,mode,switchReactionTime,1);
      gamepad.press(switchProperty[3].switchModeButtonNumber-1);
      gamepad.send();
    } else if(switchDState && switchDPrevState) {
      gamepad.release(switchProperty[3].switchModeButtonNumber-1);
      gamepad.send();
    }

    //Update previous state of buttons 
    switchAPrevState = switchAState;
    switchBPrevState = switchBState;
    switchCPrevState = switchCState;
    switchDPrevState = switchDState;
    
    delay(100);

}


//***SETUP SWITCH MODE FUNCTION***//

void switchSetup() {

    switchMode=SWITCH_MODE;
    switchReactionTime = SWITCH_REACTION_TIME;

    //Serial print settings 
    Serial.print("Switch Mode: ");
    Serial.println(switchMode);


    Serial.print("Reaction Time(ms): ");
    Serial.println(switchReactionTime);
}

//***INIT LED FEEDBACK FUNCTION***//

void initLedFeedback(){
  setLedBlink(2,500,modeProperty[switchMode-1].modeColorNumber,LED_ACTION_BRIGHTNESS);
  delay(5);
  updateLedColor(modeProperty[switchMode-1].modeColorNumber,LED_BRIGHTNESS);
  delay(5);
}
