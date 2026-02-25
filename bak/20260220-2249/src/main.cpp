#include <Arduino.h>
#include "settings.h"
#include <Joystick.h>
#include <Encoder.h>
#include "EncoderAdvanced.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

Adafruit_NeoPixel pixels(NUMPIXELS, LED, NEO_GRB + NEO_KHZ800);


uint8_t hidReportId = 0x06; // - Default: 0x03 - Indicates the joystick's HID report ID. This value must be unique if you are creating multiple instances of Joystick. Do not use 0x01 or 0x02 as they are used by the built-in Arduino Keyboard and Mouse libraries.
const uint8_t buttonCount = 5;


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

int currentXAxisValue;
int currentYAxisValue;
long encoderlastvalue;


int const buttonPins[buttonCount] = {
  Rbutton1,
  Rbutton2,
  Rbutton3,
  encButt,
  joyButt,
};
int lastButtonValue[buttonCount+2]; //+Encoder
int lastXAxisValue = -1;
int lastYAxisValue = -1;


void setup()
{
  controller.setYAxisRange(-511, 511);
  controller.setXAxisRange(511, -511);
  controller.begin(false);   //(initAutoSendState=Aus -> sendupdate notwendig)
  
  controller.setRzAxis(EncoderStartVal);
  controller.sendState();

  //encoder.attachButtons(5,6);   // Button 0 = CW, Button 1 = CCW  -> Encoder Advanced
  encoderAdv.attachAxis(&EncoderStartVal); // Encoder Advanced


  for (int i = 0; i < buttonCount; i++)
  {
    pinMode(buttonPins[i], INPUT_PULLUP);
    lastButtonValue[i] = -1;
  }

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  pixels.begin();
  for(byte a=0; a<NUMPIXELS; a++)
  {
    pixels.setPixelColor(a, pixels.Color(255, 0, 255));//RGB
  } 
  pixels.show();


}

void loop()
{
  bool sendUpdate = false;
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

  currentYAxisValue = analogRead(AXIS_Y_PIN);
  currentXAxisValue = analogRead(AXIS_X_PIN);
  
  currentXAxisValue = (map(currentXAxisValue,0,1023,-511,+511)+5)*1;
  currentYAxisValue = (map(currentYAxisValue,0,1023,-511,+511)+7)*1;

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
  if(encodervalue != encoderlastvalue && encactiveButton == -1){
    if(encodervalue < encoderlastvalue){
      controller.setButton(6, 1);
      sendUpdate = true;
      encactiveButton=1;
    }
    else if (encodervalue > encoderlastvalue){
      controller.setButton(5, 1);
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
  if(encoderAdv.update(controller)== true){
    controller.setRzAxis(EncoderStartVal);
    sendUpdate = true;
  }



  if (sendUpdate)
  {
    controller.sendState();
  }
  delay(1);
}
