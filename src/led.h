#pragma once
#include <Arduino.h>
#include <settings.h>
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#include <avr/pgmspace.h>

extern byte posLed;

// Struktur für eine Farbe
struct ColorDef {
  const char* name;
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

const char color0[] PROGMEM = "Aus";
const char color1[] PROGMEM = "Aus";
const char color2[] PROGMEM = "Rot";
const char color3[] PROGMEM = "Gruen";
const char color4[] PROGMEM = "Blau";
const char color5[] PROGMEM = "Gelb";
const char color6[] PROGMEM = "Weiss";
const char color7[] PROGMEM = "Lila";
const char color8[] PROGMEM = "Sky";
const char color9[] PROGMEM = "Pink";
const char color10[] PROGMEM = "Orange";
const char color11[] PROGMEM = "Orange";
const char* const color[] PROGMEM = {
  color0, color1, color2, color3, color4, color5,
  color6, color7, color8, color9, color10, color11
};



const ColorDef colors[] PROGMEM = {
  {color0,   0,   0,   0},       // Aus
  {color1,   0,   0,   0},       // Aus
  {color2, 255,   0,   0},       // Rot
  {color3,   0, 255,   0},       // Gruen
  {color4,   0,   0, 255},       // Blau
  {color5, 200, 130,   0},       // Gelb
  {color6, 255, 240, 220},       // Weiss
  {color7, 100,   0, 255},       // Lila
  {color8,   0, 255, 255},       // Sky
  {color9, 255,  0, 255},       // Pink
  {color10, 255, 70,   0},        // Orange
  {color11, 255, 165,   0}        // Orange
};


Adafruit_NeoPixel pixels(NUMPIXELS, LED, NEO_GRB + NEO_KHZ800);

void LEDinit(void){
    pixels.begin();
}


void setLEDcolor (byte pixelmode)
  {
    
    pixels.clear();
    if (pixelmode==0){
        pixelmode = EEPROM.read(101);
        if (pixelmode == 0){
            EEPROM.update(101, 1);
        }
    }
        uint8_t r = pgm_read_byte(&colors[pixelmode].r);
        uint8_t g = pgm_read_byte(&colors[pixelmode].g);
        uint8_t b = pgm_read_byte(&colors[pixelmode].b);
      for(byte a = 0; a < NUMPIXELS; a++)
        {
            pixels.setPixelColor(a, pixels.Color(r, g, b));
        }

    pixels.show();
  }


  void setLEDencAchse(int value) {
        // Encoder Wert-----------------------------------------------------------
        if(EEPROM.read(102) == 2){ // Encoder Wert
            value = map(value, 0, 1023, 0, 255);


            uint8_t r = led_Enc_startR + ((led_Enc_endR - led_Enc_startR) * value) / 255;
            uint8_t g = led_Enc_startG + ((led_Enc_endG - led_Enc_startG) * value) / 255;
            uint8_t b = led_Enc_startB + ((led_Enc_endB - led_Enc_startB) * value) / 255;

            for (int i = 0; i < NUMPIXELS; i++) {
                pixels.setPixelColor(i, pixels.Color(r, g, b));
            }
            pixels.show();
        }
    }

    void setLEDrotation(uint8_t ledrotval) {
        // Rotation-----------------------------------------------------------
        if(EEPROM.read(102) == 1){ // Rotation
            //Serial.println(ledrotval);
            byte rotcolor = EEPROM.read(101);
            //Serial.println(EEPROM.read(101));
            uint8_t baseR = pgm_read_byte(&colors[rotcolor].r);
            uint8_t baseG = pgm_read_byte(&colors[rotcolor].g);
            uint8_t baseB = pgm_read_byte(&colors[rotcolor].b);
                uint8_t dimR=(baseR * led_Rot_dim) / 255;
                uint8_t dimG=(baseG * led_Rot_dim) / 255;
                uint8_t dimB=(baseB * led_Rot_dim) / 255;

                pixels.clear();
                for(byte i=0; i<NUMPIXELS; i++) // Alle LEDs gedimmt.
                    {
                    pixels.setPixelColor(i, pixels.Color(dimR, dimG, dimB));
                    }  
                
                //>>>> Hier die helle LED!
                    posLed=posLed-ledrotval;
                        if(posLed == NUMPIXELS)
                        {
                            posLed=0;
                        }
                        if(posLed > 250)
                        {
                            posLed=NUMPIXELS-1;
                        }
                        pixels.setPixelColor(posLed, pixels.Color(baseR, baseG, baseB));

        pixels.show();
        }
  }