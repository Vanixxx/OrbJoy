//Settings
#pragma once
//Pins:
#define LED                 15         // Neopixel PIN
#define Enc_DT              5          // DT Encoder                      
#define Enc_CLK             4          // CLK Encoder        
#define AXIS_X_PIN          A2         // VRY Joystick
#define AXIS_Y_PIN          A3         // VRX Joystick
#define joyButt             A1         // SW Joystick
#define encButt             6          // SW Encoder
#define Rbutton1            7          // Rear Button
#define Rbutton2            8          // Rear Button    
#define Rbutton3            9          // Rear Button    

#define disp_timeout        60000     // time to turn off the display and led (60 seconds * 1000), 0 = kein Timeout
#define NUMPIXELS           7          // Number of led on the strip (default 7)

#define EncoderMode         0          // 0 = Aus, 1 = Buttons, 2 = Achse

#define led_Enc_startR      0           //Cycle Mode = Achse: Start Farbe Rot
#define led_Enc_startG      0           //Cycle Mode = Achse: Start Farbe Gruen
#define led_Enc_startB      255         //Cycle Mode = Achse: Start Farbe Blau
#define led_Enc_endR        255         //Cycle Mode = Achse: End Farbe Rot
#define led_Enc_endG        0           //Cycle Mode = Achse: End Farbe Gruen
#define led_Enc_endB        0           //Cycle Mode = Achse: End Farbe Blau

#define led_Rot_dim         25          //Cycle Mode = Rotaton: nicht aktive LED, 0-255 (0=0% 255=100%)

#define Xdeadzone            10          // X Deadzone
#define Ydeadzone            10          // X Deadzone
#define xyFactor             160          // Faktor X/Y 100 = 100%, 200 = 200%, Muss in Deadzone betrachtet werden, ToDo
int EncoderStartVal =        512;        // Encoder Achse, Startwert (0-1023)
