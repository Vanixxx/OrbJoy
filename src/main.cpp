#include <Arduino.h>
#include <EEPROM.h>
#include <Bounce2.h>
#include "settings.h"
#include <Joystick.h>
#include <Encoder.h>
#include "EncoderAdvanced.h"
#include "joystick_in.h"
//#include <Adafruit_NeoPixel.h>
#include "display.h"
#include "menu.h"
#include <avr/pgmspace.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
//#include "led.h"
//Adafruit_NeoPixel pixels(NUMPIXELS, LED, NEO_GRB + NEO_KHZ800);


uint8_t hidReportId = 0x06; // - Default: 0x03 - Indicates the joystick's HID report ID. This value must be unique if you are creating multiple instances of Joystick. Do not use 0x01 or 0x02 as they are used by the built-in Arduino Keyboard and Mouse libraries.

//Buttons:
const uint8_t buttonCount = 5;
int const buttonPins[buttonCount] = {
  Rbutton1,
  Rbutton2,
  Rbutton3,
  encButt,
  joyButt,
};
int lastButtonValue[buttonCount+2]; //+Encoder
    //Bounce
Bounce rButton1D = Bounce();
//Bounce rButton2D = Bounce();
Bounce rButton3D = Bounce();
//Bounce joyButtD = Bounce();
//Bounce encButtD = Bounce();

//https://github.com/MHeironimus/ArduinoJoystickLibrary/tree/master?tab=readme-ov-file#joystick-library-api
Joystick_ controller(hidReportId, JOYSTICK_TYPE_JOYSTICK, 
                     buttonCount+2, 0,        // Button Count, Hat Switch Count (Rundblickschalter)
                     true, true, false,     // X and Y,  Z Axis
                     false, false, true,   // Rotation X, Rotation Y, Rotation Z
                     false, false,          // Rudder, Throttle
                     false, false, false);  // No accelerator, brake, or steering


//-----------------------------------Encoder--------------------------------------------------------
Encoder encoder1(Enc_CLK,Enc_DT);
unsigned long encpulseStart = 0;
const unsigned long encpulseLength = 50; // 10 ms Impuls
int encactiveButton = -1;
EncoderAdvanced encoderAdv(Enc_CLK,Enc_DT); //Für Achse
int EncoderAdvRz_lastvalue;
bool inMenu = false;  
int EncoderVal = EncoderStartVal;
byte posLed = 0;

int currentXAxisValue;
int currentYAxisValue;
long encoderlastvalue;
int YZero, XZero = 0;



int lastXAxisValue = -1;
int lastYAxisValue = -1;
bool sendUpdate;

unsigned long lastTimeActive = 0;
bool displayState = true;


void setup()
{

  controller.setYAxisRange(-511, 511);
  controller.setXAxisRange(511, -511);
  YZero = analogRead(AXIS_Y_PIN);  
  XZero = analogRead(AXIS_X_PIN);  

  controller.begin(false);   //(initAutoSendState=Aus -> sendupdate notwendig)
  
  controller.setRzAxis(EncoderStartVal);
  controller.sendState();

  //encoder.attachButtons(5,6);   // Button 0 = CW, Button 1 = CCW  -> Encoder Advanced
  encoderAdv.attachAxis(&EncoderVal); // Encoder Advanced

  rButton1D.attach(Rbutton1);
  //rButton2D.attach(Rbutton2);
  rButton3D.attach(Rbutton3);
  //joyButtD.attach(joyButt);
  //encButtonD.attach(encButt);
  rButton1D.interval(25);
  //rButton2D.interval(25);
  rButton3D.interval(25);
  //joyButtD.interval(25);
  //encButtonD.interval(25);
  for (int i = 0; i < buttonCount; i++)
  {
    pinMode(buttonPins[i], INPUT_PULLUP);
    lastButtonValue[i] = -1;
  }


  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  LEDinit();
  if(EEPROM.read(102 == 0)){ setLEDcolor(EEPROM.read(101));}
  setLEDencAchse(EncoderStartVal);
  setLEDrotation(0);

  display.begin(0x3C, true); // Address 0x3C default     // comment if your oled is SSD1306
  display.setRotation(2);
  showHomeScreen();

  //Serial.begin(115200);
  lastTimeActive = millis();

}

