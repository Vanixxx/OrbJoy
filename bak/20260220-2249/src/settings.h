//Settings

#define LED               15         // Neopixel PIN
#define NUMPIXELS         7          // Number of led on the strip (default 7)
#define Enc_DT            5          // DT Encoder                      
#define Enc_CLK           4          // CLK Encoder        
#define AXIS_X_PIN        A2         // VRY Joystick
#define AXIS_Y_PIN        A3         // VRX Joystick
#define joyButt           A1         // SW Joystick
#define encButt           6          // SW Encoder
#define Rbutton1          7          // Rear Button
#define Rbutton2          8          // Rear Button    
#define Rbutton3          9          // Rear Button    
#define timeout           60000      // time to turn off the display and led (60 seconds * 1000)
#define EncoderMode       0          // 0 = Aus, 1 = Buttons, 2 = Achse
int EncoderStartVal =     512;        // Encoder Achse, Startwert (0-1023)