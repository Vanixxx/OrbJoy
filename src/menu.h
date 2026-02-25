#pragma once
#include <Arduino.h>
#include <Encoder.h>
#include <Bounce2.h>
#include <Joystick.h>
#include <EEPROM.h>
#include "display.h"
#include "settings.h"
#include "led.h"
//#include <avr/wdt.h>

extern bool inMenu;
extern Encoder encoder1;   // nur Deklaration!
extern Adafruit_NeoPixel pixels;
extern Bounce rButton1D;   // nur Deklaration!
extern Bounce rButton3D;   // nur Deklaration!
extern bool sendUpdate;
extern Joystick_ controller;  // nur Deklaration!
extern unsigned long lastTimeActive;
extern int EncoderVal;

long rotatelastval;
byte sellast=-1;
bool rbutton1_last = false;



#include <avr/pgmspace.h>



void mainmenu();

uint8_t rotatemenu (void)
  {
    long rotatevalue = encoder1.read()/4;
    uint8_t ret = 0;
    if(rotatevalue != rotatelastval){
        if(rotatevalue < rotatelastval){
        ret=+1;
        rotatelastval = rotatevalue;
        }
        else if (rotatevalue > rotatelastval){
        ret=-1;
        rotatelastval = rotatevalue;
        }
        setLEDrotation(-ret);
        lastTimeActive = millis();
    }
    return ret;
  }


void menu_encoder(void)
  {
   byte sel = EEPROM.read(100);
   inMenu = true;
   uint8_t item = 3;
    do
      {
        rButton1D.update();
        rButton3D.update();
        sel=sel+rotatemenu();
        if(sel == item+1)
          {
            sel=item;
          }
        if(sel > 250)
          {
            sel=0;
          }
        switch (sel)
          {
            case 0:
                if(sel!=sellast){
                    display.clearDisplay();
                    display.setTextSize(1);
                    display.setCursor(10,10);
                    display.println("1/3  -  Encoder");
                    //display.setCursor(10,50);
                    //display.println("Neustart notwendig");
                    display.setTextSize(2);
                    display.setCursor(11,25);
                    display.println("Button");
                    display.display();
                    sellast=sel;
                }
            break;
      
            case 1:
                if(sel!=sellast){
                    display.clearDisplay();
                    display.setTextSize(1);
                    display.setCursor(10,10);
                    display.println("2/3  -  Encoder");
                    //display.setCursor(10,50);
                    //display.println("Neustart notwendig");
                    display.setTextSize(2);
                    display.setCursor(11,25);
                    display.println("Achse");
                    display.display();
                    sellast=sel;
                }
            break;
      
            case 2:
                if(sel!=sellast){           
                    display.clearDisplay();
                    display.setTextSize(1);
                    display.setCursor(10,10);
                    display.println("3/3  -  Encoder"); 
                    //display.setCursor(10,50);
                    //display.println("Neustart notwendig");
                    display.setTextSize(2);
                    display.setCursor(11,25);
                    display.println("Beides");
                    display.display();
                    sellast=sel;
                }
            break;
          }
        //if (digitalRead(Rbutton1)==LOW and rbutton1_last == HIGH)
        if (rButton1D.fell()) 
        {
            if(sel==1){
                    controller.setButton(5, 0);
                    controller.setButton(6, 0);
                    sendUpdate=true;
            }
            if(sel==2){
                    controller.setRzAxis(EncoderStartVal);
                    sendUpdate = true;
            }
            EEPROM.update(100, sel);
                display.clearDisplay();
                display.setTextSize(1);
                display.setCursor(10,10);
                display.print(sel+1);
                display.print("/");
                display.print(item);
                display.println(" - Encoder");
                //display.setCursor(10,50);
                //display.println("Neustart");
                display.setTextSize(2);
                display.setCursor(11,25);
                display.println("Save");
                display.display();
                lastTimeActive = millis();
                delay(1000);
                    /*display.clearDisplay();
                    display.setTextSize(2);
                    display.setCursor(11,25);
                    display.println("Reboot");
                    //Reboot
                    wdt_enable(WDTO_15MS);   // Watchdog auf 15 ms
                    while (1) {}             // Warten bis Reset kommt*/

            inMenu = false;
            sellast=-1;
        }
        if (rButton3D.fell()) 
        {
                display.clearDisplay();
                display.setTextSize(1);
                display.setCursor(10,10);
                display.print(sel+1);
                display.print("/");
                display.print(item);
                display.println(" - Encoder");
                display.setTextSize(2);
                display.setCursor(11,25);
                display.println("Abbruch");
                display.display();
                lastTimeActive = millis();
                delay(500);
            inMenu = false;
            sellast=-1;
        }
      } while (inMenu==true); 
      mainmenu();

  }