void loop()
{
  rButton1D.update();
  //rButton2D.update();
  //rButton3D.update();
  //joyButtD.update();
  //encButtonD.update();
  sendUpdate = false;

  for (int i = 0; i < buttonCount; i++)
  {
    const int buttonValue = digitalRead(buttonPins[i]);

    if (buttonValue != lastButtonValue[i])
    {
      controller.setButton(i, !buttonValue);
      lastButtonValue[i] = buttonValue;
      sendUpdate = true;
    }
  }

  currentYAxisValue = (analogRead(AXIS_Y_PIN) - YZero);
  currentXAxisValue = (analogRead(AXIS_X_PIN) - XZero);
  
  //currentXAxisValue = (map(currentXAxisValue,0,1023,-511,+511)+5)*1;
  //currentYAxisValue = (map(currentYAxisValue,0,1023,-511,+511)+7)*1;
  currentXAxisValue = applyDeadzone(currentXAxisValue, Xdeadzone);
  currentYAxisValue = applyDeadzone(currentYAxisValue, Ydeadzone);
  //currentXAxisValue = currentXAxisValue * xyFactor /100;
  //currentYAxisValue = currentYAxisValue * xyFactor /100;

  if (currentXAxisValue != lastXAxisValue)
  {
    controller.setXAxis(currentXAxisValue);
    lastXAxisValue = currentXAxisValue;
    sendUpdate = true;
  }


  if (currentYAxisValue != lastYAxisValue)
  {
    controller.setYAxis(currentYAxisValue);
    lastYAxisValue = currentYAxisValue;
    sendUpdate = true;
  }

  // Encoder ==================================================================================
  long encodervalue = encoder1.read()/4;
  //Serial.println(encodervalue);
  if(EEPROM.read(100) != 1){ //Nicht Achsen Modus
                  if(encodervalue != encoderlastvalue && encactiveButton == -1){
                    if(encodervalue < encoderlastvalue){
                      controller.setButton(6, 1);
                      setLEDrotation(-1);
                      sendUpdate = true;
                      encactiveButton=1;
                    }
                    else if (encodervalue > encoderlastvalue){
                      controller.setButton(5, 1);
                      setLEDrotation(+1);
                      sendUpdate = true;
                      encactiveButton=1;
                    }
                    encoderlastvalue = encodervalue;
                    encpulseStart = millis();

                  }
                  if (encactiveButton != -1 && millis() - encpulseStart >= encpulseLength) {

                    controller.setButton(5, 0);
                    controller.setButton(6, 0);
                    sendUpdate=true;

                    encactiveButton = -1;    
                  }
  }


  if(EEPROM.read(100) != 0){ //Nicht Button Modus
              if(encoderAdv.update(controller)== true){
                controller.setRzAxis(EncoderVal);
                setLEDencAchse(EncoderVal);
                sendUpdate = true;
              }
  }              


  if (sendUpdate)
  {
    controller.sendState();
    lastTimeActive = millis();
    if (!displayState) {
          showHomeScreen();
          if(EEPROM.read(102 == 0)){ setLEDcolor(EEPROM.read(101));}
          setLEDencAchse(EncoderStartVal);
          setLEDrotation(0);
          displayState = true;
    }
  }


//Menu:
    //if((digitalRead(joyButt) == LOW) && (digitalRead(Rbutton1)== LOW))
    if((digitalRead(joyButt) == LOW) && (rButton1D.currentDuration() > 1000) && (rButton1D.read()==LOW))
      {
        delay(500);
        //Keyboard.releaseAll();
        //Mouse.release(MOUSE_MIDDLE);
        //Mouse.release(MOUSE_LEFT);
        //Mouse.release(MOUSE_RIGHT);
        for (int i = 0; i < buttonCount+2; i++)
        { 
          controller.setButton(i, 0); 
          lastButtonValue[i] = 0;
        }
        currentXAxisValue = 0;
        controller.setXAxis(currentXAxisValue);
        currentYAxisValue = 0;
        controller.setYAxis(currentYAxisValue);
        EncoderVal = EncoderStartVal;
        controller.setRzAxis(EncoderStartVal);
        controller.sendState();
        mainmenu();
        showHomeScreen();
      }


//Timeout Display:
  if (displayState and (millis() - lastTimeActive >= disp_timeout) and disp_timeout != 0) {
      display.clearDisplay();
      display.display();
      for(byte i=0; i<NUMPIXELS; i++)
      {
        pixels.setPixelColor(i, pixels.Color(0, 0, 0));//Off
      }
      pixels.show();
      displayState = false;
      
  }
  delay(1);
}
