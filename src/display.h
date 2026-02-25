//*******************************************************************************************//
//                                           Display Set                                     //
//*******************************************************************************************//

/////////////// SSD1306 ////////////////////////

// UNCOMMENT ALL IN THIS SECTION IF YOU CAN USE A SH11XX LIBRERY
// also uncomments the inherent initialization 

//#include <Adafruit_SSD1306.h>              
//Adafruit_SSD1306 display(4);      
//#define SSD1306 HIGH
//#define SH110X_WHITE WHITE

/////////////// SH110x ////////////////////////

// COMMENT ALL IN THIS SECTION IF YOU CAN USE A SSD1306 LIBRERY
// also uncomments the inherent initialization
#pragma once
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, &Wire, 4);
#define SSD1306 LOW
#define WHITE SH110X_WHITE


void drawCentreString(const char *buf, int x, int y)
{
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(buf, x, y, &x1, &y1, &w, &h); //calc width of new string
    display.setCursor(x - w / 2, y);
    display.print(buf);
}


void showHomeScreen(){
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(2);
  display.setCursor(30,11);
  display.print("OrbJoy"); 
    display.setTextSize(1);
    //display.setCursor(38,45);
    //display.println("Left Hand");
    drawCentreString("Left Hand", 64, 45);
  display.display();

}