void menu_ledcolor(void)
  {
    //const String color[11] = {"", "Off", "Red", "Green", "Blue", "Yellow", "White", "Purple", "Sky", "Pink", "Orange"};
    //const String color[12] = {"", "Aus", "Rot", "Gruen", "Blau", "Gelb", "Weiß", "Lila", "Sky", "Pink", "Orange",""};
    //const String color[12] = {"", "Aus", "1", "2", "3", "4", "5", "6", "7", "8", "9",""};
    char buffer[10];  // muss groß genug sein!

    /*for(byte a=0; a<12; a++)
            {
                Serial.print("color[");
                Serial.print(a);
                Serial.print("] = \"");
                Serial.print(color[a]);
                Serial.println("\"");
                delay(50);
            }*/
            
    byte sel = EEPROM.read(101);
    inMenu = true;
    uint8_t item = 10;
    do
      {
        rButton1D.update();
        rButton3D.update();
        sel=sel+rotatemenu();
          if(sel == item+1)
            {
                sel=item;
            }
            if(sel > 250 or sel < 1)
            {
                sel=1;
            }

    
            if(sel!=sellast)
            {
                strcpy_P(buffer, (char*)pgm_read_word(&(color[sel])));

                    display.clearDisplay();
                    display.setTextSize(1);
                    display.setCursor(10,10);
                    display.print(sel);
                    display.print("/");
                    display.print(item);
                    display.println(" - LED Farbe");
                    display.setTextSize(2);
                    display.setCursor(11,25);
                    display.println(buffer);
                    //Serial.println(sel);
                    display.display(); 
                    //LED:
                    setLEDcolor(sel);

                    sellast=sel;
            }

                //if (digitalRead(Rbutton1)==LOW and rbutton1_last == HIGH)
        if (rButton1D.fell()) 
        {
            EEPROM.update(101, sel);
                display.clearDisplay();
                display.setTextSize(1);
                display.setCursor(10,10);
                display.print(sel);
                display.print("/");
                display.print(item);
                display.println(" - LED Farbe");
                display.setTextSize(2);
                display.setCursor(11,25);
                display.println("Save");
                display.display();
                lastTimeActive = millis();
                delay(1000);

            sellast=-1;
            inMenu = false;
            
            //mainmenu();
        }
        if (rButton3D.fell()) 
        {       
                setLEDcolor(EEPROM.read(101));
                display.clearDisplay();
                display.setTextSize(1);
                display.setCursor(10,10);
                display.print(EEPROM.read(101));
                display.print("/");
                display.print(item);
                display.println(" - LED Farbe");
                display.setTextSize(2);
                display.setCursor(11,25);
                display.println("Abbruch");
                display.display();
                lastTimeActive = millis();
                delay(500);
            sellast=-1;
            inMenu = false;    
        }
      } while (inMenu==true); 
      mainmenu();
  }

void menu_ledCycle(void)
  {
   byte sel = EEPROM.read(102);
   inMenu = true;
   uint8_t item = 3;
    do
      {
        rButton1D.update();
        rButton3D.update();
        sel=sel+rotatemenu();
        if(sel == item+1)
          {
            sel=item;
          }
        if(sel > 250)
          {
            sel=0;
          }

        switch (sel)
          {
            case 0:
                if(sel!=sellast){
                    display.clearDisplay();
                    display.setTextSize(1);
                    display.setCursor(10,10);
                    display.print(sel+1);
                    display.print("/");
                    display.print(item);
                    display.println(" - LED Cycle");
                    display.setTextSize(2);
                    display.setCursor(11,25);
                    display.println("Aus");
                    display.display();
                    sellast=sel;
                }
            break;
      
            case 1:
                if(sel!=sellast){
                    display.clearDisplay();
                    display.setTextSize(1);
                    display.setCursor(10,10);
                    display.print(sel+1);
                    display.print("/");
                    display.print(item);
                    display.println(" - LED Cycle");
                    //display.setCursor(10,50);
                    //display.println("Neustart notwendig");
                    display.setTextSize(2);
                    display.setCursor(11,25);
                    display.println("Rotation");
                    display.display();
                    sellast=sel;
                }
            break;
      
            case 2:
                if(sel!=sellast){           
                    display.clearDisplay();
                    display.setTextSize(1);
                    display.setCursor(10,10);
                    display.print(sel+1);
                    display.print("/");
                    display.print(item);
                    display.println(" - LED Cycle");
                    //display.setCursor(10,50);
                    //display.println("Neustart notwendig");
                    display.setTextSize(2);
                    display.setCursor(11,25);
                    display.println("Enc Achse");
                    display.display();
                    sellast=sel;
                }
            break;
          }
        if (rButton1D.fell()) //Speichern
        {
            EEPROM.update(102, sel);
            if(sel==0){ //Rotation oder Achse
                setLEDcolor(EEPROM.read(101));
            }                
            setLEDencAchse(EncoderVal);
            setLEDrotation(0);
            
                display.clearDisplay();
                display.setTextSize(1);
                display.setCursor(10,10);
                display.print(sel+1);
                display.print("/");
                display.print(item);
                display.println(" - LED Cycle");
                //display.setCursor(10,50);
                //display.println("Neustart");
                display.setTextSize(2);
                display.setCursor(11,25);
                display.println("Save");
                display.display();
                lastTimeActive = millis();


                delay(1000);
                    /*display.clearDisplay();
                    display.setTextSize(2);
                    display.setCursor(11,25);
                    display.println("Reboot");
                    //Reboot
                    wdt_enable(WDTO_15MS);   // Watchdog auf 15 ms
                    while (1) {}             // Warten bis Reset kommt*/

            inMenu = false;
            sellast=-1;
        }
        if (rButton3D.fell()) 
        {
                display.clearDisplay();
                display.setTextSize(1);
                display.setCursor(10,10);
                display.print(sel+1);
                display.print("/");
                display.print(item);
                display.println(" - LED Cycle");
                display.setTextSize(2);
                display.setCursor(11,25);
                display.println("Abbruch");
                display.display();
                lastTimeActive = millis();
                delay(500);
            inMenu = false;
            sellast=-1;
        }
      } while (inMenu==true); 
      mainmenu();

  }



