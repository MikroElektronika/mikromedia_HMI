#include "Mikromedia_HMI_50C_Demo_objects.h"
#include "Mikromedia_HMI_50C_Demo_resources.h"
#include "built_in.h"
#include "FT812_Types.h"


// Extern memory interface
// MMC/SD Connections
sbit Mmc_Chip_Select   at GPIO_PIN21_bit;
// end of MMC/SD
// VTFT Get Data globals
short rfHandle;
char* rfName;
unsigned long rfStartSect;
// end of VTFT Get Data
// End of extern memory interface


// TFT module connections
sbit FT812_RST at GPIO_PIN62_bit;
sbit FT812_CS at GPIO_PIN34_bit;
sbit FT812_RST_Direction at GPIO_PIN62_bit;
sbit FT812_CS_Direction at GPIO_PIN34_bit;
// End TFT module connections

// Object type constants
// Usage: VTFT stack internally
const VTFT_OT_BUTTON       = 0;
const VTFT_OT_LABEL        = 4;
const VTFT_OT_IMAGE        = 6;
const VTFT_OT_CIRCLEBUTTON = 10;
const VTFT_OT_BOX          = 12;
const VTFT_OT_CHECKBOX     = 20;
// ~Object type constants

// Event type constants
// Usage: OnEvent
const VTFT_EVT_UP    = 0;
const VTFT_EVT_DOWN  = 1;
const VTFT_EVT_CLICK = 2;
const VTFT_EVT_PRESS = 3;
// ~Event type constants

// Sound action constants
// Usage: sound event action property and ProcessEvent
const VTFT_SNDACT_NONE  = 0;
const VTFT_SNDACT_PLAY  = 1;
const VTFT_SNDACT_STOP  = 2;
// ~Sound action constants

// Resource loading constants.
// Usage: DrawScreenO and LoadCurrentScreenResToGRAM
const VTFT_LOAD_RES_NONE    = 0x00; // do not load g-ram resources
const VTFT_LOAD_RES_STATIC  = 0x01; // load g-ram resources for static objects
const VTFT_LOAD_RES_DYNAMIC = 0x02; // load g-ram resources for dynamic objects
const VTFT_LOAD_RES_ALL     = 0x03; // load g-ram resources for all objects
// ~Resource loading constants

// Display effect constants
// Usage: DrawScreenO
const VTFT_DISPLAY_EFF_NONE         = 0x00; // no effect when switching between screens
const VTFT_DISPLAY_EFF_LIGHTS_FADE  = 0x04; // backlight: fade out before, fade in after drawing new screen
const VTFT_DISPLAY_EFF_LIGHTS_OFF   = 0x08; // backlight: turn off before, turn on after drawing new screen
// ~Display effect constants

// Stack flags
// Usage: internally used by VTFT stack
const VTFT_INT_REPAINT_ON_DOWN     = 1 << 0;
const VTFT_INT_REPAINT_ON_UP       = 1 << 1;
const VTFT_INT_BRING_TO_FRONT      = 1 << 2;
const VTFT_INT_INTRINSIC_CLICK_EFF = 1 << 3;
// ~Stack flags

// Table of object draw handlers
// Use object type constants to access coresponding object draw handler
const TPointer DrawHandlerTable[44] = {
  &DrawButton,       // Button draw handler
  0,                 // CButton draw handler not used
  0,                 // ButtonRound draw handler not used
  0,                 // CButtonRound draw handler not used
  &DrawLabel,        // Label draw handler
  0,                 // CLabel draw handler not used
  &DrawImage,        // Image draw handler
  0,                 // CImage draw handler not used
  0,                 // Circle draw handler not used
  0,                 // CCircle draw handler not used
  &DrawCircleButton, // CircleButton draw handler
  0,                 // CCircleButton draw handler not used
  &DrawBox,          // Box draw handler
  0,                 // CBox draw handler not used
  0,                 // BoxRound draw handler not used
  0,                 // CBoxRound draw handler not used
  0,                 // Line draw handler not used
  0,                 // CLine draw handler not used
  0,                 // Polygon draw handler not used
  0,                 // CPolygon draw handler not used
  &DrawCheckBox,     // CheckBox draw handler
  0,                 // RadioButton draw handler not used
  0,                 // ProgressBar draw handler not used
  0,                 // Audio draw handler not used
  0,                 // EveClock draw handler not used
  0,                 // EveGauge draw handler not used
  0,                 // EveDial draw handler not used
  0,                 // EveKeys draw handler not used
  0,                 // CEveKeys draw handler not used
  0,                 // EveProgressBar draw handler not used
  0,                 // EveScrollBar draw handler not used
  0,                 // EveToggle draw handler not used
  0,                 // EveSlider draw handler not used
  0,                 // EveSpinner draw handler not used
  0,                 // EveScreenSaver draw handler not used
  0,                 // EveSketch draw handler not used
  0,                 // EveButton draw handler not used
  0,                 // CEveButton draw handler not used
  0,                 // EveGradient draw handler not used
  0,                 // CEveGradient draw handler not used
  0,                 // EveText draw handler not used
  0,                 // CEveText draw handler not used
  0,                 // EveNumber draw handler not used
  0                  // CEveNumber draw handler not used
};
// ~Table of draw handler pointers


// Default configuration parameters
const TFT812Display VTFT_FT812_CONFIG_DISPLAY =
{
  12000000,        // Frequency          = main clock frequency
  0,               // OutRenderMode      = 0 normal, 1 write, 2 read
  0,               // RenderReadScanLine = scanline for read render mode
  0,               // RenderWriteTrigger = trigger for write render mode (read only)
  1056,            // hCycle             = number if horizontal cycles for display
  46,              // hOffset            = horizontal offset from starting signal
  800,             // hSize              = width resolution
  0,               // hSync0             = hsync falls
  10,              // hSync1             = hsync rise
  525,             // vCycle             = number of vertical cycles for display
  23,              // vOffset            = vertical offset from start signal
  480,             // vSize              = height resolution
  0,               // vSync0             = vsync falls
  10,              // vSync1             = vsync rise
  1,               // Rotate             = rotate display
  0x01B6,          // OutBits            = output bits resolution
  0,               // OutDither          = output number of bits
  0x0000,          // OutSwizzle         = output swizzle
  0,               // OutCSpread         = output clock spread enable
  1,               // PClockPolarity     = clock polarity: 0 - rising edge, 1 - falling edge
  1,               // PClock             = clock prescaler of FT812: - 0 means disable and >0 means 48MHz/pclock
};

const TFT812GPIO VTFT_FT812_CONFIG_GPIO =
{
  0xFFF0,          // GPIODIR = GPIO direction: 1 - output, 0 - input (16bit wide)
  0xFFFF,          // GPIO    = GPIO data latch
};

const TFT812PWM VTFT_FT812_CONFIG_PWM =
{
  250,             // Freq = PWM frequency - 14 bits
  128,             // Duty = PWM duty cycle, 0 to 128 is the range
};

const TFT812Interrupt VTFT_FT812_CONFIG_INTERRUPT =
{
  0,               // Flags  = interrupt flags (read only)
  0,               // Enable = global interrupt enable: 1 - enabled, 0 - disabled
  255,             // Mask   = interrupt mask value (individual interrupt enable): 1 - masked/disabled, 0 - enabled
};

const TFT812Sound VTFT_FT812_CONFIG_SOUND =
{
  0,               // Volume
  {0,              // Effect
  0},              // Pitch
  0,               // Play
};

const TFT812Audio VTFT_FT812_CONFIG_AUDIO =
{
  0,               // Volume
  0,               // StartAddress
  0,               // Length
  0,               // ReadPtr
  8000,            // Frequency
  0,               // Format
  0,               // Loop = audio playback mode
  0,               // Play
};

// Global variables

TTouchStat TouchS = {0};


/////////////////////////
TScreen *CurrentScreen = 0;


TScreen Home;

TBox   Box34;
TImage Image75;
TImage Image76;
TBox   Box2;
TEvent Box2_OnClick;
TImage Image74;
TImage Image72;
TImage Image73;
TImage Image71;
TBox   Box1;
TEvent Box1_OnClick;
TImage Image43;
TImage Image42;
TImage Image40;
TImage Image5;
TImage Image41;
TImage Image64;
TImage Image45;
TImage Image44;
TBox   Box35;
TEvent Box35_OnClick;
TImage Image39;
TBox   Box12;
TEvent Box12_OnClick;
TBox   Box19;
TBox   Box18;
TBox   Box17;
TBox   Box16;
TBox   Box15;
TBox   Box14;
TBox   Box21;
TEvent Box21_OnClick;
TBox   Box20;
TEvent Box20_OnClick;
TBox   Box13;
TEvent Box13_OnClick;
TBox   Box8;
TEvent Box8_OnClick;
TBox   Box7;
TEvent Box7_OnClick;
TBox   Box6;
TEvent Box6_OnClick;
TBox   Box5;
TEvent Box5_OnClick;
TBox   Box4;
TEvent Box4_OnClick;
TBox   Box3;
TEvent Box3_OnClick;
TBox   Box31;
TEvent Box31_OnClick;
TImage Image1;
TImage Image20;
TImage Image19;
TImage Image18;
TImage Image17;
TImage Image16;
TImage Image9;
TImage Image8;
TImage Image7;
TImage Image6;
TImage Image15;
TImage Image14;
TImage Image13;
TImage Image12;
TImage Image11;
TImage Image10;
TImage Image3;
TImage Image4;
TEvent Image4_OnClick;
TImage Image38;
TImage Image37;
TEvent Image37_OnClick;
TImage Image36;
TEvent Image36_OnClick;
TImage Image35;
TEvent Image35_OnClick;
TImage Image29;
TImage Image30;
TImage Image31;
TImage Image32;
TImage Image33;
TImage Image34;
TImage Image27;
TImage Image25;
TImage Image26;
TImage Image23;
TImage Image24;
TImage Image21;
TImage Image22;
TLabel Label1;
char   Label1_Caption[13] = "Current Mode";
TLabel Label2;
char   Label2_Caption[13] = "HOME";
TLabel Label3;
char   Label3_Caption[13] = "Current Mode";
TLabel Label4;
char   Label4_Caption[13] = "MEDIA";
TLabel Label5;
char   Label5_Caption[6] = "Night";
TLabel Label7;
char   Label7_Caption[9] = "Security";
TLabel Label8;
char   Label8_Caption[12] = "DISARMED";
TLabel Label9;
char   Label9_Caption[9] = "Entrance";
TLabel Label10;
char   Label10_Caption[7] = "Garden";
TLabel Label11;
char   Label11_Caption[7] = "Garage";
TLabel Label12;
char   Label12_Caption[8] = "Kitchen";
TLabel Label13;
char   Label13_Caption[5] = "Gate";
TLabel Label14;
char   Label14_Caption[6] = "Ratio";
TLabel Label15;
char   Label15_Caption[11] = "System Log";
TLabel Label16;
char   Label16_Caption[23] = "No Events";
TLabel Label17;
char   Label17_Caption[11] = "Gren Power";
TLabel Label18;
char   Label18_Caption[14] = "Solar charge:";
TLabel Label19;
char   Label19_Caption[9] = "Battery:";
TLabel Label20;
char   Label20_Caption[11] = "Time left:";
TLabel Label21;
char   Label21_Caption[16] = "    25%";
TLabel Label22;
char   Label22_Caption[16] = "    25%";
TLabel Label23;
char   Label23_Caption[16] = "    03:00h";
TBox   Box28;
TEvent Box28_OnClick;
TLabel Label35;
char   Label35_Caption[13] = "Music Center";
TLabel Label36;
char   Label36_Caption[7] = "Dining";
TLabel Label37;
char   Label37_Caption[8] = "Kitchen";
TLabel Label38;
char   Label38_Caption[5] = "Sofa";
TLabel Label39;
char   Label39_Caption[9] = "Entrance";
TLabel Label40;
char   Label40_Caption[8] = "Bedroom";
TLabel Label41;
char   Label41_Caption[5] = "Kids";
TLabel Label42;
char   Label42_Caption[9] = "Settings";
TLabel Label61;
char   Label61_Caption[6] = "Today";
TLabel Label68;
char   Label68_Caption[16] = "Tomorrow: 10/16";
TLabel Label69;
char   Label69_Caption[2] = "C";
TLabel Label70;
char   Label70_Caption[4] = "0 %";
TLabel Label73;
char   Label73_Caption[7] = " 14";
TLabel Label74;
char   Label74_Caption[2] = ":";
TLabel Label75;
char   Label75_Caption[7] = " 30";
TLabel Label76;
char   Label76_Caption[7] = " 25.";
TLabel Label77;
char   Label77_Caption[7] = "  4.";
TLabel Label78;
char   Label78_Caption[11] = "  2016.";
TLabel Label58;
char   Label58_Caption[3] = "14";
TLabel Label59;
char   Label59_Caption[14] = "Humidity: 56%";
TLabel Label60;
char   Label60_Caption[2] = "C";

TLabel *const code Home_Labels[43] = {
  &Label1,              
  &Label2,              
  &Label3,              
  &Label4,              
  &Label5,              
  &Label7,              
  &Label8,              
  &Label9,              
  &Label10,             
  &Label11,             
  &Label12,             
  &Label13,             
  &Label14,             
  &Label15,             
  &Label16,             
  &Label17,             
  &Label18,             
  &Label19,             
  &Label20,             
  &Label21,             
  &Label22,             
  &Label23,             
  &Label35,             
  &Label36,             
  &Label37,             
  &Label38,             
  &Label39,             
  &Label40,             
  &Label41,             
  &Label42,             
  &Label61,             
  &Label68,             
  &Label69,             
  &Label70,             
  &Label73,             
  &Label74,             
  &Label75,             
  &Label76,             
  &Label77,             
  &Label78,             
  &Label58,             
  &Label59,             
  &Label60              
};

TImage *const code Home_Images[50] = {
  &Image75,             
  &Image76,             
  &Image74,             
  &Image72,             
  &Image73,             
  &Image71,             
  &Image43,             
  &Image42,             
  &Image40,             
  &Image5,              
  &Image41,             
  &Image64,             
  &Image45,             
  &Image44,             
  &Image39,             
  &Image1,              
  &Image20,             
  &Image19,             
  &Image18,             
  &Image17,             
  &Image16,             
  &Image9,              
  &Image8,              
  &Image7,              
  &Image6,              
  &Image15,             
  &Image14,             
  &Image13,             
  &Image12,             
  &Image11,             
  &Image10,             
  &Image3,              
  &Image4,              
  &Image38,             
  &Image37,             
  &Image36,             
  &Image35,             
  &Image29,             
  &Image30,             
  &Image31,             
  &Image32,             
  &Image33,             
  &Image34,             
  &Image27,             
  &Image25,             
  &Image26,             
  &Image23,             
  &Image24,             
  &Image21,             
  &Image22              
};

TBox *const code Home_Boxes[22] = {
  &Box34,               
  &Box2,                
  &Box1,                
  &Box35,               
  &Box12,               
  &Box19,               
  &Box18,               
  &Box17,               
  &Box16,               
  &Box15,               
  &Box14,               
  &Box21,               
  &Box20,               
  &Box13,               
  &Box8,                
  &Box7,                
  &Box6,                
  &Box5,                
  &Box4,                
  &Box3,                
  &Box31,               
  &Box28                
};


TScreen Temperature_Mode;

TBox          Box45;
TImage        Image60;
TBox          Box40;
TEvent        Box40_OnClick;
TBox          Box39;
TEvent        Box39_OnClick;
TBox          Box38;
TEvent        Box38_OnClick;
TBox          Box37;
TEvent        Box37_OnClick;
TImage        Image51;
TImage        Image50;
TImage        Image49;
TImage        Image48;
TImage        Image47;
TImage        Image46;
TImage        Image28;
TImage        Image2;
TButton       Button30;
TEvent        Button30_OnClick;
char          Button30_Caption[2] = "<";
TBox          Box22;
TButton       Button47;
char          Button47_Caption[12] = "TEMPERATURE";
TBox          Box41;
TBox          Box42;
TBox          Box43;
TBox          Box44;
TBox          Box46;
TBox          Box47;
TBox          Box48;
TCircleButton CircleButton1;
TEvent        CircleButton1_OnPress;
char          CircleButton1_Caption[2] = "-";
TCircleButton CircleButton2;
TEvent        CircleButton2_OnPress;
char          CircleButton2_Caption[2] = "-";
TCircleButton CircleButton3;
TEvent        CircleButton3_OnPress;
char          CircleButton3_Caption[2] = "+";
TCircleButton CircleButton4;
TEvent        CircleButton4_OnPress;
char          CircleButton4_Caption[2] = "+";
TBox          Box23;
TBox          Box49;
TLabel        Label29;
char          Label29_Caption[13] = "Temperature:";
TLabel        Label31;
char          Label31_Caption[10] = "Humidity:";
TLabel        Label32;
char          Label32_Caption[5] = "HOME";
TLabel        Label33;
char          Label33_Caption[6] = "NIGHT";
TLabel        Label46;
char          Label46_Caption[4] = "DAY";
TLabel        Label47;
char          Label47_Caption[6] = "PARTY";
TLabel        Label86;
char          Label86_Caption[2] = "C";
TLabel        Label87;
char          Label87_Caption[2] = "%";
TLabel        Label95;
char          Label95_Caption[11] = "    15";
TLabel        Label96;
char          Label96_Caption[11] = "    20";

TButton *const code Temperature_Mode_Buttons[2] = {
  &Button30,            
  &Button47             
};

TLabel *const code Temperature_Mode_Labels[10] = {
  &Label29,             
  &Label31,             
  &Label32,             
  &Label33,             
  &Label46,             
  &Label47,             
  &Label86,             
  &Label87,             
  &Label95,             
  &Label96              
};

TImage *const code Temperature_Mode_Images[9] = {
  &Image60,             
  &Image51,             
  &Image50,             
  &Image49,             
  &Image48,             
  &Image47,             
  &Image46,             
  &Image28,             
  &Image2               
};

TCircleButton *const code Temperature_Mode_CircleButtons[4] = {
  &CircleButton1,       
  &CircleButton2,       
  &CircleButton3,       
  &CircleButton4        
};

TBox *const code Temperature_Mode_Boxes[15] = {
  &Box45,               
  &Box40,               
  &Box39,               
  &Box38,               
  &Box37,               
  &Box22,               
  &Box41,               
  &Box42,               
  &Box43,               
  &Box44,               
  &Box46,               
  &Box47,               
  &Box48,               
  &Box23,               
  &Box49                
};


TScreen Light_Mode;

TBox          Box61;
TImage        Image61;
TBox          Box55;
TEvent        Box55_OnClick;
TBox          Box53;
TEvent        Box53_OnClick;
TBox          Box52;
TEvent        Box52_OnClick;
TBox          Box51;
TEvent        Box51_OnClick;
TImage        Image59;
TImage        Image58;
TImage        Image57;
TImage        Image56;
TImage        Image55;
TImage        Image54;
TImage        Image53;
TImage        Image52;
TButton       Button31;
TEvent        Button31_OnClick;
char          Button31_Caption[2] = "<";
TBox          Box50;
TButton       Button32;
char          Button32_Caption[9] = "LIGHTING";
TBox          Box56;
TBox          Box57;
TBox          Box58;
TBox          Box59;
TBox          Box60;
TBox          Box62;
TBox          Box63;
TCircleButton CircleButton5;
TEvent        CircleButton5_OnPress;
char          CircleButton5_Caption[2] = "-";
TCircleButton CircleButton6;
TEvent        CircleButton6_OnPress;
char          CircleButton6_Caption[2] = "-";
TCircleButton CircleButton7;
TEvent        CircleButton7_OnPress;
char          CircleButton7_Caption[2] = "+";
TCircleButton CircleButton8;
TEvent        CircleButton8_OnPress;
char          CircleButton8_Caption[2] = "+";
TBox          Box64;
TBox          Box65;
TLabel        Label48;
char          Label48_Caption[6] = "MEDIA";
TLabel        Label49;
char          Label49_Caption[6] = "RELAX";
TLabel        Label50;
char          Label50_Caption[7] = "DINNER";
TLabel        Label51;
char          Label51_Caption[6] = "PARTY";
TLabel        Label52;
char          Label52_Caption[8] = "Dimmer:";
TLabel        Label53;
char          Label53_Caption[13] = "Temperature:";
TLabel        Label88;
char          Label88_Caption[2] = "%";
TLabel        Label89;
char          Label89_Caption[2] = "C";
TLabel        Label97;
char          Label97_Caption[11] = "     0";
TLabel        Label98;
char          Label98_Caption[11] = "    15";

TButton *const code Light_Mode_Buttons[2] = {
  &Button31,            
  &Button32             
};

TLabel *const code Light_Mode_Labels[10] = {
  &Label48,             
  &Label49,             
  &Label50,             
  &Label51,             
  &Label52,             
  &Label53,             
  &Label88,             
  &Label89,             
  &Label97,             
  &Label98              
};

TImage *const code Light_Mode_Images[9] = {
  &Image61,             
  &Image59,             
  &Image58,             
  &Image57,             
  &Image56,             
  &Image55,             
  &Image54,             
  &Image53,             
  &Image52              
};

TCircleButton *const code Light_Mode_CircleButtons[4] = {
  &CircleButton5,       
  &CircleButton6,       
  &CircleButton7,       
  &CircleButton8        
};

TBox *const code Light_Mode_Boxes[15] = {
  &Box61,               
  &Box55,               
  &Box53,               
  &Box52,               
  &Box51,               
  &Box50,               
  &Box56,               
  &Box57,               
  &Box58,               
  &Box59,               
  &Box60,               
  &Box62,               
  &Box63,               
  &Box64,               
  &Box65                
};


TScreen Keyboard_Screen;

TButton   Button37;
char      Button37_Caption[9] = "SETTINGS";
TButton   Button38;
TEvent    Button38_OnClick;
char      Button38_Caption[2] = "<";
TBox      Box11;
TBox      Box24;
TEvent    Box24_OnClick;
TBox      Box76;
TEvent    Box76_OnClick;
TBox      Box77;
TEvent    Box77_OnClick;
TBox      Box78;
TEvent    Box78_OnClick;
TCheckBox CheckBox1;
char      CheckBox1_Caption[1] = "";
TCheckBox CheckBox2;
char      CheckBox2_Caption[1] = "";
TCheckBox CheckBox3;
char      CheckBox3_Caption[1] = "";
TCheckBox CheckBox4;
char      CheckBox4_Caption[1] = "";
TLabel    Label6;
char      Label6_Caption[15] = "Audio feedback";
TLabel    Label26;
char      Label26_Caption[16] = "Haptic feedback";
TLabel    Label27;
char      Label27_Caption[11] = "Power save";
TLabel    Label28;
char      Label28_Caption[10] = "Demo mode";

TButton *const code Keyboard_Screen_Buttons[2] = {
  &Button37,            
  &Button38             
};

TLabel *const code Keyboard_Screen_Labels[4] = {
  &Label6,              
  &Label26,             
  &Label27,             
  &Label28              
};

TBox *const code Keyboard_Screen_Boxes[5] = {
  &Box11,               
  &Box24,               
  &Box76,               
  &Box77,               
  &Box78                
};

TCheckBox *const code Keyboard_Screen_CheckBoxes[4] = {
  &CheckBox1,           
  &CheckBox2,           
  &CheckBox3,           
  &CheckBox4            
};


TScreen System_Log_Screen;

TBox    Box25;
TEvent  Box25_OnClick;
TImage  Image69;
TImage  Image70;
TBox    Box10;
TEvent  Box10_OnClick;
TImage  Image67;
TImage  Image68;
TBox    Box30;
TImage  Image66;
TImage  Image65;
TButton Button12;
TEvent  Button12_OnClick;
char    Button12_Caption[2] = "<";
TBox    Box9;
TButton Button13;
TEvent  Button13_OnClick;
char    Button13_Caption[2] = "1";
TButton Button14;
TEvent  Button14_OnClick;
char    Button14_Caption[2] = "2";
TButton Button15;
TEvent  Button15_OnClick;
char    Button15_Caption[2] = "3";
TButton Button16;
TEvent  Button16_OnClick;
char    Button16_Caption[2] = "4";
TButton Button17;
TEvent  Button17_OnClick;
char    Button17_Caption[2] = "5";
TButton Button18;
TEvent  Button18_OnClick;
char    Button18_Caption[2] = "6";
TButton Button19;
TEvent  Button19_OnClick;
char    Button19_Caption[2] = "7";
TButton Button20;
TEvent  Button20_OnClick;
char    Button20_Caption[2] = "8";
TButton Button21;
TEvent  Button21_OnClick;
char    Button21_Caption[2] = "9";
TButton Button22;
TEvent  Button22_OnClick;
char    Button22_Caption[2] = "0";
TButton Button23;
TEvent  Button23_OnClick;
char    Button23_Caption[2] = "A";
TButton Button24;
TEvent  Button24_OnClick;
char    Button24_Caption[2] = "B";
TButton Button25;
TEvent  Button25_OnClick;
char    Button25_Caption[2] = "C";
TButton Button26;
TEvent  Button26_OnClick;
char    Button26_Caption[2] = "D";
TButton Button27;
TEvent  Button27_OnClick;
char    Button27_Caption[3] = "cl";
TButton Button28;
TEvent  Button28_OnClick;
char    Button28_Caption[3] = "ok";
TButton Button29;
char    Button29_Caption[9] = "SECURITY";
TBox    Box36;
TLabel  Label24;
char    Label24_Caption[13] = " ";
TLabel  Label25;
char    Label25_Caption[15] = "DISARMED";
TLabel  Label54;
char    Label54_Caption[6] = "NIGHT";
TLabel  Label55;
char    Label55_Caption[4] = "DAY";

TButton *const code System_Log_Screen_Buttons[18] = {
  &Button12,            
  &Button13,            
  &Button14,            
  &Button15,            
  &Button16,            
  &Button17,            
  &Button18,            
  &Button19,            
  &Button20,            
  &Button21,            
  &Button22,            
  &Button23,            
  &Button24,            
  &Button25,            
  &Button26,            
  &Button27,            
  &Button28,            
  &Button29             
};

TLabel *const code System_Log_Screen_Labels[4] = {
  &Label24,             
  &Label25,             
  &Label54,             
  &Label55              
};

TImage *const code System_Log_Screen_Images[6] = {
  &Image69,             
  &Image70,             
  &Image67,             
  &Image68,             
  &Image66,             
  &Image65              
};

TBox *const code System_Log_Screen_Boxes[5] = {
  &Box25,               
  &Box10,               
  &Box30,               
  &Box9,                
  &Box36                
};


TScreen Green_Power_Screen;

TButton Button33;
char    Button33_Caption[11] = "SYSTEM LOG";
TButton Button34;
TEvent  Button34_OnClick;
char    Button34_Caption[2] = "<";
TBox    Box26;
TBox    Box66;
TLabel  Label62;
char    Label62_Caption[67] = " ";
TLabel  Label63;
char    Label63_Caption[67] = " ";
TLabel  Label64;
char    Label64_Caption[67] = " ";
TLabel  Label65;
char    Label65_Caption[67] = " ";
TLabel  Label66;
char    Label66_Caption[67] = " ";
TLabel  Label67;
char    Label67_Caption[67] = " ";
TLabel  Label104;
char    Label104_Caption[67] = " ";
TLabel  Label105;
char    Label105_Caption[67] = " ";

TButton *const code Green_Power_Screen_Buttons[2] = {
  &Button33,            
  &Button34             
};

TLabel *const code Green_Power_Screen_Labels[8] = {
  &Label62,             
  &Label63,             
  &Label64,             
  &Label65,             
  &Label66,             
  &Label67,             
  &Label104,            
  &Label105             
};

TBox *const code Green_Power_Screen_Boxes[2] = {
  &Box26,               
  &Box66                
};


TScreen Time_Screen;

TBox    Box67;
TImage  Image63;
TBox    Box74;
TImage  Image62;
TButton Button35;
char    Button35_Caption[12] = "GREEN POWER";
TButton Button36;
TEvent  Button36_OnClick;
char    Button36_Caption[2] = "<";
TBox    Box27;
TBox    Box68;
TBox    Box69;
TBox    Box70;
TBox    Box71;
TBox    Box72;
TBox    Box73;
TBox    Box75;
TLabel  Label56;
char    Label56_Caption[10] = "TIME LEFT";
TLabel  Label30;
char    Label30_Caption[6] = "SOLAR";
TLabel  Label57;
char    Label57_Caption[8] = "BATTERY";
TLabel  Label82;
char    Label82_Caption[8] = "Running";
TLabel  Label83;
char    Label83_Caption[6] = "GREEN";
TLabel  Label84;
char    Label84_Caption[11] = "Energy Now";
TLabel  Label85;
char    Label85_Caption[14] = "Daily Average";
TLabel  Label90;
char    Label90_Caption[3] = "/h";
TLabel  Label91;
char    Label91_Caption[4] = "kWh";
TLabel  Label92;
char    Label92_Caption[5] = "0,30";
TLabel  Label93;
char    Label93_Caption[5] = "8,03";
TLabel  Label94;
char    Label94_Caption[11] = "   03:00";
TLabel  Label99;
char    Label99_Caption[11] = "   25";
TLabel  Label100;
char    Label100_Caption[11] = "   25";
TLabel  Label79;
char    Label79_Caption[2] = "h";
TLabel  Label80;
char    Label80_Caption[2] = "%";
TLabel  Label81;
char    Label81_Caption[2] = "%";

TButton *const code Time_Screen_Buttons[2] = {
  &Button35,            
  &Button36             
};

TLabel *const code Time_Screen_Labels[17] = {
  &Label56,             
  &Label30,             
  &Label57,             
  &Label82,             
  &Label83,             
  &Label84,             
  &Label85,             
  &Label90,             
  &Label91,             
  &Label92,             
  &Label93,             
  &Label94,             
  &Label99,             
  &Label100,            
  &Label79,             
  &Label80,             
  &Label81              
};

TImage *const code Time_Screen_Images[2] = {
  &Image63,             
  &Image62              
};

TBox *const code Time_Screen_Boxes[10] = {
  &Box67,               
  &Box74,               
  &Box27,               
  &Box68,               
  &Box69,               
  &Box70,               
  &Box71,               
  &Box72,               
  &Box73,               
  &Box75                
};


TScreen Settings_Screen;

TBox    Box33;
TLabel  Label34;
char    Label34_Caption[21] = "Hours:  ";
TLabel  Label44;
char    Label44_Caption[21] = "Minutes:  ";
TLabel  Label45;
char    Label45_Caption[21] = "Day:  ";
TLabel  Label71;
char    Label71_Caption[21] = "Month:  ";
TLabel  Label72;
char    Label72_Caption[21] = "Year:  ";
TButton Button48;
TEvent  Button48_OnClick;
char    Button48_Caption[2] = "<";
TBox    Box79;
TButton Button1;
TEvent  Button1_OnClick;
char    Button1_Caption[2] = "1";
TButton Button2;
TEvent  Button2_OnClick;
char    Button2_Caption[2] = "2";
TButton Button3;
TEvent  Button3_OnClick;
char    Button3_Caption[2] = "3";
TButton Button4;
TEvent  Button4_OnClick;
char    Button4_Caption[2] = "4";
TButton Button5;
TEvent  Button5_OnClick;
char    Button5_Caption[2] = "5";
TButton Button6;
TEvent  Button6_OnClick;
char    Button6_Caption[2] = "6";
TButton Button7;
TEvent  Button7_OnClick;
char    Button7_Caption[2] = "7";
TButton Button8;
TEvent  Button8_OnClick;
char    Button8_Caption[2] = "8";
TButton Button9;
TEvent  Button9_OnClick;
char    Button9_Caption[2] = "9";
TButton Button10;
TEvent  Button10_OnClick;
char    Button10_Caption[2] = "0";
TButton Button39;
TEvent  Button39_OnClick;
char    Button39_Caption[6] = "Hours";
TButton Button40;
TEvent  Button40_OnClick;
char    Button40_Caption[8] = "Minutes";
TButton Button41;
TEvent  Button41_OnClick;
char    Button41_Caption[4] = "Day";
TButton Button42;
TEvent  Button42_OnClick;
char    Button42_Caption[6] = "Month";
TButton Button43;
TEvent  Button43_OnClick;
char    Button43_Caption[3] = "cl";
TButton Button44;
TEvent  Button44_OnClick;
char    Button44_Caption[3] = "ok";
TButton Button45;
char    Button45_Caption[5] = "TIME";
TButton Button46;
TEvent  Button46_OnClick;
char    Button46_Caption[5] = "Year";

TButton *const code Settings_Screen_Buttons[19] = {
  &Button48,            
  &Button1,             
  &Button2,             
  &Button3,             
  &Button4,             
  &Button5,             
  &Button6,             
  &Button7,             
  &Button8,             
  &Button9,             
  &Button10,            
  &Button39,            
  &Button40,            
  &Button41,            
  &Button42,            
  &Button43,            
  &Button44,            
  &Button45,            
  &Button46             
};

TLabel *const code Settings_Screen_Labels[5] = {
  &Label34,             
  &Label44,             
  &Label45,             
  &Label71,             
  &Label72              
};

TBox *const code Settings_Screen_Boxes[2] = {
  &Box33,               
  &Box79                
};


static char IsInsideObject(TObjInfo *AObjInfo, unsigned int X, unsigned int Y) {
  TRect  *ptrPressRect = 0;
  TRect  *ptrPressCircle = 0;

  if ((AObjInfo->HitX == X) && (AObjInfo->HitY == Y))
    return 1;

  switch (AObjInfo->Type) {
    // Button
    case VTFT_OT_BUTTON: {
      ptrPressRect = (TRect *)&(((TButton *)AObjInfo->Obj)->Left);
      break;
    }
    // Label
    case VTFT_OT_LABEL: {
      ptrPressRect = (TRect *)&(((TLabel *)AObjInfo->Obj)->Left);
      break;
    }
    // Image
    case VTFT_OT_IMAGE: {
      ptrPressRect = (TRect *)&(((TImage *)AObjInfo->Obj)->Left);
      break;
    }
    // CircleButton
    case VTFT_OT_CIRCLEBUTTON: {
      ptrPressCircle = (TRect *)&(((TCircleButton *)AObjInfo->Obj)->Left);
      break;
    }
    // Box
    case VTFT_OT_BOX: {
      ptrPressRect = (TRect *)&(((TBox *)AObjInfo->Obj)->Left);
      break;
    }
    // CheckBox
    case VTFT_OT_CHECKBOX: {
      ptrPressRect = (TRect *)&(((TCheckBox *)AObjInfo->Obj)->Left);
      break;
    }
  }

  if (ptrPressRect) {
    if ((ptrPressRect->Left <= X) && (ptrPressRect->Left+ptrPressRect->Width-1 >= X) &&
        (ptrPressRect->Top  <= Y) && (ptrPressRect->Top+ptrPressRect->Height-1 >= Y))
      return 1;
  }
  else if (ptrPressCircle) {
    if ((ptrPressCircle->Left <= X) && (ptrPressCircle->Left+ptrPressCircle->Width*2-1 >= X) &&
        (ptrPressCircle->Top  <= Y) && (ptrPressCircle->Top+ptrPressCircle->Width*2-1 >= Y))
      return 1;
  }

  return 0;
}

void DrawButton(TButton *AButton) {
  int textWidth;
  int textHeight;
  
  if (AButton->Visible) {
    if ((VTFT_OT_BUTTON == TouchS.ActObjInfo.Type) && (AButton == (TButton *)TouchS.ActObjInfo.Obj)) {
      if (AButton->Gradient != _FT812_BRUSH_GR_NONE) {
        FT812_Canvas_BrushGradient(_FT812_BRUSH_STYLE_SOLID, AButton->Gradient, AButton->Press_Color, AButton->Press_ColorTo, AButton->Opacity);
      }
      else {
        FT812_Canvas_BrushSingleColor(_FT812_BRUSH_STYLE_SOLID, AButton->Press_Color, AButton->Opacity);
      }
    }
    else {
      if (AButton->Gradient != _FT812_BRUSH_GR_NONE) {
        FT812_Canvas_BrushGradient(_FT812_BRUSH_STYLE_SOLID, AButton->Gradient, AButton->Color, AButton->ColorTo, AButton->Opacity);
      }
      else {
        FT812_Canvas_BrushSingleColor(_FT812_BRUSH_STYLE_SOLID, AButton->Color, AButton->Opacity);
      }
    }

    FT812_Canvas_Pen(AButton->Pen_Width, AButton->Pen_Color, AButton->Opacity);

    if (AButton->Tag)
      FT812_Canvas_Tag(AButton->Tag);

    FT812_Screen_Box(AButton->Left, AButton->Top, AButton->Left+AButton->Width-1, AButton->Top+AButton->Height-1);
    if (AButton->FontHandle >= 16)
      FT812_Canvas_FontSystem(AButton->FontHandle, AButton->Font_Color, AButton->Opacity);
    else
      FT812_Canvas_FontExt(AButton->FontHandle, AButton->FontName, AButton->Source, AButton->Font_Color, AButton->Opacity);
    
    textWidth  = FT812_Canvas_TextWidth(AButton->Caption);
    textHeight = FT812_Canvas_TextHeight();
    if (AButton->TextAlign == taLeft)
      FT812_Screen_TextPos(AButton->Left+(AButton->Pen_Width)+1, AButton->Top+(AButton->Height-textHeight)/2, AButton->Caption);
    else if (AButton->TextAlign == taCenter)
      FT812_Screen_TextPos(AButton->Left+(AButton->Width-textWidth)/2, AButton->Top+(AButton->Height-textHeight)/2, AButton->Caption);
    else if (AButton->TextAlign == taRight)
      FT812_Screen_TextPos(AButton->Left+AButton->Width-textWidth-1, AButton->Top+(AButton->Height-textHeight)/2, AButton->Caption);
  }
}

void DrawLabel(TLabel *ALabel) {
  if (ALabel->Visible) {
    if (ALabel->FontHandle >= 16)
      FT812_Canvas_FontSystem(ALabel->FontHandle, ALabel->Font_Color, ALabel->Opacity);
    else
      FT812_Canvas_FontExt(ALabel->FontHandle, ALabel->FontName, ALabel->Source, ALabel->Font_Color, ALabel->Opacity);

    if (ALabel->Tag)
      FT812_Canvas_Tag(ALabel->Tag);

    FT812_Screen_TextPos(ALabel->Left, ALabel->Top, ALabel->Caption);
  }
}

void DrawImage(TImage *AImage) {
  TFT812BmpConfig bmpCfg;
  TFT812Rect currRect;
  TFT812Point rotationCenter;
  char format;
  unsigned int width, height;

  if (AImage->Visible) {
    if (AImage->Tag)
      FT812_Canvas_Tag(AImage->Tag);

    if (AImage->Picture_Type != _FT812_BITMAP_FORMAT_JPEG) {
      FT812_Canvas_BmpGetInfoExt(AImage->Picture_Name, &format, &width, &height);
      FT812_Canvas_BmpParamsToCfg(AImage->Source, width, height, format, &bmpCfg);
      FT812_Canvas_Bitmap(0, &bmpCfg, AImage->Blend_Color, AImage->Opacity);
      FT812_Canvas_PixelFormat12p4();
      FT812_Canvas_CurrRectSet(FT812_Canvas_IntTo12p4(AImage->Left), FT812_Canvas_IntTo12p4(AImage->Top), FT812_Canvas_IntTo12p4(width), FT812_Canvas_IntTo12p4(height));
      FT812_Canvas_Transform_Identity();
      rotationCenter.X = FT812_Canvas_IntTo12p4(AImage->RotationCenterLeft);
      rotationCenter.Y = FT812_Canvas_IntTo12p4(AImage->RotationCenterTop);
      FT812_Canvas_Transform_RotateAndScale(AImage->RotationAngle, &rotationCenter, AImage->ScaleX * 65536, AImage->ScaleY * 65536);
      FT812_Canvas_Transform_Set();
      FT812_Canvas_CurrRectGet(&currRect.Left, &currRect.Top, &currRect.Width, &currRect.Height);

      bmpCfg.Size.Width  = FT812_Canvas_12p4ToInt(currRect.Width);
      bmpCfg.Size.Height = FT812_Canvas_12p4ToInt(currRect.Height);
      FT812_Canvas_Bitmap(0, &bmpCfg, AImage->Blend_Color, AImage->Opacity);
      FT812_Screen_Bitmap(currRect.Left, currRect.Top);
      FT812_Canvas_Transform_Identity();
      FT812_Canvas_Transform_Set();
      FT812_Canvas_PixelFormatInt();
    }
  }
}

void DrawCircleButton(TCircleButton *ACircleButton) {
  int textWidth;
  int textHeight;
  
  if (ACircleButton->Visible) {
    if ((VTFT_OT_CIRCLEBUTTON == TouchS.ActObjInfo.Type) && (ACircleButton == (TCircleButton *)TouchS.ActObjInfo.Obj))
      FT812_Canvas_BrushSingleColor(_FT812_BRUSH_STYLE_SOLID, ACircleButton->Press_Color, ACircleButton->Opacity);
    else 
      FT812_Canvas_BrushSingleColor(_FT812_BRUSH_STYLE_SOLID, ACircleButton->Color, ACircleButton->Opacity);

    FT812_Canvas_Pen(ACircleButton->Pen_Width, ACircleButton->Pen_Color, ACircleButton->Opacity);

    if (ACircleButton->Tag)
      FT812_Canvas_Tag(ACircleButton->Tag);

    FT812_Screen_Circle(ACircleButton->Left+ACircleButton->Radius, 
                      ACircleButton->Top+ACircleButton->Radius, 
                      ACircleButton->Radius);
    if (ACircleButton->FontHandle >= 16)
      FT812_Canvas_FontSystem(ACircleButton->FontHandle, ACircleButton->Font_Color, ACircleButton->Opacity);
    else
      FT812_Canvas_FontExt(ACircleButton->FontHandle, ACircleButton->FontName, ACircleButton->Source, ACircleButton->Font_Color, ACircleButton->Opacity);
    
    textWidth  = FT812_Canvas_TextWidth(ACircleButton->Caption);
    textHeight = FT812_Canvas_TextHeight();
    if (ACircleButton->TextAlign == taLeft)
      FT812_Screen_TextPos(ACircleButton->Left+ACircleButton->Pen_Width+1, ACircleButton->Top+ACircleButton->Radius-textHeight/2, ACircleButton->Caption);
    else if (ACircleButton->TextAlign == taCenter)
      FT812_Screen_TextPos(ACircleButton->Left+ACircleButton->Radius-textWidth/2, ACircleButton->Top+ACircleButton->Radius-textHeight/2, ACircleButton->Caption);
    else if (ACircleButton->TextAlign == taRight)
      FT812_Screen_TextPos(ACircleButton->Left+ACircleButton->Radius*2-textWidth-1, ACircleButton->Top+ACircleButton->Radius-textHeight/2, ACircleButton->Caption);
  }
}

void DrawBox(TBox *ABox) {
  if (ABox->Visible) {
    if ((VTFT_OT_BOX == TouchS.ActObjInfo.Type) && (ABox == (TBox *)TouchS.ActObjInfo.Obj)) {
      if (ABox->Gradient != _FT812_BRUSH_GR_NONE) {
        FT812_Canvas_BrushGradient(_FT812_BRUSH_STYLE_SOLID, ABox->Gradient, ABox->Press_Color, ABox->Press_ColorTo, ABox->Opacity);
      }
      else {
        FT812_Canvas_BrushSingleColor(_FT812_BRUSH_STYLE_SOLID, ABox->Press_Color, ABox->Opacity);
      }
    }
    else {
      if (ABox->Gradient != _FT812_BRUSH_GR_NONE) {
        FT812_Canvas_BrushGradient(_FT812_BRUSH_STYLE_SOLID, ABox->Gradient, ABox->Color, ABox->ColorTo, ABox->Opacity);
      }
      else {
        FT812_Canvas_BrushSingleColor(_FT812_BRUSH_STYLE_SOLID, ABox->Color, ABox->Opacity);
      }
    }

    FT812_Canvas_Pen(ABox->Pen_Width, ABox->Pen_Color, ABox->Opacity);

    if (ABox->Tag)
      FT812_Canvas_Tag(ABox->Tag);

    FT812_Screen_Box(ABox->Left, ABox->Top, ABox->Left+ABox->Width-1, ABox->Top+ABox->Height-1);
  }
}

void DrawCheckBox(TCheckBox *ACheckBox) {
  unsigned char textHeight;
  unsigned int  textLeft;
  unsigned int  textTop;
  unsigned int  rectLeft;
  unsigned int  rectTop;
  unsigned int  checkOffset;
  
  if (ACheckBox->Visible) {
    if ((VTFT_OT_CHECKBOX == TouchS.ActObjInfo.Type) && (ACheckBox == (TCheckBox *)TouchS.ActObjInfo.Obj))
      FT812_Canvas_BrushSingleColor(_FT812_BRUSH_STYLE_SOLID, ACheckBox->Press_Color, ACheckBox->Opacity);
    else 
      FT812_Canvas_BrushSingleColor(_FT812_BRUSH_STYLE_SOLID, ACheckBox->Color, ACheckBox->Opacity);

    FT812_Canvas_Pen(ACheckBox->Pen_Width, ACheckBox->Pen_Color, ACheckBox->Opacity);

    if (ACheckBox->FontHandle >= 16)
      FT812_Canvas_FontSystem(ACheckBox->FontHandle, ACheckBox->Font_Color, ACheckBox->Opacity);
    else
      FT812_Canvas_FontExt(ACheckBox->FontHandle, ACheckBox->FontName, ACheckBox->Source, ACheckBox->Font_Color, ACheckBox->Opacity);

    if (ACheckBox->Tag)
      FT812_Canvas_Tag(ACheckBox->Tag);

    textHeight  = FT812_Canvas_TextHeight();
    textTop     = ACheckBox->Top+(ACheckBox->Height-textHeight)/2;
    rectTop     = ACheckBox->Top;
    checkOffset = ACheckBox->Height/5;

    if (ACheckBox->TextAlign == taLeft) {
      textLeft = ACheckBox->Left+ACheckBox->Height+4;
      rectLeft = ACheckBox->Left;
    }
    else if (ACheckBox->TextAlign == taRight) {
      textLeft = ACheckBox->Left;
      rectLeft = ACheckBox->Left+ACheckBox->Width-ACheckBox->Height;
    }

    FT812_Screen_BoxRound(rectLeft, rectTop, rectLeft+ACheckBox->Height-1, rectTop+ACheckBox->Height-1, ACheckBox->Corner_Radius);
    if (ACheckBox->Checked) {
      FT812_Canvas_Pen(ACheckBox->Height/8, ACheckBox->Pen_Color, ACheckBox->Opacity);
      FT812_Screen_Line(rectLeft+checkOffset+1, rectTop+ACheckBox->Height/2, rectLeft+ACheckBox->Height/2, rectTop+ACheckBox->Height-checkOffset-1);
      FT812_Screen_Line(rectLeft+ACheckBox->Height/2, rectTop+ACheckBox->Height-checkOffset-1, rectLeft+ACheckBox->Height-checkOffset-1, rectTop+checkOffset+1);
    }

    FT812_Screen_TextPos(textLeft, textTop, ACheckBox->Caption);
  }
}

void SetIdenticalImageSources(TPointer APictureName, unsigned long ASource) {
  char i;
  TImage *pImage;
  void *const code *ptrO;

  // Image
  i    = CurrentScreen->ImagesCount;
  ptrO = CurrentScreen->Images;
  while (i--) {
    pImage = (TImage *)(*ptrO);
    if (pImage->Picture_Name == APictureName)
      pImage->Source = ASource;
    ptrO++;
  }
}

static void ClearDynObjSource() {
  char i;
  TImage *pImage;
  void *const code *ptrO;

  // Image
  i    = CurrentScreen->ImagesCount;
  ptrO = CurrentScreen->Images;
  while (i--) {
    pImage = (TImage *)(*ptrO);
    pImage->Source = 0xFFFFFFFF;
    ptrO++;
  }
}

void LoadCurrentScreenResToGRAM(char loadOptions) {
  char i;
  long currSource = -1;
  long tmpDynResStart;
  TImage *pImage;
  void *const code *ptrO;

  // clear dynamic resource addresses first, if necessary
  if (loadOptions & VTFT_LOAD_RES_DYNAMIC)
    ClearDynObjSource();

  // dynamic resources allocation
  if (loadOptions & VTFT_LOAD_RES_DYNAMIC) {

    tmpDynResStart = CurrentScreen->DynResStart;
    if (FT812_RES_BeginLoad(tmpDynResStart)) {
      return;
    }

    // Image
    i    = CurrentScreen->ImagesCount;
    ptrO = CurrentScreen->Images;
    while (i--) {
      pImage = (TImage *)(*ptrO);
      if (pImage->Source == 0xFFFFFFFF) {
        currSource = FT812_Res_LoadBitmapExt(pImage->Picture_Name);
        pImage->Source = currSource;
        SetIdenticalImageSources(pImage->Picture_Name, currSource);
      }
      ptrO++;
    }

    FT812_RES_EndLoad();
  }
}

void DrawObject(TPointer aObj, char aObjType) {
  TDrawHandler drawHandler;

  drawHandler = DrawHandlerTable[aObjType];
  if (drawHandler)
    drawHandler(aObj);
}

void DrawScreenO(TScreen *aScreen, char aOptions) {
  unsigned short cOrder, saveOrder;
  signed   int   actObjOrder;
  unsigned short pwmDuty;
  // counter variables
  char cntButton;
  char cntLabel;
  char cntImage;
  char cntCircleButton;
  char cntBox;
  char cntCheckBox;
  // pointer variables
  TButton       *const code *pButton;
  TLabel        *const code *pLabel;
  TImage        *const code *pImage;
  TCircleButton *const code *pCircleButton;
  TBox          *const code *pBox;
  TCheckBox     *const code *pCheckBox;

  // process screen switching effects
  if (aOptions & VTFT_DISPLAY_EFF_LIGHTS_FADE) {
    FT812_PWM_Get(0, &pwmDuty);
    FT812_PWM_FadeOut(pwmDuty, 0, pwmDuty/32? pwmDuty/32 : 1, 1);
  }
  else if (aOptions & VTFT_DISPLAY_EFF_LIGHTS_OFF) {
    FT812_PWM_Get(0, &pwmDuty);
    FT812_PWM_Duty(0);
  }

  if (CurrentScreen != aScreen) {
    // clear active object when drawing to new screen
    memset(&TouchS.ActObjInfo, 0, sizeof(TObjInfo));
  }

  CurrentScreen = aScreen;

  LoadCurrentScreenResToGRAM(aOptions);

  // init counter variables
  cntButton       = CurrentScreen->ButtonsCount;
  cntLabel        = CurrentScreen->LabelsCount;
  cntImage        = CurrentScreen->ImagesCount;
  cntCircleButton = CurrentScreen->CircleButtonsCount;
  cntBox          = CurrentScreen->BoxesCount;
  cntCheckBox     = CurrentScreen->CheckBoxesCount;
  // init pointer variables
  pButton       = CurrentScreen->Buttons;
  pLabel        = CurrentScreen->Labels;
  pImage        = CurrentScreen->Images;
  pCircleButton = CurrentScreen->CircleButtons;
  pBox          = CurrentScreen->Boxes;
  pCheckBox     = CurrentScreen->CheckBoxes;

  FT812_Screen_BeginUpdateCP();

  if ((FT812_Controller.Display.Width != CurrentScreen->Width) ||
      (FT812_Controller.Display.Height != CurrentScreen->Height)) {
    FT812_Controller.Display.Width = CurrentScreen->Width;
    FT812_Controller.Display.Height = CurrentScreen->Height;
    FT812_Canvas_UnClip();

    if (FT812_Controller.Display.Width > FT812_Controller.Display.Height) {
      FT812_CP_CmdSetRotate(VTFT_FT812_CONFIG_DISPLAY.Rotate180 % 2);
    }
    else {
      FT812_CP_CmdSetRotate((VTFT_FT812_CONFIG_DISPLAY.Rotate180 % 2) + 2);
    }
  }

  FT812_Canvas_BrushSingleColor(_FT812_BRUSH_STYLE_SOLID, CurrentScreen->Color, 255);
  FT812_Canvas_Tag(0);
  FT812_Screen_Clear(_FT812_CLEAR_ALL);
  FT812_CP_CmdStop();

  actObjOrder = -1;
  if (TouchS.ActObjInfo.Obj)
    if (TouchS.ActObjInfo.Flags & VTFT_INT_BRING_TO_FRONT)
      actObjOrder = TouchS.ActObjInfo.Order;

  cOrder = 0;
  while (cOrder < CurrentScreen->ObjectsCount) {
    saveOrder = cOrder;
    if (pButton) {
      while ((*pButton)->Order == cOrder) {
        if (actObjOrder != cOrder) // draw pressed object at the end
          DrawButton(*pButton);
        cOrder++;
        pButton++;
        cntButton--;
        if (!cntButton) {
          pButton = 0;
          break;
        }
      }
      if (saveOrder != cOrder)
        continue;
    }

    if (pLabel) {
      while ((*pLabel)->Order == cOrder) {
        if (actObjOrder != cOrder) // draw pressed object at the end
          DrawLabel(*pLabel);
        cOrder++;
        pLabel++;
        cntLabel--;
        if (!cntLabel) {
          pLabel = 0;
          break;
        }
      }
      if (saveOrder != cOrder)
        continue;
    }

    if (pImage) {
      while ((*pImage)->Order == cOrder) {
        if (actObjOrder != cOrder) // draw pressed object at the end
          DrawImage(*pImage);
        cOrder++;
        pImage++;
        cntImage--;
        if (!cntImage) {
          pImage = 0;
          break;
        }
      }
      if (saveOrder != cOrder)
        continue;
    }

    if (pCircleButton) {
      while ((*pCircleButton)->Order == cOrder) {
        if (actObjOrder != cOrder) // draw pressed object at the end
          DrawCircleButton(*pCircleButton);
        cOrder++;
        pCircleButton++;
        cntCircleButton--;
        if (!cntCircleButton) {
          pCircleButton = 0;
          break;
        }
      }
      if (saveOrder != cOrder)
        continue;
    }

    if (pBox) {
      while ((*pBox)->Order == cOrder) {
        if (actObjOrder != cOrder) // draw pressed object at the end
          DrawBox(*pBox);
        cOrder++;
        pBox++;
        cntBox--;
        if (!cntBox) {
          pBox = 0;
          break;
        }
      }
      if (saveOrder != cOrder)
        continue;
    }

    if (pCheckBox) {
      while ((*pCheckBox)->Order == cOrder) {
        if (actObjOrder != cOrder) // draw pressed object at the end
          DrawCheckBox(*pCheckBox);
        cOrder++;
        pCheckBox++;
        cntCheckBox--;
        if (!cntCheckBox) {
          pCheckBox = 0;
          break;
        }
      }
      if (saveOrder != cOrder)
        continue;
    }

    cOrder++;
  }

  // draw pressed object now
  if (TouchS.ActObjInfo.Obj)
    DrawObject(TouchS.ActObjInfo.Obj, TouchS.ActObjInfo.Type);

  FT812_Screen_EndUpdate();
  FT812_Screen_Show();

  // process screen switching effects
  if (aOptions & VTFT_DISPLAY_EFF_LIGHTS_FADE) {
    FT812_PWM_FadeIn(0, pwmDuty, 1, 3);
  }
  else if (aOptions & VTFT_DISPLAY_EFF_LIGHTS_OFF) {
    FT812_PWM_Duty(pwmDuty);
  }

}

void DrawScreen(TScreen *aScreen) {
  if (aScreen != CurrentScreen)
    DrawScreenO(aScreen, VTFT_LOAD_RES_ALL | VTFT_DISPLAY_EFF_LIGHTS_FADE);
  else
    DrawScreenO(aScreen, VTFT_LOAD_RES_NONE);
}

char GetActiveObjectByXY(int X, int Y, TObjInfo *AObjInfo) {
  char i;
  int  hiOrder;
  TButton       *pButton;
  TLabel        *pLabel;
  TImage        *pImage;
  TCircleButton *pCircleButton;
  TBox          *pBox;
  TCheckBox     *pCheckBox;
  void *const code *ptrO;

  // clear current object info
  memset(AObjInfo, 0, sizeof(TObjInfo));

  // Find object with highest order at specified position.
  // Objects lists are sorted by object order ascending.
  hiOrder = -1;

  // Button
  i    = CurrentScreen->ButtonsCount;
  ptrO = CurrentScreen->Buttons+CurrentScreen->ButtonsCount-1;
  while (i--) {
    pButton = (TButton *)(*ptrO);
    if (pButton->Order < hiOrder)
      break;
    if (pButton->Active) {
      if ((pButton->Left <= X) && (pButton->Left+pButton->Width-1 >= X) &&
          (pButton->Top  <= Y) && (pButton->Top+pButton->Height-1 >= Y)) {
        AObjInfo->Obj   = (void *)pButton;
        AObjInfo->Type  = VTFT_OT_BUTTON;
        AObjInfo->Order = pButton->Order;
        AObjInfo->Flags = VTFT_INT_BRING_TO_FRONT;
        if ((pButton->Press_Color != pButton->Color) ||
            ((pButton->Gradient != _FT812_BRUSH_GR_NONE) &&
             (pButton->Press_ColorTo != pButton->ColorTo)))
          AObjInfo->Flags |= VTFT_INT_REPAINT_ON_DOWN | VTFT_INT_REPAINT_ON_UP;

        hiOrder         = pButton->Order;

        break;
      }
    }
    ptrO--;
  }

  // Label
  i    = CurrentScreen->LabelsCount;
  ptrO = CurrentScreen->Labels+CurrentScreen->LabelsCount-1;
  while (i--) {
    pLabel = (TLabel *)(*ptrO);
    if (pLabel->Order < hiOrder)
      break;
    if (pLabel->Active) {
      if ((pLabel->Left <= X) && (pLabel->Left+pLabel->Width-1 >= X) &&
          (pLabel->Top  <= Y) && (pLabel->Top+pLabel->Height-1 >= Y)) {
        AObjInfo->Obj   = (void *)pLabel;
        AObjInfo->Type  = VTFT_OT_LABEL;
        AObjInfo->Order = pLabel->Order;
        AObjInfo->Flags = VTFT_INT_BRING_TO_FRONT;

        hiOrder         = pLabel->Order;

        break;
      }
    }
    ptrO--;
  }

  // Image
  i    = CurrentScreen->ImagesCount;
  ptrO = CurrentScreen->Images+CurrentScreen->ImagesCount-1;
  while (i--) {
    pImage = (TImage *)(*ptrO);
    if (pImage->Order < hiOrder)
      break;
    if (pImage->Active) {
      if ((pImage->Left <= X) && (pImage->Left+pImage->Width-1 >= X) &&
          (pImage->Top  <= Y) && (pImage->Top+pImage->Height-1 >= Y)) {
        AObjInfo->Obj   = (void *)pImage;
        AObjInfo->Type  = VTFT_OT_IMAGE;
        AObjInfo->Order = pImage->Order;
        AObjInfo->Flags = VTFT_INT_BRING_TO_FRONT;

        hiOrder         = pImage->Order;

        break;
      }
    }
    ptrO--;
  }

  // CircleButton
  i    = CurrentScreen->CircleButtonsCount;
  ptrO = CurrentScreen->CircleButtons+CurrentScreen->CircleButtonsCount-1;
  while (i--) {
    pCircleButton = (TCircleButton *)(*ptrO);
    if (pCircleButton->Order < hiOrder)
      break;
    if (pCircleButton->Active) {
      // for circle objects common object width is its radius property
      if ((pCircleButton->Left <= X) && (pCircleButton->Left+pCircleButton->Radius*2-1 >= X) &&
          (pCircleButton->Top  <= Y) && (pCircleButton->Top+pCircleButton->Radius*2-1 >= Y)) {
        AObjInfo->Obj   = (void *)pCircleButton;
        AObjInfo->Type  = VTFT_OT_CIRCLEBUTTON;
        AObjInfo->Order = pCircleButton->Order;
        AObjInfo->Flags = VTFT_INT_BRING_TO_FRONT;
        if (pCircleButton->Press_Color != pCircleButton->Color)
          AObjInfo->Flags |= VTFT_INT_REPAINT_ON_DOWN | VTFT_INT_REPAINT_ON_UP;

        hiOrder         = pCircleButton->Order;

        break;
      }
    }
    ptrO--;
  }

  // Box
  i    = CurrentScreen->BoxesCount;
  ptrO = CurrentScreen->Boxes+CurrentScreen->BoxesCount-1;
  while (i--) {
    pBox = (TBox *)(*ptrO);
    if (pBox->Order < hiOrder)
      break;
    if (pBox->Active) {
      if ((pBox->Left <= X) && (pBox->Left+pBox->Width-1 >= X) &&
          (pBox->Top  <= Y) && (pBox->Top+pBox->Height-1 >= Y)) {
        AObjInfo->Obj   = (void *)pBox;
        AObjInfo->Type  = VTFT_OT_BOX;
        AObjInfo->Order = pBox->Order;
        AObjInfo->Flags = VTFT_INT_BRING_TO_FRONT;
        if ((pBox->Press_Color != pBox->Color) ||
            ((pBox->Gradient != _FT812_BRUSH_GR_NONE) &&
             (pBox->Press_ColorTo != pBox->ColorTo)))
          AObjInfo->Flags |= VTFT_INT_REPAINT_ON_DOWN | VTFT_INT_REPAINT_ON_UP;

        hiOrder         = pBox->Order;

        break;
      }
    }
    ptrO--;
  }

  // CheckBox
  i    = CurrentScreen->CheckBoxesCount;
  ptrO = CurrentScreen->CheckBoxes+CurrentScreen->CheckBoxesCount-1;
  while (i--) {
    pCheckBox = (TCheckBox *)(*ptrO);
    if (pCheckBox->Order < hiOrder)
      break;
    if (pCheckBox->Active) {
      if ((pCheckBox->Left <= X) && (pCheckBox->Left+pCheckBox->Width-1 >= X) &&
          (pCheckBox->Top  <= Y) && (pCheckBox->Top+pCheckBox->Height-1 >= Y)) {
        AObjInfo->Obj   = (void *)pCheckBox;
        AObjInfo->Type  = VTFT_OT_CHECKBOX;
        AObjInfo->Order = pCheckBox->Order;
        AObjInfo->Flags = VTFT_INT_BRING_TO_FRONT;
        AObjInfo->Flags |= VTFT_INT_INTRINSIC_CLICK_EFF;
        if (pCheckBox->Press_Color != pCheckBox->Color)
          AObjInfo->Flags |= VTFT_INT_REPAINT_ON_DOWN | VTFT_INT_REPAINT_ON_UP;

        hiOrder         = pCheckBox->Order;

        break;
      }
    }
    ptrO--;
  }

  if (AObjInfo->Obj) {
    AObjInfo->HitX = X;
    AObjInfo->HitY = Y;
    return 1;
  }
  else {
    return 0;
  }
}

char GetActiveObjectByTag(char ATag, TObjInfo *AObjInfo) {
  char i;
  TButton       *pButton;
  TLabel        *pLabel;
  TImage        *pImage;
  TCircleButton *pCircleButton;
  TBox          *pBox;
  TCheckBox     *pCheckBox;
  void *const code *ptrO;

  // clear current object info
  memset(AObjInfo, 0, sizeof(TObjInfo));

  // Find object with specified tag value.

  // Button
  i    = CurrentScreen->ButtonsCount;
  ptrO = CurrentScreen->Buttons+CurrentScreen->ButtonsCount-1;
  while (i--) {
    pButton = (TButton *)(*ptrO);
    if (pButton->Tag == ATag) {
      if (pButton->Active) {
        AObjInfo->Obj   = (void *)pButton;
        AObjInfo->Type  = VTFT_OT_BUTTON;
        AObjInfo->Order = pButton->Order;
        AObjInfo->Flags = VTFT_INT_BRING_TO_FRONT;
        if ((pButton->Press_Color != pButton->Color) ||
            ((pButton->Gradient != _FT812_BRUSH_GR_NONE) &&
             (pButton->Press_ColorTo != pButton->ColorTo)))
          AObjInfo->Flags |= VTFT_INT_REPAINT_ON_DOWN | VTFT_INT_REPAINT_ON_UP;

      }
      break;
    }
    ptrO--;
  }

  // Label
  i    = CurrentScreen->LabelsCount;
  ptrO = CurrentScreen->Labels+CurrentScreen->LabelsCount-1;
  while (i--) {
    pLabel = (TLabel *)(*ptrO);
    if (pLabel->Tag == ATag) {
      if (pLabel->Active) {
        AObjInfo->Obj   = (void *)pLabel;
        AObjInfo->Type  = VTFT_OT_LABEL;
        AObjInfo->Order = pLabel->Order;
        AObjInfo->Flags = VTFT_INT_BRING_TO_FRONT;
      }
      break;
    }
    ptrO--;
  }

  // Image
  i    = CurrentScreen->ImagesCount;
  ptrO = CurrentScreen->Images+CurrentScreen->ImagesCount-1;
  while (i--) {
    pImage = (TImage *)(*ptrO);
    if (pImage->Tag == ATag) {
      if (pImage->Active) {
        AObjInfo->Obj   = (void *)pImage;
        AObjInfo->Type  = VTFT_OT_IMAGE;
        AObjInfo->Order = pImage->Order;
        AObjInfo->Flags = VTFT_INT_BRING_TO_FRONT;
      }
      break;
    }
    ptrO--;
  }

  // CircleButton
  i    = CurrentScreen->CircleButtonsCount;
  ptrO = CurrentScreen->CircleButtons+CurrentScreen->CircleButtonsCount-1;
  while (i--) {
    pCircleButton = (TCircleButton *)(*ptrO);
    if (pCircleButton->Tag == ATag) {
      if (pCircleButton->Active) {
        AObjInfo->Obj   = (void *)pCircleButton;
        AObjInfo->Type  = VTFT_OT_CIRCLEBUTTON;
        AObjInfo->Order = pCircleButton->Order;
        AObjInfo->Flags = VTFT_INT_BRING_TO_FRONT;
        if (pCircleButton->Press_Color != pCircleButton->Color)
          AObjInfo->Flags |= VTFT_INT_REPAINT_ON_DOWN | VTFT_INT_REPAINT_ON_UP;

      }
      break;
    }
    ptrO--;
  }

  // Box
  i    = CurrentScreen->BoxesCount;
  ptrO = CurrentScreen->Boxes+CurrentScreen->BoxesCount-1;
  while (i--) {
    pBox = (TBox *)(*ptrO);
    if (pBox->Tag == ATag) {
      if (pBox->Active) {
        AObjInfo->Obj   = (void *)pBox;
        AObjInfo->Type  = VTFT_OT_BOX;
        AObjInfo->Order = pBox->Order;
        AObjInfo->Flags = VTFT_INT_BRING_TO_FRONT;
        if ((pBox->Press_Color != pBox->Color) ||
            ((pBox->Gradient != _FT812_BRUSH_GR_NONE) &&
             (pBox->Press_ColorTo != pBox->ColorTo)))
          AObjInfo->Flags |= VTFT_INT_REPAINT_ON_DOWN | VTFT_INT_REPAINT_ON_UP;

      }
      break;
    }
    ptrO--;
  }

  // CheckBox
  i    = CurrentScreen->CheckBoxesCount;
  ptrO = CurrentScreen->CheckBoxes+CurrentScreen->CheckBoxesCount-1;
  while (i--) {
    pCheckBox = (TCheckBox *)(*ptrO);
    if (pCheckBox->Tag == ATag) {
      if (pCheckBox->Active) {
        AObjInfo->Obj   = (void *)pCheckBox;
        AObjInfo->Type  = VTFT_OT_CHECKBOX;
        AObjInfo->Order = pCheckBox->Order;
        AObjInfo->Flags = VTFT_INT_BRING_TO_FRONT;
        AObjInfo->Flags |= VTFT_INT_INTRINSIC_CLICK_EFF;
        if (pCheckBox->Press_Color != pCheckBox->Color)
          AObjInfo->Flags |= VTFT_INT_REPAINT_ON_DOWN | VTFT_INT_REPAINT_ON_UP;

      }
      break;
    }
    ptrO--;
  }

  if (AObjInfo->Obj) {
    AObjInfo->HitTag = ATag;
    return 1;
  }
  else {
    return 0;
  }
}

static void ProcessEvent(TEvent *pEvent) {
  if (pEvent) {
    if (pEvent->Sound.SndAct == VTFT_SNDACT_PLAY) 
      FT812_Sound_SetAndPlay(pEvent->Sound.Effect, pEvent->Sound.Pitch, pEvent->Sound.Volume);
    else if (pEvent->Sound.SndAct == VTFT_SNDACT_STOP) 
      FT812_Sound_Stop();
    if (pEvent->Action) 
      pEvent->Action();
  }
}

static void ProcessCEvent(TCEvent *pEventC) {
  if (pEventC) {
    if (pEventC->Sound.SndAct == VTFT_SNDACT_PLAY) 
      FT812_Sound_SetAndPlay(pEventC->Sound.Effect, pEventC->Sound.Pitch, pEventC->Sound.Volume);
    else if (pEventC->Sound.SndAct == VTFT_SNDACT_STOP) 
      FT812_Sound_Stop();
    if (pEventC->Action) 
      pEventC->Action();
  }
}

static void OnEvent(TObjInfo *AObjInfo, char AEventType){
  TEvent **ppEvent;
  TEvent  *pEvent = 0;

  switch (AObjInfo->Type) {
    // Button
    case VTFT_OT_BUTTON: {
      ppEvent = &(((TButton *)AObjInfo->Obj)->OnUp);
      pEvent  = ppEvent[AEventType];
      break;
    }
    // Label
    case VTFT_OT_LABEL: {
      ppEvent = &(((TLabel *)AObjInfo->Obj)->OnUp);
      pEvent  = ppEvent[AEventType];
      break;
    }
    // Image
    case VTFT_OT_IMAGE: {
      ppEvent = &(((TImage *)AObjInfo->Obj)->OnUp);
      pEvent  = ppEvent[AEventType];
      break;
    }
    // CircleButton
    case VTFT_OT_CIRCLEBUTTON: {
      ppEvent = &(((TCircleButton *)AObjInfo->Obj)->OnUp);
      pEvent  = ppEvent[AEventType];
      break;
    }
    // Box
    case VTFT_OT_BOX: {
      ppEvent = &(((TBox *)AObjInfo->Obj)->OnUp);
      pEvent  = ppEvent[AEventType];
      break;
    }
    // CheckBox
    case VTFT_OT_CHECKBOX: {
      ppEvent = &(((TCheckBox *)AObjInfo->Obj)->OnUp);
      pEvent  = ppEvent[AEventType];
      break;
    }
  } // end switch

  if (pEvent) {
    ProcessEvent(pEvent);
  }
}

static void ProcessIntrinsicClickEffects(TObjInfo *AObjInfo) {

  if (!(AObjInfo->Flags & VTFT_INT_INTRINSIC_CLICK_EFF))
    return;

  switch (AObjInfo->Type) {
    case VTFT_OT_CHECKBOX:
    {
      ((TCheckBox *)AObjInfo->Obj)->Checked ^= 1;
      break;
    }
  }
}

static void Process_TP_Press() {
  // Screen Event
  if (CurrentScreen->Active)
    if ((CurrentScreen->SniffObjectEvents) || (!TouchS.ActObjInfo.Obj))
      ProcessEvent(CurrentScreen->OnPress);

  // Object Event
  if (!TouchS.ActObjInfo.Obj)
    return;

  OnEvent(&TouchS.ActObjInfo, VTFT_EVT_PRESS);
}

static void Process_TP_Up() {
  char     isClick;
  TObjInfo actObj;

  // Screen Event
  if (CurrentScreen->Active)
    if ((CurrentScreen->SniffObjectEvents) || (!TouchS.ActObjInfo.Obj))
      ProcessEvent(CurrentScreen->OnUp);

  actObj = TouchS.ActObjInfo;
  // Cler active object info
  memset(&TouchS.ActObjInfo, 0, sizeof(TObjInfo));

  // Object Event
  if (!actObj.Obj)
    return;

  isClick = IsInsideObject(&actObj, TouchS.X, TouchS.Y);

  if (isClick) {
    ProcessIntrinsicClickEffects(&actObj);
  }

  if (actObj.Flags & VTFT_INT_REPAINT_ON_UP)
    DrawScreen(CurrentScreen);

  OnEvent(&actObj, VTFT_EVT_UP);
  if (isClick)
    OnEvent(&actObj, VTFT_EVT_CLICK);
}

static void Process_TP_Down() {
  // Search for active object
  if (TouchS.Tag) {        // objects must not have zero for tag value
    if (TouchS.Tag != 255) // can not search objects by default tag value
      GetActiveObjectByTag(TouchS.Tag, &TouchS.ActObjInfo);
    if (!TouchS.ActObjInfo.Obj) // object not found by tag, search by coordinates
      GetActiveObjectByXY(TouchS.X, TouchS.Y, &TouchS.ActObjInfo);
  }

  // Screen Event
  if (CurrentScreen->Active)
    if ((CurrentScreen->SniffObjectEvents) || (!TouchS.ActObjInfo.Obj))
      ProcessEvent(CurrentScreen->OnDown);

  // Object Event
  if (!TouchS.ActObjInfo.Obj)
    return;

  if (TouchS.ActObjInfo.Flags & VTFT_INT_REPAINT_ON_DOWN)
    DrawScreen(CurrentScreen);

  OnEvent(&TouchS.ActObjInfo, VTFT_EVT_DOWN);
}

static void Process_TP_TagChange() {
  // Screen Event
  if (CurrentScreen->Active)
    ProcessEvent(CurrentScreen->OnTagChange);
}

void ProcessVTFTStack() {
  char         Tag, oldTag;
  unsigned int X, Y;

  oldTag = TouchS.Tag;

  if (FT812_Touch_GetTagXY(&X, &Y) == 1) {
    FT812_Touch_GetTag(&Tag);

    TouchS.Tag = Tag;
    TouchS.X = X;
    TouchS.Y = Y;

    if (!TouchS.Pressed) {
      TouchS.Pressed = 1;
      Process_TP_Down();
    }

    Process_TP_Press();
  }
  else if (TouchS.Pressed) {
    Process_TP_Up();

    TouchS.Tag = 0;
    TouchS.X   = X;
    TouchS.Y   = Y;

    TouchS.Pressed = 0;
  }

  if (oldTag != TouchS.Tag)
    Process_TP_TagChange();
}

static void InitObjects() {
  // Home: Init block start
  Home.Color              = 0x0000;
  Home.Width              = 800;
  Home.Height             = 480;
  Home.ObjectsCount       = 115;
  Home.ButtonsCount       = 0;
  Home.Buttons            = 0;
  Home.LabelsCount        = 43;
  Home.Labels             = Home_Labels;
  Home.ImagesCount        = 50;
  Home.Images             = Home_Images;
  Home.CircleButtonsCount = 0;
  Home.CircleButtons      = 0;
  Home.BoxesCount         = 22;
  Home.Boxes              = Home_Boxes;
  Home.CheckBoxesCount    = 0;
  Home.CheckBoxes         = 0;
  Home.DynResStart        = 0;
  Home.Active             = 1;
  Home.SniffObjectEvents  = 0;
  Home.OnUp               = 0;
  Home.OnDown             = 0;
  Home.OnTagChange        = 0;
  Home.OnPress            = 0;

  // Temperature_Mode: Init block start
  Temperature_Mode.Color              = 0x0000;
  Temperature_Mode.Width              = 800;
  Temperature_Mode.Height             = 480;
  Temperature_Mode.ObjectsCount       = 40;
  Temperature_Mode.ButtonsCount       = 2;
  Temperature_Mode.Buttons            = Temperature_Mode_Buttons;
  Temperature_Mode.LabelsCount        = 10;
  Temperature_Mode.Labels             = Temperature_Mode_Labels;
  Temperature_Mode.ImagesCount        = 9;
  Temperature_Mode.Images             = Temperature_Mode_Images;
  Temperature_Mode.CircleButtonsCount = 4;
  Temperature_Mode.CircleButtons      = Temperature_Mode_CircleButtons;
  Temperature_Mode.BoxesCount         = 15;
  Temperature_Mode.Boxes              = Temperature_Mode_Boxes;
  Temperature_Mode.CheckBoxesCount    = 0;
  Temperature_Mode.CheckBoxes         = 0;
  Temperature_Mode.DynResStart        = 0;
  Temperature_Mode.Active             = 1;
  Temperature_Mode.SniffObjectEvents  = 0;
  Temperature_Mode.OnUp               = 0;
  Temperature_Mode.OnDown             = 0;
  Temperature_Mode.OnTagChange        = 0;
  Temperature_Mode.OnPress            = 0;

  // Light_Mode: Init block start
  Light_Mode.Color              = 0x0000;
  Light_Mode.Width              = 800;
  Light_Mode.Height             = 480;
  Light_Mode.ObjectsCount       = 40;
  Light_Mode.ButtonsCount       = 2;
  Light_Mode.Buttons            = Light_Mode_Buttons;
  Light_Mode.LabelsCount        = 10;
  Light_Mode.Labels             = Light_Mode_Labels;
  Light_Mode.ImagesCount        = 9;
  Light_Mode.Images             = Light_Mode_Images;
  Light_Mode.CircleButtonsCount = 4;
  Light_Mode.CircleButtons      = Light_Mode_CircleButtons;
  Light_Mode.BoxesCount         = 15;
  Light_Mode.Boxes              = Light_Mode_Boxes;
  Light_Mode.CheckBoxesCount    = 0;
  Light_Mode.CheckBoxes         = 0;
  Light_Mode.DynResStart        = 0;
  Light_Mode.Active             = 1;
  Light_Mode.SniffObjectEvents  = 0;
  Light_Mode.OnUp               = 0;
  Light_Mode.OnDown             = 0;
  Light_Mode.OnTagChange        = 0;
  Light_Mode.OnPress            = 0;

  // Keyboard_Screen: Init block start
  Keyboard_Screen.Color              = 0x0000;
  Keyboard_Screen.Width              = 800;
  Keyboard_Screen.Height             = 480;
  Keyboard_Screen.ObjectsCount       = 15;
  Keyboard_Screen.ButtonsCount       = 2;
  Keyboard_Screen.Buttons            = Keyboard_Screen_Buttons;
  Keyboard_Screen.LabelsCount        = 4;
  Keyboard_Screen.Labels             = Keyboard_Screen_Labels;
  Keyboard_Screen.ImagesCount        = 0;
  Keyboard_Screen.Images             = 0;
  Keyboard_Screen.CircleButtonsCount = 0;
  Keyboard_Screen.CircleButtons      = 0;
  Keyboard_Screen.BoxesCount         = 5;
  Keyboard_Screen.Boxes              = Keyboard_Screen_Boxes;
  Keyboard_Screen.CheckBoxesCount    = 4;
  Keyboard_Screen.CheckBoxes         = Keyboard_Screen_CheckBoxes;
  Keyboard_Screen.DynResStart        = 0;
  Keyboard_Screen.Active             = 1;
  Keyboard_Screen.SniffObjectEvents  = 0;
  Keyboard_Screen.OnUp               = 0;
  Keyboard_Screen.OnDown             = 0;
  Keyboard_Screen.OnTagChange        = 0;
  Keyboard_Screen.OnPress            = 0;

  // System_Log_Screen: Init block start
  System_Log_Screen.Color              = 0x0000;
  System_Log_Screen.Width              = 800;
  System_Log_Screen.Height             = 480;
  System_Log_Screen.ObjectsCount       = 33;
  System_Log_Screen.ButtonsCount       = 18;
  System_Log_Screen.Buttons            = System_Log_Screen_Buttons;
  System_Log_Screen.LabelsCount        = 4;
  System_Log_Screen.Labels             = System_Log_Screen_Labels;
  System_Log_Screen.ImagesCount        = 6;
  System_Log_Screen.Images             = System_Log_Screen_Images;
  System_Log_Screen.CircleButtonsCount = 0;
  System_Log_Screen.CircleButtons      = 0;
  System_Log_Screen.BoxesCount         = 5;
  System_Log_Screen.Boxes              = System_Log_Screen_Boxes;
  System_Log_Screen.CheckBoxesCount    = 0;
  System_Log_Screen.CheckBoxes         = 0;
  System_Log_Screen.DynResStart        = 0;
  System_Log_Screen.Active             = 1;
  System_Log_Screen.SniffObjectEvents  = 0;
  System_Log_Screen.OnUp               = 0;
  System_Log_Screen.OnDown             = 0;
  System_Log_Screen.OnTagChange        = 0;
  System_Log_Screen.OnPress            = 0;

  // Green_Power_Screen: Init block start
  Green_Power_Screen.Color              = 0x0000;
  Green_Power_Screen.Width              = 800;
  Green_Power_Screen.Height             = 480;
  Green_Power_Screen.ObjectsCount       = 12;
  Green_Power_Screen.ButtonsCount       = 2;
  Green_Power_Screen.Buttons            = Green_Power_Screen_Buttons;
  Green_Power_Screen.LabelsCount        = 8;
  Green_Power_Screen.Labels             = Green_Power_Screen_Labels;
  Green_Power_Screen.ImagesCount        = 0;
  Green_Power_Screen.Images             = 0;
  Green_Power_Screen.CircleButtonsCount = 0;
  Green_Power_Screen.CircleButtons      = 0;
  Green_Power_Screen.BoxesCount         = 2;
  Green_Power_Screen.Boxes              = Green_Power_Screen_Boxes;
  Green_Power_Screen.CheckBoxesCount    = 0;
  Green_Power_Screen.CheckBoxes         = 0;
  Green_Power_Screen.DynResStart        = 0;
  Green_Power_Screen.Active             = 1;
  Green_Power_Screen.SniffObjectEvents  = 0;
  Green_Power_Screen.OnUp               = 0;
  Green_Power_Screen.OnDown             = 0;
  Green_Power_Screen.OnTagChange        = 0;
  Green_Power_Screen.OnPress            = 0;

  // Time_Screen: Init block start
  Time_Screen.Color              = 0x0000;
  Time_Screen.Width              = 800;
  Time_Screen.Height             = 480;
  Time_Screen.ObjectsCount       = 31;
  Time_Screen.ButtonsCount       = 2;
  Time_Screen.Buttons            = Time_Screen_Buttons;
  Time_Screen.LabelsCount        = 17;
  Time_Screen.Labels             = Time_Screen_Labels;
  Time_Screen.ImagesCount        = 2;
  Time_Screen.Images             = Time_Screen_Images;
  Time_Screen.CircleButtonsCount = 0;
  Time_Screen.CircleButtons      = 0;
  Time_Screen.BoxesCount         = 10;
  Time_Screen.Boxes              = Time_Screen_Boxes;
  Time_Screen.CheckBoxesCount    = 0;
  Time_Screen.CheckBoxes         = 0;
  Time_Screen.DynResStart        = 0;
  Time_Screen.Active             = 1;
  Time_Screen.SniffObjectEvents  = 0;
  Time_Screen.OnUp               = 0;
  Time_Screen.OnDown             = 0;
  Time_Screen.OnTagChange        = 0;
  Time_Screen.OnPress            = 0;

  // Settings_Screen: Init block start
  Settings_Screen.Color              = 0x0000;
  Settings_Screen.Width              = 800;
  Settings_Screen.Height             = 480;
  Settings_Screen.ObjectsCount       = 26;
  Settings_Screen.ButtonsCount       = 19;
  Settings_Screen.Buttons            = Settings_Screen_Buttons;
  Settings_Screen.LabelsCount        = 5;
  Settings_Screen.Labels             = Settings_Screen_Labels;
  Settings_Screen.ImagesCount        = 0;
  Settings_Screen.Images             = 0;
  Settings_Screen.CircleButtonsCount = 0;
  Settings_Screen.CircleButtons      = 0;
  Settings_Screen.BoxesCount         = 2;
  Settings_Screen.Boxes              = Settings_Screen_Boxes;
  Settings_Screen.CheckBoxesCount    = 0;
  Settings_Screen.CheckBoxes         = 0;
  Settings_Screen.DynResStart        = 0;
  Settings_Screen.Active             = 1;
  Settings_Screen.SniffObjectEvents  = 0;
  Settings_Screen.OnUp               = 0;
  Settings_Screen.OnDown             = 0;
  Settings_Screen.OnTagChange        = 0;
  Settings_Screen.OnPress            = 0;

  Box34.OwnerScreen   = &Home;
  Box34.Order         = 0;
  Box34.Visible       = 1;
  Box34.Opacity       = 255;
  Box34.Tag           = 255;
  Box34.Left          = 208;
  Box34.Top           = 6;
  Box34.Width         = 389;
  Box34.Height        = 148;
  Box34.Pen_Width     = 1;
  Box34.Pen_Color     = 0x0000;
  Box34.Color         = 0x00FF;
  Box34.Press_Color   = 0xFFFFFF;
  Box34.ColorTo       = 0xFFFFFF;
  Box34.Press_ColorTo = 0xC0C0C0;
  Box34.Gradient      = _FT812_BRUSH_GR_NONE;
  Box34.Active        = 1;
  Box34.OnUp          = 0;
  Box34.OnDown        = 0;
  Box34.OnClick       = 0;
  Box34.OnPress       = 0;

  Image75.OwnerScreen   = &Home;
  Image75.Order         = 1;
  Image75.Visible       = 0;
  Image75.Opacity       = 255;
  Image75.Tag           = 255;
  Image75.Left          = 503;
  Image75.Top           = 48;
  Image75.Width         = 42;
  Image75.Height        = 40;
  Image75.Picture_Name  = deg2_bmp;
  Image75.Picture_Type  = 7;
  Image75.Picture_Ratio = 1;
  Image75.Blend_Color   = 0xFFFFFF;
  Image75.Source        = -1L;
  Image75.RotationAngle = 0;
  Image75.RotationCenterLeft = 509;
  Image75.RotationCenterTop = 51;
  Image75.ScaleX        = 0.1500;
  Image75.ScaleY        = 0.1500;
  Image75.Active        = 0;
  Image75.OnUp          = 0;
  Image75.OnDown        = 0;
  Image75.OnClick       = 0;
  Image75.OnPress       = 0;

  Image76.OwnerScreen   = &Home;
  Image76.Order         = 2;
  Image76.Visible       = 0;
  Image76.Opacity       = 255;
  Image76.Tag           = 255;
  Image76.Left          = 378;
  Image76.Top           = 127;
  Image76.Width         = 42;
  Image76.Height        = 40;
  Image76.Picture_Name  = deg2_bmp;
  Image76.Picture_Type  = 7;
  Image76.Picture_Ratio = 1;
  Image76.Blend_Color   = 0xFFFFFF;
  Image76.Source        = -1L;
  Image76.RotationAngle = 0;
  Image76.RotationCenterLeft = 381;
  Image76.RotationCenterTop = 130;
  Image76.ScaleX        = 0.1500;
  Image76.ScaleY        = 0.1500;
  Image76.Active        = 0;
  Image76.OnUp          = 0;
  Image76.OnDown        = 0;
  Image76.OnClick       = 0;
  Image76.OnPress       = 0;

  Box2.OwnerScreen   = &Home;
  Box2.Order         = 3;
  Box2.Visible       = 1;
  Box2.Opacity       = 255;
  Box2.Tag           = 255;
  Box2.Left          = 602;
  Box2.Top           = 236;
  Box2.Width         = 192;
  Box2.Height        = 148;
  Box2.Pen_Width     = 1;
  Box2.Pen_Color     = 0x0000;
  Box2.Color         = 0x808080;
  Box2.Press_Color   = 0xFFFFFF;
  Box2.ColorTo       = 0xFFFFFF;
  Box2.Press_ColorTo = 0xC0C0C0;
  Box2.Gradient      = _FT812_BRUSH_GR_NONE;
  Box2.Active        = 1;
  Box2.OnUp          = 0;
  Box2.OnDown        = 0;
  Box2.OnClick       = &Box2_OnClick;
  Box2.OnPress       = 0;

  Box2_OnClick.Action       = Box2OnClick;
  Box2_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Box2_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Box2_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Box2_OnClick.Sound.Volume = 255;

  Image74.OwnerScreen   = &Home;
  Image74.Order         = 4;
  Image74.Visible       = 0;
  Image74.Opacity       = 255;
  Image74.Tag           = 255;
  Image74.Left          = 620;
  Image74.Top           = 300;
  Image74.Width         = 69;
  Image74.Height        = 64;
  Image74.Picture_Name  = peo4_bmp;
  Image74.Picture_Type  = 7;
  Image74.Picture_Ratio = 1;
  Image74.Blend_Color   = 0xFFFFFF;
  Image74.Source        = -1L;
  Image74.RotationAngle = 0;
  Image74.RotationCenterLeft = 642;
  Image74.RotationCenterTop = 320;
  Image74.ScaleX        = 0.6300;
  Image74.ScaleY        = 0.6300;
  Image74.Active        = 0;
  Image74.OnUp          = 0;
  Image74.OnDown        = 0;
  Image74.OnClick       = 0;
  Image74.OnPress       = 0;

  Image72.OwnerScreen   = &Home;
  Image72.Order         = 5;
  Image72.Visible       = 0;
  Image72.Opacity       = 255;
  Image72.Tag           = 255;
  Image72.Left          = 620;
  Image72.Top           = 300;
  Image72.Width         = 62;
  Image72.Height        = 59;
  Image72.Picture_Name  = book3_bmp;
  Image72.Picture_Type  = 7;
  Image72.Picture_Ratio = 1;
  Image72.Blend_Color   = 0xFFFFFF;
  Image72.Source        = -1L;
  Image72.RotationAngle = 0;
  Image72.RotationCenterLeft = 643;
  Image72.RotationCenterTop = 322;
  Image72.ScaleX        = 0.7300;
  Image72.ScaleY        = 0.7300;
  Image72.Active        = 0;
  Image72.OnUp          = 0;
  Image72.OnDown        = 0;
  Image72.OnClick       = 0;
  Image72.OnPress       = 0;

  Image73.OwnerScreen   = &Home;
  Image73.Order         = 6;
  Image73.Visible       = 0;
  Image73.Opacity       = 255;
  Image73.Tag           = 255;
  Image73.Left          = 620;
  Image73.Top           = 300;
  Image73.Width         = 56;
  Image73.Height        = 66;
  Image73.Picture_Name  = dish3_bmp;
  Image73.Picture_Type  = 7;
  Image73.Picture_Ratio = 1;
  Image73.Blend_Color   = 0xFFFFFF;
  Image73.Source        = -1L;
  Image73.RotationAngle = 0;
  Image73.RotationCenterLeft = 639;
  Image73.RotationCenterTop = 322;
  Image73.ScaleX        = 0.6600;
  Image73.ScaleY        = 0.6600;
  Image73.Active        = 0;
  Image73.OnUp          = 0;
  Image73.OnDown        = 0;
  Image73.OnClick       = 0;
  Image73.OnPress       = 0;

  Image71.OwnerScreen   = &Home;
  Image71.Order         = 7;
  Image71.Visible       = 1;
  Image71.Opacity       = 255;
  Image71.Tag           = 255;
  Image71.Left          = 620;
  Image71.Top           = 300;
  Image71.Width         = 67;
  Image71.Height        = 55;
  Image71.Picture_Name  = media3_bmp;
  Image71.Picture_Type  = 7;
  Image71.Picture_Ratio = 1;
  Image71.Blend_Color   = 0xFFFFFF;
  Image71.Source        = -1L;
  Image71.RotationAngle = 0;
  Image71.RotationCenterLeft = 646;
  Image71.RotationCenterTop = 320;
  Image71.ScaleX        = 0.7500;
  Image71.ScaleY        = 0.7500;
  Image71.Active        = 0;
  Image71.OnUp          = 0;
  Image71.OnDown        = 0;
  Image71.OnClick       = 0;
  Image71.OnPress       = 0;

  Box1.OwnerScreen   = &Home;
  Box1.Order         = 8;
  Box1.Visible       = 1;
  Box1.Opacity       = 255;
  Box1.Tag           = 255;
  Box1.Left          = 208;
  Box1.Top           = 158;
  Box1.Width         = 292;
  Box1.Height        = 73;
  Box1.Pen_Width     = 1;
  Box1.Pen_Color     = 0x0000;
  Box1.Color         = 0x00FF;
  Box1.Press_Color   = 0xFFFFFF;
  Box1.ColorTo       = 0xFFFFFF;
  Box1.Press_ColorTo = 0xC0C0C0;
  Box1.Gradient      = _FT812_BRUSH_GR_NONE;
  Box1.Active        = 1;
  Box1.OnUp          = 0;
  Box1.OnDown        = 0;
  Box1.OnClick       = &Box1_OnClick;
  Box1.OnPress       = 0;

  Box1_OnClick.Action       = Box1OnClick;
  Box1_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Box1_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Box1_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Box1_OnClick.Sound.Volume = 255;

  Image43.OwnerScreen   = &Home;
  Image43.Order         = 9;
  Image43.Visible       = 0;
  Image43.Opacity       = 255;
  Image43.Tag           = 255;
  Image43.Left          = 285;
  Image43.Top           = 176;
  Image43.Width         = 69;
  Image43.Height        = 64;
  Image43.Picture_Name  = peo3_bmp;
  Image43.Picture_Type  = 7;
  Image43.Picture_Ratio = 1;
  Image43.Blend_Color   = 0xFFFFFF;
  Image43.Source        = -1L;
  Image43.RotationAngle = 0;
  Image43.RotationCenterLeft = 300;
  Image43.RotationCenterTop = 196;
  Image43.ScaleX        = 0.6300;
  Image43.ScaleY        = 0.6300;
  Image43.Active        = 0;
  Image43.OnUp          = 0;
  Image43.OnDown        = 0;
  Image43.OnClick       = 0;
  Image43.OnPress       = 0;

  Image42.OwnerScreen   = &Home;
  Image42.Order         = 10;
  Image42.Visible       = 0;
  Image42.Opacity       = 255;
  Image42.Tag           = 255;
  Image42.Left          = 285;
  Image42.Top           = 176;
  Image42.Width         = 58;
  Image42.Height        = 54;
  Image42.Picture_Name  = moon5_bmp;
  Image42.Picture_Type  = 7;
  Image42.Picture_Ratio = 1;
  Image42.Blend_Color   = 0xFFFFFF;
  Image42.Source        = -1L;
  Image42.RotationAngle = 0;
  Image42.RotationCenterLeft = 300;
  Image42.RotationCenterTop = 195;
  Image42.ScaleX        = 0.7500;
  Image42.ScaleY        = 0.7500;
  Image42.Active        = 0;
  Image42.OnUp          = 0;
  Image42.OnDown        = 0;
  Image42.OnClick       = 0;
  Image42.OnPress       = 0;

  Image40.OwnerScreen   = &Home;
  Image40.Order         = 11;
  Image40.Visible       = 0;
  Image40.Opacity       = 255;
  Image40.Tag           = 255;
  Image40.Left          = 285;
  Image40.Top           = 176;
  Image40.Width         = 63;
  Image40.Height        = 64;
  Image40.Picture_Name  = sun5_bmp;
  Image40.Picture_Type  = 7;
  Image40.Picture_Ratio = 1;
  Image40.Blend_Color   = 0xFFFFFF;
  Image40.Source        = -1L;
  Image40.RotationAngle = 0;
  Image40.RotationCenterLeft = 300;
  Image40.RotationCenterTop = 197;
  Image40.ScaleX        = 0.6600;
  Image40.ScaleY        = 0.6600;
  Image40.Active        = 0;
  Image40.OnUp          = 0;
  Image40.OnDown        = 0;
  Image40.OnClick       = 0;
  Image40.OnPress       = 0;

  Image5.OwnerScreen   = &Home;
  Image5.Order         = 12;
  Image5.Visible       = 1;
  Image5.Opacity       = 255;
  Image5.Tag           = 255;
  Image5.Left          = 285;
  Image5.Top           = 176;
  Image5.Width         = 59;
  Image5.Height        = 57;
  Image5.Picture_Name  = house3_bmp;
  Image5.Picture_Type  = 7;
  Image5.Picture_Ratio = 1;
  Image5.Blend_Color   = 0xFFFFFF;
  Image5.Source        = -1L;
  Image5.RotationAngle = 0;
  Image5.RotationCenterLeft = 300;
  Image5.RotationCenterTop = 197;
  Image5.ScaleX        = 0.7500;
  Image5.ScaleY        = 0.7500;
  Image5.Active        = 0;
  Image5.OnUp          = 0;
  Image5.OnDown        = 0;
  Image5.OnClick       = 0;
  Image5.OnPress       = 0;

  Image41.OwnerScreen   = &Home;
  Image41.Order         = 13;
  Image41.Visible       = 1;
  Image41.Opacity       = 255;
  Image41.Tag           = 255;
  Image41.Left          = 503;
  Image41.Top           = 48;
  Image41.Width         = 42;
  Image41.Height        = 40;
  Image41.Picture_Name  = deg_bmp;
  Image41.Picture_Type  = 7;
  Image41.Picture_Ratio = 1;
  Image41.Blend_Color   = 0xFFFFFF;
  Image41.Source        = -1L;
  Image41.RotationAngle = 0;
  Image41.RotationCenterLeft = 509;
  Image41.RotationCenterTop = 51;
  Image41.ScaleX        = 0.1500;
  Image41.ScaleY        = 0.1500;
  Image41.Active        = 0;
  Image41.OnUp          = 0;
  Image41.OnDown        = 0;
  Image41.OnClick       = 0;
  Image41.OnPress       = 0;

  Image64.OwnerScreen   = &Home;
  Image64.Order         = 14;
  Image64.Visible       = 0;
  Image64.Opacity       = 255;
  Image64.Tag           = 255;
  Image64.Left          = 526;
  Image64.Top           = 127;
  Image64.Width         = 35;
  Image64.Height        = 38;
  Image64.Picture_Name  = umbre2_bmp;
  Image64.Picture_Type  = 7;
  Image64.Picture_Ratio = 1;
  Image64.Blend_Color   = 0xFFFFFF;
  Image64.Source        = -1L;
  Image64.RotationAngle = 0;
  Image64.RotationCenterLeft = 541;
  Image64.RotationCenterTop = 138;
  Image64.ScaleX        = 0.5500;
  Image64.ScaleY        = 0.5500;
  Image64.Active        = 0;
  Image64.OnUp          = 0;
  Image64.OnDown        = 0;
  Image64.OnClick       = 0;
  Image64.OnPress       = 0;

  Image45.OwnerScreen   = &Home;
  Image45.Order         = 15;
  Image45.Visible       = 1;
  Image45.Opacity       = 255;
  Image45.Tag           = 255;
  Image45.Left          = 526;
  Image45.Top           = 127;
  Image45.Width         = 35;
  Image45.Height        = 38;
  Image45.Picture_Name  = umbre_bmp;
  Image45.Picture_Type  = 7;
  Image45.Picture_Ratio = 1;
  Image45.Blend_Color   = 0xFFFFFF;
  Image45.Source        = -1L;
  Image45.RotationAngle = 0;
  Image45.RotationCenterLeft = 541;
  Image45.RotationCenterTop = 138;
  Image45.ScaleX        = 0.5500;
  Image45.ScaleY        = 0.5500;
  Image45.Active        = 0;
  Image45.OnUp          = 0;
  Image45.OnDown        = 0;
  Image45.OnClick       = 0;
  Image45.OnPress       = 0;

  Image44.OwnerScreen   = &Home;
  Image44.Order         = 16;
  Image44.Visible       = 1;
  Image44.Opacity       = 255;
  Image44.Tag           = 255;
  Image44.Left          = 378;
  Image44.Top           = 127;
  Image44.Width         = 42;
  Image44.Height        = 40;
  Image44.Picture_Name  = deg_bmp;
  Image44.Picture_Type  = 7;
  Image44.Picture_Ratio = 1;
  Image44.Blend_Color   = 0xFFFFFF;
  Image44.Source        = -1L;
  Image44.RotationAngle = 0;
  Image44.RotationCenterLeft = 382;
  Image44.RotationCenterTop = 130;
  Image44.ScaleX        = 0.1500;
  Image44.ScaleY        = 0.1500;
  Image44.Active        = 0;
  Image44.OnUp          = 0;
  Image44.OnDown        = 0;
  Image44.OnClick       = 0;
  Image44.OnPress       = 0;

  Box35.OwnerScreen   = &Home;
  Box35.Order         = 17;
  Box35.Visible       = 1;
  Box35.Opacity       = 255;
  Box35.Tag           = 255;
  Box35.Left          = 504;
  Box35.Top           = 158;
  Box35.Width         = 93;
  Box35.Height        = 73;
  Box35.Pen_Width     = 1;
  Box35.Pen_Color     = 0x0000;
  Box35.Color         = 0x00FF;
  Box35.Press_Color   = 0xFFFFFF;
  Box35.ColorTo       = 0xFFFFFF;
  Box35.Press_ColorTo = 0xC0C0C0;
  Box35.Gradient      = _FT812_BRUSH_GR_NONE;
  Box35.Active        = 1;
  Box35.OnUp          = 0;
  Box35.OnDown        = 0;
  Box35.OnClick       = &Box35_OnClick;
  Box35.OnPress       = 0;

  Box35_OnClick.Action       = Box35OnClick;
  Box35_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Box35_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Box35_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Box35_OnClick.Sound.Volume = 255;

  Image39.OwnerScreen   = &Home;
  Image39.Order         = 18;
  Image39.Visible       = 1;
  Image39.Opacity       = 255;
  Image39.Tag           = 255;
  Image39.Left          = 527;
  Image39.Top           = 166;
  Image39.Width         = 47;
  Image39.Height        = 40;
  Image39.Picture_Name  = slide3_bmp;
  Image39.Picture_Type  = 7;
  Image39.Picture_Ratio = 1;
  Image39.Blend_Color   = 0xFFFFFF;
  Image39.Source        = -1L;
  Image39.RotationAngle = 0;
  Image39.RotationCenterLeft = 551;
  Image39.RotationCenterTop = 186;
  Image39.ScaleX        = 1.0000;
  Image39.ScaleY        = 1.0000;
  Image39.Active        = 0;
  Image39.OnUp          = 0;
  Image39.OnDown        = 0;
  Image39.OnClick       = 0;
  Image39.OnPress       = 0;

  Box12.OwnerScreen   = &Home;
  Box12.Order         = 19;
  Box12.Visible       = 1;
  Box12.Opacity       = 255;
  Box12.Tag           = 255;
  Box12.Left          = 602;
  Box12.Top           = 389;
  Box12.Width         = 192;
  Box12.Height        = 82;
  Box12.Pen_Width     = 1;
  Box12.Pen_Color     = 0x0000;
  Box12.Color         = 0x808080;
  Box12.Press_Color   = 0xFFFFFF;
  Box12.ColorTo       = 0xFFFFFF;
  Box12.Press_ColorTo = 0xC0C0C0;
  Box12.Gradient      = _FT812_BRUSH_GR_NONE;
  Box12.Active        = 1;
  Box12.OnUp          = 0;
  Box12.OnDown        = 0;
  Box12.OnClick       = &Box12_OnClick;
  Box12.OnPress       = 0;

  Box12_OnClick.Action       = Box12OnClick;
  Box12_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Box12_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Box12_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Box12_OnClick.Sound.Volume = 255;

  Box19.OwnerScreen   = &Home;
  Box19.Order         = 20;
  Box19.Visible       = 1;
  Box19.Opacity       = 255;
  Box19.Tag           = 255;
  Box19.Left          = 504;
  Box19.Top           = 389;
  Box19.Width         = 93;
  Box19.Height        = 82;
  Box19.Pen_Width     = 1;
  Box19.Pen_Color     = 0x0000;
  Box19.Color         = 0x8000FF;
  Box19.Press_Color   = 0xFFFFFF;
  Box19.ColorTo       = 0xFFFFFF;
  Box19.Press_ColorTo = 0xC0C0C0;
  Box19.Gradient      = _FT812_BRUSH_GR_NONE;
  Box19.Active        = 1;
  Box19.OnUp          = 0;
  Box19.OnDown        = 0;
  Box19.OnClick       = 0;
  Box19.OnPress       = 0;

  Box18.OwnerScreen   = &Home;
  Box18.Order         = 21;
  Box18.Visible       = 1;
  Box18.Opacity       = 255;
  Box18.Tag           = 255;
  Box18.Left          = 405;
  Box18.Top           = 389;
  Box18.Width         = 94;
  Box18.Height        = 82;
  Box18.Pen_Width     = 1;
  Box18.Pen_Color     = 0x0000;
  Box18.Color         = 0x8000FF;
  Box18.Press_Color   = 0xFFFFFF;
  Box18.ColorTo       = 0xFFFFFF;
  Box18.Press_ColorTo = 0xC0C0C0;
  Box18.Gradient      = _FT812_BRUSH_GR_NONE;
  Box18.Active        = 1;
  Box18.OnUp          = 0;
  Box18.OnDown        = 0;
  Box18.OnClick       = 0;
  Box18.OnPress       = 0;

  Box17.OwnerScreen   = &Home;
  Box17.Order         = 22;
  Box17.Visible       = 1;
  Box17.Opacity       = 255;
  Box17.Tag           = 255;
  Box17.Left          = 307;
  Box17.Top           = 389;
  Box17.Width         = 93;
  Box17.Height        = 82;
  Box17.Pen_Width     = 1;
  Box17.Pen_Color     = 0x0000;
  Box17.Color         = 0x8000FF;
  Box17.Press_Color   = 0xFFFFFF;
  Box17.ColorTo       = 0xFFFFFF;
  Box17.Press_ColorTo = 0xC0C0C0;
  Box17.Gradient      = _FT812_BRUSH_GR_NONE;
  Box17.Active        = 1;
  Box17.OnUp          = 0;
  Box17.OnDown        = 0;
  Box17.OnClick       = 0;
  Box17.OnPress       = 0;

  Box16.OwnerScreen   = &Home;
  Box16.Order         = 23;
  Box16.Visible       = 1;
  Box16.Opacity       = 255;
  Box16.Tag           = 255;
  Box16.Left          = 208;
  Box16.Top           = 389;
  Box16.Width         = 94;
  Box16.Height        = 82;
  Box16.Pen_Width     = 1;
  Box16.Pen_Color     = 0x0000;
  Box16.Color         = 0x8000FF;
  Box16.Press_Color   = 0xFFFFFF;
  Box16.ColorTo       = 0xFFFFFF;
  Box16.Press_ColorTo = 0xC0C0C0;
  Box16.Gradient      = _FT812_BRUSH_GR_NONE;
  Box16.Active        = 1;
  Box16.OnUp          = 0;
  Box16.OnDown        = 0;
  Box16.OnClick       = 0;
  Box16.OnPress       = 0;

  Box15.OwnerScreen   = &Home;
  Box15.Order         = 24;
  Box15.Visible       = 1;
  Box15.Opacity       = 255;
  Box15.Tag           = 255;
  Box15.Left          = 110;
  Box15.Top           = 389;
  Box15.Width         = 93;
  Box15.Height        = 82;
  Box15.Pen_Width     = 1;
  Box15.Pen_Color     = 0x0000;
  Box15.Color         = 0x8000FF;
  Box15.Press_Color   = 0xFFFFFF;
  Box15.ColorTo       = 0xFFFFFF;
  Box15.Press_ColorTo = 0xC0C0C0;
  Box15.Gradient      = _FT812_BRUSH_GR_NONE;
  Box15.Active        = 1;
  Box15.OnUp          = 0;
  Box15.OnDown        = 0;
  Box15.OnClick       = 0;
  Box15.OnPress       = 0;

  Box14.OwnerScreen   = &Home;
  Box14.Order         = 25;
  Box14.Visible       = 1;
  Box14.Opacity       = 255;
  Box14.Tag           = 255;
  Box14.Left          = 11;
  Box14.Top           = 389;
  Box14.Width         = 94;
  Box14.Height        = 82;
  Box14.Pen_Width     = 1;
  Box14.Pen_Color     = 0x0000;
  Box14.Color         = 0x8000FF;
  Box14.Press_Color   = 0xFFFFFF;
  Box14.ColorTo       = 0xFFFFFF;
  Box14.Press_ColorTo = 0xC0C0C0;
  Box14.Gradient      = _FT812_BRUSH_GR_NONE;
  Box14.Active        = 1;
  Box14.OnUp          = 0;
  Box14.OnDown        = 0;
  Box14.OnClick       = 0;
  Box14.OnPress       = 0;

  Box21.OwnerScreen   = &Home;
  Box21.Order         = 26;
  Box21.Visible       = 1;
  Box21.Opacity       = 255;
  Box21.Tag           = 255;
  Box21.Left          = 405;
  Box21.Top           = 236;
  Box21.Width         = 192;
  Box21.Height        = 148;
  Box21.Pen_Width     = 1;
  Box21.Pen_Color     = 0x0000;
  Box21.Color         = 0x8040;
  Box21.Press_Color   = 0xFFFFFF;
  Box21.ColorTo       = 0xFFFFFF;
  Box21.Press_ColorTo = 0xC0C0C0;
  Box21.Gradient      = _FT812_BRUSH_GR_NONE;
  Box21.Active        = 1;
  Box21.OnUp          = 0;
  Box21.OnDown        = 0;
  Box21.OnClick       = &Box21_OnClick;
  Box21.OnPress       = 0;

  Box21_OnClick.Action       = Box21OnClick;
  Box21_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Box21_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Box21_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Box21_OnClick.Sound.Volume = 255;

  Box20.OwnerScreen   = &Home;
  Box20.Order         = 27;
  Box20.Visible       = 1;
  Box20.Opacity       = 255;
  Box20.Tag           = 255;
  Box20.Left          = 208;
  Box20.Top           = 236;
  Box20.Width         = 192;
  Box20.Height        = 148;
  Box20.Pen_Width     = 1;
  Box20.Pen_Color     = 0x0000;
  Box20.Color         = 0xFF8000;
  Box20.Press_Color   = 0xFFFFFF;
  Box20.ColorTo       = 0xFFFFFF;
  Box20.Press_ColorTo = 0xC0C0C0;
  Box20.Gradient      = _FT812_BRUSH_GR_NONE;
  Box20.Active        = 1;
  Box20.OnUp          = 0;
  Box20.OnDown        = 0;
  Box20.OnClick       = &Box20_OnClick;
  Box20.OnPress       = 0;

  Box20_OnClick.Action       = Box20OnClick;
  Box20_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Box20_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Box20_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Box20_OnClick.Sound.Volume = 255;

  Box13.OwnerScreen   = &Home;
  Box13.Order         = 28;
  Box13.Visible       = 1;
  Box13.Opacity       = 255;
  Box13.Tag           = 255;
  Box13.Left          = 11;
  Box13.Top           = 236;
  Box13.Width         = 192;
  Box13.Height        = 148;
  Box13.Pen_Width     = 1;
  Box13.Pen_Color     = 0x0000;
  Box13.Color         = 0x8000FF;
  Box13.Press_Color   = 0xFFFFFF;
  Box13.ColorTo       = 0xFFFFFF;
  Box13.Press_ColorTo = 0xC0C0C0;
  Box13.Gradient      = _FT812_BRUSH_GR_NONE;
  Box13.Active        = 1;
  Box13.OnUp          = 0;
  Box13.OnDown        = 0;
  Box13.OnClick       = &Box13_OnClick;
  Box13.OnPress       = 0;

  Box13_OnClick.Action       = Box13OnClick;
  Box13_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Box13_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Box13_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Box13_OnClick.Sound.Volume = 255;

  Box8.OwnerScreen   = &Home;
  Box8.Order         = 29;
  Box8.Visible       = 1;
  Box8.Opacity       = 255;
  Box8.Tag           = 255;
  Box8.Left          = 701;
  Box8.Top           = 158;
  Box8.Width         = 93;
  Box8.Height        = 73;
  Box8.Pen_Width     = 1;
  Box8.Pen_Color     = 0x0000;
  Box8.Color         = 0x808080;
  Box8.Press_Color   = 0xFFFFFF;
  Box8.ColorTo       = 0xFFFFFF;
  Box8.Press_ColorTo = 0xC0C0C0;
  Box8.Gradient      = _FT812_BRUSH_GR_NONE;
  Box8.Active        = 1;
  Box8.OnUp          = 0;
  Box8.OnDown        = 0;
  Box8.OnClick       = &Box8_OnClick;
  Box8.OnPress       = 0;

  Box8_OnClick.Action       = Box8OnClick;
  Box8_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Box8_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Box8_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Box8_OnClick.Sound.Volume = 255;

  Box7.OwnerScreen   = &Home;
  Box7.Order         = 30;
  Box7.Visible       = 1;
  Box7.Opacity       = 255;
  Box7.Tag           = 255;
  Box7.Left          = 602;
  Box7.Top           = 158;
  Box7.Width         = 94;
  Box7.Height        = 73;
  Box7.Pen_Width     = 1;
  Box7.Pen_Color     = 0x0000;
  Box7.Color         = 0x808080;
  Box7.Press_Color   = 0xFFFFFF;
  Box7.ColorTo       = 0xFFFFFF;
  Box7.Press_ColorTo = 0xC0C0C0;
  Box7.Gradient      = _FT812_BRUSH_GR_NONE;
  Box7.Active        = 1;
  Box7.OnUp          = 0;
  Box7.OnDown        = 0;
  Box7.OnClick       = &Box7_OnClick;
  Box7.OnPress       = 0;

  Box7_OnClick.Action       = Box7OnClick;
  Box7_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Box7_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Box7_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Box7_OnClick.Sound.Volume = 255;

  Box6.OwnerScreen   = &Home;
  Box6.Order         = 31;
  Box6.Visible       = 1;
  Box6.Opacity       = 255;
  Box6.Tag           = 255;
  Box6.Left          = 701;
  Box6.Top           = 82;
  Box6.Width         = 93;
  Box6.Height        = 72;
  Box6.Pen_Width     = 1;
  Box6.Pen_Color     = 0x0000;
  Box6.Color         = 0x808080;
  Box6.Press_Color   = 0xFFFFFF;
  Box6.ColorTo       = 0xFFFFFF;
  Box6.Press_ColorTo = 0xC0C0C0;
  Box6.Gradient      = _FT812_BRUSH_GR_NONE;
  Box6.Active        = 1;
  Box6.OnUp          = 0;
  Box6.OnDown        = 0;
  Box6.OnClick       = &Box6_OnClick;
  Box6.OnPress       = 0;

  Box6_OnClick.Action       = Box6OnClick;
  Box6_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Box6_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Box6_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Box6_OnClick.Sound.Volume = 255;

  Box5.OwnerScreen   = &Home;
  Box5.Order         = 32;
  Box5.Visible       = 1;
  Box5.Opacity       = 255;
  Box5.Tag           = 255;
  Box5.Left          = 602;
  Box5.Top           = 82;
  Box5.Width         = 94;
  Box5.Height        = 72;
  Box5.Pen_Width     = 1;
  Box5.Pen_Color     = 0x0000;
  Box5.Color         = 0x808080;
  Box5.Press_Color   = 0xFFFFFF;
  Box5.ColorTo       = 0xFFFFFF;
  Box5.Press_ColorTo = 0xC0C0C0;
  Box5.Gradient      = _FT812_BRUSH_GR_NONE;
  Box5.Active        = 1;
  Box5.OnUp          = 0;
  Box5.OnDown        = 0;
  Box5.OnClick       = &Box5_OnClick;
  Box5.OnPress       = 0;

  Box5_OnClick.Action       = Box5OnClick;
  Box5_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Box5_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Box5_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Box5_OnClick.Sound.Volume = 255;

  Box4.OwnerScreen   = &Home;
  Box4.Order         = 33;
  Box4.Visible       = 1;
  Box4.Opacity       = 255;
  Box4.Tag           = 255;
  Box4.Left          = 701;
  Box4.Top           = 6;
  Box4.Width         = 93;
  Box4.Height        = 72;
  Box4.Pen_Width     = 1;
  Box4.Pen_Color     = 0x0000;
  Box4.Color         = 0x808080;
  Box4.Press_Color   = 0xFFFFFF;
  Box4.ColorTo       = 0xFFFFFF;
  Box4.Press_ColorTo = 0xC0C0C0;
  Box4.Gradient      = _FT812_BRUSH_GR_NONE;
  Box4.Active        = 1;
  Box4.OnUp          = 0;
  Box4.OnDown        = 0;
  Box4.OnClick       = &Box4_OnClick;
  Box4.OnPress       = 0;

  Box4_OnClick.Action       = Box4OnClick;
  Box4_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Box4_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Box4_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Box4_OnClick.Sound.Volume = 255;

  Box3.OwnerScreen   = &Home;
  Box3.Order         = 34;
  Box3.Visible       = 1;
  Box3.Opacity       = 255;
  Box3.Tag           = 255;
  Box3.Left          = 602;
  Box3.Top           = 6;
  Box3.Width         = 94;
  Box3.Height        = 72;
  Box3.Pen_Width     = 1;
  Box3.Pen_Color     = 0x0000;
  Box3.Color         = 0x808080;
  Box3.Press_Color   = 0xFFFFFF;
  Box3.ColorTo       = 0xFFFFFF;
  Box3.Press_ColorTo = 0xC0C0C0;
  Box3.Gradient      = _FT812_BRUSH_GR_NONE;
  Box3.Active        = 1;
  Box3.OnUp          = 0;
  Box3.OnDown        = 0;
  Box3.OnClick       = &Box3_OnClick;
  Box3.OnPress       = 0;

  Box3_OnClick.Action       = Box3OnClick;
  Box3_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Box3_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Box3_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Box3_OnClick.Sound.Volume = 255;

  Box31.OwnerScreen   = &Home;
  Box31.Order         = 35;
  Box31.Visible       = 1;
  Box31.Opacity       = 255;
  Box31.Tag           = 255;
  Box31.Left          = 11;
  Box31.Top           = 158;
  Box31.Width         = 192;
  Box31.Height        = 73;
  Box31.Pen_Width     = 1;
  Box31.Pen_Color     = 0x0000;
  Box31.Color         = 0x808040;
  Box31.Press_Color   = 0xFFFFFF;
  Box31.ColorTo       = 0xFFFFFF;
  Box31.Press_ColorTo = 0xC0C0C0;
  Box31.Gradient      = _FT812_BRUSH_GR_NONE;
  Box31.Active        = 1;
  Box31.OnUp          = 0;
  Box31.OnDown        = 0;
  Box31.OnClick       = &Box31_OnClick;
  Box31.OnPress       = 0;

  Box31_OnClick.Action       = Box31OnClick;
  Box31_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Box31_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Box31_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Box31_OnClick.Sound.Volume = 255;

  Image1.OwnerScreen   = &Home;
  Image1.Order         = 36;
  Image1.Visible       = 1;
  Image1.Opacity       = 255;
  Image1.Tag           = 255;
  Image1.Left          = 676;
  Image1.Top           = 400;
  Image1.Width         = 44;
  Image1.Height        = 40;
  Image1.Picture_Name  = moon1_bmp;
  Image1.Picture_Type  = 7;
  Image1.Picture_Ratio = 1;
  Image1.Blend_Color   = 0xFFFFFF;
  Image1.Source        = -1L;
  Image1.RotationAngle = 0;
  Image1.RotationCenterLeft = 698;
  Image1.RotationCenterTop = 446;
  Image1.ScaleX        = 1.0000;
  Image1.ScaleY        = 1.0000;
  Image1.Active        = 0;
  Image1.OnUp          = 0;
  Image1.OnDown        = 0;
  Image1.OnClick       = 0;
  Image1.OnPress       = 0;

  Image20.OwnerScreen   = &Home;
  Image20.Order         = 37;
  Image20.Visible       = 1;
  Image20.Opacity       = 255;
  Image20.Tag           = 255;
  Image20.Left          = 527;
  Image20.Top           = 400;
  Image20.Width         = 48;
  Image20.Height        = 40;
  Image20.Picture_Name  = unlock_bmp;
  Image20.Picture_Type  = 7;
  Image20.Picture_Ratio = 1;
  Image20.Blend_Color   = 0xFFFFFF;
  Image20.Source        = -1L;
  Image20.RotationAngle = 0;
  Image20.RotationCenterLeft = 551;
  Image20.RotationCenterTop = 446;
  Image20.ScaleX        = 1.0000;
  Image20.ScaleY        = 1.0000;
  Image20.Active        = 1;
  Image20.OnUp          = 0;
  Image20.OnDown        = 0;
  Image20.OnClick       = 0;
  Image20.OnPress       = 0;

  Image19.OwnerScreen   = &Home;
  Image19.Order         = 38;
  Image19.Visible       = 1;
  Image19.Opacity       = 255;
  Image19.Tag           = 255;
  Image19.Left          = 428;
  Image19.Top           = 400;
  Image19.Width         = 48;
  Image19.Height        = 40;
  Image19.Picture_Name  = unlock_bmp;
  Image19.Picture_Type  = 7;
  Image19.Picture_Ratio = 1;
  Image19.Blend_Color   = 0xFFFFFF;
  Image19.Source        = -1L;
  Image19.RotationAngle = 0;
  Image19.RotationCenterLeft = 452;
  Image19.RotationCenterTop = 446;
  Image19.ScaleX        = 1.0000;
  Image19.ScaleY        = 1.0000;
  Image19.Active        = 1;
  Image19.OnUp          = 0;
  Image19.OnDown        = 0;
  Image19.OnClick       = 0;
  Image19.OnPress       = 0;

  Image18.OwnerScreen   = &Home;
  Image18.Order         = 39;
  Image18.Visible       = 1;
  Image18.Opacity       = 255;
  Image18.Tag           = 255;
  Image18.Left          = 330;
  Image18.Top           = 400;
  Image18.Width         = 48;
  Image18.Height        = 40;
  Image18.Picture_Name  = unlock_bmp;
  Image18.Picture_Type  = 7;
  Image18.Picture_Ratio = 1;
  Image18.Blend_Color   = 0xFFFFFF;
  Image18.Source        = -1L;
  Image18.RotationAngle = 0;
  Image18.RotationCenterLeft = 354;
  Image18.RotationCenterTop = 446;
  Image18.ScaleX        = 1.0000;
  Image18.ScaleY        = 1.0000;
  Image18.Active        = 1;
  Image18.OnUp          = 0;
  Image18.OnDown        = 0;
  Image18.OnClick       = 0;
  Image18.OnPress       = 0;

  Image17.OwnerScreen   = &Home;
  Image17.Order         = 40;
  Image17.Visible       = 1;
  Image17.Opacity       = 255;
  Image17.Tag           = 255;
  Image17.Left          = 231;
  Image17.Top           = 400;
  Image17.Width         = 48;
  Image17.Height        = 40;
  Image17.Picture_Name  = unlock_bmp;
  Image17.Picture_Type  = 7;
  Image17.Picture_Ratio = 1;
  Image17.Blend_Color   = 0xFFFFFF;
  Image17.Source        = -1L;
  Image17.RotationAngle = 0;
  Image17.RotationCenterLeft = 255;
  Image17.RotationCenterTop = 446;
  Image17.ScaleX        = 1.0000;
  Image17.ScaleY        = 1.0000;
  Image17.Active        = 1;
  Image17.OnUp          = 0;
  Image17.OnDown        = 0;
  Image17.OnClick       = 0;
  Image17.OnPress       = 0;

  Image16.OwnerScreen   = &Home;
  Image16.Order         = 41;
  Image16.Visible       = 1;
  Image16.Opacity       = 255;
  Image16.Tag           = 255;
  Image16.Left          = 132;
  Image16.Top           = 400;
  Image16.Width         = 48;
  Image16.Height        = 40;
  Image16.Picture_Name  = unlock_bmp;
  Image16.Picture_Type  = 7;
  Image16.Picture_Ratio = 1;
  Image16.Blend_Color   = 0xFFFFFF;
  Image16.Source        = -1L;
  Image16.RotationAngle = 0;
  Image16.RotationCenterLeft = 156;
  Image16.RotationCenterTop = 446;
  Image16.ScaleX        = 1.0000;
  Image16.ScaleY        = 1.0000;
  Image16.Active        = 1;
  Image16.OnUp          = 0;
  Image16.OnDown        = 0;
  Image16.OnClick       = 0;
  Image16.OnPress       = 0;

  Image9.OwnerScreen   = &Home;
  Image9.Order         = 42;
  Image9.Visible       = 1;
  Image9.Opacity       = 255;
  Image9.Tag           = 255;
  Image9.Left          = 33;
  Image9.Top           = 400;
  Image9.Width         = 48;
  Image9.Height        = 40;
  Image9.Picture_Name  = unlock_bmp;
  Image9.Picture_Type  = 7;
  Image9.Picture_Ratio = 1;
  Image9.Blend_Color   = 0xFFFFFF;
  Image9.Source        = -1L;
  Image9.RotationAngle = 0;
  Image9.RotationCenterLeft = 57;
  Image9.RotationCenterTop = 446;
  Image9.ScaleX        = 1.0000;
  Image9.ScaleY        = 1.0000;
  Image9.Active        = 1;
  Image9.OnUp          = 0;
  Image9.OnDown        = 0;
  Image9.OnClick       = 0;
  Image9.OnPress       = 0;

  Image8.OwnerScreen   = &Home;
  Image8.Order         = 43;
  Image8.Visible       = 1;
  Image8.Opacity       = 255;
  Image8.Tag           = 255;
  Image8.Left          = 474;
  Image8.Top           = 277;
  Image8.Width         = 54;
  Image8.Height        = 40;
  Image8.Picture_Name  = baterry1_bmp;
  Image8.Picture_Type  = 7;
  Image8.Picture_Ratio = 1;
  Image8.Blend_Color   = 0xFFFFFF;
  Image8.Source        = -1L;
  Image8.RotationAngle = 0;
  Image8.RotationCenterLeft = 501;
  Image8.RotationCenterTop = 297;
  Image8.ScaleX        = 1.0000;
  Image8.ScaleY        = 1.0000;
  Image8.Active        = 1;
  Image8.OnUp          = 0;
  Image8.OnDown        = 0;
  Image8.OnClick       = 0;
  Image8.OnPress       = 0;

  Image7.OwnerScreen   = &Home;
  Image7.Order         = 44;
  Image7.Visible       = 1;
  Image7.Opacity       = 255;
  Image7.Tag           = 255;
  Image7.Left          = 283;
  Image7.Top           = 276;
  Image7.Width         = 42;
  Image7.Height        = 40;
  Image7.Picture_Name  = smile1_bmp;
  Image7.Picture_Type  = 7;
  Image7.Picture_Ratio = 1;
  Image7.Blend_Color   = 0xFFFFFF;
  Image7.Source        = -1L;
  Image7.RotationAngle = 0;
  Image7.RotationCenterLeft = 304;
  Image7.RotationCenterTop = 296;
  Image7.ScaleX        = 1.0000;
  Image7.ScaleY        = 1.0000;
  Image7.Active        = 0;
  Image7.OnUp          = 0;
  Image7.OnDown        = 0;
  Image7.OnClick       = 0;
  Image7.OnPress       = 0;

  Image6.OwnerScreen   = &Home;
  Image6.Order         = 45;
  Image6.Visible       = 1;
  Image6.Opacity       = 255;
  Image6.Tag           = 255;
  Image6.Left          = 86;
  Image6.Top           = 276;
  Image6.Width         = 42;
  Image6.Height        = 40;
  Image6.Picture_Name  = ring_bmp;
  Image6.Picture_Type  = 7;
  Image6.Picture_Ratio = 1;
  Image6.Blend_Color   = 0xFFFFFF;
  Image6.Source        = -1L;
  Image6.RotationAngle = 0;
  Image6.RotationCenterLeft = 107;
  Image6.RotationCenterTop = 296;
  Image6.ScaleX        = 1.0000;
  Image6.ScaleY        = 1.0000;
  Image6.Active        = 0;
  Image6.OnUp          = 0;
  Image6.OnDown        = 0;
  Image6.OnClick       = 0;
  Image6.OnPress       = 0;

  Image15.OwnerScreen   = &Home;
  Image15.Order         = 46;
  Image15.Visible       = 1;
  Image15.Opacity       = 255;
  Image15.Tag           = 255;
  Image15.Left          = 727;
  Image15.Top           = 164;
  Image15.Width         = 41;
  Image15.Height        = 40;
  Image15.Picture_Name  = light1_bmp;
  Image15.Picture_Type  = 7;
  Image15.Picture_Ratio = 1;
  Image15.Blend_Color   = 0xFFFFFF;
  Image15.Source        = -1L;
  Image15.RotationAngle = 0;
  Image15.RotationCenterLeft = 748;
  Image15.RotationCenterTop = 184;
  Image15.ScaleX        = 1.0000;
  Image15.ScaleY        = 1.0000;
  Image15.Active        = 0;
  Image15.OnUp          = 0;
  Image15.OnDown        = 0;
  Image15.OnClick       = 0;
  Image15.OnPress       = 0;

  Image14.OwnerScreen   = &Home;
  Image14.Order         = 47;
  Image14.Visible       = 1;
  Image14.Opacity       = 255;
  Image14.Tag           = 255;
  Image14.Left          = 629;
  Image14.Top           = 165;
  Image14.Width         = 41;
  Image14.Height        = 40;
  Image14.Picture_Name  = light1_bmp;
  Image14.Picture_Type  = 7;
  Image14.Picture_Ratio = 1;
  Image14.Blend_Color   = 0xFFFFFF;
  Image14.Source        = -1L;
  Image14.RotationAngle = 0;
  Image14.RotationCenterLeft = 649;
  Image14.RotationCenterTop = 185;
  Image14.ScaleX        = 1.0000;
  Image14.ScaleY        = 1.0000;
  Image14.Active        = 0;
  Image14.OnUp          = 0;
  Image14.OnDown        = 0;
  Image14.OnClick       = 0;
  Image14.OnPress       = 0;

  Image13.OwnerScreen   = &Home;
  Image13.Order         = 48;
  Image13.Visible       = 1;
  Image13.Opacity       = 255;
  Image13.Tag           = 255;
  Image13.Left          = 727;
  Image13.Top           = 86;
  Image13.Width         = 41;
  Image13.Height        = 40;
  Image13.Picture_Name  = light1_bmp;
  Image13.Picture_Type  = 7;
  Image13.Picture_Ratio = 1;
  Image13.Blend_Color   = 0xFFFFFF;
  Image13.Source        = -1L;
  Image13.RotationAngle = 0;
  Image13.RotationCenterLeft = 748;
  Image13.RotationCenterTop = 106;
  Image13.ScaleX        = 1.0000;
  Image13.ScaleY        = 1.0000;
  Image13.Active        = 0;
  Image13.OnUp          = 0;
  Image13.OnDown        = 0;
  Image13.OnClick       = 0;
  Image13.OnPress       = 0;

  Image12.OwnerScreen   = &Home;
  Image12.Order         = 49;
  Image12.Visible       = 1;
  Image12.Opacity       = 255;
  Image12.Tag           = 255;
  Image12.Left          = 629;
  Image12.Top           = 87;
  Image12.Width         = 41;
  Image12.Height        = 40;
  Image12.Picture_Name  = light1_bmp;
  Image12.Picture_Type  = 7;
  Image12.Picture_Ratio = 1;
  Image12.Blend_Color   = 0xFFFFFF;
  Image12.Source        = -1L;
  Image12.RotationAngle = 0;
  Image12.RotationCenterLeft = 649;
  Image12.RotationCenterTop = 107;
  Image12.ScaleX        = 1.0000;
  Image12.ScaleY        = 1.0000;
  Image12.Active        = 0;
  Image12.OnUp          = 0;
  Image12.OnDown        = 0;
  Image12.OnClick       = 0;
  Image12.OnPress       = 0;

  Image11.OwnerScreen   = &Home;
  Image11.Order         = 50;
  Image11.Visible       = 1;
  Image11.Opacity       = 255;
  Image11.Tag           = 255;
  Image11.Left          = 727;
  Image11.Top           = 12;
  Image11.Width         = 41;
  Image11.Height        = 40;
  Image11.Picture_Name  = light1_bmp;
  Image11.Picture_Type  = 7;
  Image11.Picture_Ratio = 1;
  Image11.Blend_Color   = 0xFFFFFF;
  Image11.Source        = -1L;
  Image11.RotationAngle = 0;
  Image11.RotationCenterLeft = 748;
  Image11.RotationCenterTop = 32;
  Image11.ScaleX        = 1.0000;
  Image11.ScaleY        = 1.0000;
  Image11.Active        = 0;
  Image11.OnUp          = 0;
  Image11.OnDown        = 0;
  Image11.OnClick       = 0;
  Image11.OnPress       = 0;

  Image10.OwnerScreen   = &Home;
  Image10.Order         = 51;
  Image10.Visible       = 1;
  Image10.Opacity       = 255;
  Image10.Tag           = 255;
  Image10.Left          = 629;
  Image10.Top           = 13;
  Image10.Width         = 41;
  Image10.Height        = 40;
  Image10.Picture_Name  = light1_bmp;
  Image10.Picture_Type  = 7;
  Image10.Picture_Ratio = 1;
  Image10.Blend_Color   = 0xFFFFFF;
  Image10.Source        = -1L;
  Image10.RotationAngle = 0;
  Image10.RotationCenterLeft = 649;
  Image10.RotationCenterTop = 33;
  Image10.ScaleX        = 1.0000;
  Image10.ScaleY        = 1.0000;
  Image10.Active        = 0;
  Image10.OnUp          = 0;
  Image10.OnDown        = 0;
  Image10.OnClick       = 0;
  Image10.OnPress       = 0;

  Image3.OwnerScreen   = &Home;
  Image3.Order         = 52;
  Image3.Visible       = 1;
  Image3.Opacity       = 255;
  Image3.Tag           = 255;
  Image3.Left          = 280;
  Image3.Top           = 48;
  Image3.Width         = 43;
  Image3.Height        = 40;
  Image3.Picture_Name  = sun1_bmp;
  Image3.Picture_Type  = 7;
  Image3.Picture_Ratio = 1;
  Image3.Blend_Color   = 0xFFFFFF;
  Image3.Source        = -1L;
  Image3.RotationAngle = 0;
  Image3.RotationCenterLeft = 309;
  Image3.RotationCenterTop = 74;
  Image3.ScaleX        = 1.3300;
  Image3.ScaleY        = 1.3300;
  Image3.Active        = 0;
  Image3.OnUp          = 0;
  Image3.OnDown        = 0;
  Image3.OnClick       = 0;
  Image3.OnPress       = 0;

  Image4.OwnerScreen   = &Home;
  Image4.Order         = 53;
  Image4.Visible       = 1;
  Image4.Opacity       = 255;
  Image4.Tag           = 255;
  Image4.Left          = 83;
  Image4.Top           = 161;
  Image4.Width         = 49;
  Image4.Height        = 40;
  Image4.Picture_Name  = note_bmp;
  Image4.Picture_Type  = 7;
  Image4.Picture_Ratio = 1;
  Image4.Blend_Color   = 0xFFFFFF;
  Image4.Source        = -1L;
  Image4.RotationAngle = 0;
  Image4.RotationCenterLeft = 108;
  Image4.RotationCenterTop = 181;
  Image4.ScaleX        = 1.0000;
  Image4.ScaleY        = 1.0000;
  Image4.Active        = 1;
  Image4.OnUp          = 0;
  Image4.OnDown        = 0;
  Image4.OnClick       = &Image4_OnClick;
  Image4.OnPress       = 0;

  Image4_OnClick.Action       = Box31OnClick;
  Image4_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Image4_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Image4_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Image4_OnClick.Sound.Volume = 255;

  Image38.OwnerScreen   = &Home;
  Image38.Order         = 54;
  Image38.Visible       = 0;
  Image38.Opacity       = 255;
  Image38.Tag           = 255;
  Image38.Left          = 280;
  Image38.Top           = 48;
  Image38.Width         = 43;
  Image38.Height        = 40;
  Image38.Picture_Name  = sun2_bmp;
  Image38.Picture_Type  = 7;
  Image38.Picture_Ratio = 1;
  Image38.Blend_Color   = 0xFFFFFF;
  Image38.Source        = -1L;
  Image38.RotationAngle = 0;
  Image38.RotationCenterLeft = 309;
  Image38.RotationCenterTop = 74;
  Image38.ScaleX        = 1.3300;
  Image38.ScaleY        = 1.3300;
  Image38.Active        = 0;
  Image38.OnUp          = 0;
  Image38.OnDown        = 0;
  Image38.OnClick       = 0;
  Image38.OnPress       = 0;

  Image37.OwnerScreen   = &Home;
  Image37.Order         = 55;
  Image37.Visible       = 0;
  Image37.Opacity       = 255;
  Image37.Tag           = 255;
  Image37.Left          = 474;
  Image37.Top           = 277;
  Image37.Width         = 54;
  Image37.Height        = 40;
  Image37.Picture_Name  = baterry2_bmp;
  Image37.Picture_Type  = 7;
  Image37.Picture_Ratio = 1;
  Image37.Blend_Color   = 0xFFFFFF;
  Image37.Source        = -1L;
  Image37.RotationAngle = 0;
  Image37.RotationCenterLeft = 501;
  Image37.RotationCenterTop = 297;
  Image37.ScaleX        = 1.0000;
  Image37.ScaleY        = 1.0000;
  Image37.Active        = 0;
  Image37.OnUp          = 0;
  Image37.OnDown        = 0;
  Image37.OnClick       = &Image37_OnClick;
  Image37.OnPress       = 0;

  Image37_OnClick.Action       = Box21OnClick;
  Image37_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Image37_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Image37_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Image37_OnClick.Sound.Volume = 255;

  Image36.OwnerScreen   = &Home;
  Image36.Order         = 56;
  Image36.Visible       = 0;
  Image36.Opacity       = 255;
  Image36.Tag           = 255;
  Image36.Left          = 283;
  Image36.Top           = 276;
  Image36.Width         = 42;
  Image36.Height        = 40;
  Image36.Picture_Name  = smile2_bmp;
  Image36.Picture_Type  = 7;
  Image36.Picture_Ratio = 1;
  Image36.Blend_Color   = 0xFFFFFF;
  Image36.Source        = -1L;
  Image36.RotationAngle = 0;
  Image36.RotationCenterLeft = 304;
  Image36.RotationCenterTop = 296;
  Image36.ScaleX        = 1.0000;
  Image36.ScaleY        = 1.0000;
  Image36.Active        = 0;
  Image36.OnUp          = 0;
  Image36.OnDown        = 0;
  Image36.OnClick       = &Image36_OnClick;
  Image36.OnPress       = 0;

  Image36_OnClick.Action       = Box20OnClick;
  Image36_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Image36_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Image36_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Image36_OnClick.Sound.Volume = 255;

  Image35.OwnerScreen   = &Home;
  Image35.Order         = 57;
  Image35.Visible       = 0;
  Image35.Opacity       = 255;
  Image35.Tag           = 255;
  Image35.Left          = 86;
  Image35.Top           = 276;
  Image35.Width         = 42;
  Image35.Height        = 40;
  Image35.Picture_Name  = ring2_bmp;
  Image35.Picture_Type  = 7;
  Image35.Picture_Ratio = 1;
  Image35.Blend_Color   = 0xFFFFFF;
  Image35.Source        = -1L;
  Image35.RotationAngle = 0;
  Image35.RotationCenterLeft = 107;
  Image35.RotationCenterTop = 296;
  Image35.ScaleX        = 1.0000;
  Image35.ScaleY        = 1.0000;
  Image35.Active        = 0;
  Image35.OnUp          = 0;
  Image35.OnDown        = 0;
  Image35.OnClick       = &Image35_OnClick;
  Image35.OnPress       = 0;

  Image35_OnClick.Action       = Box13OnClick;
  Image35_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Image35_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Image35_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Image35_OnClick.Sound.Volume = 255;

  Image29.OwnerScreen   = &Home;
  Image29.Order         = 58;
  Image29.Visible       = 0;
  Image29.Opacity       = 255;
  Image29.Tag           = 255;
  Image29.Left          = 37;
  Image29.Top           = 400;
  Image29.Width         = 40;
  Image29.Height        = 40;
  Image29.Picture_Name  = lock_bmp;
  Image29.Picture_Type  = 7;
  Image29.Picture_Ratio = 1;
  Image29.Blend_Color   = 0xFFFFFF;
  Image29.Source        = -1L;
  Image29.RotationAngle = 0;
  Image29.RotationCenterLeft = 57;
  Image29.RotationCenterTop = 446;
  Image29.ScaleX        = 1.0000;
  Image29.ScaleY        = 1.0000;
  Image29.Active        = 1;
  Image29.OnUp          = 0;
  Image29.OnDown        = 0;
  Image29.OnClick       = 0;
  Image29.OnPress       = 0;

  Image30.OwnerScreen   = &Home;
  Image30.Order         = 59;
  Image30.Visible       = 0;
  Image30.Opacity       = 255;
  Image30.Tag           = 255;
  Image30.Left          = 136;
  Image30.Top           = 400;
  Image30.Width         = 40;
  Image30.Height        = 40;
  Image30.Picture_Name  = lock_bmp;
  Image30.Picture_Type  = 7;
  Image30.Picture_Ratio = 1;
  Image30.Blend_Color   = 0xFFFFFF;
  Image30.Source        = -1L;
  Image30.RotationAngle = 0;
  Image30.RotationCenterLeft = 156;
  Image30.RotationCenterTop = 446;
  Image30.ScaleX        = 1.0000;
  Image30.ScaleY        = 1.0000;
  Image30.Active        = 1;
  Image30.OnUp          = 0;
  Image30.OnDown        = 0;
  Image30.OnClick       = 0;
  Image30.OnPress       = 0;

  Image31.OwnerScreen   = &Home;
  Image31.Order         = 60;
  Image31.Visible       = 0;
  Image31.Opacity       = 255;
  Image31.Tag           = 255;
  Image31.Left          = 235;
  Image31.Top           = 400;
  Image31.Width         = 40;
  Image31.Height        = 40;
  Image31.Picture_Name  = lock_bmp;
  Image31.Picture_Type  = 7;
  Image31.Picture_Ratio = 1;
  Image31.Blend_Color   = 0xFFFFFF;
  Image31.Source        = -1L;
  Image31.RotationAngle = 0;
  Image31.RotationCenterLeft = 255;
  Image31.RotationCenterTop = 446;
  Image31.ScaleX        = 1.0000;
  Image31.ScaleY        = 1.0000;
  Image31.Active        = 1;
  Image31.OnUp          = 0;
  Image31.OnDown        = 0;
  Image31.OnClick       = 0;
  Image31.OnPress       = 0;

  Image32.OwnerScreen   = &Home;
  Image32.Order         = 61;
  Image32.Visible       = 0;
  Image32.Opacity       = 255;
  Image32.Tag           = 255;
  Image32.Left          = 334;
  Image32.Top           = 400;
  Image32.Width         = 40;
  Image32.Height        = 40;
  Image32.Picture_Name  = lock_bmp;
  Image32.Picture_Type  = 7;
  Image32.Picture_Ratio = 1;
  Image32.Blend_Color   = 0xFFFFFF;
  Image32.Source        = -1L;
  Image32.RotationAngle = 0;
  Image32.RotationCenterLeft = 354;
  Image32.RotationCenterTop = 446;
  Image32.ScaleX        = 1.0000;
  Image32.ScaleY        = 1.0000;
  Image32.Active        = 1;
  Image32.OnUp          = 0;
  Image32.OnDown        = 0;
  Image32.OnClick       = 0;
  Image32.OnPress       = 0;

  Image33.OwnerScreen   = &Home;
  Image33.Order         = 62;
  Image33.Visible       = 0;
  Image33.Opacity       = 255;
  Image33.Tag           = 255;
  Image33.Left          = 432;
  Image33.Top           = 400;
  Image33.Width         = 40;
  Image33.Height        = 40;
  Image33.Picture_Name  = lock_bmp;
  Image33.Picture_Type  = 7;
  Image33.Picture_Ratio = 1;
  Image33.Blend_Color   = 0xFFFFFF;
  Image33.Source        = -1L;
  Image33.RotationAngle = 0;
  Image33.RotationCenterLeft = 452;
  Image33.RotationCenterTop = 446;
  Image33.ScaleX        = 1.0000;
  Image33.ScaleY        = 1.0000;
  Image33.Active        = 1;
  Image33.OnUp          = 0;
  Image33.OnDown        = 0;
  Image33.OnClick       = 0;
  Image33.OnPress       = 0;

  Image34.OwnerScreen   = &Home;
  Image34.Order         = 63;
  Image34.Visible       = 0;
  Image34.Opacity       = 255;
  Image34.Tag           = 255;
  Image34.Left          = 531;
  Image34.Top           = 400;
  Image34.Width         = 40;
  Image34.Height        = 40;
  Image34.Picture_Name  = lock_bmp;
  Image34.Picture_Type  = 7;
  Image34.Picture_Ratio = 1;
  Image34.Blend_Color   = 0xFFFFFF;
  Image34.Source        = -1L;
  Image34.RotationAngle = 0;
  Image34.RotationCenterLeft = 551;
  Image34.RotationCenterTop = 446;
  Image34.ScaleX        = 1.0000;
  Image34.ScaleY        = 1.0000;
  Image34.Active        = 1;
  Image34.OnUp          = 0;
  Image34.OnDown        = 0;
  Image34.OnClick       = 0;
  Image34.OnPress       = 0;

  Image27.OwnerScreen   = &Home;
  Image27.Order         = 64;
  Image27.Visible       = 0;
  Image27.Opacity       = 255;
  Image27.Tag           = 255;
  Image27.Left          = 676;
  Image27.Top           = 400;
  Image27.Width         = 44;
  Image27.Height        = 40;
  Image27.Picture_Name  = moon2_bmp;
  Image27.Picture_Type  = 7;
  Image27.Picture_Ratio = 1;
  Image27.Blend_Color   = 0xFFFFFF;
  Image27.Source        = -1L;
  Image27.RotationAngle = 0;
  Image27.RotationCenterLeft = 698;
  Image27.RotationCenterTop = 446;
  Image27.ScaleX        = 1.0000;
  Image27.ScaleY        = 1.0000;
  Image27.Active        = 0;
  Image27.OnUp          = 0;
  Image27.OnDown        = 0;
  Image27.OnClick       = 0;
  Image27.OnPress       = 0;

  Image25.OwnerScreen   = &Home;
  Image25.Order         = 65;
  Image25.Visible       = 0;
  Image25.Opacity       = 255;
  Image25.Tag           = 255;
  Image25.Left          = 629;
  Image25.Top           = 165;
  Image25.Width         = 41;
  Image25.Height        = 40;
  Image25.Picture_Name  = light2_bmp;
  Image25.Picture_Type  = 7;
  Image25.Picture_Ratio = 1;
  Image25.Blend_Color   = 0xFFFFFF;
  Image25.Source        = -1L;
  Image25.RotationAngle = 0;
  Image25.RotationCenterLeft = 657;
  Image25.RotationCenterTop = 185;
  Image25.ScaleX        = 1.0000;
  Image25.ScaleY        = 1.0000;
  Image25.Active        = 0;
  Image25.OnUp          = 0;
  Image25.OnDown        = 0;
  Image25.OnClick       = 0;
  Image25.OnPress       = 0;

  Image26.OwnerScreen   = &Home;
  Image26.Order         = 66;
  Image26.Visible       = 0;
  Image26.Opacity       = 255;
  Image26.Tag           = 255;
  Image26.Left          = 727;
  Image26.Top           = 164;
  Image26.Width         = 41;
  Image26.Height        = 40;
  Image26.Picture_Name  = light2_bmp;
  Image26.Picture_Type  = 7;
  Image26.Picture_Ratio = 1;
  Image26.Blend_Color   = 0xFFFFFF;
  Image26.Source        = -1L;
  Image26.RotationAngle = 0;
  Image26.RotationCenterLeft = 756;
  Image26.RotationCenterTop = 184;
  Image26.ScaleX        = 1.0000;
  Image26.ScaleY        = 1.0000;
  Image26.Active        = 0;
  Image26.OnUp          = 0;
  Image26.OnDown        = 0;
  Image26.OnClick       = 0;
  Image26.OnPress       = 0;

  Image23.OwnerScreen   = &Home;
  Image23.Order         = 67;
  Image23.Visible       = 0;
  Image23.Opacity       = 255;
  Image23.Tag           = 255;
  Image23.Left          = 629;
  Image23.Top           = 87;
  Image23.Width         = 41;
  Image23.Height        = 40;
  Image23.Picture_Name  = light2_bmp;
  Image23.Picture_Type  = 7;
  Image23.Picture_Ratio = 1;
  Image23.Blend_Color   = 0xFFFFFF;
  Image23.Source        = -1L;
  Image23.RotationAngle = 0;
  Image23.RotationCenterLeft = 657;
  Image23.RotationCenterTop = 107;
  Image23.ScaleX        = 1.0000;
  Image23.ScaleY        = 1.0000;
  Image23.Active        = 0;
  Image23.OnUp          = 0;
  Image23.OnDown        = 0;
  Image23.OnClick       = 0;
  Image23.OnPress       = 0;

  Image24.OwnerScreen   = &Home;
  Image24.Order         = 68;
  Image24.Visible       = 0;
  Image24.Opacity       = 255;
  Image24.Tag           = 255;
  Image24.Left          = 727;
  Image24.Top           = 86;
  Image24.Width         = 41;
  Image24.Height        = 40;
  Image24.Picture_Name  = light2_bmp;
  Image24.Picture_Type  = 7;
  Image24.Picture_Ratio = 1;
  Image24.Blend_Color   = 0xFFFFFF;
  Image24.Source        = -1L;
  Image24.RotationAngle = 0;
  Image24.RotationCenterLeft = 756;
  Image24.RotationCenterTop = 106;
  Image24.ScaleX        = 1.0000;
  Image24.ScaleY        = 1.0000;
  Image24.Active        = 0;
  Image24.OnUp          = 0;
  Image24.OnDown        = 0;
  Image24.OnClick       = 0;
  Image24.OnPress       = 0;

  Image21.OwnerScreen   = &Home;
  Image21.Order         = 69;
  Image21.Visible       = 0;
  Image21.Opacity       = 255;
  Image21.Tag           = 255;
  Image21.Left          = 629;
  Image21.Top           = 13;
  Image21.Width         = 41;
  Image21.Height        = 40;
  Image21.Picture_Name  = light2_bmp;
  Image21.Picture_Type  = 7;
  Image21.Picture_Ratio = 1;
  Image21.Blend_Color   = 0xFFFFFF;
  Image21.Source        = -1L;
  Image21.RotationAngle = 0;
  Image21.RotationCenterLeft = 657;
  Image21.RotationCenterTop = 33;
  Image21.ScaleX        = 1.0000;
  Image21.ScaleY        = 1.0000;
  Image21.Active        = 0;
  Image21.OnUp          = 0;
  Image21.OnDown        = 0;
  Image21.OnClick       = 0;
  Image21.OnPress       = 0;

  Image22.OwnerScreen   = &Home;
  Image22.Order         = 70;
  Image22.Visible       = 0;
  Image22.Opacity       = 255;
  Image22.Tag           = 255;
  Image22.Left          = 727;
  Image22.Top           = 12;
  Image22.Width         = 41;
  Image22.Height        = 40;
  Image22.Picture_Name  = light2_bmp;
  Image22.Picture_Type  = 7;
  Image22.Picture_Ratio = 1;
  Image22.Blend_Color   = 0xFFFFFF;
  Image22.Source        = -1L;
  Image22.RotationAngle = 0;
  Image22.RotationCenterLeft = 756;
  Image22.RotationCenterTop = 32;
  Image22.ScaleX        = 1.0000;
  Image22.ScaleY        = 1.0000;
  Image22.Active        = 0;
  Image22.OnUp          = 0;
  Image22.OnDown        = 0;
  Image22.OnClick       = 0;
  Image22.OnPress       = 0;

  Label1.OwnerScreen = &Home;
  Label1.Order       = 71;
  Label1.Visible     = 1;
  Label1.Opacity     = 255;
  Label1.Tag         = 255;
  Label1.Left        = 309;
  Label1.Top         = 159;
  Label1.Width       = 82;
  Label1.Height      = 17;
  Label1.Caption     = Label1_Caption;
  Label1.FontName    = 27;
  Label1.Font_Color  = 0xFFFFFF;
  Label1.FontHandle  = 27;
  Label1.Source      = -1UL;
  Label1.Active      = 0;
  Label1.OnUp        = 0;
  Label1.OnDown      = 0;
  Label1.OnClick     = 0;
  Label1.OnPress     = 0;

  Label2.OwnerScreen = &Home;
  Label2.Order       = 72;
  Label2.Visible     = 1;
  Label2.Opacity     = 255;
  Label2.Tag         = 255;
  Label2.Left        = 380;
  Label2.Top         = 191;
  Label2.Width       = 38;
  Label2.Height      = 17;
  Label2.Caption     = Label2_Caption;
  Label2.FontName    = 27;
  Label2.Font_Color  = 0xFFFFFF;
  Label2.FontHandle  = 27;
  Label2.Source      = -1UL;
  Label2.Active      = 0;
  Label2.OnUp        = 0;
  Label2.OnDown      = 0;
  Label2.OnClick     = 0;
  Label2.OnPress     = 0;

  Label3.OwnerScreen = &Home;
  Label3.Order       = 73;
  Label3.Visible     = 1;
  Label3.Opacity     = 255;
  Label3.Tag         = 255;
  Label3.Left        = 646;
  Label3.Top         = 251;
  Label3.Width       = 82;
  Label3.Height      = 17;
  Label3.Caption     = Label3_Caption;
  Label3.FontName    = 27;
  Label3.Font_Color  = 0xFFFFFF;
  Label3.FontHandle  = 27;
  Label3.Source      = -1UL;
  Label3.Active      = 0;
  Label3.OnUp        = 0;
  Label3.OnDown      = 0;
  Label3.OnClick     = 0;
  Label3.OnPress     = 0;

  Label4.OwnerScreen = &Home;
  Label4.Order       = 74;
  Label4.Visible     = 1;
  Label4.Opacity     = 255;
  Label4.Tag         = 255;
  Label4.Left        = 700;
  Label4.Top         = 310;
  Label4.Width       = 50;
  Label4.Height      = 21;
  Label4.Caption     = Label4_Caption;
  Label4.FontName    = 28;
  Label4.Font_Color  = 0xFFFFFF;
  Label4.FontHandle  = 28;
  Label4.Source      = -1UL;
  Label4.Active      = 0;
  Label4.OnUp        = 0;
  Label4.OnDown      = 0;
  Label4.OnClick     = 0;
  Label4.OnPress     = 0;

  Label5.OwnerScreen = &Home;
  Label5.Order       = 75;
  Label5.Visible     = 1;
  Label5.Opacity     = 255;
  Label5.Tag         = 255;
  Label5.Left        = 676;
  Label5.Top         = 449;
  Label5.Width       = 33;
  Label5.Height      = 17;
  Label5.Caption     = Label5_Caption;
  Label5.FontName    = 27;
  Label5.Font_Color  = 0xFFFFFF;
  Label5.FontHandle  = 27;
  Label5.Source      = -1UL;
  Label5.Active      = 0;
  Label5.OnUp        = 0;
  Label5.OnDown      = 0;
  Label5.OnClick     = 0;
  Label5.OnPress     = 0;

  Label7.OwnerScreen = &Home;
  Label7.Order       = 76;
  Label7.Visible     = 1;
  Label7.Opacity     = 255;
  Label7.Tag         = 255;
  Label7.Left        = 75;
  Label7.Top         = 251;
  Label7.Width       = 49;
  Label7.Height      = 17;
  Label7.Caption     = Label7_Caption;
  Label7.FontName    = 27;
  Label7.Font_Color  = 0xFFFFFF;
  Label7.FontHandle  = 27;
  Label7.Source      = -1UL;
  Label7.Active      = 0;
  Label7.OnUp        = 0;
  Label7.OnDown      = 0;
  Label7.OnClick     = 0;
  Label7.OnPress     = 0;

  Label8.OwnerScreen = &Home;
  Label8.Order       = 77;
  Label8.Visible     = 1;
  Label8.Opacity     = 255;
  Label8.Tag         = 255;
  Label8.Left        = 65;
  Label8.Top         = 334;
  Label8.Width       = 66;
  Label8.Height      = 17;
  Label8.Caption     = Label8_Caption;
  Label8.FontName    = 27;
  Label8.Font_Color  = 0xFFFFFF;
  Label8.FontHandle  = 27;
  Label8.Source      = -1UL;
  Label8.Active      = 0;
  Label8.OnUp        = 0;
  Label8.OnDown      = 0;
  Label8.OnClick     = 0;
  Label8.OnPress     = 0;

  Label9.OwnerScreen = &Home;
  Label9.Order       = 78;
  Label9.Visible     = 1;
  Label9.Opacity     = 255;
  Label9.Tag         = 255;
  Label9.Left        = 25;
  Label9.Top         = 448;
  Label9.Width       = 53;
  Label9.Height      = 17;
  Label9.Caption     = Label9_Caption;
  Label9.FontName    = 27;
  Label9.Font_Color  = 0xFFFFFF;
  Label9.FontHandle  = 27;
  Label9.Source      = -1UL;
  Label9.Active      = 1;
  Label9.OnUp        = 0;
  Label9.OnDown      = 0;
  Label9.OnClick     = 0;
  Label9.OnPress     = 0;

  Label10.OwnerScreen = &Home;
  Label10.Order       = 79;
  Label10.Visible     = 1;
  Label10.Opacity     = 255;
  Label10.Tag         = 255;
  Label10.Left        = 129;
  Label10.Top         = 448;
  Label10.Width       = 43;
  Label10.Height      = 17;
  Label10.Caption     = Label10_Caption;
  Label10.FontName    = 27;
  Label10.Font_Color  = 0xFFFFFF;
  Label10.FontHandle  = 27;
  Label10.Source      = -1UL;
  Label10.Active      = 1;
  Label10.OnUp        = 0;
  Label10.OnDown      = 0;
  Label10.OnClick     = 0;
  Label10.OnPress     = 0;

  Label11.OwnerScreen = &Home;
  Label11.Order       = 80;
  Label11.Visible     = 1;
  Label11.Opacity     = 255;
  Label11.Tag         = 255;
  Label11.Left        = 228;
  Label11.Top         = 448;
  Label11.Width       = 43;
  Label11.Height      = 17;
  Label11.Caption     = Label11_Caption;
  Label11.FontName    = 27;
  Label11.Font_Color  = 0xFFFFFF;
  Label11.FontHandle  = 27;
  Label11.Source      = -1UL;
  Label11.Active      = 1;
  Label11.OnUp        = 0;
  Label11.OnDown      = 0;
  Label11.OnClick     = 0;
  Label11.OnPress     = 0;

  Label12.OwnerScreen = &Home;
  Label12.Order       = 81;
  Label12.Visible     = 1;
  Label12.Opacity     = 255;
  Label12.Tag         = 255;
  Label12.Left        = 326;
  Label12.Top         = 448;
  Label12.Width       = 46;
  Label12.Height      = 17;
  Label12.Caption     = Label12_Caption;
  Label12.FontName    = 27;
  Label12.Font_Color  = 0xFFFFFF;
  Label12.FontHandle  = 27;
  Label12.Source      = -1UL;
  Label12.Active      = 1;
  Label12.OnUp        = 0;
  Label12.OnDown      = 0;
  Label12.OnClick     = 0;
  Label12.OnPress     = 0;

  Label13.OwnerScreen = &Home;
  Label13.Order       = 82;
  Label13.Visible     = 1;
  Label13.Opacity     = 255;
  Label13.Tag         = 255;
  Label13.Left        = 433;
  Label13.Top         = 448;
  Label13.Width       = 28;
  Label13.Height      = 17;
  Label13.Caption     = Label13_Caption;
  Label13.FontName    = 27;
  Label13.Font_Color  = 0xFFFFFF;
  Label13.FontHandle  = 27;
  Label13.Source      = -1UL;
  Label13.Active      = 1;
  Label13.OnUp        = 0;
  Label13.OnDown      = 0;
  Label13.OnClick     = 0;
  Label13.OnPress     = 0;

  Label14.OwnerScreen = &Home;
  Label14.Order       = 83;
  Label14.Visible     = 1;
  Label14.Opacity     = 255;
  Label14.Tag         = 255;
  Label14.Left        = 528;
  Label14.Top         = 448;
  Label14.Width       = 31;
  Label14.Height      = 17;
  Label14.Caption     = Label14_Caption;
  Label14.FontName    = 27;
  Label14.Font_Color  = 0xFFFFFF;
  Label14.FontHandle  = 27;
  Label14.Source      = -1UL;
  Label14.Active      = 1;
  Label14.OnUp        = 0;
  Label14.OnDown      = 0;
  Label14.OnClick     = 0;
  Label14.OnPress     = 0;

  Label15.OwnerScreen = &Home;
  Label15.Order       = 84;
  Label15.Visible     = 1;
  Label15.Opacity     = 255;
  Label15.Tag         = 255;
  Label15.Left        = 259;
  Label15.Top         = 251;
  Label15.Width       = 71;
  Label15.Height      = 17;
  Label15.Caption     = Label15_Caption;
  Label15.FontName    = 27;
  Label15.Font_Color  = 0xFFFFFF;
  Label15.FontHandle  = 27;
  Label15.Source      = -1UL;
  Label15.Active      = 0;
  Label15.OnUp        = 0;
  Label15.OnDown      = 0;
  Label15.OnClick     = 0;
  Label15.OnPress     = 0;

  Label16.OwnerScreen = &Home;
  Label16.Order       = 85;
  Label16.Visible     = 1;
  Label16.Opacity     = 255;
  Label16.Tag         = 255;
  Label16.Left        = 267;
  Label16.Top         = 334;
  Label16.Width       = 61;
  Label16.Height      = 17;
  Label16.Caption     = Label16_Caption;
  Label16.FontName    = 27;
  Label16.Font_Color  = 0xFFFFFF;
  Label16.FontHandle  = 27;
  Label16.Source      = -1UL;
  Label16.Active      = 0;
  Label16.OnUp        = 0;
  Label16.OnDown      = 0;
  Label16.OnClick     = 0;
  Label16.OnPress     = 0;

  Label17.OwnerScreen = &Home;
  Label17.Order       = 86;
  Label17.Visible     = 1;
  Label17.Opacity     = 255;
  Label17.Tag         = 255;
  Label17.Left        = 458;
  Label17.Top         = 251;
  Label17.Width       = 70;
  Label17.Height      = 17;
  Label17.Caption     = Label17_Caption;
  Label17.FontName    = 27;
  Label17.Font_Color  = 0xFFFFFF;
  Label17.FontHandle  = 27;
  Label17.Source      = -1UL;
  Label17.Active      = 1;
  Label17.OnUp        = 0;
  Label17.OnDown      = 0;
  Label17.OnClick     = 0;
  Label17.OnPress     = 0;

  Label18.OwnerScreen = &Home;
  Label18.Order       = 87;
  Label18.Visible     = 1;
  Label18.Opacity     = 255;
  Label18.Tag         = 255;
  Label18.Left        = 440;
  Label18.Top         = 326;
  Label18.Width       = 67;
  Label18.Height      = 15;
  Label18.Caption     = Label18_Caption;
  Label18.FontName    = 26;
  Label18.Font_Color  = 0xFFFFFF;
  Label18.FontHandle  = 26;
  Label18.Source      = -1UL;
  Label18.Active      = 1;
  Label18.OnUp        = 0;
  Label18.OnDown      = 0;
  Label18.OnClick     = 0;
  Label18.OnPress     = 0;

  Label19.OwnerScreen = &Home;
  Label19.Order       = 88;
  Label19.Visible     = 1;
  Label19.Opacity     = 255;
  Label19.Tag         = 255;
  Label19.Left        = 440;
  Label19.Top         = 339;
  Label19.Width       = 40;
  Label19.Height      = 15;
  Label19.Caption     = Label19_Caption;
  Label19.FontName    = 26;
  Label19.Font_Color  = 0xFFFFFF;
  Label19.FontHandle  = 26;
  Label19.Source      = -1UL;
  Label19.Active      = 1;
  Label19.OnUp        = 0;
  Label19.OnDown      = 0;
  Label19.OnClick     = 0;
  Label19.OnPress     = 0;

  Label20.OwnerScreen = &Home;
  Label20.Order       = 89;
  Label20.Visible     = 1;
  Label20.Opacity     = 255;
  Label20.Tag         = 255;
  Label20.Left        = 440;
  Label20.Top         = 353;
  Label20.Width       = 47;
  Label20.Height      = 15;
  Label20.Caption     = Label20_Caption;
  Label20.FontName    = 26;
  Label20.Font_Color  = 0xFFFFFF;
  Label20.FontHandle  = 26;
  Label20.Source      = -1UL;
  Label20.Active      = 1;
  Label20.OnUp        = 0;
  Label20.OnDown      = 0;
  Label20.OnClick     = 0;
  Label20.OnPress     = 0;

  Label21.OwnerScreen = &Home;
  Label21.Order       = 90;
  Label21.Visible     = 1;
  Label21.Opacity     = 255;
  Label21.Tag         = 255;
  Label21.Left        = 520;
  Label21.Top         = 326;
  Label21.Width       = 32;
  Label21.Height      = 15;
  Label21.Caption     = Label21_Caption;
  Label21.FontName    = 26;
  Label21.Font_Color  = 0xFFFFFF;
  Label21.FontHandle  = 26;
  Label21.Source      = -1UL;
  Label21.Active      = 1;
  Label21.OnUp        = 0;
  Label21.OnDown      = 0;
  Label21.OnClick     = 0;
  Label21.OnPress     = 0;

  Label22.OwnerScreen = &Home;
  Label22.Order       = 91;
  Label22.Visible     = 1;
  Label22.Opacity     = 255;
  Label22.Tag         = 255;
  Label22.Left        = 520;
  Label22.Top         = 339;
  Label22.Width       = 32;
  Label22.Height      = 15;
  Label22.Caption     = Label22_Caption;
  Label22.FontName    = 26;
  Label22.Font_Color  = 0xFFFFFF;
  Label22.FontHandle  = 26;
  Label22.Source      = -1UL;
  Label22.Active      = 1;
  Label22.OnUp        = 0;
  Label22.OnDown      = 0;
  Label22.OnClick     = 0;
  Label22.OnPress     = 0;

  Label23.OwnerScreen = &Home;
  Label23.Order       = 92;
  Label23.Visible     = 1;
  Label23.Opacity     = 255;
  Label23.Tag         = 255;
  Label23.Left        = 520;
  Label23.Top         = 353;
  Label23.Width       = 46;
  Label23.Height      = 15;
  Label23.Caption     = Label23_Caption;
  Label23.FontName    = 26;
  Label23.Font_Color  = 0xFFFFFF;
  Label23.FontHandle  = 26;
  Label23.Source      = -1UL;
  Label23.Active      = 1;
  Label23.OnUp        = 0;
  Label23.OnDown      = 0;
  Label23.OnClick     = 0;
  Label23.OnPress     = 0;

  Box28.OwnerScreen   = &Home;
  Box28.Order         = 93;
  Box28.Visible       = 1;
  Box28.Opacity       = 255;
  Box28.Tag           = 255;
  Box28.Left          = 11;
  Box28.Top           = 6;
  Box28.Width         = 192;
  Box28.Height        = 148;
  Box28.Pen_Width     = 1;
  Box28.Pen_Color     = 0x0000;
  Box28.Color         = 0xFF0000;
  Box28.Press_Color   = 0xFFFFFF;
  Box28.ColorTo       = 0xFFFFFF;
  Box28.Press_ColorTo = 0xC0C0C0;
  Box28.Gradient      = _FT812_BRUSH_GR_NONE;
  Box28.Active        = 1;
  Box28.OnUp          = 0;
  Box28.OnDown        = 0;
  Box28.OnClick       = &Box28_OnClick;
  Box28.OnPress       = 0;

  Box28_OnClick.Action       = Box28OnClick;
  Box28_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Box28_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Box28_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Box28_OnClick.Sound.Volume = 255;

  Label35.OwnerScreen = &Home;
  Label35.Order       = 94;
  Label35.Visible     = 1;
  Label35.Opacity     = 255;
  Label35.Tag         = 255;
  Label35.Left        = 60;
  Label35.Top         = 207;
  Label35.Width       = 79;
  Label35.Height      = 17;
  Label35.Caption     = Label35_Caption;
  Label35.FontName    = 27;
  Label35.Font_Color  = 0xFFFFFF;
  Label35.FontHandle  = 27;
  Label35.Source      = -1UL;
  Label35.Active      = 1;
  Label35.OnUp        = 0;
  Label35.OnDown      = 0;
  Label35.OnClick     = 0;
  Label35.OnPress     = 0;

  Label36.OwnerScreen = &Home;
  Label36.Order       = 95;
  Label36.Visible     = 1;
  Label36.Opacity     = 255;
  Label36.Tag         = 255;
  Label36.Left        = 626;
  Label36.Top         = 56;
  Label36.Width       = 39;
  Label36.Height      = 17;
  Label36.Caption     = Label36_Caption;
  Label36.FontName    = 27;
  Label36.Font_Color  = 0xFFFFFF;
  Label36.FontHandle  = 27;
  Label36.Source      = -1UL;
  Label36.Active      = 0;
  Label36.OnUp        = 0;
  Label36.OnDown      = 0;
  Label36.OnClick     = 0;
  Label36.OnPress     = 0;

  Label37.OwnerScreen = &Home;
  Label37.Order       = 96;
  Label37.Visible     = 1;
  Label37.Opacity     = 255;
  Label37.Tag         = 255;
  Label37.Left        = 717;
  Label37.Top         = 56;
  Label37.Width       = 46;
  Label37.Height      = 17;
  Label37.Caption     = Label37_Caption;
  Label37.FontName    = 27;
  Label37.Font_Color  = 0xFFFFFF;
  Label37.FontHandle  = 27;
  Label37.Source      = -1UL;
  Label37.Active      = 0;
  Label37.OnUp        = 0;
  Label37.OnDown      = 0;
  Label37.OnClick     = 0;
  Label37.OnPress     = 0;

  Label38.OwnerScreen = &Home;
  Label38.Order       = 97;
  Label38.Visible     = 1;
  Label38.Opacity     = 255;
  Label38.Tag         = 255;
  Label38.Left        = 630;
  Label38.Top         = 131;
  Label38.Width       = 28;
  Label38.Height      = 17;
  Label38.Caption     = Label38_Caption;
  Label38.FontName    = 27;
  Label38.Font_Color  = 0xFFFFFF;
  Label38.FontHandle  = 27;
  Label38.Source      = -1UL;
  Label38.Active      = 0;
  Label38.OnUp        = 0;
  Label38.OnDown      = 0;
  Label38.OnClick     = 0;
  Label38.OnPress     = 0;

  Label39.OwnerScreen = &Home;
  Label39.Order       = 98;
  Label39.Visible     = 1;
  Label39.Opacity     = 255;
  Label39.Tag         = 255;
  Label39.Left        = 712;
  Label39.Top         = 131;
  Label39.Width       = 53;
  Label39.Height      = 17;
  Label39.Caption     = Label39_Caption;
  Label39.FontName    = 27;
  Label39.Font_Color  = 0xFFFFFF;
  Label39.FontHandle  = 27;
  Label39.Source      = -1UL;
  Label39.Active      = 0;
  Label39.OnUp        = 0;
  Label39.OnDown      = 0;
  Label39.OnClick     = 0;
  Label39.OnPress     = 0;

  Label40.OwnerScreen = &Home;
  Label40.Order       = 99;
  Label40.Visible     = 1;
  Label40.Opacity     = 255;
  Label40.Tag         = 255;
  Label40.Left        = 616;
  Label40.Top         = 207;
  Label40.Width       = 55;
  Label40.Height      = 17;
  Label40.Caption     = Label40_Caption;
  Label40.FontName    = 27;
  Label40.Font_Color  = 0xFFFFFF;
  Label40.FontHandle  = 27;
  Label40.Source      = -1UL;
  Label40.Active      = 0;
  Label40.OnUp        = 0;
  Label40.OnDown      = 0;
  Label40.OnClick     = 0;
  Label40.OnPress     = 0;

  Label41.OwnerScreen = &Home;
  Label41.Order       = 100;
  Label41.Visible     = 1;
  Label41.Opacity     = 255;
  Label41.Tag         = 255;
  Label41.Left        = 730;
  Label41.Top         = 207;
  Label41.Width       = 26;
  Label41.Height      = 17;
  Label41.Caption     = Label41_Caption;
  Label41.FontName    = 27;
  Label41.Font_Color  = 0xFFFFFF;
  Label41.FontHandle  = 27;
  Label41.Source      = -1UL;
  Label41.Active      = 0;
  Label41.OnUp        = 0;
  Label41.OnDown      = 0;
  Label41.OnClick     = 0;
  Label41.OnPress     = 0;

  Label42.OwnerScreen = &Home;
  Label42.Order       = 101;
  Label42.Visible     = 1;
  Label42.Opacity     = 255;
  Label42.Tag         = 255;
  Label42.Left        = 517;
  Label42.Top         = 207;
  Label42.Width       = 50;
  Label42.Height      = 17;
  Label42.Caption     = Label42_Caption;
  Label42.FontName    = 27;
  Label42.Font_Color  = 0xFFFFFF;
  Label42.FontHandle  = 27;
  Label42.Source      = -1UL;
  Label42.Active      = 1;
  Label42.OnUp        = 0;
  Label42.OnDown      = 0;
  Label42.OnClick     = 0;
  Label42.OnPress     = 0;

  Label61.OwnerScreen = &Home;
  Label61.Order       = 102;
  Label61.Visible     = 1;
  Label61.Opacity     = 255;
  Label61.Tag         = 255;
  Label61.Left        = 370;
  Label61.Top         = 21;
  Label61.Width       = 55;
  Label61.Height      = 25;
  Label61.Caption     = Label61_Caption;
  Label61.FontName    = 29;
  Label61.Font_Color  = 0xFFFFFF;
  Label61.FontHandle  = 29;
  Label61.Source      = -1UL;
  Label61.Active      = 1;
  Label61.OnUp        = 0;
  Label61.OnDown      = 0;
  Label61.OnClick     = 0;
  Label61.OnPress     = 0;

  Label68.OwnerScreen = &Home;
  Label68.Order       = 103;
  Label68.Visible     = 1;
  Label68.Opacity     = 255;
  Label68.Tag         = 255;
  Label68.Left        = 242;
  Label68.Top         = 129;
  Label68.Width       = 104;
  Label68.Height      = 17;
  Label68.Caption     = Label68_Caption;
  Label68.FontName    = 27;
  Label68.Font_Color  = 0xFFFFFF;
  Label68.FontHandle  = 27;
  Label68.Source      = -1UL;
  Label68.Active      = 0;
  Label68.OnUp        = 0;
  Label68.OnDown      = 0;
  Label68.OnClick     = 0;
  Label68.OnPress     = 0;

  Label69.OwnerScreen = &Home;
  Label69.Order       = 104;
  Label69.Visible     = 1;
  Label69.Opacity     = 255;
  Label69.Tag         = 255;
  Label69.Left        = 385;
  Label69.Top         = 127;
  Label69.Width       = 7;
  Label69.Height      = 15;
  Label69.Caption     = Label69_Caption;
  Label69.FontName    = 26;
  Label69.Font_Color  = 0xFFFFFF;
  Label69.FontHandle  = 26;
  Label69.Source      = -1UL;
  Label69.Active      = 0;
  Label69.OnUp        = 0;
  Label69.OnDown      = 0;
  Label69.OnClick     = 0;
  Label69.OnPress     = 0;

  Label70.OwnerScreen = &Home;
  Label70.Order       = 105;
  Label70.Visible     = 1;
  Label70.Opacity     = 255;
  Label70.Tag         = 255;
  Label70.Left        = 549;
  Label70.Top         = 129;
  Label70.Width       = 21;
  Label70.Height      = 17;
  Label70.Caption     = Label70_Caption;
  Label70.FontName    = 27;
  Label70.Font_Color  = 0xFFFFFF;
  Label70.FontHandle  = 27;
  Label70.Source      = -1UL;
  Label70.Active      = 0;
  Label70.OnUp        = 0;
  Label70.OnDown      = 0;
  Label70.OnClick     = 0;
  Label70.OnPress     = 0;

  Label73.OwnerScreen = &Home;
  Label73.Order       = 106;
  Label73.Visible     = 1;
  Label73.Opacity     = 255;
  Label73.Tag         = 255;
  Label73.Left        = 60;
  Label73.Top         = 93;
  Label73.Width       = 34;
  Label73.Height      = 32;
  Label73.Caption     = Label73_Caption;
  Label73.FontName    = 30;
  Label73.Font_Color  = 0xFFFFFF;
  Label73.FontHandle  = 30;
  Label73.Source      = -1UL;
  Label73.Active      = 1;
  Label73.OnUp        = 0;
  Label73.OnDown      = 0;
  Label73.OnClick     = 0;
  Label73.OnPress     = 0;

  Label74.OwnerScreen = &Home;
  Label74.Order       = 107;
  Label74.Visible     = 1;
  Label74.Opacity     = 255;
  Label74.Tag         = 255;
  Label74.Left        = 107;
  Label74.Top         = 91;
  Label74.Width       = 6;
  Label74.Height      = 32;
  Label74.Caption     = Label74_Caption;
  Label74.FontName    = 30;
  Label74.Font_Color  = 0xFFFFFF;
  Label74.FontHandle  = 30;
  Label74.Source      = -1UL;
  Label74.Active      = 1;
  Label74.OnUp        = 0;
  Label74.OnDown      = 0;
  Label74.OnClick     = 0;
  Label74.OnPress     = 0;

  Label75.OwnerScreen = &Home;
  Label75.Order       = 108;
  Label75.Visible     = 1;
  Label75.Opacity     = 255;
  Label75.Tag         = 255;
  Label75.Left        = 115;
  Label75.Top         = 93;
  Label75.Width       = 34;
  Label75.Height      = 32;
  Label75.Caption     = Label75_Caption;
  Label75.FontName    = 30;
  Label75.Font_Color  = 0xFFFFFF;
  Label75.FontHandle  = 30;
  Label75.Source      = -1UL;
  Label75.Active      = 1;
  Label75.OnUp        = 0;
  Label75.OnDown      = 0;
  Label75.OnClick     = 0;
  Label75.OnPress     = 0;

  Label76.OwnerScreen = &Home;
  Label76.Order       = 109;
  Label76.Visible     = 1;
  Label76.Opacity     = 255;
  Label76.Tag         = 255;
  Label76.Left        = 46;
  Label76.Top         = 46;
  Label76.Width       = 32;
  Label76.Height      = 25;
  Label76.Caption     = Label76_Caption;
  Label76.FontName    = 29;
  Label76.Font_Color  = 0xFFFFFF;
  Label76.FontHandle  = 29;
  Label76.Source      = -1UL;
  Label76.Active      = 1;
  Label76.OnUp        = 0;
  Label76.OnDown      = 0;
  Label76.OnClick     = 0;
  Label76.OnPress     = 0;

  Label77.OwnerScreen = &Home;
  Label77.Order       = 110;
  Label77.Visible     = 1;
  Label77.Opacity     = 255;
  Label77.Tag         = 255;
  Label77.Left        = 81;
  Label77.Top         = 46;
  Label77.Width       = 26;
  Label77.Height      = 25;
  Label77.Caption     = Label77_Caption;
  Label77.FontName    = 29;
  Label77.Font_Color  = 0xFFFFFF;
  Label77.FontHandle  = 29;
  Label77.Source      = -1UL;
  Label77.Active      = 1;
  Label77.OnUp        = 0;
  Label77.OnDown      = 0;
  Label77.OnClick     = 0;
  Label77.OnPress     = 0;

  Label78.OwnerScreen = &Home;
  Label78.Order       = 111;
  Label78.Visible     = 1;
  Label78.Opacity     = 255;
  Label78.Tag         = 255;
  Label78.Left        = 107;
  Label78.Top         = 46;
  Label78.Width       = 59;
  Label78.Height      = 25;
  Label78.Caption     = Label78_Caption;
  Label78.FontName    = 29;
  Label78.Font_Color  = 0xFFFFFF;
  Label78.FontHandle  = 29;
  Label78.Source      = -1UL;
  Label78.Active      = 1;
  Label78.OnUp        = 0;
  Label78.OnDown      = 0;
  Label78.OnClick     = 0;
  Label78.OnPress     = 0;

  Label58.OwnerScreen = &Home;
  Label58.Order       = 112;
  Label58.Visible     = 1;
  Label58.Opacity     = 255;
  Label58.Tag         = 255;
  Label58.Left        = 470;
  Label58.Top         = 50;
  Label58.Width       = 28;
  Label58.Height      = 32;
  Label58.Caption     = Label58_Caption;
  Label58.FontName    = 30;
  Label58.Font_Color  = 0xFFFFFF;
  Label58.FontHandle  = 30;
  Label58.Source      = -1UL;
  Label58.Active      = 0;
  Label58.OnUp        = 0;
  Label58.OnDown      = 0;
  Label58.OnClick     = 0;
  Label58.OnPress     = 0;

  Label59.OwnerScreen = &Home;
  Label59.Order       = 113;
  Label59.Visible     = 1;
  Label59.Opacity     = 255;
  Label59.Tag         = 255;
  Label59.Left        = 447;
  Label59.Top         = 85;
  Label59.Width       = 86;
  Label59.Height      = 17;
  Label59.Caption     = Label59_Caption;
  Label59.FontName    = 27;
  Label59.Font_Color  = 0xFFFFFF;
  Label59.FontHandle  = 27;
  Label59.Source      = -1UL;
  Label59.Active      = 0;
  Label59.OnUp        = 0;
  Label59.OnDown      = 0;
  Label59.OnClick     = 0;
  Label59.OnPress     = 0;

  Label60.OwnerScreen = &Home;
  Label60.Order       = 114;
  Label60.Visible     = 1;
  Label60.Opacity     = 255;
  Label60.Tag         = 255;
  Label60.Left        = 510;
  Label60.Top         = 48;
  Label60.Width       = 10;
  Label60.Height      = 21;
  Label60.Caption     = Label60_Caption;
  Label60.FontName    = 28;
  Label60.Font_Color  = 0xFFFFFF;
  Label60.FontHandle  = 28;
  Label60.Source      = -1UL;
  Label60.Active      = 0;
  Label60.OnUp        = 0;
  Label60.OnDown      = 0;
  Label60.OnClick     = 0;
  Label60.OnPress     = 0;

  Box45.OwnerScreen   = &Temperature_Mode;
  Box45.Order         = 0;
  Box45.Visible       = 1;
  Box45.Opacity       = 255;
  Box45.Tag           = 255;
  Box45.Left          = 329;
  Box45.Top           = 71;
  Box45.Width         = 461;
  Box45.Height        = 80;
  Box45.Pen_Width     = 1;
  Box45.Pen_Color     = 0x0000;
  Box45.Color         = 0x8040;
  Box45.Press_Color   = 0xFFFFFF;
  Box45.ColorTo       = 0xFFFFFF;
  Box45.Press_ColorTo = 0xC0C0C0;
  Box45.Gradient      = _FT812_BRUSH_GR_NONE;
  Box45.Active        = 1;
  Box45.OnUp          = 0;
  Box45.OnDown        = 0;
  Box45.OnClick       = 0;
  Box45.OnPress       = 0;

  Image60.OwnerScreen   = &Temperature_Mode;
  Image60.Order         = 1;
  Image60.Visible       = 1;
  Image60.Opacity       = 255;
  Image60.Tag           = 255;
  Image60.Left          = 720;
  Image60.Top           = 97;
  Image60.Width         = 50;
  Image60.Height        = 50;
  Image60.Picture_Name  = circle2_bmp;
  Image60.Picture_Type  = 7;
  Image60.Picture_Ratio = 1;
  Image60.Blend_Color   = 0xFFFFFF;
  Image60.Source        = -1L;
  Image60.RotationAngle = 0;
  Image60.RotationCenterLeft = 724;
  Image60.RotationCenterTop = 101;
  Image60.ScaleX        = 0.1500;
  Image60.ScaleY        = 0.1500;
  Image60.Active        = 1;
  Image60.OnUp          = 0;
  Image60.OnDown        = 0;
  Image60.OnClick       = 0;
  Image60.OnPress       = 0;

  Box40.OwnerScreen   = &Temperature_Mode;
  Box40.Order         = 2;
  Box40.Visible       = 1;
  Box40.Opacity       = 255;
  Box40.Tag           = 255;
  Box40.Left          = 8;
  Box40.Top           = 335;
  Box40.Width         = 313;
  Box40.Height        = 80;
  Box40.Pen_Width     = 1;
  Box40.Pen_Color     = 0x0000;
  Box40.Color         = 0x0080;
  Box40.Press_Color   = 0xFFFFFF;
  Box40.ColorTo       = 0xFFFFFF;
  Box40.Press_ColorTo = 0xC0C0C0;
  Box40.Gradient      = _FT812_BRUSH_GR_NONE;
  Box40.Active        = 1;
  Box40.OnUp          = 0;
  Box40.OnDown        = 0;
  Box40.OnClick       = &Box40_OnClick;
  Box40.OnPress       = 0;

  Box40_OnClick.Action       = Box40OnClick;
  Box40_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Box40_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Box40_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Box40_OnClick.Sound.Volume = 255;

  Box39.OwnerScreen   = &Temperature_Mode;
  Box39.Order         = 3;
  Box39.Visible       = 1;
  Box39.Opacity       = 255;
  Box39.Tag           = 255;
  Box39.Left          = 8;
  Box39.Top           = 247;
  Box39.Width         = 313;
  Box39.Height        = 80;
  Box39.Pen_Width     = 1;
  Box39.Pen_Color     = 0x0000;
  Box39.Color         = 0x0080;
  Box39.Press_Color   = 0xFFFFFF;
  Box39.ColorTo       = 0xFFFFFF;
  Box39.Press_ColorTo = 0xC0C0C0;
  Box39.Gradient      = _FT812_BRUSH_GR_NONE;
  Box39.Active        = 1;
  Box39.OnUp          = 0;
  Box39.OnDown        = 0;
  Box39.OnClick       = &Box39_OnClick;
  Box39.OnPress       = 0;

  Box39_OnClick.Action       = Box39OnClick;
  Box39_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Box39_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Box39_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Box39_OnClick.Sound.Volume = 255;

  Box38.OwnerScreen   = &Temperature_Mode;
  Box38.Order         = 4;
  Box38.Visible       = 1;
  Box38.Opacity       = 255;
  Box38.Tag           = 255;
  Box38.Left          = 8;
  Box38.Top           = 159;
  Box38.Width         = 313;
  Box38.Height        = 80;
  Box38.Pen_Width     = 1;
  Box38.Pen_Color     = 0x0000;
  Box38.Color         = 0x0080;
  Box38.Press_Color   = 0xFFFFFF;
  Box38.ColorTo       = 0xFFFFFF;
  Box38.Press_ColorTo = 0xC0C0C0;
  Box38.Gradient      = _FT812_BRUSH_GR_NONE;
  Box38.Active        = 1;
  Box38.OnUp          = 0;
  Box38.OnDown        = 0;
  Box38.OnClick       = &Box38_OnClick;
  Box38.OnPress       = 0;

  Box38_OnClick.Action       = Box38OnClick;
  Box38_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Box38_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Box38_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Box38_OnClick.Sound.Volume = 255;

  Box37.OwnerScreen   = &Temperature_Mode;
  Box37.Order         = 5;
  Box37.Visible       = 1;
  Box37.Opacity       = 255;
  Box37.Tag           = 255;
  Box37.Left          = 8;
  Box37.Top           = 71;
  Box37.Width         = 313;
  Box37.Height        = 80;
  Box37.Pen_Width     = 1;
  Box37.Pen_Color     = 0x0000;
  Box37.Color         = 0x0080;
  Box37.Press_Color   = 0xFFFFFF;
  Box37.ColorTo       = 0xFFFFFF;
  Box37.Press_ColorTo = 0xC0C0C0;
  Box37.Gradient      = _FT812_BRUSH_GR_NONE;
  Box37.Active        = 1;
  Box37.OnUp          = 0;
  Box37.OnDown        = 0;
  Box37.OnClick       = &Box37_OnClick;
  Box37.OnPress       = 0;

  Box37_OnClick.Action       = Box37OnClick;
  Box37_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Box37_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Box37_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Box37_OnClick.Sound.Volume = 255;

  Image51.OwnerScreen   = &Temperature_Mode;
  Image51.Order         = 6;
  Image51.Visible       = 0;
  Image51.Opacity       = 255;
  Image51.Tag           = 255;
  Image51.Left          = 50;
  Image51.Top           = 349;
  Image51.Width         = 69;
  Image51.Height        = 64;
  Image51.Picture_Name  = peo2_bmp;
  Image51.Picture_Type  = 7;
  Image51.Picture_Ratio = 1;
  Image51.Blend_Color   = 0xFFFFFF;
  Image51.Source        = -1L;
  Image51.RotationAngle = 0;
  Image51.RotationCenterLeft = 78;
  Image51.RotationCenterTop = 375;
  Image51.ScaleX        = 0.8000;
  Image51.ScaleY        = 0.8000;
  Image51.Active        = 0;
  Image51.OnUp          = 0;
  Image51.OnDown        = 0;
  Image51.OnClick       = 0;
  Image51.OnPress       = 0;

  Image50.OwnerScreen   = &Temperature_Mode;
  Image50.Order         = 7;
  Image50.Visible       = 1;
  Image50.Opacity       = 255;
  Image50.Tag           = 255;
  Image50.Left          = 50;
  Image50.Top           = 349;
  Image50.Width         = 69;
  Image50.Height        = 64;
  Image50.Picture_Name  = peo1_bmp;
  Image50.Picture_Type  = 7;
  Image50.Picture_Ratio = 1;
  Image50.Blend_Color   = 0xFFFFFF;
  Image50.Source        = -1L;
  Image50.RotationAngle = 0;
  Image50.RotationCenterLeft = 77;
  Image50.RotationCenterTop = 375;
  Image50.ScaleX        = 0.8000;
  Image50.ScaleY        = 0.8000;
  Image50.Active        = 0;
  Image50.OnUp          = 0;
  Image50.OnDown        = 0;
  Image50.OnClick       = 0;
  Image50.OnPress       = 0;

  Image49.OwnerScreen   = &Temperature_Mode;
  Image49.Order         = 8;
  Image49.Visible       = 0;
  Image49.Opacity       = 255;
  Image49.Tag           = 255;
  Image49.Left          = 48;
  Image49.Top           = 258;
  Image49.Width         = 63;
  Image49.Height        = 64;
  Image49.Picture_Name  = sun4_bmp;
  Image49.Picture_Type  = 7;
  Image49.Picture_Ratio = 1;
  Image49.Blend_Color   = 0xFFFFFF;
  Image49.Source        = -1L;
  Image49.RotationAngle = 0;
  Image49.RotationCenterLeft = 76;
  Image49.RotationCenterTop = 287;
  Image49.ScaleX        = 0.9000;
  Image49.ScaleY        = 0.9000;
  Image49.Active        = 0;
  Image49.OnUp          = 0;
  Image49.OnDown        = 0;
  Image49.OnClick       = 0;
  Image49.OnPress       = 0;

  Image48.OwnerScreen   = &Temperature_Mode;
  Image48.Order         = 9;
  Image48.Visible       = 1;
  Image48.Opacity       = 255;
  Image48.Tag           = 255;
  Image48.Left          = 48;
  Image48.Top           = 258;
  Image48.Width         = 63;
  Image48.Height        = 64;
  Image48.Picture_Name  = sun3_bmp;
  Image48.Picture_Type  = 7;
  Image48.Picture_Ratio = 1;
  Image48.Blend_Color   = 0xFFFFFF;
  Image48.Source        = -1L;
  Image48.RotationAngle = 0;
  Image48.RotationCenterLeft = 76;
  Image48.RotationCenterTop = 286;
  Image48.ScaleX        = 0.9000;
  Image48.ScaleY        = 0.9000;
  Image48.Active        = 0;
  Image48.OnUp          = 0;
  Image48.OnDown        = 0;
  Image48.OnClick       = 0;
  Image48.OnPress       = 0;

  Image47.OwnerScreen   = &Temperature_Mode;
  Image47.Order         = 10;
  Image47.Visible       = 0;
  Image47.Opacity       = 255;
  Image47.Tag           = 255;
  Image47.Left          = 51;
  Image47.Top           = 175;
  Image47.Width         = 58;
  Image47.Height        = 54;
  Image47.Picture_Name  = moon4_bmp;
  Image47.Picture_Type  = 7;
  Image47.Picture_Ratio = 1;
  Image47.Blend_Color   = 0xFFFFFF;
  Image47.Source        = -1L;
  Image47.RotationAngle = 0;
  Image47.RotationCenterLeft = 77;
  Image47.RotationCenterTop = 199;
  Image47.ScaleX        = 0.9000;
  Image47.ScaleY        = 0.9000;
  Image47.Active        = 0;
  Image47.OnUp          = 0;
  Image47.OnDown        = 0;
  Image47.OnClick       = 0;
  Image47.OnPress       = 0;

  Image46.OwnerScreen   = &Temperature_Mode;
  Image46.Order         = 11;
  Image46.Visible       = 1;
  Image46.Opacity       = 255;
  Image46.Tag           = 255;
  Image46.Left          = 51;
  Image46.Top           = 175;
  Image46.Width         = 58;
  Image46.Height        = 54;
  Image46.Picture_Name  = moon3_bmp;
  Image46.Picture_Type  = 7;
  Image46.Picture_Ratio = 1;
  Image46.Blend_Color   = 0xFFFFFF;
  Image46.Source        = -1L;
  Image46.RotationAngle = 0;
  Image46.RotationCenterLeft = 77;
  Image46.RotationCenterTop = 199;
  Image46.ScaleX        = 0.9000;
  Image46.ScaleY        = 0.9000;
  Image46.Active        = 0;
  Image46.OnUp          = 0;
  Image46.OnDown        = 0;
  Image46.OnClick       = 0;
  Image46.OnPress       = 0;

  Image28.OwnerScreen   = &Temperature_Mode;
  Image28.Order         = 12;
  Image28.Visible       = 1;
  Image28.Opacity       = 255;
  Image28.Tag           = 255;
  Image28.Left          = 45;
  Image28.Top           = 82;
  Image28.Width         = 59;
  Image28.Height        = 57;
  Image28.Picture_Name  = house2_bmp;
  Image28.Picture_Type  = 7;
  Image28.Picture_Ratio = 1;
  Image28.Blend_Color   = 0xFFFFFF;
  Image28.Source        = -1L;
  Image28.RotationAngle = 0;
  Image28.RotationCenterLeft = 76;
  Image28.RotationCenterTop = 111;
  Image28.ScaleX        = 1.0000;
  Image28.ScaleY        = 1.0000;
  Image28.Active        = 0;
  Image28.OnUp          = 0;
  Image28.OnDown        = 0;
  Image28.OnClick       = 0;
  Image28.OnPress       = 0;

  Image2.OwnerScreen   = &Temperature_Mode;
  Image2.Order         = 13;
  Image2.Visible       = 0;
  Image2.Opacity       = 255;
  Image2.Tag           = 255;
  Image2.Left          = 45;
  Image2.Top           = 82;
  Image2.Width         = 59;
  Image2.Height        = 57;
  Image2.Picture_Name  = house1_bmp;
  Image2.Picture_Type  = 7;
  Image2.Picture_Ratio = 1;
  Image2.Blend_Color   = 0xFFFFFF;
  Image2.Source        = -1L;
  Image2.RotationAngle = 0;
  Image2.RotationCenterLeft = 76;
  Image2.RotationCenterTop = 111;
  Image2.ScaleX        = 1.0000;
  Image2.ScaleY        = 1.0000;
  Image2.Active        = 0;
  Image2.OnUp          = 0;
  Image2.OnDown        = 0;
  Image2.OnClick       = 0;
  Image2.OnPress       = 0;

  Button30.OwnerScreen   = &Temperature_Mode;
  Button30.Order         = 14;
  Button30.Visible       = 1;
  Button30.Opacity       = 255;
  Button30.Tag           = 255;
  Button30.Left          = 8;
  Button30.Top           = 423;
  Button30.Width         = 50;
  Button30.Height        = 50;
  Button30.Pen_Width     = 1;
  Button30.Pen_Color     = 0x0000;
  Button30.Color         = 0x808080;
  Button30.Press_Color   = 0xFFFFFF;
  Button30.ColorTo       = 0xFFFFFF;
  Button30.Press_ColorTo = 0xC0C0C0;
  Button30.Gradient      = _FT812_BRUSH_GR_NONE;
  Button30.Caption       = Button30_Caption;
  Button30.TextAlign     = taCenter;
  Button30.FontName      = 30;
  Button30.Font_Color    = 0xFFFFFF;
  Button30.FontHandle    = 30;
  Button30.Source        = -1UL;
  Button30.Active        = 1;
  Button30.OnUp          = 0;
  Button30.OnDown        = 0;
  Button30.OnClick       = &Button30_OnClick;
  Button30.OnPress       = 0;

  Button30_OnClick.Action       = Button12OnClick;
  Button30_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button30_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button30_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button30_OnClick.Sound.Volume = 255;

  Box22.OwnerScreen   = &Temperature_Mode;
  Box22.Order         = 15;
  Box22.Visible       = 1;
  Box22.Opacity       = 255;
  Box22.Tag           = 255;
  Box22.Left          = 66;
  Box22.Top           = 423;
  Box22.Width         = 724;
  Box22.Height        = 50;
  Box22.Pen_Width     = 1;
  Box22.Pen_Color     = 0x0000;
  Box22.Color         = 0x404040;
  Box22.Press_Color   = 0xFFFFFF;
  Box22.ColorTo       = 0xFFFFFF;
  Box22.Press_ColorTo = 0xC0C0C0;
  Box22.Gradient      = _FT812_BRUSH_GR_NONE;
  Box22.Active        = 1;
  Box22.OnUp          = 0;
  Box22.OnDown        = 0;
  Box22.OnClick       = 0;
  Box22.OnPress       = 0;

  Button47.OwnerScreen   = &Temperature_Mode;
  Button47.Order         = 16;
  Button47.Visible       = 1;
  Button47.Opacity       = 255;
  Button47.Tag           = 255;
  Button47.Left          = 8;
  Button47.Top           = 8;
  Button47.Width         = 782;
  Button47.Height        = 55;
  Button47.Pen_Width     = 1;
  Button47.Pen_Color     = 0x0000;
  Button47.Color         = 0x00FF;
  Button47.Press_Color   = 0xFFFFFF;
  Button47.ColorTo       = 0xFFFFFF;
  Button47.Press_ColorTo = 0xC0C0C0;
  Button47.Gradient      = _FT812_BRUSH_GR_NONE;
  Button47.Caption       = Button47_Caption;
  Button47.TextAlign     = taCenter;
  Button47.FontName      = 31;
  Button47.Font_Color    = 0xFFFFFF;
  Button47.FontHandle    = 31;
  Button47.Source        = -1UL;
  Button47.Active        = 1;
  Button47.OnUp          = 0;
  Button47.OnDown        = 0;
  Button47.OnClick       = 0;
  Button47.OnPress       = 0;

  Box41.OwnerScreen   = &Temperature_Mode;
  Box41.Order         = 17;
  Box41.Visible       = 1;
  Box41.Opacity       = 255;
  Box41.Tag           = 255;
  Box41.Left          = 329;
  Box41.Top           = 159;
  Box41.Width         = 80;
  Box41.Height        = 80;
  Box41.Pen_Width     = 1;
  Box41.Pen_Color     = 0x0000;
  Box41.Color         = 0x00FF;
  Box41.Press_Color   = 0xFFFFFF;
  Box41.ColorTo       = 0xFFFFFF;
  Box41.Press_ColorTo = 0xC0C0C0;
  Box41.Gradient      = _FT812_BRUSH_GR_NONE;
  Box41.Active        = 0;
  Box41.OnUp          = 0;
  Box41.OnDown        = 0;
  Box41.OnClick       = 0;
  Box41.OnPress       = 0;

  Box42.OwnerScreen   = &Temperature_Mode;
  Box42.Order         = 18;
  Box42.Visible       = 1;
  Box42.Opacity       = 255;
  Box42.Tag           = 255;
  Box42.Left          = 710;
  Box42.Top           = 159;
  Box42.Width         = 80;
  Box42.Height        = 80;
  Box42.Pen_Width     = 1;
  Box42.Pen_Color     = 0x0000;
  Box42.Color         = 0x00FF;
  Box42.Press_Color   = 0xFFFFFF;
  Box42.ColorTo       = 0xFFFFFF;
  Box42.Press_ColorTo = 0xC0C0C0;
  Box42.Gradient      = _FT812_BRUSH_GR_NONE;
  Box42.Active        = 0;
  Box42.OnUp          = 0;
  Box42.OnDown        = 0;
  Box42.OnClick       = 0;
  Box42.OnPress       = 0;

  Box43.OwnerScreen   = &Temperature_Mode;
  Box43.Order         = 19;
  Box43.Visible       = 1;
  Box43.Opacity       = 255;
  Box43.Tag           = 255;
  Box43.Left          = 329;
  Box43.Top           = 335;
  Box43.Width         = 80;
  Box43.Height        = 80;
  Box43.Pen_Width     = 1;
  Box43.Pen_Color     = 0x0000;
  Box43.Color         = 0x00FF;
  Box43.Press_Color   = 0xFFFFFF;
  Box43.ColorTo       = 0xFFFFFF;
  Box43.Press_ColorTo = 0xC0C0C0;
  Box43.Gradient      = _FT812_BRUSH_GR_NONE;
  Box43.Active        = 0;
  Box43.OnUp          = 0;
  Box43.OnDown        = 0;
  Box43.OnClick       = 0;
  Box43.OnPress       = 0;

  Box44.OwnerScreen   = &Temperature_Mode;
  Box44.Order         = 20;
  Box44.Visible       = 1;
  Box44.Opacity       = 255;
  Box44.Tag           = 255;
  Box44.Left          = 710;
  Box44.Top           = 335;
  Box44.Width         = 80;
  Box44.Height        = 80;
  Box44.Pen_Width     = 1;
  Box44.Pen_Color     = 0x0000;
  Box44.Color         = 0x00FF;
  Box44.Press_Color   = 0xFFFFFF;
  Box44.ColorTo       = 0xFFFFFF;
  Box44.Press_ColorTo = 0xC0C0C0;
  Box44.Gradient      = _FT812_BRUSH_GR_NONE;
  Box44.Active        = 0;
  Box44.OnUp          = 0;
  Box44.OnDown        = 0;
  Box44.OnClick       = 0;
  Box44.OnPress       = 0;

  Box46.OwnerScreen   = &Temperature_Mode;
  Box46.Order         = 21;
  Box46.Visible       = 1;
  Box46.Opacity       = 255;
  Box46.Tag           = 255;
  Box46.Left          = 329;
  Box46.Top           = 247;
  Box46.Width         = 461;
  Box46.Height        = 80;
  Box46.Pen_Width     = 1;
  Box46.Pen_Color     = 0x0000;
  Box46.Color         = 0x8040;
  Box46.Press_Color   = 0xFFFFFF;
  Box46.ColorTo       = 0xFFFFFF;
  Box46.Press_ColorTo = 0xC0C0C0;
  Box46.Gradient      = _FT812_BRUSH_GR_NONE;
  Box46.Active        = 1;
  Box46.OnUp          = 0;
  Box46.OnDown        = 0;
  Box46.OnClick       = 0;
  Box46.OnPress       = 0;

  Box47.OwnerScreen   = &Temperature_Mode;
  Box47.Order         = 22;
  Box47.Visible       = 1;
  Box47.Opacity       = 255;
  Box47.Tag           = 255;
  Box47.Left          = 417;
  Box47.Top           = 159;
  Box47.Width         = 285;
  Box47.Height        = 80;
  Box47.Pen_Width     = 1;
  Box47.Pen_Color     = 0x0000;
  Box47.Color         = 0x80FFFF;
  Box47.Press_Color   = 0xFFFFFF;
  Box47.ColorTo       = 0xFFFF00;
  Box47.Press_ColorTo = 0xC0C0C0;
  Box47.Gradient      = _FT812_BRUSH_GR_LEFT_TO_RIGHT;
  Box47.Active        = 1;
  Box47.OnUp          = 0;
  Box47.OnDown        = 0;
  Box47.OnClick       = 0;
  Box47.OnPress       = 0;

  Box48.OwnerScreen   = &Temperature_Mode;
  Box48.Order         = 23;
  Box48.Visible       = 1;
  Box48.Opacity       = 255;
  Box48.Tag           = 255;
  Box48.Left          = 417;
  Box48.Top           = 335;
  Box48.Width         = 285;
  Box48.Height        = 80;
  Box48.Pen_Width     = 1;
  Box48.Pen_Color     = 0x0000;
  Box48.Color         = 0x00FF;
  Box48.Press_Color   = 0xFFFFFF;
  Box48.ColorTo       = 0xFFFFFF;
  Box48.Press_ColorTo = 0xC0C0C0;
  Box48.Gradient      = _FT812_BRUSH_GR_NONE;
  Box48.Active        = 1;
  Box48.OnUp          = 0;
  Box48.OnDown        = 0;
  Box48.OnClick       = 0;
  Box48.OnPress       = 0;

  CircleButton1.OwnerScreen   = &Temperature_Mode;
  CircleButton1.Order         = 24;
  CircleButton1.Visible       = 1;
  CircleButton1.Opacity       = 255;
  CircleButton1.Tag           = 255;
  CircleButton1.Left          = 336;
  CircleButton1.Top           = 166;
  CircleButton1.Radius        = 33;
  CircleButton1.Pen_Width     = 1;
  CircleButton1.Pen_Color     = 0xFFFFFF;
  CircleButton1.Color         = 0x00FF;
  CircleButton1.Press_Color   = 0xE2E2E2;
  CircleButton1.Caption       = CircleButton1_Caption;
  CircleButton1.TextAlign     = taCenter;
  CircleButton1.FontName      = 30;
  CircleButton1.Font_Color    = 0xFFFFFF;
  CircleButton1.FontHandle    = 30;
  CircleButton1.Source        = -1UL;
  CircleButton1.Active        = 1;
  CircleButton1.OnUp          = 0;
  CircleButton1.OnDown        = 0;
  CircleButton1.OnClick       = 0;
  CircleButton1.OnPress       = &CircleButton1_OnPress;

  CircleButton1_OnPress.Action       = CircleButton1OnPress;
  CircleButton1_OnPress.Sound.SndAct = VTFT_SNDACT_NONE;
  CircleButton1_OnPress.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  CircleButton1_OnPress.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  CircleButton1_OnPress.Sound.Volume = 255;

  CircleButton2.OwnerScreen   = &Temperature_Mode;
  CircleButton2.Order         = 25;
  CircleButton2.Visible       = 1;
  CircleButton2.Opacity       = 255;
  CircleButton2.Tag           = 255;
  CircleButton2.Left          = 336;
  CircleButton2.Top           = 342;
  CircleButton2.Radius        = 33;
  CircleButton2.Pen_Width     = 1;
  CircleButton2.Pen_Color     = 0xFFFFFF;
  CircleButton2.Color         = 0x00FF;
  CircleButton2.Press_Color   = 0xE2E2E2;
  CircleButton2.Caption       = CircleButton2_Caption;
  CircleButton2.TextAlign     = taCenter;
  CircleButton2.FontName      = 30;
  CircleButton2.Font_Color    = 0xFFFFFF;
  CircleButton2.FontHandle    = 30;
  CircleButton2.Source        = -1UL;
  CircleButton2.Active        = 1;
  CircleButton2.OnUp          = 0;
  CircleButton2.OnDown        = 0;
  CircleButton2.OnClick       = 0;
  CircleButton2.OnPress       = &CircleButton2_OnPress;

  CircleButton2_OnPress.Action       = CircleButton2OnPress;
  CircleButton2_OnPress.Sound.SndAct = VTFT_SNDACT_NONE;
  CircleButton2_OnPress.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  CircleButton2_OnPress.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  CircleButton2_OnPress.Sound.Volume = 255;

  CircleButton3.OwnerScreen   = &Temperature_Mode;
  CircleButton3.Order         = 26;
  CircleButton3.Visible       = 1;
  CircleButton3.Opacity       = 255;
  CircleButton3.Tag           = 255;
  CircleButton3.Left          = 717;
  CircleButton3.Top           = 166;
  CircleButton3.Radius        = 33;
  CircleButton3.Pen_Width     = 1;
  CircleButton3.Pen_Color     = 0xFFFFFF;
  CircleButton3.Color         = 0x00FF;
  CircleButton3.Press_Color   = 0xE2E2E2;
  CircleButton3.Caption       = CircleButton3_Caption;
  CircleButton3.TextAlign     = taCenter;
  CircleButton3.FontName      = 30;
  CircleButton3.Font_Color    = 0xFFFFFF;
  CircleButton3.FontHandle    = 30;
  CircleButton3.Source        = -1UL;
  CircleButton3.Active        = 1;
  CircleButton3.OnUp          = 0;
  CircleButton3.OnDown        = 0;
  CircleButton3.OnClick       = 0;
  CircleButton3.OnPress       = &CircleButton3_OnPress;

  CircleButton3_OnPress.Action       = CircleButton3OnPress;
  CircleButton3_OnPress.Sound.SndAct = VTFT_SNDACT_NONE;
  CircleButton3_OnPress.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  CircleButton3_OnPress.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  CircleButton3_OnPress.Sound.Volume = 255;

  CircleButton4.OwnerScreen   = &Temperature_Mode;
  CircleButton4.Order         = 27;
  CircleButton4.Visible       = 1;
  CircleButton4.Opacity       = 255;
  CircleButton4.Tag           = 255;
  CircleButton4.Left          = 717;
  CircleButton4.Top           = 342;
  CircleButton4.Radius        = 33;
  CircleButton4.Pen_Width     = 1;
  CircleButton4.Pen_Color     = 0xFFFFFF;
  CircleButton4.Color         = 0x00FF;
  CircleButton4.Press_Color   = 0xE2E2E2;
  CircleButton4.Caption       = CircleButton4_Caption;
  CircleButton4.TextAlign     = taCenter;
  CircleButton4.FontName      = 30;
  CircleButton4.Font_Color    = 0xFFFFFF;
  CircleButton4.FontHandle    = 30;
  CircleButton4.Source        = -1UL;
  CircleButton4.Active        = 1;
  CircleButton4.OnUp          = 0;
  CircleButton4.OnDown        = 0;
  CircleButton4.OnClick       = 0;
  CircleButton4.OnPress       = &CircleButton4_OnPress;

  CircleButton4_OnPress.Action       = CircleButton4OnPress;
  CircleButton4_OnPress.Sound.SndAct = VTFT_SNDACT_NONE;
  CircleButton4_OnPress.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  CircleButton4_OnPress.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  CircleButton4_OnPress.Sound.Volume = 255;

  Box23.OwnerScreen   = &Temperature_Mode;
  Box23.Order         = 28;
  Box23.Visible       = 1;
  Box23.Opacity       = 255;
  Box23.Tag           = 255;
  Box23.Left          = 417;
  Box23.Top           = 159;
  Box23.Width         = 25;
  Box23.Height        = 80;
  Box23.Pen_Width     = 1;
  Box23.Pen_Color     = 0xFFFFFF;
  Box23.Color         = 0x80C0;
  Box23.Press_Color   = 0xFFFFFF;
  Box23.ColorTo       = 0xFFFFFF;
  Box23.Press_ColorTo = 0xC0C0C0;
  Box23.Gradient      = _FT812_BRUSH_GR_NONE;
  Box23.Active        = 1;
  Box23.OnUp          = 0;
  Box23.OnDown        = 0;
  Box23.OnClick       = 0;
  Box23.OnPress       = 0;

  Box49.OwnerScreen   = &Temperature_Mode;
  Box49.Order         = 29;
  Box49.Visible       = 1;
  Box49.Opacity       = 255;
  Box49.Tag           = 255;
  Box49.Left          = 417;
  Box49.Top           = 335;
  Box49.Width         = 25;
  Box49.Height        = 80;
  Box49.Pen_Width     = 1;
  Box49.Pen_Color     = 0xFFFFFF;
  Box49.Color         = 0x80C0;
  Box49.Press_Color   = 0xFFFFFF;
  Box49.ColorTo       = 0xFFFFFF;
  Box49.Press_ColorTo = 0xC0C0C0;
  Box49.Gradient      = _FT812_BRUSH_GR_NONE;
  Box49.Active        = 1;
  Box49.OnUp          = 0;
  Box49.OnDown        = 0;
  Box49.OnClick       = 0;
  Box49.OnPress       = 0;

  Label29.OwnerScreen = &Temperature_Mode;
  Label29.Order       = 30;
  Label29.Visible     = 1;
  Label29.Opacity     = 255;
  Label29.Tag         = 255;
  Label29.Left        = 356;
  Label29.Top         = 98;
  Label29.Width       = 118;
  Label29.Height      = 25;
  Label29.Caption     = Label29_Caption;
  Label29.FontName    = 29;
  Label29.Font_Color  = 0xFFFFFF;
  Label29.FontHandle  = 29;
  Label29.Source      = -1UL;
  Label29.Active      = 1;
  Label29.OnUp        = 0;
  Label29.OnDown      = 0;
  Label29.OnClick     = 0;
  Label29.OnPress     = 0;

  Label31.OwnerScreen = &Temperature_Mode;
  Label31.Order       = 31;
  Label31.Visible     = 1;
  Label31.Opacity     = 255;
  Label31.Tag         = 255;
  Label31.Left        = 356;
  Label31.Top         = 274;
  Label31.Width       = 84;
  Label31.Height      = 25;
  Label31.Caption     = Label31_Caption;
  Label31.FontName    = 29;
  Label31.Font_Color  = 0xFFFFFF;
  Label31.FontHandle  = 29;
  Label31.Source      = -1UL;
  Label31.Active      = 1;
  Label31.OnUp        = 0;
  Label31.OnDown      = 0;
  Label31.OnClick     = 0;
  Label31.OnPress     = 0;

  Label32.OwnerScreen = &Temperature_Mode;
  Label32.Order       = 32;
  Label32.Visible     = 1;
  Label32.Opacity     = 255;
  Label32.Tag         = 255;
  Label32.Left        = 209;
  Label32.Top         = 103;
  Label32.Width       = 55;
  Label32.Height      = 25;
  Label32.Caption     = Label32_Caption;
  Label32.FontName    = 29;
  Label32.Font_Color  = 0xFFFF00;
  Label32.FontHandle  = 29;
  Label32.Source      = -1UL;
  Label32.Active      = 0;
  Label32.OnUp        = 0;
  Label32.OnDown      = 0;
  Label32.OnClick     = 0;
  Label32.OnPress     = 0;

  Label33.OwnerScreen = &Temperature_Mode;
  Label33.Order       = 33;
  Label33.Visible     = 1;
  Label33.Opacity     = 255;
  Label33.Tag         = 255;
  Label33.Left        = 208;
  Label33.Top         = 186;
  Label33.Width       = 58;
  Label33.Height      = 25;
  Label33.Caption     = Label33_Caption;
  Label33.FontName    = 29;
  Label33.Font_Color  = 0xFFFFFF;
  Label33.FontHandle  = 29;
  Label33.Source      = -1UL;
  Label33.Active      = 0;
  Label33.OnUp        = 0;
  Label33.OnDown      = 0;
  Label33.OnClick     = 0;
  Label33.OnPress     = 0;

  Label46.OwnerScreen = &Temperature_Mode;
  Label46.Order       = 34;
  Label46.Visible     = 1;
  Label46.Opacity     = 255;
  Label46.Tag         = 255;
  Label46.Left        = 218;
  Label46.Top         = 274;
  Label46.Width       = 38;
  Label46.Height      = 25;
  Label46.Caption     = Label46_Caption;
  Label46.FontName    = 29;
  Label46.Font_Color  = 0xFFFFFF;
  Label46.FontHandle  = 29;
  Label46.Source      = -1UL;
  Label46.Active      = 0;
  Label46.OnUp        = 0;
  Label46.OnDown      = 0;
  Label46.OnClick     = 0;
  Label46.OnPress     = 0;

  Label47.OwnerScreen = &Temperature_Mode;
  Label47.Order       = 35;
  Label47.Visible     = 1;
  Label47.Opacity     = 255;
  Label47.Tag         = 255;
  Label47.Left        = 206;
  Label47.Top         = 362;
  Label47.Width       = 61;
  Label47.Height      = 25;
  Label47.Caption     = Label47_Caption;
  Label47.FontName    = 29;
  Label47.Font_Color  = 0xFFFFFF;
  Label47.FontHandle  = 29;
  Label47.Source      = -1UL;
  Label47.Active      = 0;
  Label47.OnUp        = 0;
  Label47.OnDown      = 0;
  Label47.OnClick     = 0;
  Label47.OnPress     = 0;

  Label86.OwnerScreen = &Temperature_Mode;
  Label86.Order       = 36;
  Label86.Visible     = 1;
  Label86.Opacity     = 255;
  Label86.Tag         = 255;
  Label86.Left        = 731;
  Label86.Top         = 98;
  Label86.Width       = 12;
  Label86.Height      = 25;
  Label86.Caption     = Label86_Caption;
  Label86.FontName    = 29;
  Label86.Font_Color  = 0xFFFFFF;
  Label86.FontHandle  = 29;
  Label86.Source      = -1UL;
  Label86.Active      = 1;
  Label86.OnUp        = 0;
  Label86.OnDown      = 0;
  Label86.OnClick     = 0;
  Label86.OnPress     = 0;

  Label87.OwnerScreen = &Temperature_Mode;
  Label87.Order       = 37;
  Label87.Visible     = 1;
  Label87.Opacity     = 255;
  Label87.Tag         = 255;
  Label87.Left        = 729;
  Label87.Top         = 274;
  Label87.Width       = 14;
  Label87.Height      = 25;
  Label87.Caption     = Label87_Caption;
  Label87.FontName    = 29;
  Label87.Font_Color  = 0xFFFFFF;
  Label87.FontHandle  = 29;
  Label87.Source      = -1UL;
  Label87.Active      = 1;
  Label87.OnUp        = 0;
  Label87.OnDown      = 0;
  Label87.OnClick     = 0;
  Label87.OnPress     = 0;

  Label95.OwnerScreen = &Temperature_Mode;
  Label95.Order       = 38;
  Label95.Visible     = 1;
  Label95.Opacity     = 255;
  Label95.Tag         = 255;
  Label95.Left        = 666;
  Label95.Top         = 98;
  Label95.Width       = 41;
  Label95.Height      = 25;
  Label95.Caption     = Label95_Caption;
  Label95.FontName    = 29;
  Label95.Font_Color  = 0xFFFFFF;
  Label95.FontHandle  = 29;
  Label95.Source      = -1UL;
  Label95.Active      = 1;
  Label95.OnUp        = 0;
  Label95.OnDown      = 0;
  Label95.OnClick     = 0;
  Label95.OnPress     = 0;

  Label96.OwnerScreen = &Temperature_Mode;
  Label96.Order       = 39;
  Label96.Visible     = 1;
  Label96.Opacity     = 255;
  Label96.Tag         = 255;
  Label96.Left        = 666;
  Label96.Top         = 274;
  Label96.Width       = 41;
  Label96.Height      = 25;
  Label96.Caption     = Label96_Caption;
  Label96.FontName    = 29;
  Label96.Font_Color  = 0xFFFFFF;
  Label96.FontHandle  = 29;
  Label96.Source      = -1UL;
  Label96.Active      = 1;
  Label96.OnUp        = 0;
  Label96.OnDown      = 0;
  Label96.OnClick     = 0;
  Label96.OnPress     = 0;

  Box61.OwnerScreen   = &Light_Mode;
  Box61.Order         = 0;
  Box61.Visible       = 1;
  Box61.Opacity       = 255;
  Box61.Tag           = 255;
  Box61.Left          = 329;
  Box61.Top           = 247;
  Box61.Width         = 461;
  Box61.Height        = 80;
  Box61.Pen_Width     = 1;
  Box61.Pen_Color     = 0x0000;
  Box61.Color         = 0x8040;
  Box61.Press_Color   = 0xFFFFFF;
  Box61.ColorTo       = 0xFFFFFF;
  Box61.Press_ColorTo = 0xC0C0C0;
  Box61.Gradient      = _FT812_BRUSH_GR_NONE;
  Box61.Active        = 1;
  Box61.OnUp          = 0;
  Box61.OnDown        = 0;
  Box61.OnClick       = 0;
  Box61.OnPress       = 0;

  Image61.OwnerScreen   = &Light_Mode;
  Image61.Order         = 1;
  Image61.Visible       = 1;
  Image61.Opacity       = 255;
  Image61.Tag           = 255;
  Image61.Left          = 720;
  Image61.Top           = 273;
  Image61.Width         = 50;
  Image61.Height        = 50;
  Image61.Picture_Name  = circle2_bmp;
  Image61.Picture_Type  = 7;
  Image61.Picture_Ratio = 1;
  Image61.Blend_Color   = 0xFFFFFF;
  Image61.Source        = -1L;
  Image61.RotationAngle = 0;
  Image61.RotationCenterLeft = 724;
  Image61.RotationCenterTop = 277;
  Image61.ScaleX        = 0.1500;
  Image61.ScaleY        = 0.1500;
  Image61.Active        = 1;
  Image61.OnUp          = 0;
  Image61.OnDown        = 0;
  Image61.OnClick       = 0;
  Image61.OnPress       = 0;

  Box55.OwnerScreen   = &Light_Mode;
  Box55.Order         = 2;
  Box55.Visible       = 1;
  Box55.Opacity       = 255;
  Box55.Tag           = 255;
  Box55.Left          = 7;
  Box55.Top           = 335;
  Box55.Width         = 313;
  Box55.Height        = 80;
  Box55.Pen_Width     = 1;
  Box55.Pen_Color     = 0x0000;
  Box55.Color         = 0x0080;
  Box55.Press_Color   = 0xFFFFFF;
  Box55.ColorTo       = 0xFFFFFF;
  Box55.Press_ColorTo = 0xC0C0C0;
  Box55.Gradient      = _FT812_BRUSH_GR_NONE;
  Box55.Active        = 1;
  Box55.OnUp          = 0;
  Box55.OnDown        = 0;
  Box55.OnClick       = &Box55_OnClick;
  Box55.OnPress       = 0;

  Box55_OnClick.Action       = Box55OnClick;
  Box55_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Box55_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Box55_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Box55_OnClick.Sound.Volume = 255;

  Box53.OwnerScreen   = &Light_Mode;
  Box53.Order         = 3;
  Box53.Visible       = 1;
  Box53.Opacity       = 255;
  Box53.Tag           = 255;
  Box53.Left          = 8;
  Box53.Top           = 247;
  Box53.Width         = 313;
  Box53.Height        = 80;
  Box53.Pen_Width     = 1;
  Box53.Pen_Color     = 0x0000;
  Box53.Color         = 0x0080;
  Box53.Press_Color   = 0xFFFFFF;
  Box53.ColorTo       = 0xFFFFFF;
  Box53.Press_ColorTo = 0xC0C0C0;
  Box53.Gradient      = _FT812_BRUSH_GR_NONE;
  Box53.Active        = 1;
  Box53.OnUp          = 0;
  Box53.OnDown        = 0;
  Box53.OnClick       = &Box53_OnClick;
  Box53.OnPress       = 0;

  Box53_OnClick.Action       = Box53OnClick;
  Box53_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Box53_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Box53_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Box53_OnClick.Sound.Volume = 255;

  Box52.OwnerScreen   = &Light_Mode;
  Box52.Order         = 4;
  Box52.Visible       = 1;
  Box52.Opacity       = 255;
  Box52.Tag           = 255;
  Box52.Left          = 8;
  Box52.Top           = 159;
  Box52.Width         = 313;
  Box52.Height        = 80;
  Box52.Pen_Width     = 1;
  Box52.Pen_Color     = 0x0000;
  Box52.Color         = 0x0080;
  Box52.Press_Color   = 0xFFFFFF;
  Box52.ColorTo       = 0xFFFFFF;
  Box52.Press_ColorTo = 0xC0C0C0;
  Box52.Gradient      = _FT812_BRUSH_GR_NONE;
  Box52.Active        = 1;
  Box52.OnUp          = 0;
  Box52.OnDown        = 0;
  Box52.OnClick       = &Box52_OnClick;
  Box52.OnPress       = 0;

  Box52_OnClick.Action       = Box52OnClick;
  Box52_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Box52_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Box52_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Box52_OnClick.Sound.Volume = 255;

  Box51.OwnerScreen   = &Light_Mode;
  Box51.Order         = 5;
  Box51.Visible       = 1;
  Box51.Opacity       = 255;
  Box51.Tag           = 255;
  Box51.Left          = 8;
  Box51.Top           = 71;
  Box51.Width         = 313;
  Box51.Height        = 80;
  Box51.Pen_Width     = 1;
  Box51.Pen_Color     = 0x0000;
  Box51.Color         = 0x0080;
  Box51.Press_Color   = 0xFFFFFF;
  Box51.ColorTo       = 0xFFFFFF;
  Box51.Press_ColorTo = 0xC0C0C0;
  Box51.Gradient      = _FT812_BRUSH_GR_NONE;
  Box51.Active        = 1;
  Box51.OnUp          = 0;
  Box51.OnDown        = 0;
  Box51.OnClick       = &Box51_OnClick;
  Box51.OnPress       = 0;

  Box51_OnClick.Action       = Box51OnClick;
  Box51_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Box51_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Box51_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Box51_OnClick.Sound.Volume = 255;

  Image59.OwnerScreen   = &Light_Mode;
  Image59.Order         = 6;
  Image59.Visible       = 0;
  Image59.Opacity       = 255;
  Image59.Tag           = 255;
  Image59.Left          = 50;
  Image59.Top           = 349;
  Image59.Width         = 69;
  Image59.Height        = 64;
  Image59.Picture_Name  = peo2_bmp;
  Image59.Picture_Type  = 7;
  Image59.Picture_Ratio = 1;
  Image59.Blend_Color   = 0xFFFFFF;
  Image59.Source        = -1L;
  Image59.RotationAngle = 0;
  Image59.RotationCenterLeft = 78;
  Image59.RotationCenterTop = 375;
  Image59.ScaleX        = 0.8000;
  Image59.ScaleY        = 0.8000;
  Image59.Active        = 0;
  Image59.OnUp          = 0;
  Image59.OnDown        = 0;
  Image59.OnClick       = 0;
  Image59.OnPress       = 0;

  Image58.OwnerScreen   = &Light_Mode;
  Image58.Order         = 7;
  Image58.Visible       = 1;
  Image58.Opacity       = 255;
  Image58.Tag           = 255;
  Image58.Left          = 50;
  Image58.Top           = 349;
  Image58.Width         = 69;
  Image58.Height        = 64;
  Image58.Picture_Name  = peo1_bmp;
  Image58.Picture_Type  = 7;
  Image58.Picture_Ratio = 1;
  Image58.Blend_Color   = 0xFFFFFF;
  Image58.Source        = -1L;
  Image58.RotationAngle = 0;
  Image58.RotationCenterLeft = 78;
  Image58.RotationCenterTop = 375;
  Image58.ScaleX        = 0.8000;
  Image58.ScaleY        = 0.8000;
  Image58.Active        = 0;
  Image58.OnUp          = 0;
  Image58.OnDown        = 0;
  Image58.OnClick       = 0;
  Image58.OnPress       = 0;

  Image57.OwnerScreen   = &Light_Mode;
  Image57.Order         = 8;
  Image57.Visible       = 0;
  Image57.Opacity       = 255;
  Image57.Tag           = 255;
  Image57.Left          = 56;
  Image57.Top           = 261;
  Image57.Width         = 56;
  Image57.Height        = 66;
  Image57.Picture_Name  = dish2_bmp;
  Image57.Picture_Type  = 7;
  Image57.Picture_Ratio = 1;
  Image57.Blend_Color   = 0xFFFFFF;
  Image57.Source        = -1L;
  Image57.RotationAngle = 0;
  Image57.RotationCenterLeft = 79;
  Image57.RotationCenterTop = 287;
  Image57.ScaleX        = 0.8000;
  Image57.ScaleY        = 0.8000;
  Image57.Active        = 0;
  Image57.OnUp          = 0;
  Image57.OnDown        = 0;
  Image57.OnClick       = 0;
  Image57.OnPress       = 0;

  Image56.OwnerScreen   = &Light_Mode;
  Image56.Order         = 9;
  Image56.Visible       = 1;
  Image56.Opacity       = 255;
  Image56.Tag           = 255;
  Image56.Left          = 56;
  Image56.Top           = 261;
  Image56.Width         = 56;
  Image56.Height        = 66;
  Image56.Picture_Name  = dish1_bmp;
  Image56.Picture_Type  = 7;
  Image56.Picture_Ratio = 1;
  Image56.Blend_Color   = 0xFFFFFF;
  Image56.Source        = -1L;
  Image56.RotationAngle = 0;
  Image56.RotationCenterLeft = 78;
  Image56.RotationCenterTop = 287;
  Image56.ScaleX        = 0.8000;
  Image56.ScaleY        = 0.8000;
  Image56.Active        = 0;
  Image56.OnUp          = 0;
  Image56.OnDown        = 0;
  Image56.OnClick       = 0;
  Image56.OnPress       = 0;

  Image55.OwnerScreen   = &Light_Mode;
  Image55.Order         = 10;
  Image55.Visible       = 0;
  Image55.Opacity       = 255;
  Image55.Tag           = 255;
  Image55.Left          = 50;
  Image55.Top           = 172;
  Image55.Width         = 62;
  Image55.Height        = 59;
  Image55.Picture_Name  = book2_bmp;
  Image55.Picture_Type  = 7;
  Image55.Picture_Ratio = 1;
  Image55.Blend_Color   = 0xFFFFFF;
  Image55.Source        = -1L;
  Image55.RotationAngle = 0;
  Image55.RotationCenterLeft = 78;
  Image55.RotationCenterTop = 200;
  Image55.ScaleX        = 0.9000;
  Image55.ScaleY        = 0.9000;
  Image55.Active        = 0;
  Image55.OnUp          = 0;
  Image55.OnDown        = 0;
  Image55.OnClick       = 0;
  Image55.OnPress       = 0;

  Image54.OwnerScreen   = &Light_Mode;
  Image54.Order         = 11;
  Image54.Visible       = 1;
  Image54.Opacity       = 255;
  Image54.Tag           = 255;
  Image54.Left          = 50;
  Image54.Top           = 172;
  Image54.Width         = 62;
  Image54.Height        = 59;
  Image54.Picture_Name  = book1_bmp;
  Image54.Picture_Type  = 7;
  Image54.Picture_Ratio = 1;
  Image54.Blend_Color   = 0xFFFFFF;
  Image54.Source        = -1L;
  Image54.RotationAngle = 0;
  Image54.RotationCenterLeft = 78;
  Image54.RotationCenterTop = 200;
  Image54.ScaleX        = 0.9000;
  Image54.ScaleY        = 0.9000;
  Image54.Active        = 0;
  Image54.OnUp          = 0;
  Image54.OnDown        = 0;
  Image54.OnClick       = 0;
  Image54.OnPress       = 0;

  Image53.OwnerScreen   = &Light_Mode;
  Image53.Order         = 12;
  Image53.Visible       = 1;
  Image53.Opacity       = 255;
  Image53.Tag           = 255;
  Image53.Left          = 46;
  Image53.Top           = 86;
  Image53.Width         = 67;
  Image53.Height        = 55;
  Image53.Picture_Name  = media2_bmp;
  Image53.Picture_Type  = 7;
  Image53.Picture_Ratio = 1;
  Image53.Blend_Color   = 0xFFFFFF;
  Image53.Source        = -1L;
  Image53.RotationAngle = 0;
  Image53.RotationCenterLeft = 76;
  Image53.RotationCenterTop = 111;
  Image53.ScaleX        = 0.9000;
  Image53.ScaleY        = 0.9000;
  Image53.Active        = 0;
  Image53.OnUp          = 0;
  Image53.OnDown        = 0;
  Image53.OnClick       = 0;
  Image53.OnPress       = 0;

  Image52.OwnerScreen   = &Light_Mode;
  Image52.Order         = 13;
  Image52.Visible       = 0;
  Image52.Opacity       = 255;
  Image52.Tag           = 255;
  Image52.Left          = 46;
  Image52.Top           = 86;
  Image52.Width         = 67;
  Image52.Height        = 55;
  Image52.Picture_Name  = media1_bmp;
  Image52.Picture_Type  = 7;
  Image52.Picture_Ratio = 1;
  Image52.Blend_Color   = 0xFFFFFF;
  Image52.Source        = -1L;
  Image52.RotationAngle = 0;
  Image52.RotationCenterLeft = 75;
  Image52.RotationCenterTop = 112;
  Image52.ScaleX        = 0.9000;
  Image52.ScaleY        = 0.9000;
  Image52.Active        = 0;
  Image52.OnUp          = 0;
  Image52.OnDown        = 0;
  Image52.OnClick       = 0;
  Image52.OnPress       = 0;

  Button31.OwnerScreen   = &Light_Mode;
  Button31.Order         = 14;
  Button31.Visible       = 1;
  Button31.Opacity       = 255;
  Button31.Tag           = 255;
  Button31.Left          = 8;
  Button31.Top           = 423;
  Button31.Width         = 50;
  Button31.Height        = 50;
  Button31.Pen_Width     = 1;
  Button31.Pen_Color     = 0x0000;
  Button31.Color         = 0x808080;
  Button31.Press_Color   = 0xFFFFFF;
  Button31.ColorTo       = 0xFFFFFF;
  Button31.Press_ColorTo = 0xC0C0C0;
  Button31.Gradient      = _FT812_BRUSH_GR_NONE;
  Button31.Caption       = Button31_Caption;
  Button31.TextAlign     = taCenter;
  Button31.FontName      = 30;
  Button31.Font_Color    = 0xFFFFFF;
  Button31.FontHandle    = 30;
  Button31.Source        = -1UL;
  Button31.Active        = 1;
  Button31.OnUp          = 0;
  Button31.OnDown        = 0;
  Button31.OnClick       = &Button31_OnClick;
  Button31.OnPress       = 0;

  Button31_OnClick.Action       = Button12OnClick;
  Button31_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button31_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button31_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button31_OnClick.Sound.Volume = 255;

  Box50.OwnerScreen   = &Light_Mode;
  Box50.Order         = 15;
  Box50.Visible       = 1;
  Box50.Opacity       = 255;
  Box50.Tag           = 255;
  Box50.Left          = 66;
  Box50.Top           = 423;
  Box50.Width         = 724;
  Box50.Height        = 50;
  Box50.Pen_Width     = 1;
  Box50.Pen_Color     = 0x0000;
  Box50.Color         = 0x404040;
  Box50.Press_Color   = 0xFFFFFF;
  Box50.ColorTo       = 0xFFFFFF;
  Box50.Press_ColorTo = 0xC0C0C0;
  Box50.Gradient      = _FT812_BRUSH_GR_NONE;
  Box50.Active        = 1;
  Box50.OnUp          = 0;
  Box50.OnDown        = 0;
  Box50.OnClick       = 0;
  Box50.OnPress       = 0;

  Button32.OwnerScreen   = &Light_Mode;
  Button32.Order         = 16;
  Button32.Visible       = 1;
  Button32.Opacity       = 255;
  Button32.Tag           = 255;
  Button32.Left          = 8;
  Button32.Top           = 8;
  Button32.Width         = 782;
  Button32.Height        = 55;
  Button32.Pen_Width     = 1;
  Button32.Pen_Color     = 0x0000;
  Button32.Color         = 0x808080;
  Button32.Press_Color   = 0xFFFFFF;
  Button32.ColorTo       = 0xFFFFFF;
  Button32.Press_ColorTo = 0xC0C0C0;
  Button32.Gradient      = _FT812_BRUSH_GR_NONE;
  Button32.Caption       = Button32_Caption;
  Button32.TextAlign     = taCenter;
  Button32.FontName      = 31;
  Button32.Font_Color    = 0xFFFFFF;
  Button32.FontHandle    = 31;
  Button32.Source        = -1UL;
  Button32.Active        = 1;
  Button32.OnUp          = 0;
  Button32.OnDown        = 0;
  Button32.OnClick       = 0;
  Button32.OnPress       = 0;

  Box56.OwnerScreen   = &Light_Mode;
  Box56.Order         = 17;
  Box56.Visible       = 1;
  Box56.Opacity       = 255;
  Box56.Tag           = 255;
  Box56.Left          = 329;
  Box56.Top           = 159;
  Box56.Width         = 80;
  Box56.Height        = 80;
  Box56.Pen_Width     = 1;
  Box56.Pen_Color     = 0x0000;
  Box56.Color         = 0x00FF;
  Box56.Press_Color   = 0xFFFFFF;
  Box56.ColorTo       = 0xFFFFFF;
  Box56.Press_ColorTo = 0xC0C0C0;
  Box56.Gradient      = _FT812_BRUSH_GR_NONE;
  Box56.Active        = 0;
  Box56.OnUp          = 0;
  Box56.OnDown        = 0;
  Box56.OnClick       = 0;
  Box56.OnPress       = 0;

  Box57.OwnerScreen   = &Light_Mode;
  Box57.Order         = 18;
  Box57.Visible       = 1;
  Box57.Opacity       = 255;
  Box57.Tag           = 255;
  Box57.Left          = 710;
  Box57.Top           = 159;
  Box57.Width         = 80;
  Box57.Height        = 80;
  Box57.Pen_Width     = 1;
  Box57.Pen_Color     = 0x0000;
  Box57.Color         = 0x00FF;
  Box57.Press_Color   = 0xFFFFFF;
  Box57.ColorTo       = 0xFFFFFF;
  Box57.Press_ColorTo = 0xC0C0C0;
  Box57.Gradient      = _FT812_BRUSH_GR_NONE;
  Box57.Active        = 0;
  Box57.OnUp          = 0;
  Box57.OnDown        = 0;
  Box57.OnClick       = 0;
  Box57.OnPress       = 0;

  Box58.OwnerScreen   = &Light_Mode;
  Box58.Order         = 19;
  Box58.Visible       = 1;
  Box58.Opacity       = 255;
  Box58.Tag           = 255;
  Box58.Left          = 329;
  Box58.Top           = 335;
  Box58.Width         = 80;
  Box58.Height        = 80;
  Box58.Pen_Width     = 1;
  Box58.Pen_Color     = 0x0000;
  Box58.Color         = 0x00FF;
  Box58.Press_Color   = 0xFFFFFF;
  Box58.ColorTo       = 0xFFFFFF;
  Box58.Press_ColorTo = 0xC0C0C0;
  Box58.Gradient      = _FT812_BRUSH_GR_NONE;
  Box58.Active        = 0;
  Box58.OnUp          = 0;
  Box58.OnDown        = 0;
  Box58.OnClick       = 0;
  Box58.OnPress       = 0;

  Box59.OwnerScreen   = &Light_Mode;
  Box59.Order         = 20;
  Box59.Visible       = 1;
  Box59.Opacity       = 255;
  Box59.Tag           = 255;
  Box59.Left          = 710;
  Box59.Top           = 335;
  Box59.Width         = 80;
  Box59.Height        = 80;
  Box59.Pen_Width     = 1;
  Box59.Pen_Color     = 0x0000;
  Box59.Color         = 0x00FF;
  Box59.Press_Color   = 0xFFFFFF;
  Box59.ColorTo       = 0xFFFFFF;
  Box59.Press_ColorTo = 0xC0C0C0;
  Box59.Gradient      = _FT812_BRUSH_GR_NONE;
  Box59.Active        = 0;
  Box59.OnUp          = 0;
  Box59.OnDown        = 0;
  Box59.OnClick       = 0;
  Box59.OnPress       = 0;

  Box60.OwnerScreen   = &Light_Mode;
  Box60.Order         = 21;
  Box60.Visible       = 1;
  Box60.Opacity       = 255;
  Box60.Tag           = 255;
  Box60.Left          = 329;
  Box60.Top           = 71;
  Box60.Width         = 461;
  Box60.Height        = 80;
  Box60.Pen_Width     = 1;
  Box60.Pen_Color     = 0x0000;
  Box60.Color         = 0x8040;
  Box60.Press_Color   = 0xFFFFFF;
  Box60.ColorTo       = 0xFFFFFF;
  Box60.Press_ColorTo = 0xC0C0C0;
  Box60.Gradient      = _FT812_BRUSH_GR_NONE;
  Box60.Active        = 1;
  Box60.OnUp          = 0;
  Box60.OnDown        = 0;
  Box60.OnClick       = 0;
  Box60.OnPress       = 0;

  Box62.OwnerScreen   = &Light_Mode;
  Box62.Order         = 22;
  Box62.Visible       = 1;
  Box62.Opacity       = 255;
  Box62.Tag           = 255;
  Box62.Left          = 417;
  Box62.Top           = 159;
  Box62.Width         = 285;
  Box62.Height        = 80;
  Box62.Pen_Width     = 1;
  Box62.Pen_Color     = 0x0000;
  Box62.Color         = 0x00FF;
  Box62.Press_Color   = 0xFFFFFF;
  Box62.ColorTo       = 0xFFFF00;
  Box62.Press_ColorTo = 0xC0C0C0;
  Box62.Gradient      = _FT812_BRUSH_GR_NONE;
  Box62.Active        = 1;
  Box62.OnUp          = 0;
  Box62.OnDown        = 0;
  Box62.OnClick       = 0;
  Box62.OnPress       = 0;

  Box63.OwnerScreen   = &Light_Mode;
  Box63.Order         = 23;
  Box63.Visible       = 1;
  Box63.Opacity       = 255;
  Box63.Tag           = 255;
  Box63.Left          = 417;
  Box63.Top           = 335;
  Box63.Width         = 285;
  Box63.Height        = 80;
  Box63.Pen_Width     = 1;
  Box63.Pen_Color     = 0x0000;
  Box63.Color         = 0x80FFFF;
  Box63.Press_Color   = 0xFFFFFF;
  Box63.ColorTo       = 0xFFFF00;
  Box63.Press_ColorTo = 0xC0C0C0;
  Box63.Gradient      = _FT812_BRUSH_GR_LEFT_TO_RIGHT;
  Box63.Active        = 1;
  Box63.OnUp          = 0;
  Box63.OnDown        = 0;
  Box63.OnClick       = 0;
  Box63.OnPress       = 0;

  CircleButton5.OwnerScreen   = &Light_Mode;
  CircleButton5.Order         = 24;
  CircleButton5.Visible       = 1;
  CircleButton5.Opacity       = 255;
  CircleButton5.Tag           = 255;
  CircleButton5.Left          = 336;
  CircleButton5.Top           = 166;
  CircleButton5.Radius        = 33;
  CircleButton5.Pen_Width     = 1;
  CircleButton5.Pen_Color     = 0xFFFFFF;
  CircleButton5.Color         = 0x00FF;
  CircleButton5.Press_Color   = 0xE2E2E2;
  CircleButton5.Caption       = CircleButton5_Caption;
  CircleButton5.TextAlign     = taCenter;
  CircleButton5.FontName      = 30;
  CircleButton5.Font_Color    = 0xFFFFFF;
  CircleButton5.FontHandle    = 30;
  CircleButton5.Source        = -1UL;
  CircleButton5.Active        = 1;
  CircleButton5.OnUp          = 0;
  CircleButton5.OnDown        = 0;
  CircleButton5.OnClick       = 0;
  CircleButton5.OnPress       = &CircleButton5_OnPress;

  CircleButton5_OnPress.Action       = CircleButton5OnPress;
  CircleButton5_OnPress.Sound.SndAct = VTFT_SNDACT_NONE;
  CircleButton5_OnPress.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  CircleButton5_OnPress.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  CircleButton5_OnPress.Sound.Volume = 255;

  CircleButton6.OwnerScreen   = &Light_Mode;
  CircleButton6.Order         = 25;
  CircleButton6.Visible       = 1;
  CircleButton6.Opacity       = 255;
  CircleButton6.Tag           = 255;
  CircleButton6.Left          = 336;
  CircleButton6.Top           = 342;
  CircleButton6.Radius        = 33;
  CircleButton6.Pen_Width     = 1;
  CircleButton6.Pen_Color     = 0xFFFFFF;
  CircleButton6.Color         = 0x00FF;
  CircleButton6.Press_Color   = 0xE2E2E2;
  CircleButton6.Caption       = CircleButton6_Caption;
  CircleButton6.TextAlign     = taCenter;
  CircleButton6.FontName      = 30;
  CircleButton6.Font_Color    = 0xFFFFFF;
  CircleButton6.FontHandle    = 30;
  CircleButton6.Source        = -1UL;
  CircleButton6.Active        = 1;
  CircleButton6.OnUp          = 0;
  CircleButton6.OnDown        = 0;
  CircleButton6.OnClick       = 0;
  CircleButton6.OnPress       = &CircleButton6_OnPress;

  CircleButton6_OnPress.Action       = CircleButton6OnPress;
  CircleButton6_OnPress.Sound.SndAct = VTFT_SNDACT_NONE;
  CircleButton6_OnPress.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  CircleButton6_OnPress.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  CircleButton6_OnPress.Sound.Volume = 255;

  CircleButton7.OwnerScreen   = &Light_Mode;
  CircleButton7.Order         = 26;
  CircleButton7.Visible       = 1;
  CircleButton7.Opacity       = 255;
  CircleButton7.Tag           = 255;
  CircleButton7.Left          = 717;
  CircleButton7.Top           = 166;
  CircleButton7.Radius        = 33;
  CircleButton7.Pen_Width     = 1;
  CircleButton7.Pen_Color     = 0xFFFFFF;
  CircleButton7.Color         = 0x00FF;
  CircleButton7.Press_Color   = 0xE2E2E2;
  CircleButton7.Caption       = CircleButton7_Caption;
  CircleButton7.TextAlign     = taCenter;
  CircleButton7.FontName      = 30;
  CircleButton7.Font_Color    = 0xFFFFFF;
  CircleButton7.FontHandle    = 30;
  CircleButton7.Source        = -1UL;
  CircleButton7.Active        = 1;
  CircleButton7.OnUp          = 0;
  CircleButton7.OnDown        = 0;
  CircleButton7.OnClick       = 0;
  CircleButton7.OnPress       = &CircleButton7_OnPress;

  CircleButton7_OnPress.Action       = CircleButton7OnPress;
  CircleButton7_OnPress.Sound.SndAct = VTFT_SNDACT_NONE;
  CircleButton7_OnPress.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  CircleButton7_OnPress.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  CircleButton7_OnPress.Sound.Volume = 255;

  CircleButton8.OwnerScreen   = &Light_Mode;
  CircleButton8.Order         = 27;
  CircleButton8.Visible       = 1;
  CircleButton8.Opacity       = 255;
  CircleButton8.Tag           = 255;
  CircleButton8.Left          = 717;
  CircleButton8.Top           = 342;
  CircleButton8.Radius        = 33;
  CircleButton8.Pen_Width     = 1;
  CircleButton8.Pen_Color     = 0xFFFFFF;
  CircleButton8.Color         = 0x00FF;
  CircleButton8.Press_Color   = 0xE2E2E2;
  CircleButton8.Caption       = CircleButton8_Caption;
  CircleButton8.TextAlign     = taCenter;
  CircleButton8.FontName      = 30;
  CircleButton8.Font_Color    = 0xFFFFFF;
  CircleButton8.FontHandle    = 30;
  CircleButton8.Source        = -1UL;
  CircleButton8.Active        = 1;
  CircleButton8.OnUp          = 0;
  CircleButton8.OnDown        = 0;
  CircleButton8.OnClick       = 0;
  CircleButton8.OnPress       = &CircleButton8_OnPress;

  CircleButton8_OnPress.Action       = CircleButton8OnPress;
  CircleButton8_OnPress.Sound.SndAct = VTFT_SNDACT_NONE;
  CircleButton8_OnPress.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  CircleButton8_OnPress.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  CircleButton8_OnPress.Sound.Volume = 255;

  Box64.OwnerScreen   = &Light_Mode;
  Box64.Order         = 28;
  Box64.Visible       = 1;
  Box64.Opacity       = 255;
  Box64.Tag           = 255;
  Box64.Left          = 417;
  Box64.Top           = 159;
  Box64.Width         = 25;
  Box64.Height        = 80;
  Box64.Pen_Width     = 1;
  Box64.Pen_Color     = 0xFFFFFF;
  Box64.Color         = 0x80C0;
  Box64.Press_Color   = 0xFFFFFF;
  Box64.ColorTo       = 0xFFFFFF;
  Box64.Press_ColorTo = 0xC0C0C0;
  Box64.Gradient      = _FT812_BRUSH_GR_NONE;
  Box64.Active        = 1;
  Box64.OnUp          = 0;
  Box64.OnDown        = 0;
  Box64.OnClick       = 0;
  Box64.OnPress       = 0;

  Box65.OwnerScreen   = &Light_Mode;
  Box65.Order         = 29;
  Box65.Visible       = 1;
  Box65.Opacity       = 255;
  Box65.Tag           = 255;
  Box65.Left          = 417;
  Box65.Top           = 335;
  Box65.Width         = 25;
  Box65.Height        = 80;
  Box65.Pen_Width     = 1;
  Box65.Pen_Color     = 0xFFFFFF;
  Box65.Color         = 0x80C0;
  Box65.Press_Color   = 0xFFFFFF;
  Box65.ColorTo       = 0xFFFFFF;
  Box65.Press_ColorTo = 0xC0C0C0;
  Box65.Gradient      = _FT812_BRUSH_GR_NONE;
  Box65.Active        = 1;
  Box65.OnUp          = 0;
  Box65.OnDown        = 0;
  Box65.OnClick       = 0;
  Box65.OnPress       = 0;

  Label48.OwnerScreen = &Light_Mode;
  Label48.Order       = 30;
  Label48.Visible     = 1;
  Label48.Opacity     = 255;
  Label48.Tag         = 255;
  Label48.Left        = 211;
  Label48.Top         = 103;
  Label48.Width       = 59;
  Label48.Height      = 25;
  Label48.Caption     = Label48_Caption;
  Label48.FontName    = 29;
  Label48.Font_Color  = 0xFFFF00;
  Label48.FontHandle  = 29;
  Label48.Source      = -1UL;
  Label48.Active      = 0;
  Label48.OnUp        = 0;
  Label48.OnDown      = 0;
  Label48.OnClick     = 0;
  Label48.OnPress     = 0;

  Label49.OwnerScreen = &Light_Mode;
  Label49.Order       = 31;
  Label49.Visible     = 1;
  Label49.Opacity     = 255;
  Label49.Tag         = 255;
  Label49.Left        = 212;
  Label49.Top         = 186;
  Label49.Width       = 59;
  Label49.Height      = 25;
  Label49.Caption     = Label49_Caption;
  Label49.FontName    = 29;
  Label49.Font_Color  = 0xFFFFFF;
  Label49.FontHandle  = 29;
  Label49.Source      = -1UL;
  Label49.Active      = 0;
  Label49.OnUp        = 0;
  Label49.OnDown      = 0;
  Label49.OnClick     = 0;
  Label49.OnPress     = 0;

  Label50.OwnerScreen = &Light_Mode;
  Label50.Order       = 32;
  Label50.Visible     = 1;
  Label50.Opacity     = 255;
  Label50.Tag         = 255;
  Label50.Left        = 206;
  Label50.Top         = 274;
  Label50.Width       = 70;
  Label50.Height      = 25;
  Label50.Caption     = Label50_Caption;
  Label50.FontName    = 29;
  Label50.Font_Color  = 0xFFFFFF;
  Label50.FontHandle  = 29;
  Label50.Source      = -1UL;
  Label50.Active      = 0;
  Label50.OnUp        = 0;
  Label50.OnDown      = 0;
  Label50.OnClick     = 0;
  Label50.OnPress     = 0;

  Label51.OwnerScreen = &Light_Mode;
  Label51.Order       = 33;
  Label51.Visible     = 1;
  Label51.Opacity     = 255;
  Label51.Tag         = 255;
  Label51.Left        = 210;
  Label51.Top         = 362;
  Label51.Width       = 61;
  Label51.Height      = 25;
  Label51.Caption     = Label51_Caption;
  Label51.FontName    = 29;
  Label51.Font_Color  = 0xFFFFFF;
  Label51.FontHandle  = 29;
  Label51.Source      = -1UL;
  Label51.Active      = 0;
  Label51.OnUp        = 0;
  Label51.OnDown      = 0;
  Label51.OnClick     = 0;
  Label51.OnPress     = 0;

  Label52.OwnerScreen = &Light_Mode;
  Label52.Order       = 34;
  Label52.Visible     = 1;
  Label52.Opacity     = 255;
  Label52.Tag         = 255;
  Label52.Left        = 356;
  Label52.Top         = 98;
  Label52.Width       = 74;
  Label52.Height      = 25;
  Label52.Caption     = Label52_Caption;
  Label52.FontName    = 29;
  Label52.Font_Color  = 0xFFFFFF;
  Label52.FontHandle  = 29;
  Label52.Source      = -1UL;
  Label52.Active      = 1;
  Label52.OnUp        = 0;
  Label52.OnDown      = 0;
  Label52.OnClick     = 0;
  Label52.OnPress     = 0;

  Label53.OwnerScreen = &Light_Mode;
  Label53.Order       = 35;
  Label53.Visible     = 1;
  Label53.Opacity     = 255;
  Label53.Tag         = 255;
  Label53.Left        = 356;
  Label53.Top         = 274;
  Label53.Width       = 118;
  Label53.Height      = 25;
  Label53.Caption     = Label53_Caption;
  Label53.FontName    = 29;
  Label53.Font_Color  = 0xFFFFFF;
  Label53.FontHandle  = 29;
  Label53.Source      = -1UL;
  Label53.Active      = 1;
  Label53.OnUp        = 0;
  Label53.OnDown      = 0;
  Label53.OnClick     = 0;
  Label53.OnPress     = 0;

  Label88.OwnerScreen = &Light_Mode;
  Label88.Order       = 36;
  Label88.Visible     = 1;
  Label88.Opacity     = 255;
  Label88.Tag         = 255;
  Label88.Left        = 731;
  Label88.Top         = 98;
  Label88.Width       = 14;
  Label88.Height      = 25;
  Label88.Caption     = Label88_Caption;
  Label88.FontName    = 29;
  Label88.Font_Color  = 0xFFFFFF;
  Label88.FontHandle  = 29;
  Label88.Source      = -1UL;
  Label88.Active      = 1;
  Label88.OnUp        = 0;
  Label88.OnDown      = 0;
  Label88.OnClick     = 0;
  Label88.OnPress     = 0;

  Label89.OwnerScreen = &Light_Mode;
  Label89.Order       = 37;
  Label89.Visible     = 1;
  Label89.Opacity     = 255;
  Label89.Tag         = 255;
  Label89.Left        = 729;
  Label89.Top         = 274;
  Label89.Width       = 12;
  Label89.Height      = 25;
  Label89.Caption     = Label89_Caption;
  Label89.FontName    = 29;
  Label89.Font_Color  = 0xFFFFFF;
  Label89.FontHandle  = 29;
  Label89.Source      = -1UL;
  Label89.Active      = 1;
  Label89.OnUp        = 0;
  Label89.OnDown      = 0;
  Label89.OnClick     = 0;
  Label89.OnPress     = 0;

  Label97.OwnerScreen = &Light_Mode;
  Label97.Order       = 38;
  Label97.Visible     = 1;
  Label97.Opacity     = 255;
  Label97.Tag         = 255;
  Label97.Left        = 666;
  Label97.Top         = 98;
  Label97.Width       = 35;
  Label97.Height      = 25;
  Label97.Caption     = Label97_Caption;
  Label97.FontName    = 29;
  Label97.Font_Color  = 0xFFFFFF;
  Label97.FontHandle  = 29;
  Label97.Source      = -1UL;
  Label97.Active      = 1;
  Label97.OnUp        = 0;
  Label97.OnDown      = 0;
  Label97.OnClick     = 0;
  Label97.OnPress     = 0;

  Label98.OwnerScreen = &Light_Mode;
  Label98.Order       = 39;
  Label98.Visible     = 1;
  Label98.Opacity     = 255;
  Label98.Tag         = 255;
  Label98.Left        = 666;
  Label98.Top         = 274;
  Label98.Width       = 41;
  Label98.Height      = 25;
  Label98.Caption     = Label98_Caption;
  Label98.FontName    = 29;
  Label98.Font_Color  = 0xFFFFFF;
  Label98.FontHandle  = 29;
  Label98.Source      = -1UL;
  Label98.Active      = 1;
  Label98.OnUp        = 0;
  Label98.OnDown      = 0;
  Label98.OnClick     = 0;
  Label98.OnPress     = 0;

  Button37.OwnerScreen   = &Keyboard_Screen;
  Button37.Order         = 0;
  Button37.Visible       = 1;
  Button37.Opacity       = 255;
  Button37.Tag           = 255;
  Button37.Left          = 8;
  Button37.Top           = 8;
  Button37.Width         = 782;
  Button37.Height        = 55;
  Button37.Pen_Width     = 1;
  Button37.Pen_Color     = 0x0000;
  Button37.Color         = 0x80C0;
  Button37.Press_Color   = 0xFFFFFF;
  Button37.ColorTo       = 0xFFFFFF;
  Button37.Press_ColorTo = 0xC0C0C0;
  Button37.Gradient      = _FT812_BRUSH_GR_NONE;
  Button37.Caption       = Button37_Caption;
  Button37.TextAlign     = taCenter;
  Button37.FontName      = 31;
  Button37.Font_Color    = 0xFFFFFF;
  Button37.FontHandle    = 31;
  Button37.Source        = -1UL;
  Button37.Active        = 1;
  Button37.OnUp          = 0;
  Button37.OnDown        = 0;
  Button37.OnClick       = 0;
  Button37.OnPress       = 0;

  Button38.OwnerScreen   = &Keyboard_Screen;
  Button38.Order         = 1;
  Button38.Visible       = 1;
  Button38.Opacity       = 255;
  Button38.Tag           = 255;
  Button38.Left          = 8;
  Button38.Top           = 423;
  Button38.Width         = 50;
  Button38.Height        = 50;
  Button38.Pen_Width     = 1;
  Button38.Pen_Color     = 0x0000;
  Button38.Color         = 0x808080;
  Button38.Press_Color   = 0xFFFFFF;
  Button38.ColorTo       = 0xFFFFFF;
  Button38.Press_ColorTo = 0xC0C0C0;
  Button38.Gradient      = _FT812_BRUSH_GR_NONE;
  Button38.Caption       = Button38_Caption;
  Button38.TextAlign     = taCenter;
  Button38.FontName      = 30;
  Button38.Font_Color    = 0xFFFFFF;
  Button38.FontHandle    = 30;
  Button38.Source        = -1UL;
  Button38.Active        = 1;
  Button38.OnUp          = 0;
  Button38.OnDown        = 0;
  Button38.OnClick       = &Button38_OnClick;
  Button38.OnPress       = 0;

  Button38_OnClick.Action       = Button12OnClick;
  Button38_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button38_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button38_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button38_OnClick.Sound.Volume = 255;

  Box11.OwnerScreen   = &Keyboard_Screen;
  Box11.Order         = 2;
  Box11.Visible       = 1;
  Box11.Opacity       = 255;
  Box11.Tag           = 255;
  Box11.Left          = 66;
  Box11.Top           = 423;
  Box11.Width         = 724;
  Box11.Height        = 50;
  Box11.Pen_Width     = 1;
  Box11.Pen_Color     = 0x0000;
  Box11.Color         = 0x404040;
  Box11.Press_Color   = 0xFFFFFF;
  Box11.ColorTo       = 0xFFFFFF;
  Box11.Press_ColorTo = 0xC0C0C0;
  Box11.Gradient      = _FT812_BRUSH_GR_NONE;
  Box11.Active        = 1;
  Box11.OnUp          = 0;
  Box11.OnDown        = 0;
  Box11.OnClick       = 0;
  Box11.OnPress       = 0;

  Box24.OwnerScreen   = &Keyboard_Screen;
  Box24.Order         = 3;
  Box24.Visible       = 1;
  Box24.Opacity       = 255;
  Box24.Tag           = 255;
  Box24.Left          = 8;
  Box24.Top           = 71;
  Box24.Width         = 782;
  Box24.Height        = 80;
  Box24.Pen_Width     = 1;
  Box24.Pen_Color     = 0x0000;
  Box24.Color         = 0x800080;
  Box24.Press_Color   = 0xFFFFFF;
  Box24.ColorTo       = 0xFFFFFF;
  Box24.Press_ColorTo = 0xC0C0C0;
  Box24.Gradient      = _FT812_BRUSH_GR_NONE;
  Box24.Active        = 1;
  Box24.OnUp          = 0;
  Box24.OnDown        = 0;
  Box24.OnClick       = &Box24_OnClick;
  Box24.OnPress       = 0;

  Box24_OnClick.Action       = Box24OnClick;
  Box24_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Box24_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Box24_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Box24_OnClick.Sound.Volume = 255;

  Box76.OwnerScreen   = &Keyboard_Screen;
  Box76.Order         = 4;
  Box76.Visible       = 1;
  Box76.Opacity       = 255;
  Box76.Tag           = 255;
  Box76.Left          = 8;
  Box76.Top           = 158;
  Box76.Width         = 782;
  Box76.Height        = 80;
  Box76.Pen_Width     = 1;
  Box76.Pen_Color     = 0x0000;
  Box76.Color         = 0x00FF;
  Box76.Press_Color   = 0xFFFFFF;
  Box76.ColorTo       = 0xFFFFFF;
  Box76.Press_ColorTo = 0xC0C0C0;
  Box76.Gradient      = _FT812_BRUSH_GR_NONE;
  Box76.Active        = 1;
  Box76.OnUp          = 0;
  Box76.OnDown        = 0;
  Box76.OnClick       = &Box76_OnClick;
  Box76.OnPress       = 0;

  Box76_OnClick.Action       = Box76OnClick;
  Box76_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Box76_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Box76_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Box76_OnClick.Sound.Volume = 255;

  Box77.OwnerScreen   = &Keyboard_Screen;
  Box77.Order         = 5;
  Box77.Visible       = 1;
  Box77.Opacity       = 255;
  Box77.Tag           = 255;
  Box77.Left          = 8;
  Box77.Top           = 247;
  Box77.Width         = 782;
  Box77.Height        = 80;
  Box77.Pen_Width     = 1;
  Box77.Pen_Color     = 0x0000;
  Box77.Color         = 0xFF8000;
  Box77.Press_Color   = 0xFFFFFF;
  Box77.ColorTo       = 0xFFFFFF;
  Box77.Press_ColorTo = 0xC0C0C0;
  Box77.Gradient      = _FT812_BRUSH_GR_NONE;
  Box77.Active        = 1;
  Box77.OnUp          = 0;
  Box77.OnDown        = 0;
  Box77.OnClick       = &Box77_OnClick;
  Box77.OnPress       = 0;

  Box77_OnClick.Action       = Box77OnClick;
  Box77_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Box77_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Box77_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Box77_OnClick.Sound.Volume = 255;

  Box78.OwnerScreen   = &Keyboard_Screen;
  Box78.Order         = 6;
  Box78.Visible       = 1;
  Box78.Opacity       = 255;
  Box78.Tag           = 255;
  Box78.Left          = 8;
  Box78.Top           = 335;
  Box78.Width         = 782;
  Box78.Height        = 80;
  Box78.Pen_Width     = 1;
  Box78.Pen_Color     = 0x0000;
  Box78.Color         = 0x8000;
  Box78.Press_Color   = 0xFFFFFF;
  Box78.ColorTo       = 0xFFFFFF;
  Box78.Press_ColorTo = 0xC0C0C0;
  Box78.Gradient      = _FT812_BRUSH_GR_NONE;
  Box78.Active        = 1;
  Box78.OnUp          = 0;
  Box78.OnDown        = 0;
  Box78.OnClick       = &Box78_OnClick;
  Box78.OnPress       = 0;

  Box78_OnClick.Action       = Box78OnClick;
  Box78_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Box78_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Box78_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Box78_OnClick.Sound.Volume = 255;

  CheckBox1.OwnerScreen      = &Keyboard_Screen;
  CheckBox1.Order            = 7;
  CheckBox1.Visible          = 1;
  CheckBox1.Opacity          = 255;
  CheckBox1.Tag              = 255;
  CheckBox1.Left             = 650;
  CheckBox1.Top              = 102;
  CheckBox1.Width            = 21;
  CheckBox1.Height           = 17;
  CheckBox1.Pen_Width        = 2;
  CheckBox1.Pen_Color        = 0xFFFFFF;
  CheckBox1.Background_Color = 0x0000;
  CheckBox1.Color            = 0x800080;
  CheckBox1.Press_Color      = 0xE2E2E2;
  CheckBox1.Caption          = CheckBox1_Caption;
  CheckBox1.TextAlign        = taLeft;
  CheckBox1.FontName         = 26;
  CheckBox1.Font_Color       = 0x0000;
  CheckBox1.FontHandle       = 26;
  CheckBox1.Source           = -1UL;
  CheckBox1.Corner_Radius    = 3;
  CheckBox1.Checked          = 1;
  CheckBox1.Active           = 1;
  CheckBox1.OnUp             = 0;
  CheckBox1.OnDown           = 0;
  CheckBox1.OnClick          = 0;
  CheckBox1.OnPress          = 0;

  CheckBox2.OwnerScreen      = &Keyboard_Screen;
  CheckBox2.Order            = 8;
  CheckBox2.Visible          = 1;
  CheckBox2.Opacity          = 255;
  CheckBox2.Tag              = 255;
  CheckBox2.Left             = 650;
  CheckBox2.Top              = 190;
  CheckBox2.Width            = 21;
  CheckBox2.Height           = 17;
  CheckBox2.Pen_Width        = 2;
  CheckBox2.Pen_Color        = 0xFFFFFF;
  CheckBox2.Background_Color = 0x0000;
  CheckBox2.Color            = 0x00FF;
  CheckBox2.Press_Color      = 0xE2E2E2;
  CheckBox2.Caption          = CheckBox2_Caption;
  CheckBox2.TextAlign        = taLeft;
  CheckBox2.FontName         = 26;
  CheckBox2.Font_Color       = 0x0000;
  CheckBox2.FontHandle       = 26;
  CheckBox2.Source           = -1UL;
  CheckBox2.Corner_Radius    = 3;
  CheckBox2.Checked          = 0;
  CheckBox2.Active           = 1;
  CheckBox2.OnUp             = 0;
  CheckBox2.OnDown           = 0;
  CheckBox2.OnClick          = 0;
  CheckBox2.OnPress          = 0;

  CheckBox3.OwnerScreen      = &Keyboard_Screen;
  CheckBox3.Order            = 9;
  CheckBox3.Visible          = 1;
  CheckBox3.Opacity          = 255;
  CheckBox3.Tag              = 255;
  CheckBox3.Left             = 650;
  CheckBox3.Top              = 278;
  CheckBox3.Width            = 21;
  CheckBox3.Height           = 17;
  CheckBox3.Pen_Width        = 2;
  CheckBox3.Pen_Color        = 0xFFFFFF;
  CheckBox3.Background_Color = 0x0000;
  CheckBox3.Color            = 0xFF8000;
  CheckBox3.Press_Color      = 0xE2E2E2;
  CheckBox3.Caption          = CheckBox3_Caption;
  CheckBox3.TextAlign        = taLeft;
  CheckBox3.FontName         = 26;
  CheckBox3.Font_Color       = 0x0000;
  CheckBox3.FontHandle       = 26;
  CheckBox3.Source           = -1UL;
  CheckBox3.Corner_Radius    = 3;
  CheckBox3.Checked          = 0;
  CheckBox3.Active           = 1;
  CheckBox3.OnUp             = 0;
  CheckBox3.OnDown           = 0;
  CheckBox3.OnClick          = 0;
  CheckBox3.OnPress          = 0;

  CheckBox4.OwnerScreen      = &Keyboard_Screen;
  CheckBox4.Order            = 10;
  CheckBox4.Visible          = 1;
  CheckBox4.Opacity          = 255;
  CheckBox4.Tag              = 255;
  CheckBox4.Left             = 650;
  CheckBox4.Top              = 366;
  CheckBox4.Width            = 21;
  CheckBox4.Height           = 17;
  CheckBox4.Pen_Width        = 2;
  CheckBox4.Pen_Color        = 0xFFFFFF;
  CheckBox4.Background_Color = 0x0000;
  CheckBox4.Color            = 0x8000;
  CheckBox4.Press_Color      = 0xE2E2E2;
  CheckBox4.Caption          = CheckBox4_Caption;
  CheckBox4.TextAlign        = taLeft;
  CheckBox4.FontName         = 26;
  CheckBox4.Font_Color       = 0x0000;
  CheckBox4.FontHandle       = 26;
  CheckBox4.Source           = -1UL;
  CheckBox4.Corner_Radius    = 3;
  CheckBox4.Checked          = 0;
  CheckBox4.Active           = 1;
  CheckBox4.OnUp             = 0;
  CheckBox4.OnDown           = 0;
  CheckBox4.OnClick          = 0;
  CheckBox4.OnPress          = 0;

  Label6.OwnerScreen = &Keyboard_Screen;
  Label6.Order       = 11;
  Label6.Visible     = 1;
  Label6.Opacity     = 255;
  Label6.Tag         = 255;
  Label6.Left        = 69;
  Label6.Top         = 98;
  Label6.Width       = 136;
  Label6.Height      = 25;
  Label6.Caption     = Label6_Caption;
  Label6.FontName    = 29;
  Label6.Font_Color  = 0xFFFFFF;
  Label6.FontHandle  = 29;
  Label6.Source      = -1UL;
  Label6.Active      = 0;
  Label6.OnUp        = 0;
  Label6.OnDown      = 0;
  Label6.OnClick     = 0;
  Label6.OnPress     = 0;

  Label26.OwnerScreen = &Keyboard_Screen;
  Label26.Order       = 12;
  Label26.Visible     = 1;
  Label26.Opacity     = 255;
  Label26.Tag         = 255;
  Label26.Left        = 69;
  Label26.Top         = 186;
  Label26.Width       = 142;
  Label26.Height      = 25;
  Label26.Caption     = Label26_Caption;
  Label26.FontName    = 29;
  Label26.Font_Color  = 0xFFFFFF;
  Label26.FontHandle  = 29;
  Label26.Source      = -1UL;
  Label26.Active      = 0;
  Label26.OnUp        = 0;
  Label26.OnDown      = 0;
  Label26.OnClick     = 0;
  Label26.OnPress     = 0;

  Label27.OwnerScreen = &Keyboard_Screen;
  Label27.Order       = 13;
  Label27.Visible     = 1;
  Label27.Opacity     = 255;
  Label27.Tag         = 255;
  Label27.Left        = 69;
  Label27.Top         = 274;
  Label27.Width       = 101;
  Label27.Height      = 25;
  Label27.Caption     = Label27_Caption;
  Label27.FontName    = 29;
  Label27.Font_Color  = 0xFFFFFF;
  Label27.FontHandle  = 29;
  Label27.Source      = -1UL;
  Label27.Active      = 0;
  Label27.OnUp        = 0;
  Label27.OnDown      = 0;
  Label27.OnClick     = 0;
  Label27.OnPress     = 0;

  Label28.OwnerScreen = &Keyboard_Screen;
  Label28.Order       = 14;
  Label28.Visible     = 1;
  Label28.Opacity     = 255;
  Label28.Tag         = 255;
  Label28.Left        = 69;
  Label28.Top         = 362;
  Label28.Width       = 106;
  Label28.Height      = 25;
  Label28.Caption     = Label28_Caption;
  Label28.FontName    = 29;
  Label28.Font_Color  = 0xFFFFFF;
  Label28.FontHandle  = 29;
  Label28.Source      = -1UL;
  Label28.Active      = 0;
  Label28.OnUp        = 0;
  Label28.OnDown      = 0;
  Label28.OnClick     = 0;
  Label28.OnPress     = 0;

  Box25.OwnerScreen   = &System_Log_Screen;
  Box25.Order         = 0;
  Box25.Visible       = 1;
  Box25.Opacity       = 255;
  Box25.Tag           = 255;
  Box25.Left          = 228;
  Box25.Top           = 335;
  Box25.Width         = 211;
  Box25.Height        = 80;
  Box25.Pen_Width     = 1;
  Box25.Pen_Color     = 0x0000;
  Box25.Color         = 0x0080;
  Box25.Press_Color   = 0xFFFFFF;
  Box25.ColorTo       = 0xFFFFFF;
  Box25.Press_ColorTo = 0xC0C0C0;
  Box25.Gradient      = _FT812_BRUSH_GR_NONE;
  Box25.Active        = 1;
  Box25.OnUp          = 0;
  Box25.OnDown        = 0;
  Box25.OnClick       = &Box25_OnClick;
  Box25.OnPress       = 0;

  Box25_OnClick.Action       = Box25OnClick;
  Box25_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Box25_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Box25_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Box25_OnClick.Sound.Volume = 255;

  Image69.OwnerScreen   = &System_Log_Screen;
  Image69.Order         = 1;
  Image69.Visible       = 0;
  Image69.Opacity       = 255;
  Image69.Tag           = 255;
  Image69.Left          = 259;
  Image69.Top           = 343;
  Image69.Width         = 63;
  Image69.Height        = 64;
  Image69.Picture_Name  = sun3_bmp;
  Image69.Picture_Type  = 7;
  Image69.Picture_Ratio = 1;
  Image69.Blend_Color   = 0xFFFFFF;
  Image69.Source        = -1L;
  Image69.RotationAngle = 0;
  Image69.RotationCenterLeft = 292;
  Image69.RotationCenterTop = 375;
  Image69.ScaleX        = 1.0000;
  Image69.ScaleY        = 1.0000;
  Image69.Active        = 0;
  Image69.OnUp          = 0;
  Image69.OnDown        = 0;
  Image69.OnClick       = 0;
  Image69.OnPress       = 0;

  Image70.OwnerScreen   = &System_Log_Screen;
  Image70.Order         = 2;
  Image70.Visible       = 1;
  Image70.Opacity       = 255;
  Image70.Tag           = 255;
  Image70.Left          = 259;
  Image70.Top           = 343;
  Image70.Width         = 63;
  Image70.Height        = 64;
  Image70.Picture_Name  = sun4_bmp;
  Image70.Picture_Type  = 7;
  Image70.Picture_Ratio = 1;
  Image70.Blend_Color   = 0xFFFFFF;
  Image70.Source        = -1L;
  Image70.RotationAngle = 0;
  Image70.RotationCenterLeft = 290;
  Image70.RotationCenterTop = 375;
  Image70.ScaleX        = 1.0000;
  Image70.ScaleY        = 1.0000;
  Image70.Active        = 0;
  Image70.OnUp          = 0;
  Image70.OnDown        = 0;
  Image70.OnClick       = 0;
  Image70.OnPress       = 0;

  Box10.OwnerScreen   = &System_Log_Screen;
  Box10.Order         = 3;
  Box10.Visible       = 1;
  Box10.Opacity       = 255;
  Box10.Tag           = 255;
  Box10.Left          = 8;
  Box10.Top           = 335;
  Box10.Width         = 211;
  Box10.Height        = 80;
  Box10.Pen_Width     = 1;
  Box10.Pen_Color     = 0x0000;
  Box10.Color         = 0x0080;
  Box10.Press_Color   = 0xFFFFFF;
  Box10.ColorTo       = 0xFFFFFF;
  Box10.Press_ColorTo = 0xC0C0C0;
  Box10.Gradient      = _FT812_BRUSH_GR_NONE;
  Box10.Active        = 1;
  Box10.OnUp          = 0;
  Box10.OnDown        = 0;
  Box10.OnClick       = &Box10_OnClick;
  Box10.OnPress       = 0;

  Box10_OnClick.Action       = Box10OnClick;
  Box10_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Box10_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Box10_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Box10_OnClick.Sound.Volume = 255;

  Image67.OwnerScreen   = &System_Log_Screen;
  Image67.Order         = 4;
  Image67.Visible       = 1;
  Image67.Opacity       = 255;
  Image67.Tag           = 255;
  Image67.Left          = 44;
  Image67.Top           = 348;
  Image67.Width         = 58;
  Image67.Height        = 54;
  Image67.Picture_Name  = moon3_bmp;
  Image67.Picture_Type  = 7;
  Image67.Picture_Ratio = 1;
  Image67.Blend_Color   = 0xFFFFFF;
  Image67.Source        = -1L;
  Image67.RotationAngle = 0;
  Image67.RotationCenterLeft = 73;
  Image67.RotationCenterTop = 375;
  Image67.ScaleX        = 1.0000;
  Image67.ScaleY        = 1.0000;
  Image67.Active        = 0;
  Image67.OnUp          = 0;
  Image67.OnDown        = 0;
  Image67.OnClick       = 0;
  Image67.OnPress       = 0;

  Image68.OwnerScreen   = &System_Log_Screen;
  Image68.Order         = 5;
  Image68.Visible       = 0;
  Image68.Opacity       = 255;
  Image68.Tag           = 255;
  Image68.Left          = 44;
  Image68.Top           = 348;
  Image68.Width         = 58;
  Image68.Height        = 54;
  Image68.Picture_Name  = moon4_bmp;
  Image68.Picture_Type  = 7;
  Image68.Picture_Ratio = 1;
  Image68.Blend_Color   = 0xFFFFFF;
  Image68.Source        = -1L;
  Image68.RotationAngle = 0;
  Image68.RotationCenterLeft = 73;
  Image68.RotationCenterTop = 375;
  Image68.ScaleX        = 1.0000;
  Image68.ScaleY        = 1.0000;
  Image68.Active        = 0;
  Image68.OnUp          = 0;
  Image68.OnDown        = 0;
  Image68.OnClick       = 0;
  Image68.OnPress       = 0;

  Box30.OwnerScreen   = &System_Log_Screen;
  Box30.Order         = 6;
  Box30.Visible       = 1;
  Box30.Opacity       = 255;
  Box30.Tag           = 255;
  Box30.Left          = 8;
  Box30.Top           = 71;
  Box30.Width         = 431;
  Box30.Height        = 256;
  Box30.Pen_Width     = 1;
  Box30.Pen_Color     = 0x0000;
  Box30.Color         = 0x8040;
  Box30.Press_Color   = 0xFFFFFF;
  Box30.ColorTo       = 0xFFFFFF;
  Box30.Press_ColorTo = 0xC0C0C0;
  Box30.Gradient      = _FT812_BRUSH_GR_NONE;
  Box30.Active        = 1;
  Box30.OnUp          = 0;
  Box30.OnDown        = 0;
  Box30.OnClick       = 0;
  Box30.OnPress       = 0;

  Image66.OwnerScreen   = &System_Log_Screen;
  Image66.Order         = 7;
  Image66.Visible       = 0;
  Image66.Opacity       = 255;
  Image66.Tag           = 255;
  Image66.Left          = 198;
  Image66.Top           = 174;
  Image66.Width         = 58;
  Image66.Height        = 50;
  Image66.Picture_Name  = circle1_bmp;
  Image66.Picture_Type  = 7;
  Image66.Picture_Ratio = 1;
  Image66.Blend_Color   = 0xFFFFFF;
  Image66.Source        = -1L;
  Image66.RotationAngle = 0;
  Image66.RotationCenterLeft = 228;
  Image66.RotationCenterTop = 199;
  Image66.ScaleX        = 1.0000;
  Image66.ScaleY        = 1.0000;
  Image66.Active        = 1;
  Image66.OnUp          = 0;
  Image66.OnDown        = 0;
  Image66.OnClick       = 0;
  Image66.OnPress       = 0;

  Image65.OwnerScreen   = &System_Log_Screen;
  Image65.Order         = 8;
  Image65.Visible       = 1;
  Image65.Opacity       = 255;
  Image65.Tag           = 255;
  Image65.Left          = 198;
  Image65.Top           = 174;
  Image65.Width         = 50;
  Image65.Height        = 50;
  Image65.Picture_Name  = circle2_bmp;
  Image65.Picture_Type  = 7;
  Image65.Picture_Ratio = 1;
  Image65.Blend_Color   = 0xFFFFFF;
  Image65.Source        = -1L;
  Image65.RotationAngle = 0;
  Image65.RotationCenterLeft = 224;
  Image65.RotationCenterTop = 199;
  Image65.ScaleX        = 1.0000;
  Image65.ScaleY        = 1.0000;
  Image65.Active        = 1;
  Image65.OnUp          = 0;
  Image65.OnDown        = 0;
  Image65.OnClick       = 0;
  Image65.OnPress       = 0;

  Button12.OwnerScreen   = &System_Log_Screen;
  Button12.Order         = 9;
  Button12.Visible       = 1;
  Button12.Opacity       = 255;
  Button12.Tag           = 255;
  Button12.Left          = 8;
  Button12.Top           = 423;
  Button12.Width         = 50;
  Button12.Height        = 50;
  Button12.Pen_Width     = 1;
  Button12.Pen_Color     = 0x0000;
  Button12.Color         = 0x808080;
  Button12.Press_Color   = 0xFFFFFF;
  Button12.ColorTo       = 0xFFFFFF;
  Button12.Press_ColorTo = 0xC0C0C0;
  Button12.Gradient      = _FT812_BRUSH_GR_NONE;
  Button12.Caption       = Button12_Caption;
  Button12.TextAlign     = taCenter;
  Button12.FontName      = 30;
  Button12.Font_Color    = 0xFFFFFF;
  Button12.FontHandle    = 30;
  Button12.Source        = -1UL;
  Button12.Active        = 1;
  Button12.OnUp          = 0;
  Button12.OnDown        = 0;
  Button12.OnClick       = &Button12_OnClick;
  Button12.OnPress       = 0;

  Button12_OnClick.Action       = Button12OnClick;
  Button12_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button12_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button12_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button12_OnClick.Sound.Volume = 255;

  Box9.OwnerScreen   = &System_Log_Screen;
  Box9.Order         = 10;
  Box9.Visible       = 1;
  Box9.Opacity       = 255;
  Box9.Tag           = 255;
  Box9.Left          = 66;
  Box9.Top           = 423;
  Box9.Width         = 724;
  Box9.Height        = 50;
  Box9.Pen_Width     = 1;
  Box9.Pen_Color     = 0x0000;
  Box9.Color         = 0x404040;
  Box9.Press_Color   = 0xFFFFFF;
  Box9.ColorTo       = 0xFFFFFF;
  Box9.Press_ColorTo = 0xC0C0C0;
  Box9.Gradient      = _FT812_BRUSH_GR_NONE;
  Box9.Active        = 1;
  Box9.OnUp          = 0;
  Box9.OnDown        = 0;
  Box9.OnClick       = 0;
  Box9.OnPress       = 0;

  Button13.OwnerScreen   = &System_Log_Screen;
  Button13.Order         = 11;
  Button13.Visible       = 1;
  Button13.Opacity       = 255;
  Button13.Tag           = 255;
  Button13.Left          = 447;
  Button13.Top           = 71;
  Button13.Width         = 80;
  Button13.Height        = 80;
  Button13.Pen_Width     = 1;
  Button13.Pen_Color     = 0x0000;
  Button13.Color         = 0xFFFF;
  Button13.Press_Color   = 0xFFFFFF;
  Button13.ColorTo       = 0xFFFFFF;
  Button13.Press_ColorTo = 0xC0C0C0;
  Button13.Gradient      = _FT812_BRUSH_GR_NONE;
  Button13.Caption       = Button13_Caption;
  Button13.TextAlign     = taCenter;
  Button13.FontName      = 29;
  Button13.Font_Color    = 0xFFFFFF;
  Button13.FontHandle    = 29;
  Button13.Source        = -1UL;
  Button13.Active        = 1;
  Button13.OnUp          = 0;
  Button13.OnDown        = 0;
  Button13.OnClick       = &Button13_OnClick;
  Button13.OnPress       = 0;

  Button13_OnClick.Action       = Button13OnClick;
  Button13_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button13_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button13_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button13_OnClick.Sound.Volume = 255;

  Button14.OwnerScreen   = &System_Log_Screen;
  Button14.Order         = 12;
  Button14.Visible       = 1;
  Button14.Opacity       = 255;
  Button14.Tag           = 255;
  Button14.Left          = 535;
  Button14.Top           = 71;
  Button14.Width         = 80;
  Button14.Height        = 80;
  Button14.Pen_Width     = 1;
  Button14.Pen_Color     = 0x0000;
  Button14.Color         = 0xFFFF;
  Button14.Press_Color   = 0xFFFFFF;
  Button14.ColorTo       = 0xFFFFFF;
  Button14.Press_ColorTo = 0xC0C0C0;
  Button14.Gradient      = _FT812_BRUSH_GR_NONE;
  Button14.Caption       = Button14_Caption;
  Button14.TextAlign     = taCenter;
  Button14.FontName      = 29;
  Button14.Font_Color    = 0xFFFFFF;
  Button14.FontHandle    = 29;
  Button14.Source        = -1UL;
  Button14.Active        = 1;
  Button14.OnUp          = 0;
  Button14.OnDown        = 0;
  Button14.OnClick       = &Button14_OnClick;
  Button14.OnPress       = 0;

  Button14_OnClick.Action       = Button14OnClick;
  Button14_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button14_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button14_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button14_OnClick.Sound.Volume = 255;

  Button15.OwnerScreen   = &System_Log_Screen;
  Button15.Order         = 13;
  Button15.Visible       = 1;
  Button15.Opacity       = 255;
  Button15.Tag           = 255;
  Button15.Left          = 623;
  Button15.Top           = 71;
  Button15.Width         = 80;
  Button15.Height        = 80;
  Button15.Pen_Width     = 1;
  Button15.Pen_Color     = 0x0000;
  Button15.Color         = 0xFFFF;
  Button15.Press_Color   = 0xFFFFFF;
  Button15.ColorTo       = 0xFFFFFF;
  Button15.Press_ColorTo = 0xC0C0C0;
  Button15.Gradient      = _FT812_BRUSH_GR_NONE;
  Button15.Caption       = Button15_Caption;
  Button15.TextAlign     = taCenter;
  Button15.FontName      = 29;
  Button15.Font_Color    = 0xFFFFFF;
  Button15.FontHandle    = 29;
  Button15.Source        = -1UL;
  Button15.Active        = 1;
  Button15.OnUp          = 0;
  Button15.OnDown        = 0;
  Button15.OnClick       = &Button15_OnClick;
  Button15.OnPress       = 0;

  Button15_OnClick.Action       = Button15OnClick;
  Button15_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button15_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button15_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button15_OnClick.Sound.Volume = 255;

  Button16.OwnerScreen   = &System_Log_Screen;
  Button16.Order         = 14;
  Button16.Visible       = 1;
  Button16.Opacity       = 255;
  Button16.Tag           = 255;
  Button16.Left          = 447;
  Button16.Top           = 159;
  Button16.Width         = 80;
  Button16.Height        = 80;
  Button16.Pen_Width     = 1;
  Button16.Pen_Color     = 0x0000;
  Button16.Color         = 0xFFFF;
  Button16.Press_Color   = 0xFFFFFF;
  Button16.ColorTo       = 0xFFFFFF;
  Button16.Press_ColorTo = 0xC0C0C0;
  Button16.Gradient      = _FT812_BRUSH_GR_NONE;
  Button16.Caption       = Button16_Caption;
  Button16.TextAlign     = taCenter;
  Button16.FontName      = 29;
  Button16.Font_Color    = 0xFFFFFF;
  Button16.FontHandle    = 29;
  Button16.Source        = -1UL;
  Button16.Active        = 1;
  Button16.OnUp          = 0;
  Button16.OnDown        = 0;
  Button16.OnClick       = &Button16_OnClick;
  Button16.OnPress       = 0;

  Button16_OnClick.Action       = Button16OnClick;
  Button16_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button16_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button16_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button16_OnClick.Sound.Volume = 255;

  Button17.OwnerScreen   = &System_Log_Screen;
  Button17.Order         = 15;
  Button17.Visible       = 1;
  Button17.Opacity       = 255;
  Button17.Tag           = 255;
  Button17.Left          = 535;
  Button17.Top           = 159;
  Button17.Width         = 80;
  Button17.Height        = 80;
  Button17.Pen_Width     = 1;
  Button17.Pen_Color     = 0x0000;
  Button17.Color         = 0xFFFF;
  Button17.Press_Color   = 0xFFFFFF;
  Button17.ColorTo       = 0xFFFFFF;
  Button17.Press_ColorTo = 0xC0C0C0;
  Button17.Gradient      = _FT812_BRUSH_GR_NONE;
  Button17.Caption       = Button17_Caption;
  Button17.TextAlign     = taCenter;
  Button17.FontName      = 29;
  Button17.Font_Color    = 0xFFFFFF;
  Button17.FontHandle    = 29;
  Button17.Source        = -1UL;
  Button17.Active        = 1;
  Button17.OnUp          = 0;
  Button17.OnDown        = 0;
  Button17.OnClick       = &Button17_OnClick;
  Button17.OnPress       = 0;

  Button17_OnClick.Action       = Button17OnClick;
  Button17_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button17_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button17_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button17_OnClick.Sound.Volume = 255;

  Button18.OwnerScreen   = &System_Log_Screen;
  Button18.Order         = 16;
  Button18.Visible       = 1;
  Button18.Opacity       = 255;
  Button18.Tag           = 255;
  Button18.Left          = 623;
  Button18.Top           = 159;
  Button18.Width         = 80;
  Button18.Height        = 80;
  Button18.Pen_Width     = 1;
  Button18.Pen_Color     = 0x0000;
  Button18.Color         = 0xFFFF;
  Button18.Press_Color   = 0xFFFFFF;
  Button18.ColorTo       = 0xFFFFFF;
  Button18.Press_ColorTo = 0xC0C0C0;
  Button18.Gradient      = _FT812_BRUSH_GR_NONE;
  Button18.Caption       = Button18_Caption;
  Button18.TextAlign     = taCenter;
  Button18.FontName      = 29;
  Button18.Font_Color    = 0xFFFFFF;
  Button18.FontHandle    = 29;
  Button18.Source        = -1UL;
  Button18.Active        = 1;
  Button18.OnUp          = 0;
  Button18.OnDown        = 0;
  Button18.OnClick       = &Button18_OnClick;
  Button18.OnPress       = 0;

  Button18_OnClick.Action       = Button18OnClick;
  Button18_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button18_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button18_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button18_OnClick.Sound.Volume = 255;

  Button19.OwnerScreen   = &System_Log_Screen;
  Button19.Order         = 17;
  Button19.Visible       = 1;
  Button19.Opacity       = 255;
  Button19.Tag           = 255;
  Button19.Left          = 447;
  Button19.Top           = 247;
  Button19.Width         = 80;
  Button19.Height        = 80;
  Button19.Pen_Width     = 1;
  Button19.Pen_Color     = 0x0000;
  Button19.Color         = 0xFFFF;
  Button19.Press_Color   = 0xFFFFFF;
  Button19.ColorTo       = 0xFFFFFF;
  Button19.Press_ColorTo = 0xC0C0C0;
  Button19.Gradient      = _FT812_BRUSH_GR_NONE;
  Button19.Caption       = Button19_Caption;
  Button19.TextAlign     = taCenter;
  Button19.FontName      = 29;
  Button19.Font_Color    = 0xFFFFFF;
  Button19.FontHandle    = 29;
  Button19.Source        = -1UL;
  Button19.Active        = 1;
  Button19.OnUp          = 0;
  Button19.OnDown        = 0;
  Button19.OnClick       = &Button19_OnClick;
  Button19.OnPress       = 0;

  Button19_OnClick.Action       = Button19OnClick;
  Button19_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button19_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button19_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button19_OnClick.Sound.Volume = 255;

  Button20.OwnerScreen   = &System_Log_Screen;
  Button20.Order         = 18;
  Button20.Visible       = 1;
  Button20.Opacity       = 255;
  Button20.Tag           = 255;
  Button20.Left          = 535;
  Button20.Top           = 247;
  Button20.Width         = 80;
  Button20.Height        = 80;
  Button20.Pen_Width     = 1;
  Button20.Pen_Color     = 0x0000;
  Button20.Color         = 0xFFFF;
  Button20.Press_Color   = 0xFFFFFF;
  Button20.ColorTo       = 0xFFFFFF;
  Button20.Press_ColorTo = 0xC0C0C0;
  Button20.Gradient      = _FT812_BRUSH_GR_NONE;
  Button20.Caption       = Button20_Caption;
  Button20.TextAlign     = taCenter;
  Button20.FontName      = 29;
  Button20.Font_Color    = 0xFFFFFF;
  Button20.FontHandle    = 29;
  Button20.Source        = -1UL;
  Button20.Active        = 1;
  Button20.OnUp          = 0;
  Button20.OnDown        = 0;
  Button20.OnClick       = &Button20_OnClick;
  Button20.OnPress       = 0;

  Button20_OnClick.Action       = Button20OnClick;
  Button20_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button20_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button20_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button20_OnClick.Sound.Volume = 255;

  Button21.OwnerScreen   = &System_Log_Screen;
  Button21.Order         = 19;
  Button21.Visible       = 1;
  Button21.Opacity       = 255;
  Button21.Tag           = 255;
  Button21.Left          = 623;
  Button21.Top           = 247;
  Button21.Width         = 80;
  Button21.Height        = 80;
  Button21.Pen_Width     = 1;
  Button21.Pen_Color     = 0x0000;
  Button21.Color         = 0xFFFF;
  Button21.Press_Color   = 0xFFFFFF;
  Button21.ColorTo       = 0xFFFFFF;
  Button21.Press_ColorTo = 0xC0C0C0;
  Button21.Gradient      = _FT812_BRUSH_GR_NONE;
  Button21.Caption       = Button21_Caption;
  Button21.TextAlign     = taCenter;
  Button21.FontName      = 29;
  Button21.Font_Color    = 0xFFFFFF;
  Button21.FontHandle    = 29;
  Button21.Source        = -1UL;
  Button21.Active        = 1;
  Button21.OnUp          = 0;
  Button21.OnDown        = 0;
  Button21.OnClick       = &Button21_OnClick;
  Button21.OnPress       = 0;

  Button21_OnClick.Action       = Button21OnClick;
  Button21_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button21_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button21_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button21_OnClick.Sound.Volume = 255;

  Button22.OwnerScreen   = &System_Log_Screen;
  Button22.Order         = 20;
  Button22.Visible       = 1;
  Button22.Opacity       = 255;
  Button22.Tag           = 255;
  Button22.Left          = 535;
  Button22.Top           = 335;
  Button22.Width         = 80;
  Button22.Height        = 80;
  Button22.Pen_Width     = 1;
  Button22.Pen_Color     = 0x0000;
  Button22.Color         = 0xFFFF;
  Button22.Press_Color   = 0xFFFFFF;
  Button22.ColorTo       = 0xFFFFFF;
  Button22.Press_ColorTo = 0xC0C0C0;
  Button22.Gradient      = _FT812_BRUSH_GR_NONE;
  Button22.Caption       = Button22_Caption;
  Button22.TextAlign     = taCenter;
  Button22.FontName      = 29;
  Button22.Font_Color    = 0xFFFFFF;
  Button22.FontHandle    = 29;
  Button22.Source        = -1UL;
  Button22.Active        = 1;
  Button22.OnUp          = 0;
  Button22.OnDown        = 0;
  Button22.OnClick       = &Button22_OnClick;
  Button22.OnPress       = 0;

  Button22_OnClick.Action       = Button22OnClick;
  Button22_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button22_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button22_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button22_OnClick.Sound.Volume = 255;

  Button23.OwnerScreen   = &System_Log_Screen;
  Button23.Order         = 21;
  Button23.Visible       = 1;
  Button23.Opacity       = 255;
  Button23.Tag           = 255;
  Button23.Left          = 711;
  Button23.Top           = 71;
  Button23.Width         = 80;
  Button23.Height        = 80;
  Button23.Pen_Width     = 1;
  Button23.Pen_Color     = 0x0000;
  Button23.Color         = 0x0080;
  Button23.Press_Color   = 0xFFFFFF;
  Button23.ColorTo       = 0xFFFFFF;
  Button23.Press_ColorTo = 0xC0C0C0;
  Button23.Gradient      = _FT812_BRUSH_GR_NONE;
  Button23.Caption       = Button23_Caption;
  Button23.TextAlign     = taCenter;
  Button23.FontName      = 29;
  Button23.Font_Color    = 0xFFFFFF;
  Button23.FontHandle    = 29;
  Button23.Source        = -1UL;
  Button23.Active        = 1;
  Button23.OnUp          = 0;
  Button23.OnDown        = 0;
  Button23.OnClick       = &Button23_OnClick;
  Button23.OnPress       = 0;

  Button23_OnClick.Action       = Button23OnClick;
  Button23_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button23_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button23_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button23_OnClick.Sound.Volume = 255;

  Button24.OwnerScreen   = &System_Log_Screen;
  Button24.Order         = 22;
  Button24.Visible       = 1;
  Button24.Opacity       = 255;
  Button24.Tag           = 255;
  Button24.Left          = 711;
  Button24.Top           = 159;
  Button24.Width         = 80;
  Button24.Height        = 80;
  Button24.Pen_Width     = 1;
  Button24.Pen_Color     = 0x0000;
  Button24.Color         = 0x0080;
  Button24.Press_Color   = 0xFFFFFF;
  Button24.ColorTo       = 0xFFFFFF;
  Button24.Press_ColorTo = 0xC0C0C0;
  Button24.Gradient      = _FT812_BRUSH_GR_NONE;
  Button24.Caption       = Button24_Caption;
  Button24.TextAlign     = taCenter;
  Button24.FontName      = 29;
  Button24.Font_Color    = 0xFFFFFF;
  Button24.FontHandle    = 29;
  Button24.Source        = -1UL;
  Button24.Active        = 1;
  Button24.OnUp          = 0;
  Button24.OnDown        = 0;
  Button24.OnClick       = &Button24_OnClick;
  Button24.OnPress       = 0;

  Button24_OnClick.Action       = Button24OnClick;
  Button24_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button24_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button24_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button24_OnClick.Sound.Volume = 255;

  Button25.OwnerScreen   = &System_Log_Screen;
  Button25.Order         = 23;
  Button25.Visible       = 1;
  Button25.Opacity       = 255;
  Button25.Tag           = 255;
  Button25.Left          = 711;
  Button25.Top           = 247;
  Button25.Width         = 80;
  Button25.Height        = 80;
  Button25.Pen_Width     = 1;
  Button25.Pen_Color     = 0x0000;
  Button25.Color         = 0x0080;
  Button25.Press_Color   = 0xFFFFFF;
  Button25.ColorTo       = 0xFFFFFF;
  Button25.Press_ColorTo = 0xC0C0C0;
  Button25.Gradient      = _FT812_BRUSH_GR_NONE;
  Button25.Caption       = Button25_Caption;
  Button25.TextAlign     = taCenter;
  Button25.FontName      = 29;
  Button25.Font_Color    = 0xFFFFFF;
  Button25.FontHandle    = 29;
  Button25.Source        = -1UL;
  Button25.Active        = 1;
  Button25.OnUp          = 0;
  Button25.OnDown        = 0;
  Button25.OnClick       = &Button25_OnClick;
  Button25.OnPress       = 0;

  Button25_OnClick.Action       = Button25OnClick;
  Button25_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button25_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button25_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button25_OnClick.Sound.Volume = 255;

  Button26.OwnerScreen   = &System_Log_Screen;
  Button26.Order         = 24;
  Button26.Visible       = 1;
  Button26.Opacity       = 255;
  Button26.Tag           = 255;
  Button26.Left          = 711;
  Button26.Top           = 335;
  Button26.Width         = 80;
  Button26.Height        = 80;
  Button26.Pen_Width     = 1;
  Button26.Pen_Color     = 0x0000;
  Button26.Color         = 0x0080;
  Button26.Press_Color   = 0xFFFFFF;
  Button26.ColorTo       = 0xFFFFFF;
  Button26.Press_ColorTo = 0xC0C0C0;
  Button26.Gradient      = _FT812_BRUSH_GR_NONE;
  Button26.Caption       = Button26_Caption;
  Button26.TextAlign     = taCenter;
  Button26.FontName      = 29;
  Button26.Font_Color    = 0xFFFFFF;
  Button26.FontHandle    = 29;
  Button26.Source        = -1UL;
  Button26.Active        = 1;
  Button26.OnUp          = 0;
  Button26.OnDown        = 0;
  Button26.OnClick       = &Button26_OnClick;
  Button26.OnPress       = 0;

  Button26_OnClick.Action       = Button26OnClick;
  Button26_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button26_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button26_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button26_OnClick.Sound.Volume = 255;

  Button27.OwnerScreen   = &System_Log_Screen;
  Button27.Order         = 25;
  Button27.Visible       = 1;
  Button27.Opacity       = 255;
  Button27.Tag           = 255;
  Button27.Left          = 447;
  Button27.Top           = 335;
  Button27.Width         = 80;
  Button27.Height        = 80;
  Button27.Pen_Width     = 1;
  Button27.Pen_Color     = 0x0000;
  Button27.Color         = 0xFFFF;
  Button27.Press_Color   = 0xFFFFFF;
  Button27.ColorTo       = 0xFFFFFF;
  Button27.Press_ColorTo = 0xC0C0C0;
  Button27.Gradient      = _FT812_BRUSH_GR_NONE;
  Button27.Caption       = Button27_Caption;
  Button27.TextAlign     = taCenter;
  Button27.FontName      = 29;
  Button27.Font_Color    = 0xFFFFFF;
  Button27.FontHandle    = 29;
  Button27.Source        = -1UL;
  Button27.Active        = 1;
  Button27.OnUp          = 0;
  Button27.OnDown        = 0;
  Button27.OnClick       = &Button27_OnClick;
  Button27.OnPress       = 0;

  Button27_OnClick.Action       = Button27OnClick;
  Button27_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button27_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button27_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button27_OnClick.Sound.Volume = 255;

  Button28.OwnerScreen   = &System_Log_Screen;
  Button28.Order         = 26;
  Button28.Visible       = 1;
  Button28.Opacity       = 255;
  Button28.Tag           = 255;
  Button28.Left          = 623;
  Button28.Top           = 335;
  Button28.Width         = 80;
  Button28.Height        = 80;
  Button28.Pen_Width     = 1;
  Button28.Pen_Color     = 0x0000;
  Button28.Color         = 0xFFFF;
  Button28.Press_Color   = 0xFFFFFF;
  Button28.ColorTo       = 0xFFFFFF;
  Button28.Press_ColorTo = 0xC0C0C0;
  Button28.Gradient      = _FT812_BRUSH_GR_NONE;
  Button28.Caption       = Button28_Caption;
  Button28.TextAlign     = taCenter;
  Button28.FontName      = 29;
  Button28.Font_Color    = 0xFFFFFF;
  Button28.FontHandle    = 29;
  Button28.Source        = -1UL;
  Button28.Active        = 1;
  Button28.OnUp          = 0;
  Button28.OnDown        = 0;
  Button28.OnClick       = &Button28_OnClick;
  Button28.OnPress       = 0;

  Button28_OnClick.Action       = Button28OnClick;
  Button28_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button28_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button28_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button28_OnClick.Sound.Volume = 255;

  Button29.OwnerScreen   = &System_Log_Screen;
  Button29.Order         = 27;
  Button29.Visible       = 1;
  Button29.Opacity       = 255;
  Button29.Tag           = 255;
  Button29.Left          = 8;
  Button29.Top           = 8;
  Button29.Width         = 782;
  Button29.Height        = 55;
  Button29.Pen_Width     = 1;
  Button29.Pen_Color     = 0x0000;
  Button29.Color         = 0x8000FF;
  Button29.Press_Color   = 0xFFFFFF;
  Button29.ColorTo       = 0xFFFFFF;
  Button29.Press_ColorTo = 0xC0C0C0;
  Button29.Gradient      = _FT812_BRUSH_GR_NONE;
  Button29.Caption       = Button29_Caption;
  Button29.TextAlign     = taCenter;
  Button29.FontName      = 31;
  Button29.Font_Color    = 0xFFFFFF;
  Button29.FontHandle    = 31;
  Button29.Source        = -1UL;
  Button29.Active        = 1;
  Button29.OnUp          = 0;
  Button29.OnDown        = 0;
  Button29.OnClick       = 0;
  Button29.OnPress       = 0;

  Box36.OwnerScreen   = &System_Log_Screen;
  Box36.Order         = 28;
  Box36.Visible       = 1;
  Box36.Opacity       = 255;
  Box36.Tag           = 255;
  Box36.Left          = 124;
  Box36.Top           = 100;
  Box36.Width         = 200;
  Box36.Height        = 33;
  Box36.Pen_Width     = 3;
  Box36.Pen_Color     = 0xFFFFFF;
  Box36.Color         = 0x8040;
  Box36.Press_Color   = 0xFFFFFF;
  Box36.ColorTo       = 0xFFFFFF;
  Box36.Press_ColorTo = 0xC0C0C0;
  Box36.Gradient      = _FT812_BRUSH_GR_NONE;
  Box36.Active        = 1;
  Box36.OnUp          = 0;
  Box36.OnDown        = 0;
  Box36.OnClick       = 0;
  Box36.OnPress       = 0;

  Label24.OwnerScreen = &System_Log_Screen;
  Label24.Order       = 29;
  Label24.Visible     = 1;
  Label24.Opacity     = 255;
  Label24.Tag         = 255;
  Label24.Left        = 185;
  Label24.Top         = 106;
  Label24.Width       = 0;
  Label24.Height      = 21;
  Label24.Caption     = Label24_Caption;
  Label24.FontName    = 28;
  Label24.Font_Color  = 0x0000;
  Label24.FontHandle  = 28;
  Label24.Source      = -1UL;
  Label24.Active      = 1;
  Label24.OnUp        = 0;
  Label24.OnDown      = 0;
  Label24.OnClick     = 0;
  Label24.OnPress     = 0;

  Label25.OwnerScreen = &System_Log_Screen;
  Label25.Order       = 30;
  Label25.Visible     = 1;
  Label25.Opacity     = 255;
  Label25.Tag         = 255;
  Label25.Left        = 150;
  Label25.Top         = 279;
  Label25.Width       = 122;
  Label25.Height      = 32;
  Label25.Caption     = Label25_Caption;
  Label25.FontName    = 30;
  Label25.Font_Color  = 0xFFFFFF;
  Label25.FontHandle  = 30;
  Label25.Source      = -1UL;
  Label25.Active      = 1;
  Label25.OnUp        = 0;
  Label25.OnDown      = 0;
  Label25.OnClick     = 0;
  Label25.OnPress     = 0;

  Label54.OwnerScreen = &System_Log_Screen;
  Label54.Order       = 31;
  Label54.Visible     = 1;
  Label54.Opacity     = 255;
  Label54.Tag         = 255;
  Label54.Left        = 118;
  Label54.Top         = 359;
  Label54.Width       = 73;
  Label54.Height      = 32;
  Label54.Caption     = Label54_Caption;
  Label54.FontName    = 30;
  Label54.Font_Color  = 0xFFFFFF;
  Label54.FontHandle  = 30;
  Label54.Source      = -1UL;
  Label54.Active      = 0;
  Label54.OnUp        = 0;
  Label54.OnDown      = 0;
  Label54.OnClick     = 0;
  Label54.OnPress     = 0;

  Label55.OwnerScreen = &System_Log_Screen;
  Label55.Order       = 32;
  Label55.Visible     = 1;
  Label55.Opacity     = 255;
  Label55.Tag         = 255;
  Label55.Left        = 338;
  Label55.Top         = 359;
  Label55.Width       = 48;
  Label55.Height      = 32;
  Label55.Caption     = Label55_Caption;
  Label55.FontName    = 30;
  Label55.Font_Color  = 0xFFFF00;
  Label55.FontHandle  = 30;
  Label55.Source      = -1UL;
  Label55.Active      = 0;
  Label55.OnUp        = 0;
  Label55.OnDown      = 0;
  Label55.OnClick     = 0;
  Label55.OnPress     = 0;

  Button33.OwnerScreen   = &Green_Power_Screen;
  Button33.Order         = 0;
  Button33.Visible       = 1;
  Button33.Opacity       = 255;
  Button33.Tag           = 255;
  Button33.Left          = 8;
  Button33.Top           = 8;
  Button33.Width         = 782;
  Button33.Height        = 55;
  Button33.Pen_Width     = 1;
  Button33.Pen_Color     = 0x0000;
  Button33.Color         = 0xFF8000;
  Button33.Press_Color   = 0xFFFFFF;
  Button33.ColorTo       = 0xFFFFFF;
  Button33.Press_ColorTo = 0xC0C0C0;
  Button33.Gradient      = _FT812_BRUSH_GR_NONE;
  Button33.Caption       = Button33_Caption;
  Button33.TextAlign     = taCenter;
  Button33.FontName      = 31;
  Button33.Font_Color    = 0xFFFFFF;
  Button33.FontHandle    = 31;
  Button33.Source        = -1UL;
  Button33.Active        = 1;
  Button33.OnUp          = 0;
  Button33.OnDown        = 0;
  Button33.OnClick       = 0;
  Button33.OnPress       = 0;

  Button34.OwnerScreen   = &Green_Power_Screen;
  Button34.Order         = 1;
  Button34.Visible       = 1;
  Button34.Opacity       = 255;
  Button34.Tag           = 255;
  Button34.Left          = 8;
  Button34.Top           = 423;
  Button34.Width         = 50;
  Button34.Height        = 50;
  Button34.Pen_Width     = 1;
  Button34.Pen_Color     = 0x0000;
  Button34.Color         = 0x808080;
  Button34.Press_Color   = 0xFFFFFF;
  Button34.ColorTo       = 0xFFFFFF;
  Button34.Press_ColorTo = 0xC0C0C0;
  Button34.Gradient      = _FT812_BRUSH_GR_NONE;
  Button34.Caption       = Button34_Caption;
  Button34.TextAlign     = taCenter;
  Button34.FontName      = 30;
  Button34.Font_Color    = 0xFFFFFF;
  Button34.FontHandle    = 30;
  Button34.Source        = -1UL;
  Button34.Active        = 1;
  Button34.OnUp          = 0;
  Button34.OnDown        = 0;
  Button34.OnClick       = &Button34_OnClick;
  Button34.OnPress       = 0;

  Button34_OnClick.Action       = Button12OnClick;
  Button34_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button34_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button34_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button34_OnClick.Sound.Volume = 255;

  Box26.OwnerScreen   = &Green_Power_Screen;
  Box26.Order         = 2;
  Box26.Visible       = 1;
  Box26.Opacity       = 255;
  Box26.Tag           = 255;
  Box26.Left          = 66;
  Box26.Top           = 423;
  Box26.Width         = 724;
  Box26.Height        = 50;
  Box26.Pen_Width     = 1;
  Box26.Pen_Color     = 0x0000;
  Box26.Color         = 0x404040;
  Box26.Press_Color   = 0xFFFFFF;
  Box26.ColorTo       = 0xFFFFFF;
  Box26.Press_ColorTo = 0xC0C0C0;
  Box26.Gradient      = _FT812_BRUSH_GR_NONE;
  Box26.Active        = 1;
  Box26.OnUp          = 0;
  Box26.OnDown        = 0;
  Box26.OnClick       = 0;
  Box26.OnPress       = 0;

  Box66.OwnerScreen   = &Green_Power_Screen;
  Box66.Order         = 3;
  Box66.Visible       = 1;
  Box66.Opacity       = 255;
  Box66.Tag           = 255;
  Box66.Left          = 8;
  Box66.Top           = 71;
  Box66.Width         = 782;
  Box66.Height        = 344;
  Box66.Pen_Width     = 1;
  Box66.Pen_Color     = 0x0000;
  Box66.Color         = 0x00FF;
  Box66.Press_Color   = 0xFFFFFF;
  Box66.ColorTo       = 0xFFFFFF;
  Box66.Press_ColorTo = 0xC0C0C0;
  Box66.Gradient      = _FT812_BRUSH_GR_NONE;
  Box66.Active        = 1;
  Box66.OnUp          = 0;
  Box66.OnDown        = 0;
  Box66.OnClick       = 0;
  Box66.OnPress       = 0;

  Label62.OwnerScreen = &Green_Power_Screen;
  Label62.Order       = 4;
  Label62.Visible     = 1;
  Label62.Opacity     = 255;
  Label62.Tag         = 255;
  Label62.Left        = 46;
  Label62.Top         = 94;
  Label62.Width       = 0;
  Label62.Height      = 21;
  Label62.Caption     = Label62_Caption;
  Label62.FontName    = 28;
  Label62.Font_Color  = 0xFFFFFF;
  Label62.FontHandle  = 28;
  Label62.Source      = -1UL;
  Label62.Active      = 1;
  Label62.OnUp        = 0;
  Label62.OnDown      = 0;
  Label62.OnClick     = 0;
  Label62.OnPress     = 0;

  Label63.OwnerScreen = &Green_Power_Screen;
  Label63.Order       = 5;
  Label63.Visible     = 1;
  Label63.Opacity     = 255;
  Label63.Tag         = 255;
  Label63.Left        = 46;
  Label63.Top         = 133;
  Label63.Width       = 0;
  Label63.Height      = 21;
  Label63.Caption     = Label63_Caption;
  Label63.FontName    = 28;
  Label63.Font_Color  = 0xFFFFFF;
  Label63.FontHandle  = 28;
  Label63.Source      = -1UL;
  Label63.Active      = 1;
  Label63.OnUp        = 0;
  Label63.OnDown      = 0;
  Label63.OnClick     = 0;
  Label63.OnPress     = 0;

  Label64.OwnerScreen = &Green_Power_Screen;
  Label64.Order       = 6;
  Label64.Visible     = 1;
  Label64.Opacity     = 255;
  Label64.Tag         = 255;
  Label64.Left        = 46;
  Label64.Top         = 171;
  Label64.Width       = 0;
  Label64.Height      = 21;
  Label64.Caption     = Label64_Caption;
  Label64.FontName    = 28;
  Label64.Font_Color  = 0xFFFFFF;
  Label64.FontHandle  = 28;
  Label64.Source      = -1UL;
  Label64.Active      = 1;
  Label64.OnUp        = 0;
  Label64.OnDown      = 0;
  Label64.OnClick     = 0;
  Label64.OnPress     = 0;

  Label65.OwnerScreen = &Green_Power_Screen;
  Label65.Order       = 7;
  Label65.Visible     = 1;
  Label65.Opacity     = 255;
  Label65.Tag         = 255;
  Label65.Left        = 46;
  Label65.Top         = 210;
  Label65.Width       = 0;
  Label65.Height      = 21;
  Label65.Caption     = Label65_Caption;
  Label65.FontName    = 28;
  Label65.Font_Color  = 0xFFFFFF;
  Label65.FontHandle  = 28;
  Label65.Source      = -1UL;
  Label65.Active      = 1;
  Label65.OnUp        = 0;
  Label65.OnDown      = 0;
  Label65.OnClick     = 0;
  Label65.OnPress     = 0;

  Label66.OwnerScreen = &Green_Power_Screen;
  Label66.Order       = 8;
  Label66.Visible     = 1;
  Label66.Opacity     = 255;
  Label66.Tag         = 255;
  Label66.Left        = 46;
  Label66.Top         = 248;
  Label66.Width       = 0;
  Label66.Height      = 21;
  Label66.Caption     = Label66_Caption;
  Label66.FontName    = 28;
  Label66.Font_Color  = 0xFFFFFF;
  Label66.FontHandle  = 28;
  Label66.Source      = -1UL;
  Label66.Active      = 1;
  Label66.OnUp        = 0;
  Label66.OnDown      = 0;
  Label66.OnClick     = 0;
  Label66.OnPress     = 0;

  Label67.OwnerScreen = &Green_Power_Screen;
  Label67.Order       = 9;
  Label67.Visible     = 1;
  Label67.Opacity     = 255;
  Label67.Tag         = 255;
  Label67.Left        = 46;
  Label67.Top         = 286;
  Label67.Width       = 0;
  Label67.Height      = 21;
  Label67.Caption     = Label67_Caption;
  Label67.FontName    = 28;
  Label67.Font_Color  = 0xFFFFFF;
  Label67.FontHandle  = 28;
  Label67.Source      = -1UL;
  Label67.Active      = 1;
  Label67.OnUp        = 0;
  Label67.OnDown      = 0;
  Label67.OnClick     = 0;
  Label67.OnPress     = 0;

  Label104.OwnerScreen = &Green_Power_Screen;
  Label104.Order       = 10;
  Label104.Visible     = 1;
  Label104.Opacity     = 255;
  Label104.Tag         = 255;
  Label104.Left        = 46;
  Label104.Top         = 325;
  Label104.Width       = 0;
  Label104.Height      = 21;
  Label104.Caption     = Label104_Caption;
  Label104.FontName    = 28;
  Label104.Font_Color  = 0xFFFFFF;
  Label104.FontHandle  = 28;
  Label104.Source      = -1UL;
  Label104.Active      = 1;
  Label104.OnUp        = 0;
  Label104.OnDown      = 0;
  Label104.OnClick     = 0;
  Label104.OnPress     = 0;

  Label105.OwnerScreen = &Green_Power_Screen;
  Label105.Order       = 11;
  Label105.Visible     = 1;
  Label105.Opacity     = 255;
  Label105.Tag         = 255;
  Label105.Left        = 46;
  Label105.Top         = 363;
  Label105.Width       = 0;
  Label105.Height      = 21;
  Label105.Caption     = Label105_Caption;
  Label105.FontName    = 28;
  Label105.Font_Color  = 0xFFFFFF;
  Label105.FontHandle  = 28;
  Label105.Source      = -1UL;
  Label105.Active      = 1;
  Label105.OnUp        = 0;
  Label105.OnDown      = 0;
  Label105.OnClick     = 0;
  Label105.OnPress     = 0;

  Box67.OwnerScreen   = &Time_Screen;
  Box67.Order         = 0;
  Box67.Visible       = 1;
  Box67.Opacity       = 255;
  Box67.Tag           = 255;
  Box67.Left          = 8;
  Box67.Top           = 247;
  Box67.Width         = 189;
  Box67.Height        = 168;
  Box67.Pen_Width     = 1;
  Box67.Pen_Color     = 0x0000;
  Box67.Color         = 0x8000;
  Box67.Press_Color   = 0xFFFFFF;
  Box67.ColorTo       = 0xFFFFFF;
  Box67.Press_ColorTo = 0xC0C0C0;
  Box67.Gradient      = _FT812_BRUSH_GR_NONE;
  Box67.Active        = 1;
  Box67.OnUp          = 0;
  Box67.OnDown        = 0;
  Box67.OnClick       = 0;
  Box67.OnPress       = 0;

  Image63.OwnerScreen   = &Time_Screen;
  Image63.Order         = 1;
  Image63.Visible       = 1;
  Image63.Opacity       = 255;
  Image63.Tag           = 255;
  Image63.Left          = 72;
  Image63.Top           = 305;
  Image63.Width         = 62;
  Image63.Height        = 52;
  Image63.Picture_Name  = leaf_bmp;
  Image63.Picture_Type  = 7;
  Image63.Picture_Ratio = 1;
  Image63.Blend_Color   = 0xFFFFFF;
  Image63.Source        = -1L;
  Image63.RotationAngle = 0;
  Image63.RotationCenterLeft = 102;
  Image63.RotationCenterTop = 331;
  Image63.ScaleX        = 1.0000;
  Image63.ScaleY        = 1.0000;
  Image63.Active        = 1;
  Image63.OnUp          = 0;
  Image63.OnDown        = 0;
  Image63.OnClick       = 0;
  Image63.OnPress       = 0;

  Box74.OwnerScreen   = &Time_Screen;
  Box74.Order         = 2;
  Box74.Visible       = 1;
  Box74.Opacity       = 255;
  Box74.Tag           = 255;
  Box74.Left          = 8;
  Box74.Top           = 71;
  Box74.Width         = 189;
  Box74.Height        = 80;
  Box74.Pen_Width     = 1;
  Box74.Pen_Color     = 0x0000;
  Box74.Color         = 0x808080;
  Box74.Press_Color   = 0xFFFFFF;
  Box74.ColorTo       = 0xFFFFFF;
  Box74.Press_ColorTo = 0xC0C0C0;
  Box74.Gradient      = _FT812_BRUSH_GR_NONE;
  Box74.Active        = 1;
  Box74.OnUp          = 0;
  Box74.OnDown        = 0;
  Box74.OnClick       = 0;
  Box74.OnPress       = 0;

  Image62.OwnerScreen   = &Time_Screen;
  Image62.Order         = 3;
  Image62.Visible       = 1;
  Image62.Opacity       = 255;
  Image62.Tag           = 255;
  Image62.Left          = 141;
  Image62.Top           = 112;
  Image62.Width         = 19;
  Image62.Height        = 22;
  Image62.Picture_Name  = eu_bmp;
  Image62.Picture_Type  = 7;
  Image62.Picture_Ratio = 1;
  Image62.Blend_Color   = 0xFFFFFF;
  Image62.Source        = -1L;
  Image62.RotationAngle = 0;
  Image62.RotationCenterLeft = 147;
  Image62.RotationCenterTop = 119;
  Image62.ScaleX        = 0.6000;
  Image62.ScaleY        = 0.6000;
  Image62.Active        = 1;
  Image62.OnUp          = 0;
  Image62.OnDown        = 0;
  Image62.OnClick       = 0;
  Image62.OnPress       = 0;

  Button35.OwnerScreen   = &Time_Screen;
  Button35.Order         = 4;
  Button35.Visible       = 1;
  Button35.Opacity       = 255;
  Button35.Tag           = 255;
  Button35.Left          = 8;
  Button35.Top           = 8;
  Button35.Width         = 782;
  Button35.Height        = 55;
  Button35.Pen_Width     = 1;
  Button35.Pen_Color     = 0x0000;
  Button35.Color         = 0x8080;
  Button35.Press_Color   = 0xFFFFFF;
  Button35.ColorTo       = 0xFFFFFF;
  Button35.Press_ColorTo = 0xC0C0C0;
  Button35.Gradient      = _FT812_BRUSH_GR_NONE;
  Button35.Caption       = Button35_Caption;
  Button35.TextAlign     = taCenter;
  Button35.FontName      = 31;
  Button35.Font_Color    = 0xFFFFFF;
  Button35.FontHandle    = 31;
  Button35.Source        = -1UL;
  Button35.Active        = 1;
  Button35.OnUp          = 0;
  Button35.OnDown        = 0;
  Button35.OnClick       = 0;
  Button35.OnPress       = 0;

  Button36.OwnerScreen   = &Time_Screen;
  Button36.Order         = 5;
  Button36.Visible       = 1;
  Button36.Opacity       = 255;
  Button36.Tag           = 255;
  Button36.Left          = 8;
  Button36.Top           = 423;
  Button36.Width         = 50;
  Button36.Height        = 50;
  Button36.Pen_Width     = 1;
  Button36.Pen_Color     = 0x0000;
  Button36.Color         = 0x808080;
  Button36.Press_Color   = 0xFFFFFF;
  Button36.ColorTo       = 0xFFFFFF;
  Button36.Press_ColorTo = 0xC0C0C0;
  Button36.Gradient      = _FT812_BRUSH_GR_NONE;
  Button36.Caption       = Button36_Caption;
  Button36.TextAlign     = taCenter;
  Button36.FontName      = 30;
  Button36.Font_Color    = 0xFFFFFF;
  Button36.FontHandle    = 30;
  Button36.Source        = -1UL;
  Button36.Active        = 1;
  Button36.OnUp          = 0;
  Button36.OnDown        = 0;
  Button36.OnClick       = &Button36_OnClick;
  Button36.OnPress       = 0;

  Button36_OnClick.Action       = Button12OnClick;
  Button36_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button36_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button36_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button36_OnClick.Sound.Volume = 255;

  Box27.OwnerScreen   = &Time_Screen;
  Box27.Order         = 6;
  Box27.Visible       = 1;
  Box27.Opacity       = 255;
  Box27.Tag           = 255;
  Box27.Left          = 66;
  Box27.Top           = 423;
  Box27.Width         = 724;
  Box27.Height        = 50;
  Box27.Pen_Width     = 1;
  Box27.Pen_Color     = 0x0000;
  Box27.Color         = 0x404040;
  Box27.Press_Color   = 0xFFFFFF;
  Box27.ColorTo       = 0xFFFFFF;
  Box27.Press_ColorTo = 0xC0C0C0;
  Box27.Gradient      = _FT812_BRUSH_GR_NONE;
  Box27.Active        = 1;
  Box27.OnUp          = 0;
  Box27.OnDown        = 0;
  Box27.OnClick       = 0;
  Box27.OnPress       = 0;

  Box68.OwnerScreen   = &Time_Screen;
  Box68.Order         = 7;
  Box68.Visible       = 1;
  Box68.Opacity       = 255;
  Box68.Tag           = 255;
  Box68.Left          = 206;
  Box68.Top           = 71;
  Box68.Width         = 189;
  Box68.Height        = 344;
  Box68.Pen_Width     = 1;
  Box68.Pen_Color     = 0x0000;
  Box68.Color         = 0x00FF;
  Box68.Press_Color   = 0xFFFFFF;
  Box68.ColorTo       = 0xFFFFFF;
  Box68.Press_ColorTo = 0xC0C0C0;
  Box68.Gradient      = _FT812_BRUSH_GR_NONE;
  Box68.Active        = 1;
  Box68.OnUp          = 0;
  Box68.OnDown        = 0;
  Box68.OnClick       = 0;
  Box68.OnPress       = 0;

  Box69.OwnerScreen   = &Time_Screen;
  Box69.Order         = 8;
  Box69.Visible       = 1;
  Box69.Opacity       = 255;
  Box69.Tag           = 255;
  Box69.Left          = 403;
  Box69.Top           = 71;
  Box69.Width         = 189;
  Box69.Height        = 344;
  Box69.Pen_Width     = 1;
  Box69.Pen_Color     = 0x0000;
  Box69.Color         = 0xC08000;
  Box69.Press_Color   = 0xFFFFFF;
  Box69.ColorTo       = 0xFFFFFF;
  Box69.Press_ColorTo = 0xC0C0C0;
  Box69.Gradient      = _FT812_BRUSH_GR_NONE;
  Box69.Active        = 1;
  Box69.OnUp          = 0;
  Box69.OnDown        = 0;
  Box69.OnClick       = 0;
  Box69.OnPress       = 0;

  Box70.OwnerScreen   = &Time_Screen;
  Box70.Order         = 9;
  Box70.Visible       = 1;
  Box70.Opacity       = 255;
  Box70.Tag           = 255;
  Box70.Left          = 601;
  Box70.Top           = 71;
  Box70.Width         = 189;
  Box70.Height        = 344;
  Box70.Pen_Width     = 1;
  Box70.Pen_Color     = 0x0000;
  Box70.Color         = 0x8000;
  Box70.Press_Color   = 0xFFFFFF;
  Box70.ColorTo       = 0xFFFFFF;
  Box70.Press_ColorTo = 0xC0C0C0;
  Box70.Gradient      = _FT812_BRUSH_GR_NONE;
  Box70.Active        = 1;
  Box70.OnUp          = 0;
  Box70.OnDown        = 0;
  Box70.OnClick       = 0;
  Box70.OnPress       = 0;

  Box71.OwnerScreen   = &Time_Screen;
  Box71.Order         = 10;
  Box71.Visible       = 1;
  Box71.Opacity       = 255;
  Box71.Tag           = 255;
  Box71.Left          = 206;
  Box71.Top           = 326;
  Box71.Width         = 189;
  Box71.Height        = 89;
  Box71.Pen_Width     = 1;
  Box71.Pen_Color     = 0xFFFF;
  Box71.Color         = 0xFFFF;
  Box71.Press_Color   = 0xFFFFFF;
  Box71.ColorTo       = 0xFFFFFF;
  Box71.Press_ColorTo = 0xC0C0C0;
  Box71.Gradient      = _FT812_BRUSH_GR_NONE;
  Box71.Active        = 1;
  Box71.OnUp          = 0;
  Box71.OnDown        = 0;
  Box71.OnClick       = 0;
  Box71.OnPress       = 0;

  Box72.OwnerScreen   = &Time_Screen;
  Box72.Order         = 11;
  Box72.Visible       = 1;
  Box72.Opacity       = 255;
  Box72.Tag           = 255;
  Box72.Left          = 403;
  Box72.Top           = 326;
  Box72.Width         = 189;
  Box72.Height        = 89;
  Box72.Pen_Width     = 1;
  Box72.Pen_Color     = 0xFFFF00;
  Box72.Color         = 0xFFFF00;
  Box72.Press_Color   = 0xFFFFFF;
  Box72.ColorTo       = 0xFFFFFF;
  Box72.Press_ColorTo = 0xC0C0C0;
  Box72.Gradient      = _FT812_BRUSH_GR_NONE;
  Box72.Active        = 1;
  Box72.OnUp          = 0;
  Box72.OnDown        = 0;
  Box72.OnClick       = 0;
  Box72.OnPress       = 0;

  Box73.OwnerScreen   = &Time_Screen;
  Box73.Order         = 12;
  Box73.Visible       = 1;
  Box73.Opacity       = 255;
  Box73.Tag           = 255;
  Box73.Left          = 601;
  Box73.Top           = 326;
  Box73.Width         = 189;
  Box73.Height        = 89;
  Box73.Pen_Width     = 1;
  Box73.Pen_Color     = 0xFF00;
  Box73.Color         = 0xFF00;
  Box73.Press_Color   = 0xFFFFFF;
  Box73.ColorTo       = 0xFFFFFF;
  Box73.Press_ColorTo = 0xC0C0C0;
  Box73.Gradient      = _FT812_BRUSH_GR_NONE;
  Box73.Active        = 1;
  Box73.OnUp          = 0;
  Box73.OnDown        = 0;
  Box73.OnClick       = 0;
  Box73.OnPress       = 0;

  Box75.OwnerScreen   = &Time_Screen;
  Box75.Order         = 13;
  Box75.Visible       = 1;
  Box75.Opacity       = 255;
  Box75.Tag           = 255;
  Box75.Left          = 8;
  Box75.Top           = 159;
  Box75.Width         = 189;
  Box75.Height        = 80;
  Box75.Pen_Width     = 1;
  Box75.Pen_Color     = 0x0000;
  Box75.Color         = 0x808080;
  Box75.Press_Color   = 0xFFFFFF;
  Box75.ColorTo       = 0xFFFFFF;
  Box75.Press_ColorTo = 0xC0C0C0;
  Box75.Gradient      = _FT812_BRUSH_GR_NONE;
  Box75.Active        = 1;
  Box75.OnUp          = 0;
  Box75.OnDown        = 0;
  Box75.OnClick       = 0;
  Box75.OnPress       = 0;

  Label56.OwnerScreen = &Time_Screen;
  Label56.Order       = 14;
  Label56.Visible     = 1;
  Label56.Opacity     = 255;
  Label56.Tag         = 255;
  Label56.Left        = 250;
  Label56.Top         = 383;
  Label56.Width       = 79;
  Label56.Height      = 21;
  Label56.Caption     = Label56_Caption;
  Label56.FontName    = 28;
  Label56.Font_Color  = 0x0000;
  Label56.FontHandle  = 28;
  Label56.Source      = -1UL;
  Label56.Active      = 1;
  Label56.OnUp        = 0;
  Label56.OnDown      = 0;
  Label56.OnClick     = 0;
  Label56.OnPress     = 0;

  Label30.OwnerScreen = &Time_Screen;
  Label30.Order       = 15;
  Label30.Visible     = 1;
  Label30.Opacity     = 255;
  Label30.Tag         = 255;
  Label30.Left        = 462;
  Label30.Top         = 383;
  Label30.Width       = 51;
  Label30.Height      = 21;
  Label30.Caption     = Label30_Caption;
  Label30.FontName    = 28;
  Label30.Font_Color  = 0x0000;
  Label30.FontHandle  = 28;
  Label30.Source      = -1UL;
  Label30.Active      = 1;
  Label30.OnUp        = 0;
  Label30.OnDown      = 0;
  Label30.OnClick     = 0;
  Label30.OnPress     = 0;

  Label57.OwnerScreen = &Time_Screen;
  Label57.Order       = 16;
  Label57.Visible     = 1;
  Label57.Opacity     = 255;
  Label57.Tag         = 255;
  Label57.Left        = 646;
  Label57.Top         = 383;
  Label57.Width       = 71;
  Label57.Height      = 21;
  Label57.Caption     = Label57_Caption;
  Label57.FontName    = 28;
  Label57.Font_Color  = 0x0000;
  Label57.FontHandle  = 28;
  Label57.Source      = -1UL;
  Label57.Active      = 1;
  Label57.OnUp        = 0;
  Label57.OnDown      = 0;
  Label57.OnClick     = 0;
  Label57.OnPress     = 0;

  Label82.OwnerScreen = &Time_Screen;
  Label82.Order       = 17;
  Label82.Visible     = 1;
  Label82.Opacity     = 255;
  Label82.Tag         = 255;
  Label82.Left        = 62;
  Label82.Top         = 270;
  Label82.Width       = 61;
  Label82.Height      = 21;
  Label82.Caption     = Label82_Caption;
  Label82.FontName    = 28;
  Label82.Font_Color  = 0xFFFFFF;
  Label82.FontHandle  = 28;
  Label82.Source      = -1UL;
  Label82.Active      = 1;
  Label82.OnUp        = 0;
  Label82.OnDown      = 0;
  Label82.OnClick     = 0;
  Label82.OnPress     = 0;

  Label83.OwnerScreen = &Time_Screen;
  Label83.Order       = 18;
  Label83.Visible     = 1;
  Label83.Opacity     = 255;
  Label83.Tag         = 255;
  Label83.Left        = 70;
  Label83.Top         = 368;
  Label83.Width       = 51;
  Label83.Height      = 21;
  Label83.Caption     = Label83_Caption;
  Label83.FontName    = 28;
  Label83.Font_Color  = 0xFFFFFF;
  Label83.FontHandle  = 28;
  Label83.Source      = -1UL;
  Label83.Active      = 1;
  Label83.OnUp        = 0;
  Label83.OnDown      = 0;
  Label83.OnClick     = 0;
  Label83.OnPress     = 0;

  Label84.OwnerScreen = &Time_Screen;
  Label84.Order       = 19;
  Label84.Visible     = 1;
  Label84.Opacity     = 255;
  Label84.Tag         = 255;
  Label84.Left        = 66;
  Label84.Top         = 81;
  Label84.Width       = 72;
  Label84.Height      = 17;
  Label84.Caption     = Label84_Caption;
  Label84.FontName    = 27;
  Label84.Font_Color  = 0xFFFFFF;
  Label84.FontHandle  = 27;
  Label84.Source      = -1UL;
  Label84.Active      = 1;
  Label84.OnUp        = 0;
  Label84.OnDown      = 0;
  Label84.OnClick     = 0;
  Label84.OnPress     = 0;

  Label85.OwnerScreen = &Time_Screen;
  Label85.Order       = 20;
  Label85.Visible     = 1;
  Label85.Opacity     = 255;
  Label85.Tag         = 255;
  Label85.Left        = 60;
  Label85.Top         = 169;
  Label85.Width       = 82;
  Label85.Height      = 17;
  Label85.Caption     = Label85_Caption;
  Label85.FontName    = 27;
  Label85.Font_Color  = 0xFFFFFF;
  Label85.FontHandle  = 27;
  Label85.Source      = -1UL;
  Label85.Active      = 1;
  Label85.OnUp        = 0;
  Label85.OnDown      = 0;
  Label85.OnClick     = 0;
  Label85.OnPress     = 0;

  Label90.OwnerScreen = &Time_Screen;
  Label90.Order       = 21;
  Label90.Visible     = 1;
  Label90.Opacity     = 255;
  Label90.Tag         = 255;
  Label90.Left        = 152;
  Label90.Top         = 112;
  Label90.Width       = 13;
  Label90.Height      = 17;
  Label90.Caption     = Label90_Caption;
  Label90.FontName    = 27;
  Label90.Font_Color  = 0xFFFFFF;
  Label90.FontHandle  = 27;
  Label90.Source      = -1UL;
  Label90.Active      = 1;
  Label90.OnUp        = 0;
  Label90.OnDown      = 0;
  Label90.OnClick     = 0;
  Label90.OnPress     = 0;

  Label91.OwnerScreen = &Time_Screen;
  Label91.Order       = 22;
  Label91.Visible     = 1;
  Label91.Opacity     = 255;
  Label91.Tag         = 255;
  Label91.Left        = 140;
  Label91.Top         = 200;
  Label91.Width       = 26;
  Label91.Height      = 17;
  Label91.Caption     = Label91_Caption;
  Label91.FontName    = 27;
  Label91.Font_Color  = 0xFFFFFF;
  Label91.FontHandle  = 27;
  Label91.Source      = -1UL;
  Label91.Active      = 1;
  Label91.OnUp        = 0;
  Label91.OnDown      = 0;
  Label91.OnClick     = 0;
  Label91.OnPress     = 0;

  Label92.OwnerScreen = &Time_Screen;
  Label92.Order       = 23;
  Label92.Visible     = 1;
  Label92.Opacity     = 255;
  Label92.Tag         = 255;
  Label92.Left        = 79;
  Label92.Top         = 105;
  Label92.Width       = 47;
  Label92.Height      = 32;
  Label92.Caption     = Label92_Caption;
  Label92.FontName    = 30;
  Label92.Font_Color  = 0xFFFFFF;
  Label92.FontHandle  = 30;
  Label92.Source      = -1UL;
  Label92.Active      = 1;
  Label92.OnUp        = 0;
  Label92.OnDown      = 0;
  Label92.OnClick     = 0;
  Label92.OnPress     = 0;

  Label93.OwnerScreen = &Time_Screen;
  Label93.Order       = 24;
  Label93.Visible     = 1;
  Label93.Opacity     = 255;
  Label93.Tag         = 255;
  Label93.Left        = 79;
  Label93.Top         = 193;
  Label93.Width       = 47;
  Label93.Height      = 32;
  Label93.Caption     = Label93_Caption;
  Label93.FontName    = 30;
  Label93.Font_Color  = 0xFFFFFF;
  Label93.FontHandle  = 30;
  Label93.Source      = -1UL;
  Label93.Active      = 1;
  Label93.OnUp        = 0;
  Label93.OnDown      = 0;
  Label93.OnClick     = 0;
  Label93.OnPress     = 0;

  Label94.OwnerScreen = &Time_Screen;
  Label94.Order       = 25;
  Label94.Visible     = 1;
  Label94.Opacity     = 255;
  Label94.Tag         = 255;
  Label94.Left        = 245;
  Label94.Top         = 100;
  Label94.Width       = 54;
  Label94.Height      = 21;
  Label94.Caption     = Label94_Caption;
  Label94.FontName    = 28;
  Label94.Font_Color  = 0xFFFFFF;
  Label94.FontHandle  = 28;
  Label94.Source      = -1UL;
  Label94.Active      = 1;
  Label94.OnUp        = 0;
  Label94.OnDown      = 0;
  Label94.OnClick     = 0;
  Label94.OnPress     = 0;

  Label99.OwnerScreen = &Time_Screen;
  Label99.Order       = 26;
  Label99.Visible     = 1;
  Label99.Opacity     = 255;
  Label99.Tag         = 255;
  Label99.Left        = 461;
  Label99.Top         = 100;
  Label99.Width       = 31;
  Label99.Height      = 21;
  Label99.Caption     = Label99_Caption;
  Label99.FontName    = 28;
  Label99.Font_Color  = 0xFFFFFF;
  Label99.FontHandle  = 28;
  Label99.Source      = -1UL;
  Label99.Active      = 1;
  Label99.OnUp        = 0;
  Label99.OnDown      = 0;
  Label99.OnClick     = 0;
  Label99.OnPress     = 0;

  Label100.OwnerScreen = &Time_Screen;
  Label100.Order       = 27;
  Label100.Visible     = 1;
  Label100.Opacity     = 255;
  Label100.Tag         = 255;
  Label100.Left        = 658;
  Label100.Top         = 100;
  Label100.Width       = 31;
  Label100.Height      = 21;
  Label100.Caption     = Label100_Caption;
  Label100.FontName    = 28;
  Label100.Font_Color  = 0xFFFFFF;
  Label100.FontHandle  = 28;
  Label100.Source      = -1UL;
  Label100.Active      = 1;
  Label100.OnUp        = 0;
  Label100.OnDown      = 0;
  Label100.OnClick     = 0;
  Label100.OnPress     = 0;

  Label79.OwnerScreen = &Time_Screen;
  Label79.Order       = 28;
  Label79.Visible     = 1;
  Label79.Opacity     = 255;
  Label79.Tag         = 255;
  Label79.Left        = 320;
  Label79.Top         = 100;
  Label79.Width       = 9;
  Label79.Height      = 21;
  Label79.Caption     = Label79_Caption;
  Label79.FontName    = 28;
  Label79.Font_Color  = 0xFFFFFF;
  Label79.FontHandle  = 28;
  Label79.Source      = -1UL;
  Label79.Active      = 1;
  Label79.OnUp        = 0;
  Label79.OnDown      = 0;
  Label79.OnClick     = 0;
  Label79.OnPress     = 0;

  Label80.OwnerScreen = &Time_Screen;
  Label80.Order       = 29;
  Label80.Visible     = 1;
  Label80.Opacity     = 255;
  Label80.Tag         = 255;
  Label80.Left        = 513;
  Label80.Top         = 100;
  Label80.Width       = 12;
  Label80.Height      = 21;
  Label80.Caption     = Label80_Caption;
  Label80.FontName    = 28;
  Label80.Font_Color  = 0xFFFFFF;
  Label80.FontHandle  = 28;
  Label80.Source      = -1UL;
  Label80.Active      = 1;
  Label80.OnUp        = 0;
  Label80.OnDown      = 0;
  Label80.OnClick     = 0;
  Label80.OnPress     = 0;

  Label81.OwnerScreen = &Time_Screen;
  Label81.Order       = 30;
  Label81.Visible     = 1;
  Label81.Opacity     = 255;
  Label81.Tag         = 255;
  Label81.Left        = 710;
  Label81.Top         = 100;
  Label81.Width       = 12;
  Label81.Height      = 21;
  Label81.Caption     = Label81_Caption;
  Label81.FontName    = 28;
  Label81.Font_Color  = 0xFFFFFF;
  Label81.FontHandle  = 28;
  Label81.Source      = -1UL;
  Label81.Active      = 1;
  Label81.OnUp        = 0;
  Label81.OnDown      = 0;
  Label81.OnClick     = 0;
  Label81.OnPress     = 0;

  Box33.OwnerScreen   = &Settings_Screen;
  Box33.Order         = 0;
  Box33.Visible       = 1;
  Box33.Opacity       = 255;
  Box33.Tag           = 255;
  Box33.Left          = 8;
  Box33.Top           = 71;
  Box33.Width         = 431;
  Box33.Height        = 344;
  Box33.Pen_Width     = 1;
  Box33.Pen_Color     = 0x0000;
  Box33.Color         = 0xC08040;
  Box33.Press_Color   = 0xFFFFFF;
  Box33.ColorTo       = 0xFFFFFF;
  Box33.Press_ColorTo = 0xC0C0C0;
  Box33.Gradient      = _FT812_BRUSH_GR_NONE;
  Box33.Active        = 1;
  Box33.OnUp          = 0;
  Box33.OnDown        = 0;
  Box33.OnClick       = 0;
  Box33.OnPress       = 0;

  Label34.OwnerScreen = &Settings_Screen;
  Label34.Order       = 1;
  Label34.Visible     = 1;
  Label34.Opacity     = 255;
  Label34.Tag         = 255;
  Label34.Left        = 82;
  Label34.Top         = 100;
  Label34.Width       = 49;
  Label34.Height      = 21;
  Label34.Caption     = Label34_Caption;
  Label34.FontName    = 28;
  Label34.Font_Color  = 0x400000;
  Label34.FontHandle  = 28;
  Label34.Source      = -1UL;
  Label34.Active      = 1;
  Label34.OnUp        = 0;
  Label34.OnDown      = 0;
  Label34.OnClick     = 0;
  Label34.OnPress     = 0;

  Label44.OwnerScreen = &Settings_Screen;
  Label44.Order       = 2;
  Label44.Visible     = 1;
  Label44.Opacity     = 255;
  Label44.Tag         = 255;
  Label44.Left        = 82;
  Label44.Top         = 166;
  Label44.Width       = 64;
  Label44.Height      = 21;
  Label44.Caption     = Label44_Caption;
  Label44.FontName    = 28;
  Label44.Font_Color  = 0xFFFFFF;
  Label44.FontHandle  = 28;
  Label44.Source      = -1UL;
  Label44.Active      = 1;
  Label44.OnUp        = 0;
  Label44.OnDown      = 0;
  Label44.OnClick     = 0;
  Label44.OnPress     = 0;

  Label45.OwnerScreen = &Settings_Screen;
  Label45.Order       = 3;
  Label45.Visible     = 1;
  Label45.Opacity     = 255;
  Label45.Tag         = 255;
  Label45.Left        = 82;
  Label45.Top         = 230;
  Label45.Width       = 33;
  Label45.Height      = 21;
  Label45.Caption     = Label45_Caption;
  Label45.FontName    = 28;
  Label45.Font_Color  = 0xFFFFFF;
  Label45.FontHandle  = 28;
  Label45.Source      = -1UL;
  Label45.Active      = 1;
  Label45.OnUp        = 0;
  Label45.OnDown      = 0;
  Label45.OnClick     = 0;
  Label45.OnPress     = 0;

  Label71.OwnerScreen = &Settings_Screen;
  Label71.Order       = 4;
  Label71.Visible     = 1;
  Label71.Opacity     = 255;
  Label71.Tag         = 255;
  Label71.Left        = 82;
  Label71.Top         = 294;
  Label71.Width       = 52;
  Label71.Height      = 21;
  Label71.Caption     = Label71_Caption;
  Label71.FontName    = 28;
  Label71.Font_Color  = 0xFFFFFF;
  Label71.FontHandle  = 28;
  Label71.Source      = -1UL;
  Label71.Active      = 1;
  Label71.OnUp        = 0;
  Label71.OnDown      = 0;
  Label71.OnClick     = 0;
  Label71.OnPress     = 0;

  Label72.OwnerScreen = &Settings_Screen;
  Label72.Order       = 5;
  Label72.Visible     = 1;
  Label72.Opacity     = 255;
  Label72.Tag         = 255;
  Label72.Left        = 82;
  Label72.Top         = 359;
  Label72.Width       = 38;
  Label72.Height      = 21;
  Label72.Caption     = Label72_Caption;
  Label72.FontName    = 28;
  Label72.Font_Color  = 0xFFFFFF;
  Label72.FontHandle  = 28;
  Label72.Source      = -1UL;
  Label72.Active      = 1;
  Label72.OnUp        = 0;
  Label72.OnDown      = 0;
  Label72.OnClick     = 0;
  Label72.OnPress     = 0;

  Button48.OwnerScreen   = &Settings_Screen;
  Button48.Order         = 6;
  Button48.Visible       = 1;
  Button48.Opacity       = 255;
  Button48.Tag           = 255;
  Button48.Left          = 8;
  Button48.Top           = 423;
  Button48.Width         = 50;
  Button48.Height        = 50;
  Button48.Pen_Width     = 1;
  Button48.Pen_Color     = 0x0000;
  Button48.Color         = 0x808080;
  Button48.Press_Color   = 0xFFFFFF;
  Button48.ColorTo       = 0xFFFFFF;
  Button48.Press_ColorTo = 0xC0C0C0;
  Button48.Gradient      = _FT812_BRUSH_GR_NONE;
  Button48.Caption       = Button48_Caption;
  Button48.TextAlign     = taCenter;
  Button48.FontName      = 30;
  Button48.Font_Color    = 0xFFFFFF;
  Button48.FontHandle    = 30;
  Button48.Source        = -1UL;
  Button48.Active        = 1;
  Button48.OnUp          = 0;
  Button48.OnDown        = 0;
  Button48.OnClick       = &Button48_OnClick;
  Button48.OnPress       = 0;

  Button48_OnClick.Action       = Box54OnClick;
  Button48_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button48_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button48_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button48_OnClick.Sound.Volume = 255;

  Box79.OwnerScreen   = &Settings_Screen;
  Box79.Order         = 7;
  Box79.Visible       = 1;
  Box79.Opacity       = 255;
  Box79.Tag           = 255;
  Box79.Left          = 66;
  Box79.Top           = 423;
  Box79.Width         = 724;
  Box79.Height        = 50;
  Box79.Pen_Width     = 1;
  Box79.Pen_Color     = 0x0000;
  Box79.Color         = 0x404040;
  Box79.Press_Color   = 0xFFFFFF;
  Box79.ColorTo       = 0xFFFFFF;
  Box79.Press_ColorTo = 0xC0C0C0;
  Box79.Gradient      = _FT812_BRUSH_GR_NONE;
  Box79.Active        = 1;
  Box79.OnUp          = 0;
  Box79.OnDown        = 0;
  Box79.OnClick       = 0;
  Box79.OnPress       = 0;

  Button1.OwnerScreen   = &Settings_Screen;
  Button1.Order         = 8;
  Button1.Visible       = 1;
  Button1.Opacity       = 255;
  Button1.Tag           = 255;
  Button1.Left          = 447;
  Button1.Top           = 71;
  Button1.Width         = 80;
  Button1.Height        = 80;
  Button1.Pen_Width     = 1;
  Button1.Pen_Color     = 0x0000;
  Button1.Color         = 0xFF8080;
  Button1.Press_Color   = 0xFFFFFF;
  Button1.ColorTo       = 0xFFFFFF;
  Button1.Press_ColorTo = 0xC0C0C0;
  Button1.Gradient      = _FT812_BRUSH_GR_NONE;
  Button1.Caption       = Button1_Caption;
  Button1.TextAlign     = taCenter;
  Button1.FontName      = 29;
  Button1.Font_Color    = 0xFFFFFF;
  Button1.FontHandle    = 29;
  Button1.Source        = -1UL;
  Button1.Active        = 1;
  Button1.OnUp          = 0;
  Button1.OnDown        = 0;
  Button1.OnClick       = &Button1_OnClick;
  Button1.OnPress       = 0;

  Button1_OnClick.Action       = Button1OnClick;
  Button1_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button1_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button1_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button1_OnClick.Sound.Volume = 255;

  Button2.OwnerScreen   = &Settings_Screen;
  Button2.Order         = 9;
  Button2.Visible       = 1;
  Button2.Opacity       = 255;
  Button2.Tag           = 255;
  Button2.Left          = 535;
  Button2.Top           = 71;
  Button2.Width         = 80;
  Button2.Height        = 80;
  Button2.Pen_Width     = 1;
  Button2.Pen_Color     = 0x0000;
  Button2.Color         = 0xFF8080;
  Button2.Press_Color   = 0xFFFFFF;
  Button2.ColorTo       = 0xFFFFFF;
  Button2.Press_ColorTo = 0xC0C0C0;
  Button2.Gradient      = _FT812_BRUSH_GR_NONE;
  Button2.Caption       = Button2_Caption;
  Button2.TextAlign     = taCenter;
  Button2.FontName      = 29;
  Button2.Font_Color    = 0xFFFFFF;
  Button2.FontHandle    = 29;
  Button2.Source        = -1UL;
  Button2.Active        = 1;
  Button2.OnUp          = 0;
  Button2.OnDown        = 0;
  Button2.OnClick       = &Button2_OnClick;
  Button2.OnPress       = 0;

  Button2_OnClick.Action       = Button2OnClick;
  Button2_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button2_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button2_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button2_OnClick.Sound.Volume = 255;

  Button3.OwnerScreen   = &Settings_Screen;
  Button3.Order         = 10;
  Button3.Visible       = 1;
  Button3.Opacity       = 255;
  Button3.Tag           = 255;
  Button3.Left          = 623;
  Button3.Top           = 71;
  Button3.Width         = 80;
  Button3.Height        = 80;
  Button3.Pen_Width     = 1;
  Button3.Pen_Color     = 0x0000;
  Button3.Color         = 0xFF8080;
  Button3.Press_Color   = 0xFFFFFF;
  Button3.ColorTo       = 0xFFFFFF;
  Button3.Press_ColorTo = 0xC0C0C0;
  Button3.Gradient      = _FT812_BRUSH_GR_NONE;
  Button3.Caption       = Button3_Caption;
  Button3.TextAlign     = taCenter;
  Button3.FontName      = 29;
  Button3.Font_Color    = 0xFFFFFF;
  Button3.FontHandle    = 29;
  Button3.Source        = -1UL;
  Button3.Active        = 1;
  Button3.OnUp          = 0;
  Button3.OnDown        = 0;
  Button3.OnClick       = &Button3_OnClick;
  Button3.OnPress       = 0;

  Button3_OnClick.Action       = Button3OnClick;
  Button3_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button3_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button3_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button3_OnClick.Sound.Volume = 255;

  Button4.OwnerScreen   = &Settings_Screen;
  Button4.Order         = 11;
  Button4.Visible       = 1;
  Button4.Opacity       = 255;
  Button4.Tag           = 255;
  Button4.Left          = 447;
  Button4.Top           = 159;
  Button4.Width         = 80;
  Button4.Height        = 80;
  Button4.Pen_Width     = 1;
  Button4.Pen_Color     = 0x0000;
  Button4.Color         = 0xFF8080;
  Button4.Press_Color   = 0xFFFFFF;
  Button4.ColorTo       = 0xFFFFFF;
  Button4.Press_ColorTo = 0xC0C0C0;
  Button4.Gradient      = _FT812_BRUSH_GR_NONE;
  Button4.Caption       = Button4_Caption;
  Button4.TextAlign     = taCenter;
  Button4.FontName      = 29;
  Button4.Font_Color    = 0xFFFFFF;
  Button4.FontHandle    = 29;
  Button4.Source        = -1UL;
  Button4.Active        = 1;
  Button4.OnUp          = 0;
  Button4.OnDown        = 0;
  Button4.OnClick       = &Button4_OnClick;
  Button4.OnPress       = 0;

  Button4_OnClick.Action       = Button4OnClick;
  Button4_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button4_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button4_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button4_OnClick.Sound.Volume = 255;

  Button5.OwnerScreen   = &Settings_Screen;
  Button5.Order         = 12;
  Button5.Visible       = 1;
  Button5.Opacity       = 255;
  Button5.Tag           = 255;
  Button5.Left          = 535;
  Button5.Top           = 159;
  Button5.Width         = 80;
  Button5.Height        = 80;
  Button5.Pen_Width     = 1;
  Button5.Pen_Color     = 0x0000;
  Button5.Color         = 0xFF8080;
  Button5.Press_Color   = 0xFFFFFF;
  Button5.ColorTo       = 0xFFFFFF;
  Button5.Press_ColorTo = 0xC0C0C0;
  Button5.Gradient      = _FT812_BRUSH_GR_NONE;
  Button5.Caption       = Button5_Caption;
  Button5.TextAlign     = taCenter;
  Button5.FontName      = 29;
  Button5.Font_Color    = 0xFFFFFF;
  Button5.FontHandle    = 29;
  Button5.Source        = -1UL;
  Button5.Active        = 1;
  Button5.OnUp          = 0;
  Button5.OnDown        = 0;
  Button5.OnClick       = &Button5_OnClick;
  Button5.OnPress       = 0;

  Button5_OnClick.Action       = Button5OnClick;
  Button5_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button5_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button5_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button5_OnClick.Sound.Volume = 255;

  Button6.OwnerScreen   = &Settings_Screen;
  Button6.Order         = 13;
  Button6.Visible       = 1;
  Button6.Opacity       = 255;
  Button6.Tag           = 255;
  Button6.Left          = 623;
  Button6.Top           = 159;
  Button6.Width         = 80;
  Button6.Height        = 80;
  Button6.Pen_Width     = 1;
  Button6.Pen_Color     = 0x0000;
  Button6.Color         = 0xFF8080;
  Button6.Press_Color   = 0xFFFFFF;
  Button6.ColorTo       = 0xFFFFFF;
  Button6.Press_ColorTo = 0xC0C0C0;
  Button6.Gradient      = _FT812_BRUSH_GR_NONE;
  Button6.Caption       = Button6_Caption;
  Button6.TextAlign     = taCenter;
  Button6.FontName      = 29;
  Button6.Font_Color    = 0xFFFFFF;
  Button6.FontHandle    = 29;
  Button6.Source        = -1UL;
  Button6.Active        = 1;
  Button6.OnUp          = 0;
  Button6.OnDown        = 0;
  Button6.OnClick       = &Button6_OnClick;
  Button6.OnPress       = 0;

  Button6_OnClick.Action       = Button6OnClick;
  Button6_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button6_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button6_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button6_OnClick.Sound.Volume = 255;

  Button7.OwnerScreen   = &Settings_Screen;
  Button7.Order         = 14;
  Button7.Visible       = 1;
  Button7.Opacity       = 255;
  Button7.Tag           = 255;
  Button7.Left          = 447;
  Button7.Top           = 247;
  Button7.Width         = 80;
  Button7.Height        = 80;
  Button7.Pen_Width     = 1;
  Button7.Pen_Color     = 0x0000;
  Button7.Color         = 0xFF8080;
  Button7.Press_Color   = 0xFFFFFF;
  Button7.ColorTo       = 0xFFFFFF;
  Button7.Press_ColorTo = 0xC0C0C0;
  Button7.Gradient      = _FT812_BRUSH_GR_NONE;
  Button7.Caption       = Button7_Caption;
  Button7.TextAlign     = taCenter;
  Button7.FontName      = 29;
  Button7.Font_Color    = 0xFFFFFF;
  Button7.FontHandle    = 29;
  Button7.Source        = -1UL;
  Button7.Active        = 1;
  Button7.OnUp          = 0;
  Button7.OnDown        = 0;
  Button7.OnClick       = &Button7_OnClick;
  Button7.OnPress       = 0;

  Button7_OnClick.Action       = Button7OnClick;
  Button7_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button7_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button7_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button7_OnClick.Sound.Volume = 255;

  Button8.OwnerScreen   = &Settings_Screen;
  Button8.Order         = 15;
  Button8.Visible       = 1;
  Button8.Opacity       = 255;
  Button8.Tag           = 255;
  Button8.Left          = 535;
  Button8.Top           = 247;
  Button8.Width         = 80;
  Button8.Height        = 80;
  Button8.Pen_Width     = 1;
  Button8.Pen_Color     = 0x0000;
  Button8.Color         = 0xFF8080;
  Button8.Press_Color   = 0xFFFFFF;
  Button8.ColorTo       = 0xFFFFFF;
  Button8.Press_ColorTo = 0xC0C0C0;
  Button8.Gradient      = _FT812_BRUSH_GR_NONE;
  Button8.Caption       = Button8_Caption;
  Button8.TextAlign     = taCenter;
  Button8.FontName      = 29;
  Button8.Font_Color    = 0xFFFFFF;
  Button8.FontHandle    = 29;
  Button8.Source        = -1UL;
  Button8.Active        = 1;
  Button8.OnUp          = 0;
  Button8.OnDown        = 0;
  Button8.OnClick       = &Button8_OnClick;
  Button8.OnPress       = 0;

  Button8_OnClick.Action       = Button8OnClick;
  Button8_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button8_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button8_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button8_OnClick.Sound.Volume = 255;

  Button9.OwnerScreen   = &Settings_Screen;
  Button9.Order         = 16;
  Button9.Visible       = 1;
  Button9.Opacity       = 255;
  Button9.Tag           = 255;
  Button9.Left          = 623;
  Button9.Top           = 247;
  Button9.Width         = 80;
  Button9.Height        = 80;
  Button9.Pen_Width     = 1;
  Button9.Pen_Color     = 0x0000;
  Button9.Color         = 0xFF8080;
  Button9.Press_Color   = 0xFFFFFF;
  Button9.ColorTo       = 0xFFFFFF;
  Button9.Press_ColorTo = 0xC0C0C0;
  Button9.Gradient      = _FT812_BRUSH_GR_NONE;
  Button9.Caption       = Button9_Caption;
  Button9.TextAlign     = taCenter;
  Button9.FontName      = 29;
  Button9.Font_Color    = 0xFFFFFF;
  Button9.FontHandle    = 29;
  Button9.Source        = -1UL;
  Button9.Active        = 1;
  Button9.OnUp          = 0;
  Button9.OnDown        = 0;
  Button9.OnClick       = &Button9_OnClick;
  Button9.OnPress       = 0;

  Button9_OnClick.Action       = Button9OnClick;
  Button9_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button9_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button9_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button9_OnClick.Sound.Volume = 255;

  Button10.OwnerScreen   = &Settings_Screen;
  Button10.Order         = 17;
  Button10.Visible       = 1;
  Button10.Opacity       = 255;
  Button10.Tag           = 255;
  Button10.Left          = 535;
  Button10.Top           = 335;
  Button10.Width         = 80;
  Button10.Height        = 80;
  Button10.Pen_Width     = 1;
  Button10.Pen_Color     = 0x0000;
  Button10.Color         = 0xFF8080;
  Button10.Press_Color   = 0xFFFFFF;
  Button10.ColorTo       = 0xFFFFFF;
  Button10.Press_ColorTo = 0xC0C0C0;
  Button10.Gradient      = _FT812_BRUSH_GR_NONE;
  Button10.Caption       = Button10_Caption;
  Button10.TextAlign     = taCenter;
  Button10.FontName      = 29;
  Button10.Font_Color    = 0xFFFFFF;
  Button10.FontHandle    = 29;
  Button10.Source        = -1UL;
  Button10.Active        = 1;
  Button10.OnUp          = 0;
  Button10.OnDown        = 0;
  Button10.OnClick       = &Button10_OnClick;
  Button10.OnPress       = 0;

  Button10_OnClick.Action       = Button10OnClick;
  Button10_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button10_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button10_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button10_OnClick.Sound.Volume = 255;

  Button39.OwnerScreen   = &Settings_Screen;
  Button39.Order         = 18;
  Button39.Visible       = 1;
  Button39.Opacity       = 255;
  Button39.Tag           = 255;
  Button39.Left          = 711;
  Button39.Top           = 71;
  Button39.Width         = 80;
  Button39.Height        = 62;
  Button39.Pen_Width     = 1;
  Button39.Pen_Color     = 0x0000;
  Button39.Color         = 0xFF0000;
  Button39.Press_Color   = 0xFFFFFF;
  Button39.ColorTo       = 0xFFFFFF;
  Button39.Press_ColorTo = 0xC0C0C0;
  Button39.Gradient      = _FT812_BRUSH_GR_NONE;
  Button39.Caption       = Button39_Caption;
  Button39.TextAlign     = taCenter;
  Button39.FontName      = 27;
  Button39.Font_Color    = 0xFFFFFF;
  Button39.FontHandle    = 27;
  Button39.Source        = -1UL;
  Button39.Active        = 1;
  Button39.OnUp          = 0;
  Button39.OnDown        = 0;
  Button39.OnClick       = &Button39_OnClick;
  Button39.OnPress       = 0;

  Button39_OnClick.Action       = Button39OnClick;
  Button39_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button39_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button39_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button39_OnClick.Sound.Volume = 255;

  Button40.OwnerScreen   = &Settings_Screen;
  Button40.Order         = 19;
  Button40.Visible       = 1;
  Button40.Opacity       = 255;
  Button40.Tag           = 255;
  Button40.Left          = 711;
  Button40.Top           = 141;
  Button40.Width         = 80;
  Button40.Height        = 62;
  Button40.Pen_Width     = 1;
  Button40.Pen_Color     = 0x0000;
  Button40.Color         = 0x800000;
  Button40.Press_Color   = 0xFFFFFF;
  Button40.ColorTo       = 0xFFFFFF;
  Button40.Press_ColorTo = 0xC0C0C0;
  Button40.Gradient      = _FT812_BRUSH_GR_NONE;
  Button40.Caption       = Button40_Caption;
  Button40.TextAlign     = taCenter;
  Button40.FontName      = 27;
  Button40.Font_Color    = 0xFFFFFF;
  Button40.FontHandle    = 27;
  Button40.Source        = -1UL;
  Button40.Active        = 1;
  Button40.OnUp          = 0;
  Button40.OnDown        = 0;
  Button40.OnClick       = &Button40_OnClick;
  Button40.OnPress       = 0;

  Button40_OnClick.Action       = Button40OnClick;
  Button40_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button40_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button40_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button40_OnClick.Sound.Volume = 255;

  Button41.OwnerScreen   = &Settings_Screen;
  Button41.Order         = 20;
  Button41.Visible       = 1;
  Button41.Opacity       = 255;
  Button41.Tag           = 255;
  Button41.Left          = 711;
  Button41.Top           = 211;
  Button41.Width         = 80;
  Button41.Height        = 62;
  Button41.Pen_Width     = 1;
  Button41.Pen_Color     = 0x0000;
  Button41.Color         = 0x800000;
  Button41.Press_Color   = 0xFFFFFF;
  Button41.ColorTo       = 0xFFFFFF;
  Button41.Press_ColorTo = 0xC0C0C0;
  Button41.Gradient      = _FT812_BRUSH_GR_NONE;
  Button41.Caption       = Button41_Caption;
  Button41.TextAlign     = taCenter;
  Button41.FontName      = 27;
  Button41.Font_Color    = 0xFFFFFF;
  Button41.FontHandle    = 27;
  Button41.Source        = -1UL;
  Button41.Active        = 1;
  Button41.OnUp          = 0;
  Button41.OnDown        = 0;
  Button41.OnClick       = &Button41_OnClick;
  Button41.OnPress       = 0;

  Button41_OnClick.Action       = Button41OnClick;
  Button41_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button41_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button41_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button41_OnClick.Sound.Volume = 255;

  Button42.OwnerScreen   = &Settings_Screen;
  Button42.Order         = 21;
  Button42.Visible       = 1;
  Button42.Opacity       = 255;
  Button42.Tag           = 255;
  Button42.Left          = 711;
  Button42.Top           = 281;
  Button42.Width         = 80;
  Button42.Height        = 63;
  Button42.Pen_Width     = 1;
  Button42.Pen_Color     = 0x0000;
  Button42.Color         = 0x800000;
  Button42.Press_Color   = 0xFFFFFF;
  Button42.ColorTo       = 0xFFFFFF;
  Button42.Press_ColorTo = 0xC0C0C0;
  Button42.Gradient      = _FT812_BRUSH_GR_NONE;
  Button42.Caption       = Button42_Caption;
  Button42.TextAlign     = taCenter;
  Button42.FontName      = 27;
  Button42.Font_Color    = 0xFFFFFF;
  Button42.FontHandle    = 27;
  Button42.Source        = -1UL;
  Button42.Active        = 1;
  Button42.OnUp          = 0;
  Button42.OnDown        = 0;
  Button42.OnClick       = &Button42_OnClick;
  Button42.OnPress       = 0;

  Button42_OnClick.Action       = Button42OnClick;
  Button42_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button42_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button42_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button42_OnClick.Sound.Volume = 255;

  Button43.OwnerScreen   = &Settings_Screen;
  Button43.Order         = 22;
  Button43.Visible       = 1;
  Button43.Opacity       = 255;
  Button43.Tag           = 255;
  Button43.Left          = 447;
  Button43.Top           = 335;
  Button43.Width         = 80;
  Button43.Height        = 80;
  Button43.Pen_Width     = 1;
  Button43.Pen_Color     = 0x0000;
  Button43.Color         = 0xFF8080;
  Button43.Press_Color   = 0xFFFFFF;
  Button43.ColorTo       = 0xFFFFFF;
  Button43.Press_ColorTo = 0xC0C0C0;
  Button43.Gradient      = _FT812_BRUSH_GR_NONE;
  Button43.Caption       = Button43_Caption;
  Button43.TextAlign     = taCenter;
  Button43.FontName      = 29;
  Button43.Font_Color    = 0xFFFFFF;
  Button43.FontHandle    = 29;
  Button43.Source        = -1UL;
  Button43.Active        = 1;
  Button43.OnUp          = 0;
  Button43.OnDown        = 0;
  Button43.OnClick       = &Button43_OnClick;
  Button43.OnPress       = 0;

  Button43_OnClick.Action       = Button11OnClick;
  Button43_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button43_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button43_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button43_OnClick.Sound.Volume = 255;

  Button44.OwnerScreen   = &Settings_Screen;
  Button44.Order         = 23;
  Button44.Visible       = 1;
  Button44.Opacity       = 255;
  Button44.Tag           = 255;
  Button44.Left          = 623;
  Button44.Top           = 335;
  Button44.Width         = 80;
  Button44.Height        = 80;
  Button44.Pen_Width     = 1;
  Button44.Pen_Color     = 0x0000;
  Button44.Color         = 0xFF8080;
  Button44.Press_Color   = 0xFFFFFF;
  Button44.ColorTo       = 0xFFFFFF;
  Button44.Press_ColorTo = 0xC0C0C0;
  Button44.Gradient      = _FT812_BRUSH_GR_NONE;
  Button44.Caption       = Button44_Caption;
  Button44.TextAlign     = taCenter;
  Button44.FontName      = 29;
  Button44.Font_Color    = 0xFFFFFF;
  Button44.FontHandle    = 29;
  Button44.Source        = -1UL;
  Button44.Active        = 1;
  Button44.OnUp          = 0;
  Button44.OnDown        = 0;
  Button44.OnClick       = &Button44_OnClick;
  Button44.OnPress       = 0;

  Button44_OnClick.Action       = Button44OnClick;
  Button44_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button44_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button44_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button44_OnClick.Sound.Volume = 255;

  Button45.OwnerScreen   = &Settings_Screen;
  Button45.Order         = 24;
  Button45.Visible       = 1;
  Button45.Opacity       = 255;
  Button45.Tag           = 255;
  Button45.Left          = 8;
  Button45.Top           = 8;
  Button45.Width         = 782;
  Button45.Height        = 55;
  Button45.Pen_Width     = 1;
  Button45.Pen_Color     = 0x0000;
  Button45.Color         = 0xFF0000;
  Button45.Press_Color   = 0xFFFFFF;
  Button45.ColorTo       = 0xFFFFFF;
  Button45.Press_ColorTo = 0xC0C0C0;
  Button45.Gradient      = _FT812_BRUSH_GR_NONE;
  Button45.Caption       = Button45_Caption;
  Button45.TextAlign     = taCenter;
  Button45.FontName      = 31;
  Button45.Font_Color    = 0xFFFFFF;
  Button45.FontHandle    = 31;
  Button45.Source        = -1UL;
  Button45.Active        = 1;
  Button45.OnUp          = 0;
  Button45.OnDown        = 0;
  Button45.OnClick       = 0;
  Button45.OnPress       = 0;

  Button46.OwnerScreen   = &Settings_Screen;
  Button46.Order         = 25;
  Button46.Visible       = 1;
  Button46.Opacity       = 255;
  Button46.Tag           = 255;
  Button46.Left          = 711;
  Button46.Top           = 352;
  Button46.Width         = 80;
  Button46.Height        = 63;
  Button46.Pen_Width     = 1;
  Button46.Pen_Color     = 0x0000;
  Button46.Color         = 0x800000;
  Button46.Press_Color   = 0xFFFFFF;
  Button46.ColorTo       = 0xFFFFFF;
  Button46.Press_ColorTo = 0xC0C0C0;
  Button46.Gradient      = _FT812_BRUSH_GR_NONE;
  Button46.Caption       = Button46_Caption;
  Button46.TextAlign     = taCenter;
  Button46.FontName      = 27;
  Button46.Font_Color    = 0xFFFFFF;
  Button46.FontHandle    = 27;
  Button46.Source        = -1UL;
  Button46.Active        = 1;
  Button46.OnUp          = 0;
  Button46.OnDown        = 0;
  Button46.OnClick       = &Button46_OnClick;
  Button46.OnPress       = 0;

  Button46_OnClick.Action       = Button46OnClick;
  Button46_OnClick.Sound.SndAct = VTFT_SNDACT_NONE;
  Button46_OnClick.Sound.Effect = _FT812_SOUND_XYLOPHONE;
  Button46_OnClick.Sound.Pitch  = _FT812_SOUND_PITCH_A5;
  Button46_OnClick.Sound.Volume = 255;

}

void Init_MCU() {
  // Setup FT810 SPI interface
  SPIM1_Init_Advanced(_SPI_MASTER_CLK_RATIO_8, _SPI_CFG_POLARITY_IDLE_LOW | _SPI_CFG_PHASE_CAPTURE_FALLING, _SPI_SS_LINE_NONE); 
}

char *VTFT_GetExtData(unsigned long fPos, unsigned long bytesToGet, unsigned long *pBytesGot) {
  unsigned int  scOffset;

  *pBytesGot = 0;

  if (rfHandle < 0) {
    return 0; // return with nil if handle was bad.
  }

  // We will utilize some of the fat32 implied features
  // (such as built in sector buffer) to reduce ram memory consumpiton.
  // You can't use this approach if this buffer is going to be used
  // from another thread (i.e. using fat32 routines in interrupt...)
  // In that case or if you have plenty of ram memory use separate
  // as large as possible data buffer.
  f32_sector.fSectNum = rfStartSect + fPos / 512;
  Mmc_Read_Sector(f32_sector.fSectNum, f32_sector.fSect);

  // 512 bytes sector buffer implied.
  scOffset = fPos % 512;
  if(bytesToGet > 512-scOffset)
    *pBytesGot = 512-scOffset;
  else
    *pBytesGot = bytesToGet;

  return &f32_sector.fSect + scOffset;
}

void Init_Ext_Mem() {
  rfHandle    = -1;
  rfName      = "Mikromed.RES";
  rfStartSect = 0;

  // Init SDHost interface at low speed
  Mmc_Set_Interface(_MMC_INTERFACE_SDHOST);
  SDHost_Init(128, _SDHOST_CFG_4_WIDE_BUS_MODE | _SDHOST_CFG_CLOCK_FALING_EDGE);
  Delay_ms(100);

  // Init FAT32 stack
  if (FAT32_Init() == 0) {
    //  Reinitialize SDHost at higher speed
    SDHost_Init(2, _SDHOST_CFG_4_WIDE_BUS_MODE | _SDHOST_CFG_CLOCK_FALING_EDGE);

    // Open resource file for read
    rfHandle    = FAT32_Open(rfName, 0x01);
    rfStartSect = FAT32_ClustToSect(fat32_fdesc[rfHandle]._1stClust);
  }
}

void InitVTFTStack() {
  union {
    TFT812PWM       PWM;
    TFT812GPIO      GPIO;
    TFT812Audio     Audio;
    TFT812Sound     Sound;
    TFT812Display   Display;
    TFT812Interrupt Interrupt;
  } cfg;

  Init_MCU();

  Init_Ext_Mem();


  // Init all dynamic objects
  InitObjects();

  // Init FT812 controller core and library stack
  FT812_Init();

  FT812_Core_ClockSource(_FT812_CLK_SOURCE_EXTERNAL);
  FT812_Core_SetFrequency(VTFT_FT812_CONFIG_DISPLAY.Frequency);

  // Internal modules setup
  cfg.Display = VTFT_FT812_CONFIG_DISPLAY;
  FT812_Display_SetConfig(&cfg.Display);

  cfg.Audio = VTFT_FT812_CONFIG_AUDIO;
  FT812_Audio_SetConfig(&cfg.Audio);

  cfg.Sound = VTFT_FT812_CONFIG_SOUND;
  FT812_Sound_SetConfig(&cfg.Sound);

  cfg.Interrupt = VTFT_FT812_CONFIG_INTERRUPT;
  FT812_Interrupt_SetConfig(&cfg.Interrupt);

  cfg.PWM = VTFT_FT812_CONFIG_PWM;
  FT812_PWM_SetConfig(&cfg.PWM);

  cfg.GPIO = VTFT_FT812_CONFIG_GPIO;
  FT812_GPIO_SetConfig(&cfg.GPIO);

  FT812_Touch_Mode(_FT812_TOUCH_SMPL_MODE_CONTINUOUS, _FT812_TOUCH_OP_MODE_COMPATIBILITY);

  FT812_Touch_Calibrate(_FT812_FONT_ROBOTO_SIZE_10, "Touch blinking point on the screen!", Home.Width/2, Home.Height/2);

  // External memory setup
  FT812_Register_GetExtData(VTFT_GetExtData);

  // Draw start screen
  DrawScreen(&Home);
}