void mainmenu(void)
  {
   byte sel = 0;
   sellast=-1;
   inMenu = true;
   uint8_t item = 4;
   //display.setTextColor(SH110X_WHITE);
    do
      {
        rButton1D.update();
        rButton3D.update();
        sel=sel+rotatemenu();
        if(sel == item+1)
          {
            sel=item;
          }
        if(sel > 250)
          {
            sel=0;
          }
        /*Serial.print(sel);
          Serial.print(" .  ");
          Serial.print(sellast);
                    Serial.print(" .  ");
          Serial.println(inMenu);*/
        switch (sel)
          {
            case 0:
                if(sel!=sellast){
                    display.clearDisplay();
                    display.setTextSize(1);
                    display.setCursor(10,10);
                    display.print(sel+1);
                    display.print("/");
                    display.println(item);
                    display.setTextSize(2);
                    display.setCursor(11,25);
                    display.println("Encoder");
                    display.display();
                    sellast=sel;
                }

        if (rButton1D.fell()) 
                {
                    sellast=-1;
                    display.clearDisplay();
                    display.display();
                    lastTimeActive = millis();
                    menu_encoder();
                }
            break;
      
            case 1:
                if(sel!=sellast){
                    display.clearDisplay();
                    display.setTextSize(1);
                    display.setCursor(10,10);
                    display.print(sel+1);
                    display.print("/");
                    display.println(item);
                    display.setTextSize(2);
                    display.setCursor(11,25);
                    display.println("LED Farbe");
                    display.display();
                    sellast=sel;
                }
                      //if (digitalRead(Rbutton1)==LOW and rbutton1_last == HIGH)
        if (rButton1D.fell()) 
                {
                    sellast=-1;
                    display.clearDisplay();
                    display.display();
                    lastTimeActive = millis();
                    menu_ledcolor();
                }
            break;
      
            case 2:
                if(sel!=sellast){           
                    display.clearDisplay();
                    display.setTextSize(1);
                    display.setCursor(10,10);
                    display.print(sel+1);
                    display.print("/");
                    display.println(item);
                    display.setTextSize(2);
                    display.setCursor(11,25);
                    display.println("LED Cycle");
                    display.display();
                    sellast=sel;
                }
                      //if (digitalRead(Rbutton1)==LOW and rbutton1_last == HIGH)
        if (rButton1D.fell()) 
                {
                    sellast=-1;
                    display.clearDisplay();
                    display.display();
                    lastTimeActive = millis();
                    menu_ledCycle();
                }
            break;

            case 3:
                if(sel!=sellast){    
                    display.clearDisplay();
                    display.setTextSize(1);
                    display.setCursor(10,10);
                    display.print(sel+1);
                    display.print("/");
                    display.println(item);
                    display.setTextSize(2);
                    display.setCursor(40,25);
                    display.println("EXIT");
                    display.display();
                    sellast=sel;
                }
                      //if (digitalRead(Rbutton1)==LOW and rbutton1_last == HIGH)
        if (rButton1D.fell()) 
                {
                  inMenu = false;
                  lastTimeActive = millis();
                  sellast=-1;
                }
            break;
          }
        if (rButton3D.fell()) 
        {
            inMenu = false;
            sellast=-1;
            lastTimeActive = millis();
            //delay(500);
        }
        rbutton1_last=digitalRead(Rbutton1);
      } while (inMenu==true); 
  }



