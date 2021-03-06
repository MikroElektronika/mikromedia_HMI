#include "Mikromedia_HMI_35C_Demo_objects.h"
#include "Mikromedia_HMI_35C_Demo_resources.h"
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
  48000000,        // Frequency          = main clock frequency
  0,               // OutRenderMode      = 0 normal, 1 write, 2 read
  0,               // RenderReadScanLine = scanline for read render mode
  0,               // RenderWriteTrigger = trigger for write render mode (read only)
  525,             // hCycle             = number if horizontal cycles for display
  70,              // hOffset            = horizontal offset from starting signal
  320,             // hSize              = width resolution
  0,               // hSync0             = hsync falls
  41,              // hSync1             = hsync rise
  286,             // vCycle             = number of vertical cycles for display
  12,              // vOffset            = vertical offset from start signal
  240,             // vSize              = height resolution
  0,               // vSync0             = vsync falls
  10,              // vSync1             = vsync rise
  0,               // Rotate             = rotate display
  0x01B6,          // OutBits            = output bits resolution
  0,               // OutDither          = output number of bits
  0x0000,          // OutSwizzle         = output swizzle
  0,               // OutCSpread         = output clock spread enable
  1,               // PClockPolarity     = clock polarity: 0 - rising edge, 1 - falling edge
  4,               // PClock             = clock prescaler of FT812: - 0 means disable and >0 means 48MHz/pclock
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
TImage Image44;
TBox   Box35;
TEvent Box35_OnClick;
TImage Image39;
TBox   Box12;
TEvent Box12_OnClick;
TBox   Box16;
TBox   Box15;
TBox   Box14;
TBox   Box21;
TEvent Box21_OnClick;
TBox   Box20;
TEvent Box20_OnClick;
TBox   Box13;
TEvent Box13_OnClick;
TBox   Box7;
TEvent Box7_OnClick;
TBox   Box5;
TEvent Box5_OnClick;
TBox   Box3;
TEvent Box3_OnClick;
TBox   Box31;
TEvent Box31_OnClick;
TImage Image1;
TImage Image17;
TImage Image16;
TImage Image9;
TImage Image8;
TImage Image7;
TImage Image6;
TImage Image14;
TImage Image12;
TImage Image10;
TImage Image3;
TImage Image4;
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
TImage Image27;
TImage Image25;
TImage Image23;
TImage Image21;
TLabel Label1;
char   Label1_Caption[5] = "Mode";
TLabel Label2;
char   Label2_Caption[23] = "HOME";
TLabel Label3;
char   Label3_Caption[5] = "Mode";
TLabel Label4;
char   Label4_Caption[23] = "MEDIA";
TLabel Label5;
char   Label5_Caption[6] = "Night";
TLabel Label7;
char   Label7_Caption[9] = "Security";
TLabel Label8;
char   Label8_Caption[23] = "DISARMED";
TLabel Label9;
char   Label9_Caption[9] = "Entrance";
TLabel Label10;
char   Label10_Caption[7] = "Garden";
TLabel Label11;
char   Label11_Caption[7] = "Garage";
TLabel Label15;
char   Label15_Caption[11] = "System Log";
TLabel Label16;
char   Label16_Caption[34] = "No Events";
TLabel Label17;
char   Label17_Caption[23] = "Power";
TLabel Label20;
char   Label20_Caption[23] = "Time left:";
TLabel Label23;
char   Label23_Caption[23] = "    03:00h";
TBox   Box28;
TEvent Box28_OnClick;
TLabel Label35;
char   Label35_Caption[7] = "Music ";
TLabel Label36;
char   Label36_Caption[7] = "Dining";
TLabel Label38;
char   Label38_Caption[5] = "Sofa";
TLabel Label40;
char   Label40_Caption[8] = "Bedroom";
TLabel Label42;
char   Label42_Caption[4] = "Set";
TLabel Label61;
char   Label61_Caption[6] = "Today";
TLabel Label68;
char   Label68_Caption[16] = "Tomorrow: 10/16";
TLabel Label69;
char   Label69_Caption[2] = "C";
TLabel Label73;
char   Label73_Caption[7] = " 14";
TLabel Label74;
char   Label74_Caption[2] = ":";
TLabel Label75;
char   Label75_Caption[7] = " 30";
TLabel Label76;
char   Label76_Caption[7] = " 25.";
TLabel Label77;
char   Label77_Caption[7] = "   4.";
TLabel Label78;
char   Label78_Caption[11] = "  2016.";
TLabel Label58;
char   Label58_Caption[3] = "14";
TLabel Label60;
char   Label60_Caption[2] = "C";

TLabel *const code Home_Labels[31] = {
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
  &Label15,             
  &Label16,             
  &Label17,             
  &Label20,             
  &Label23,             
  &Label35,             
  &Label36,             
  &Label38,             
  &Label40,             
  &Label42,             
  &Label61,             
  &Label68,             
  &Label69,             
  &Label73,             
  &Label74,             
  &Label75,             
  &Label76,             
  &Label77,             
  &Label78,             
  &Label58,             
  &Label60              
};

TImage *const code Home_Images[36] = {
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
  &Image44,             
  &Image39,             
  &Image1,              
  &Image17,             
  &Image16,             
  &Image9,              
  &Image8,              
  &Image7,              
  &Image6,              
  &Image14,             
  &Image12,             
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
  &Image27,             
  &Image25,             
  &Image23,             
  &Image21              
};

TBox *const code Home_Boxes[16] = {
  &Box34,               
  &Box2,                
  &Box1,                
  &Box35,               
  &Box12,               
  &Box16,               
  &Box15,               
  &Box14,               
  &Box21,               
  &Box20,               
  &Box13,               
  &Box7,                
  &Box5,                
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
char    Label25_Caption[23] = "DISARMED";
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
char    Label85_Caption[8] = "Average";
TLabel  Label90;
char    Label90_Caption[3] = "/h";
TLabel  Label91;
char    Label91_Caption[4] = "kWh";
TLabel  Label92;
char    Label92_Caption[5] = "0,30";
TLabel  Label93;
char    Label93_Caption[5] = "8,03";
TLabel  Label94;
char    Label94_Caption[23] = "    03:00";
TLabel  Label99;
char    Label99_Caption[11] = "    25";
TLabel  Label100;
char    Label100_Caption[11] = "    25";
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
char    Button40_Caption[4] = "Min";
TButton Button41;
TEvent  Button41_OnClick;
char    Button41_Caption[4] = "Day";
TButton Button42;
TEvent  Button42_OnClick;
char    Button42_Caption[4] = "Mon";
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
  Home.Width              = 320;
  Home.Height             = 240;
  Home.ObjectsCount       = 83;
  Home.ButtonsCount       = 0;
  Home.Buttons            = 0;
  Home.LabelsCount        = 31;
  Home.Labels             = Home_Labels;
  Home.ImagesCount        = 36;
  Home.Images             = Home_Images;
  Home.CircleButtonsCount = 0;
  Home.CircleButtons      = 0;
  Home.BoxesCount         = 16;
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
  Temperature_Mode.Width              = 320;
  Temperature_Mode.Height             = 240;
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
  Light_Mode.Width              = 320;
  Light_Mode.Height             = 240;
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
  Keyboard_Screen.Width              = 320;
  Keyboard_Screen.Height             = 240;
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
  System_Log_Screen.Width              = 320;
  System_Log_Screen.Height             = 240;
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
  Green_Power_Screen.Width              = 320;
  Green_Power_Screen.Height             = 240;
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
  Time_Screen.Width              = 320;
  Time_Screen.Height             = 240;
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
  Settings_Screen.Width              = 320;
  Settings_Screen.Height             = 240;
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
  Box34.Left          = 83;
  Box34.Top           = 5;
  Box34.Width         = 155;
  Box34.Height        = 76;
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
  Image75.Left          = 203;
  Image75.Top           = 34;
  Image75.Width         = 42;
  Image75.Height        = 40;
  Image75.Picture_Name  = deg2_bmp;
  Image75.Picture_Type  = 7;
  Image75.Picture_Ratio = 1;
  Image75.Blend_Color   = 0xFFFFFF;
  Image75.Source        = -1L;
  Image75.RotationAngle = 0;
  Image75.RotationCenterLeft = 209;
  Image75.RotationCenterTop = 37;
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
  Image76.Left          = 210;
  Image76.Top           = 58;
  Image76.Width         = 42;
  Image76.Height        = 40;
  Image76.Picture_Name  = deg2_bmp;
  Image76.Picture_Type  = 7;
  Image76.Picture_Ratio = 1;
  Image76.Blend_Color   = 0xFFFFFF;
  Image76.Source        = -1L;
  Image76.RotationAngle = 0;
  Image76.RotationCenterLeft = 213;
  Image76.RotationCenterTop = 61;
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
  Box2.Left          = 240;
  Box2.Top           = 122;
  Box2.Width         = 77;
  Box2.Height        = 76;
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
  Image74.Left          = 262;
  Image74.Top           = 144;
  Image74.Width         = 69;
  Image74.Height        = 64;
  Image74.Picture_Name  = peo4_bmp;
  Image74.Picture_Type  = 7;
  Image74.Picture_Ratio = 1;
  Image74.Blend_Color   = 0xFFFFFF;
  Image74.Source        = -1L;
  Image74.RotationAngle = 0;
  Image74.RotationCenterLeft = 279;
  Image74.RotationCenterTop = 160;
  Image74.ScaleX        = 0.5000;
  Image74.ScaleY        = 0.5000;
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
  Image72.Left          = 263;
  Image72.Top           = 145;
  Image72.Width         = 62;
  Image72.Height        = 59;
  Image72.Picture_Name  = book3_bmp;
  Image72.Picture_Type  = 7;
  Image72.Picture_Ratio = 1;
  Image72.Blend_Color   = 0xFFFFFF;
  Image72.Source        = -1L;
  Image72.RotationAngle = 0;
  Image72.RotationCenterLeft = 279;
  Image72.RotationCenterTop = 160;
  Image72.ScaleX        = 0.5000;
  Image72.ScaleY        = 0.5000;
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
  Image73.Left          = 265;
  Image73.Top           = 144;
  Image73.Width         = 56;
  Image73.Height        = 66;
  Image73.Picture_Name  = dish3_bmp;
  Image73.Picture_Type  = 7;
  Image73.Picture_Ratio = 1;
  Image73.Blend_Color   = 0xFFFFFF;
  Image73.Source        = -1L;
  Image73.RotationAngle = 0;
  Image73.RotationCenterLeft = 280;
  Image73.RotationCenterTop = 160;
  Image73.ScaleX        = 0.5000;
  Image73.ScaleY        = 0.5000;
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
  Image71.Left          = 262;
  Image71.Top           = 146;
  Image71.Width         = 67;
  Image71.Height        = 55;
  Image71.Picture_Name  = media3_bmp;
  Image71.Picture_Type  = 7;
  Image71.Picture_Ratio = 1;
  Image71.Blend_Color   = 0xFFFFFF;
  Image71.Source        = -1L;
  Image71.RotationAngle = 0;
  Image71.RotationCenterLeft = 280;
  Image71.RotationCenterTop = 160;
  Image71.ScaleX        = 0.5000;
  Image71.ScaleY        = 0.5000;
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
  Box1.Left          = 83;
  Box1.Top           = 83;
  Box1.Width         = 117;
  Box1.Height        = 37;
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
  Image43.Left          = 98;
  Image43.Top           = 85;
  Image43.Width         = 69;
  Image43.Height        = 64;
  Image43.Picture_Name  = peo3_bmp;
  Image43.Picture_Type  = 7;
  Image43.Picture_Ratio = 1;
  Image43.Blend_Color   = 0xFFFFFF;
  Image43.Source        = -1L;
  Image43.RotationAngle = 0;
  Image43.RotationCenterLeft = 109;
  Image43.RotationCenterTop = 101;
  Image43.ScaleX        = 0.5000;
  Image43.ScaleY        = 0.5000;
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
  Image42.Left          = 101;
  Image42.Top           = 88;
  Image42.Width         = 58;
  Image42.Height        = 54;
  Image42.Picture_Name  = moon5_bmp;
  Image42.Picture_Type  = 7;
  Image42.Picture_Ratio = 1;
  Image42.Blend_Color   = 0xFFFFFF;
  Image42.Source        = -1L;
  Image42.RotationAngle = 0;
  Image42.RotationCenterLeft = 110;
  Image42.RotationCenterTop = 100;
  Image42.ScaleX        = 0.5000;
  Image42.ScaleY        = 0.5000;
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
  Image40.Left          = 100;
  Image40.Top           = 85;
  Image40.Width         = 63;
  Image40.Height        = 64;
  Image40.Picture_Name  = sun5_bmp;
  Image40.Picture_Type  = 7;
  Image40.Picture_Ratio = 1;
  Image40.Blend_Color   = 0xFFFFFF;
  Image40.Source        = -1L;
  Image40.RotationAngle = 0;
  Image40.RotationCenterLeft = 111;
  Image40.RotationCenterTop = 101;
  Image40.ScaleX        = 0.5000;
  Image40.ScaleY        = 0.5000;
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
  Image5.Left          = 101;
  Image5.Top           = 87;
  Image5.Width         = 59;
  Image5.Height        = 57;
  Image5.Picture_Name  = house3_bmp;
  Image5.Picture_Type  = 7;
  Image5.Picture_Ratio = 1;
  Image5.Blend_Color   = 0xFFFFFF;
  Image5.Source        = -1L;
  Image5.RotationAngle = 0;
  Image5.RotationCenterLeft = 110;
  Image5.RotationCenterTop = 102;
  Image5.ScaleX        = 0.5000;
  Image5.ScaleY        = 0.5000;
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
  Image41.Left          = 203;
  Image41.Top           = 34;
  Image41.Width         = 42;
  Image41.Height        = 40;
  Image41.Picture_Name  = deg_bmp;
  Image41.Picture_Type  = 7;
  Image41.Picture_Ratio = 1;
  Image41.Blend_Color   = 0xFFFFFF;
  Image41.Source        = -1L;
  Image41.RotationAngle = 0;
  Image41.RotationCenterLeft = 209;
  Image41.RotationCenterTop = 37;
  Image41.ScaleX        = 0.1500;
  Image41.ScaleY        = 0.1500;
  Image41.Active        = 0;
  Image41.OnUp          = 0;
  Image41.OnDown        = 0;
  Image41.OnClick       = 0;
  Image41.OnPress       = 0;

  Image44.OwnerScreen   = &Home;
  Image44.Order         = 14;
  Image44.Visible       = 1;
  Image44.Opacity       = 255;
  Image44.Tag           = 255;
  Image44.Left          = 210;
  Image44.Top           = 58;
  Image44.Width         = 42;
  Image44.Height        = 40;
  Image44.Picture_Name  = deg_bmp;
  Image44.Picture_Type  = 7;
  Image44.Picture_Ratio = 1;
  Image44.Blend_Color   = 0xFFFFFF;
  Image44.Source        = -1L;
  Image44.RotationAngle = 0;
  Image44.RotationCenterLeft = 214;
  Image44.RotationCenterTop = 61;
  Image44.ScaleX        = 0.1500;
  Image44.ScaleY        = 0.1500;
  Image44.Active        = 0;
  Image44.OnUp          = 0;
  Image44.OnDown        = 0;
  Image44.OnClick       = 0;
  Image44.OnPress       = 0;

  Box35.OwnerScreen   = &Home;
  Box35.Order         = 15;
  Box35.Visible       = 1;
  Box35.Opacity       = 255;
  Box35.Tag           = 255;
  Box35.Left          = 200;
  Box35.Top           = 83;
  Box35.Width         = 38;
  Box35.Height        = 37;
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
  Image39.Order         = 16;
  Image39.Visible       = 1;
  Image39.Opacity       = 255;
  Image39.Tag           = 255;
  Image39.Left          = 209;
  Image39.Top           = 88;
  Image39.Width         = 47;
  Image39.Height        = 40;
  Image39.Picture_Name  = slide3_bmp;
  Image39.Picture_Type  = 7;
  Image39.Picture_Ratio = 1;
  Image39.Blend_Color   = 0xFFFFFF;
  Image39.Source        = -1L;
  Image39.RotationAngle = 0;
  Image39.RotationCenterLeft = 221;
  Image39.RotationCenterTop = 98;
  Image39.ScaleX        = 0.5000;
  Image39.ScaleY        = 0.5000;
  Image39.Active        = 0;
  Image39.OnUp          = 0;
  Image39.OnDown        = 0;
  Image39.OnClick       = 0;
  Image39.OnPress       = 0;

  Box12.OwnerScreen   = &Home;
  Box12.Order         = 17;
  Box12.Visible       = 1;
  Box12.Opacity       = 255;
  Box12.Tag           = 255;
  Box12.Left          = 240;
  Box12.Top           = 200;
  Box12.Width         = 77;
  Box12.Height        = 37;
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

  Box16.OwnerScreen   = &Home;
  Box16.Order         = 18;
  Box16.Visible       = 1;
  Box16.Opacity       = 255;
  Box16.Tag           = 255;
  Box16.Left          = 162;
  Box16.Top           = 200;
  Box16.Width         = 77;
  Box16.Height        = 37;
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
  Box15.Order         = 19;
  Box15.Visible       = 1;
  Box15.Opacity       = 255;
  Box15.Tag           = 255;
  Box15.Left          = 83;
  Box15.Top           = 200;
  Box15.Width         = 77;
  Box15.Height        = 37;
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
  Box14.Order         = 20;
  Box14.Visible       = 1;
  Box14.Opacity       = 255;
  Box14.Tag           = 255;
  Box14.Left          = 4;
  Box14.Top           = 200;
  Box14.Width         = 77;
  Box14.Height        = 37;
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
  Box21.Order         = 21;
  Box21.Visible       = 1;
  Box21.Opacity       = 255;
  Box21.Tag           = 255;
  Box21.Left          = 162;
  Box21.Top           = 122;
  Box21.Width         = 77;
  Box21.Height        = 76;
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
  Box20.Order         = 22;
  Box20.Visible       = 1;
  Box20.Opacity       = 255;
  Box20.Tag           = 255;
  Box20.Left          = 83;
  Box20.Top           = 122;
  Box20.Width         = 77;
  Box20.Height        = 76;
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
  Box13.Order         = 23;
  Box13.Visible       = 1;
  Box13.Opacity       = 255;
  Box13.Tag           = 255;
  Box13.Left          = 4;
  Box13.Top           = 122;
  Box13.Width         = 77;
  Box13.Height        = 76;
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

  Box7.OwnerScreen   = &Home;
  Box7.Order         = 24;
  Box7.Visible       = 1;
  Box7.Opacity       = 255;
  Box7.Tag           = 255;
  Box7.Left          = 240;
  Box7.Top           = 83;
  Box7.Width         = 77;
  Box7.Height        = 37;
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

  Box5.OwnerScreen   = &Home;
  Box5.Order         = 25;
  Box5.Visible       = 1;
  Box5.Opacity       = 255;
  Box5.Tag           = 255;
  Box5.Left          = 240;
  Box5.Top           = 44;
  Box5.Width         = 77;
  Box5.Height        = 37;
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

  Box3.OwnerScreen   = &Home;
  Box3.Order         = 26;
  Box3.Visible       = 1;
  Box3.Opacity       = 255;
  Box3.Tag           = 255;
  Box3.Left          = 240;
  Box3.Top           = 5;
  Box3.Width         = 77;
  Box3.Height        = 37;
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
  Box31.Order         = 27;
  Box31.Visible       = 1;
  Box31.Opacity       = 255;
  Box31.Tag           = 255;
  Box31.Left          = 4;
  Box31.Top           = 83;
  Box31.Width         = 77;
  Box31.Height        = 37;
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
  Image1.Order         = 28;
  Image1.Visible       = 1;
  Image1.Opacity       = 255;
  Image1.Tag           = 255;
  Image1.Left          = 268;
  Image1.Top           = 204;
  Image1.Width         = 44;
  Image1.Height        = 40;
  Image1.Picture_Name  = moon1_bmp;
  Image1.Picture_Type  = 7;
  Image1.Picture_Ratio = 1;
  Image1.Blend_Color   = 0xFFFFFF;
  Image1.Source        = -1L;
  Image1.RotationAngle = 0;
  Image1.RotationCenterLeft = 279;
  Image1.RotationCenterTop = 227;
  Image1.ScaleX        = 0.5000;
  Image1.ScaleY        = 0.5000;
  Image1.Active        = 0;
  Image1.OnUp          = 0;
  Image1.OnDown        = 0;
  Image1.OnClick       = 0;
  Image1.OnPress       = 0;

  Image17.OwnerScreen   = &Home;
  Image17.Order         = 29;
  Image17.Visible       = 1;
  Image17.Opacity       = 255;
  Image17.Tag           = 255;
  Image17.Left          = 188;
  Image17.Top           = 204;
  Image17.Width         = 48;
  Image17.Height        = 40;
  Image17.Picture_Name  = unlock_bmp;
  Image17.Picture_Type  = 7;
  Image17.Picture_Ratio = 1;
  Image17.Blend_Color   = 0xFFFFFF;
  Image17.Source        = -1L;
  Image17.RotationAngle = 0;
  Image17.RotationCenterLeft = 200;
  Image17.RotationCenterTop = 207;
  Image17.ScaleX        = 0.5000;
  Image17.ScaleY        = 0.5000;
  Image17.Active        = 1;
  Image17.OnUp          = 0;
  Image17.OnDown        = 0;
  Image17.OnClick       = 0;
  Image17.OnPress       = 0;

  Image16.OwnerScreen   = &Home;
  Image16.Order         = 30;
  Image16.Visible       = 1;
  Image16.Opacity       = 255;
  Image16.Tag           = 255;
  Image16.Left          = 109;
  Image16.Top           = 204;
  Image16.Width         = 48;
  Image16.Height        = 40;
  Image16.Picture_Name  = unlock_bmp;
  Image16.Picture_Type  = 7;
  Image16.Picture_Ratio = 1;
  Image16.Blend_Color   = 0xFFFFFF;
  Image16.Source        = -1L;
  Image16.RotationAngle = 0;
  Image16.RotationCenterLeft = 121;
  Image16.RotationCenterTop = 207;
  Image16.ScaleX        = 0.5000;
  Image16.ScaleY        = 0.5000;
  Image16.Active        = 1;
  Image16.OnUp          = 0;
  Image16.OnDown        = 0;
  Image16.OnClick       = 0;
  Image16.OnPress       = 0;

  Image9.OwnerScreen   = &Home;
  Image9.Order         = 31;
  Image9.Visible       = 1;
  Image9.Opacity       = 255;
  Image9.Tag           = 255;
  Image9.Left          = 30;
  Image9.Top           = 204;
  Image9.Width         = 48;
  Image9.Height        = 40;
  Image9.Picture_Name  = unlock_bmp;
  Image9.Picture_Type  = 7;
  Image9.Picture_Ratio = 1;
  Image9.Blend_Color   = 0xFFFFFF;
  Image9.Source        = -1L;
  Image9.RotationAngle = 0;
  Image9.RotationCenterLeft = 42;
  Image9.RotationCenterTop = 207;
  Image9.ScaleX        = 0.5000;
  Image9.ScaleY        = 0.5000;
  Image9.Active        = 1;
  Image9.OnUp          = 0;
  Image9.OnDown        = 0;
  Image9.OnClick       = 0;
  Image9.OnPress       = 0;

  Image8.OwnerScreen   = &Home;
  Image8.Order         = 32;
  Image8.Visible       = 1;
  Image8.Opacity       = 255;
  Image8.Tag           = 255;
  Image8.Left          = 182;
  Image8.Top           = 147;
  Image8.Width         = 54;
  Image8.Height        = 40;
  Image8.Picture_Name  = baterry1_bmp;
  Image8.Picture_Type  = 7;
  Image8.Picture_Ratio = 1;
  Image8.Blend_Color   = 0xFFFFFF;
  Image8.Source        = -1L;
  Image8.RotationAngle = 0;
  Image8.RotationCenterLeft = 200;
  Image8.RotationCenterTop = 160;
  Image8.ScaleX        = 0.6600;
  Image8.ScaleY        = 0.6600;
  Image8.Active        = 0;
  Image8.OnUp          = 0;
  Image8.OnDown        = 0;
  Image8.OnClick       = 0;
  Image8.OnPress       = 0;

  Image7.OwnerScreen   = &Home;
  Image7.Order         = 33;
  Image7.Visible       = 1;
  Image7.Opacity       = 255;
  Image7.Tag           = 255;
  Image7.Left          = 107;
  Image7.Top           = 147;
  Image7.Width         = 42;
  Image7.Height        = 40;
  Image7.Picture_Name  = smile1_bmp;
  Image7.Picture_Type  = 7;
  Image7.Picture_Ratio = 1;
  Image7.Blend_Color   = 0xFFFFFF;
  Image7.Source        = -1L;
  Image7.RotationAngle = 0;
  Image7.RotationCenterLeft = 121;
  Image7.RotationCenterTop = 160;
  Image7.ScaleX        = 0.6600;
  Image7.ScaleY        = 0.6600;
  Image7.Active        = 0;
  Image7.OnUp          = 0;
  Image7.OnDown        = 0;
  Image7.OnClick       = 0;
  Image7.OnPress       = 0;

  Image6.OwnerScreen   = &Home;
  Image6.Order         = 34;
  Image6.Visible       = 1;
  Image6.Opacity       = 255;
  Image6.Tag           = 255;
  Image6.Left          = 28;
  Image6.Top           = 147;
  Image6.Width         = 42;
  Image6.Height        = 40;
  Image6.Picture_Name  = ring_bmp;
  Image6.Picture_Type  = 7;
  Image6.Picture_Ratio = 1;
  Image6.Blend_Color   = 0xFFFFFF;
  Image6.Source        = -1L;
  Image6.RotationAngle = 0;
  Image6.RotationCenterLeft = 42;
  Image6.RotationCenterTop = 160;
  Image6.ScaleX        = 0.6600;
  Image6.ScaleY        = 0.6600;
  Image6.Active        = 0;
  Image6.OnUp          = 0;
  Image6.OnDown        = 0;
  Image6.OnClick       = 0;
  Image6.OnPress       = 0;

  Image14.OwnerScreen   = &Home;
  Image14.Order         = 35;
  Image14.Visible       = 1;
  Image14.Opacity       = 255;
  Image14.Tag           = 255;
  Image14.Left          = 269;
  Image14.Top           = 86;
  Image14.Width         = 41;
  Image14.Height        = 40;
  Image14.Picture_Name  = light1_bmp;
  Image14.Picture_Type  = 7;
  Image14.Picture_Ratio = 1;
  Image14.Blend_Color   = 0xFFFFFF;
  Image14.Source        = -1L;
  Image14.RotationAngle = 0;
  Image14.RotationCenterLeft = 280;
  Image14.RotationCenterTop = 98;
  Image14.ScaleX        = 0.5000;
  Image14.ScaleY        = 0.5000;
  Image14.Active        = 0;
  Image14.OnUp          = 0;
  Image14.OnDown        = 0;
  Image14.OnClick       = 0;
  Image14.OnPress       = 0;

  Image12.OwnerScreen   = &Home;
  Image12.Order         = 36;
  Image12.Visible       = 1;
  Image12.Opacity       = 255;
  Image12.Tag           = 255;
  Image12.Left          = 269;
  Image12.Top           = 47;
  Image12.Width         = 41;
  Image12.Height        = 40;
  Image12.Picture_Name  = light1_bmp;
  Image12.Picture_Type  = 7;
  Image12.Picture_Ratio = 1;
  Image12.Blend_Color   = 0xFFFFFF;
  Image12.Source        = -1L;
  Image12.RotationAngle = 0;
  Image12.RotationCenterLeft = 279;
  Image12.RotationCenterTop = 57;
  Image12.ScaleX        = 0.5000;
  Image12.ScaleY        = 0.5000;
  Image12.Active        = 0;
  Image12.OnUp          = 0;
  Image12.OnDown        = 0;
  Image12.OnClick       = 0;
  Image12.OnPress       = 0;

  Image10.OwnerScreen   = &Home;
  Image10.Order         = 37;
  Image10.Visible       = 1;
  Image10.Opacity       = 255;
  Image10.Tag           = 255;
  Image10.Left          = 269;
  Image10.Top           = 8;
  Image10.Width         = 41;
  Image10.Height        = 40;
  Image10.Picture_Name  = light1_bmp;
  Image10.Picture_Type  = 7;
  Image10.Picture_Ratio = 1;
  Image10.Blend_Color   = 0xFFFFFF;
  Image10.Source        = -1L;
  Image10.RotationAngle = 0;
  Image10.RotationCenterLeft = 280;
  Image10.RotationCenterTop = 18;
  Image10.ScaleX        = 0.5000;
  Image10.ScaleY        = 0.5000;
  Image10.Active        = 0;
  Image10.OnUp          = 0;
  Image10.OnDown        = 0;
  Image10.OnClick       = 0;
  Image10.OnPress       = 0;

  Image3.OwnerScreen   = &Home;
  Image3.Order         = 38;
  Image3.Visible       = 1;
  Image3.Opacity       = 255;
  Image3.Tag           = 255;
  Image3.Left          = 101;
  Image3.Top           = 30;
  Image3.Width         = 43;
  Image3.Height        = 40;
  Image3.Picture_Name  = sun1_bmp;
  Image3.Picture_Type  = 7;
  Image3.Picture_Ratio = 1;
  Image3.Blend_Color   = 0xFFFFFF;
  Image3.Source        = -1L;
  Image3.RotationAngle = 0;
  Image3.RotationCenterLeft = 119;
  Image3.RotationCenterTop = 41;
  Image3.ScaleX        = 0.7700;
  Image3.ScaleY        = 0.7700;
  Image3.Active        = 0;
  Image3.OnUp          = 0;
  Image3.OnDown        = 0;
  Image3.OnClick       = 0;
  Image3.OnPress       = 0;

  Image4.OwnerScreen   = &Home;
  Image4.Order         = 39;
  Image4.Visible       = 1;
  Image4.Opacity       = 255;
  Image4.Tag           = 255;
  Image4.Left          = 30;
  Image4.Top           = 86;
  Image4.Width         = 49;
  Image4.Height        = 40;
  Image4.Picture_Name  = note_bmp;
  Image4.Picture_Type  = 7;
  Image4.Picture_Ratio = 1;
  Image4.Blend_Color   = 0xFFFFFF;
  Image4.Source        = -1L;
  Image4.RotationAngle = 0;
  Image4.RotationCenterLeft = 44;
  Image4.RotationCenterTop = 96;
  Image4.ScaleX        = 0.5000;
  Image4.ScaleY        = 0.5000;
  Image4.Active        = 1;
  Image4.OnUp          = 0;
  Image4.OnDown        = 0;
  Image4.OnClick       = 0;
  Image4.OnPress       = 0;

  Image38.OwnerScreen   = &Home;
  Image38.Order         = 40;
  Image38.Visible       = 0;
  Image38.Opacity       = 255;
  Image38.Tag           = 255;
  Image38.Left          = 101;
  Image38.Top           = 30;
  Image38.Width         = 43;
  Image38.Height        = 40;
  Image38.Picture_Name  = sun2_bmp;
  Image38.Picture_Type  = 7;
  Image38.Picture_Ratio = 1;
  Image38.Blend_Color   = 0xFFFFFF;
  Image38.Source        = -1L;
  Image38.RotationAngle = 0;
  Image38.RotationCenterLeft = 119;
  Image38.RotationCenterTop = 41;
  Image38.ScaleX        = 0.7700;
  Image38.ScaleY        = 0.7700;
  Image38.Active        = 0;
  Image38.OnUp          = 0;
  Image38.OnDown        = 0;
  Image38.OnClick       = 0;
  Image38.OnPress       = 0;

  Image37.OwnerScreen   = &Home;
  Image37.Order         = 41;
  Image37.Visible       = 0;
  Image37.Opacity       = 255;
  Image37.Tag           = 255;
  Image37.Left          = 182;
  Image37.Top           = 147;
  Image37.Width         = 54;
  Image37.Height        = 40;
  Image37.Picture_Name  = baterry2_bmp;
  Image37.Picture_Type  = 7;
  Image37.Picture_Ratio = 1;
  Image37.Blend_Color   = 0xFFFFFF;
  Image37.Source        = -1L;
  Image37.RotationAngle = 0;
  Image37.RotationCenterLeft = 200;
  Image37.RotationCenterTop = 160;
  Image37.ScaleX        = 0.6600;
  Image37.ScaleY        = 0.6600;
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
  Image36.Order         = 42;
  Image36.Visible       = 0;
  Image36.Opacity       = 255;
  Image36.Tag           = 255;
  Image36.Left          = 107;
  Image36.Top           = 147;
  Image36.Width         = 42;
  Image36.Height        = 40;
  Image36.Picture_Name  = smile2_bmp;
  Image36.Picture_Type  = 7;
  Image36.Picture_Ratio = 1;
  Image36.Blend_Color   = 0xFFFFFF;
  Image36.Source        = -1L;
  Image36.RotationAngle = 0;
  Image36.RotationCenterLeft = 121;
  Image36.RotationCenterTop = 160;
  Image36.ScaleX        = 0.6600;
  Image36.ScaleY        = 0.6600;
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
  Image35.Order         = 43;
  Image35.Visible       = 0;
  Image35.Opacity       = 255;
  Image35.Tag           = 255;
  Image35.Left          = 28;
  Image35.Top           = 147;
  Image35.Width         = 42;
  Image35.Height        = 40;
  Image35.Picture_Name  = ring2_bmp;
  Image35.Picture_Type  = 7;
  Image35.Picture_Ratio = 1;
  Image35.Blend_Color   = 0xFFFFFF;
  Image35.Source        = -1L;
  Image35.RotationAngle = 0;
  Image35.RotationCenterLeft = 42;
  Image35.RotationCenterTop = 160;
  Image35.ScaleX        = 0.6600;
  Image35.ScaleY        = 0.6600;
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
  Image29.Order         = 44;
  Image29.Visible       = 0;
  Image29.Opacity       = 255;
  Image29.Tag           = 255;
  Image29.Left          = 32;
  Image29.Top           = 204;
  Image29.Width         = 40;
  Image29.Height        = 40;
  Image29.Picture_Name  = lock_bmp;
  Image29.Picture_Type  = 7;
  Image29.Picture_Ratio = 1;
  Image29.Blend_Color   = 0xFFFFFF;
  Image29.Source        = -1L;
  Image29.RotationAngle = 0;
  Image29.RotationCenterLeft = 42;
  Image29.RotationCenterTop = 207;
  Image29.ScaleX        = 0.5000;
  Image29.ScaleY        = 0.5000;
  Image29.Active        = 1;
  Image29.OnUp          = 0;
  Image29.OnDown        = 0;
  Image29.OnClick       = 0;
  Image29.OnPress       = 0;

  Image30.OwnerScreen   = &Home;
  Image30.Order         = 45;
  Image30.Visible       = 0;
  Image30.Opacity       = 255;
  Image30.Tag           = 255;
  Image30.Left          = 111;
  Image30.Top           = 204;
  Image30.Width         = 40;
  Image30.Height        = 40;
  Image30.Picture_Name  = lock_bmp;
  Image30.Picture_Type  = 7;
  Image30.Picture_Ratio = 1;
  Image30.Blend_Color   = 0xFFFFFF;
  Image30.Source        = -1L;
  Image30.RotationAngle = 0;
  Image30.RotationCenterLeft = 121;
  Image30.RotationCenterTop = 207;
  Image30.ScaleX        = 0.5000;
  Image30.ScaleY        = 0.5000;
  Image30.Active        = 1;
  Image30.OnUp          = 0;
  Image30.OnDown        = 0;
  Image30.OnClick       = 0;
  Image30.OnPress       = 0;

  Image31.OwnerScreen   = &Home;
  Image31.Order         = 46;
  Image31.Visible       = 0;
  Image31.Opacity       = 255;
  Image31.Tag           = 255;
  Image31.Left          = 190;
  Image31.Top           = 204;
  Image31.Width         = 40;
  Image31.Height        = 40;
  Image31.Picture_Name  = lock_bmp;
  Image31.Picture_Type  = 7;
  Image31.Picture_Ratio = 1;
  Image31.Blend_Color   = 0xFFFFFF;
  Image31.Source        = -1L;
  Image31.RotationAngle = 0;
  Image31.RotationCenterLeft = 200;
  Image31.RotationCenterTop = 207;
  Image31.ScaleX        = 0.5000;
  Image31.ScaleY        = 0.5000;
  Image31.Active        = 1;
  Image31.OnUp          = 0;
  Image31.OnDown        = 0;
  Image31.OnClick       = 0;
  Image31.OnPress       = 0;

  Image27.OwnerScreen   = &Home;
  Image27.Order         = 47;
  Image27.Visible       = 0;
  Image27.Opacity       = 255;
  Image27.Tag           = 255;
  Image27.Left          = 268;
  Image27.Top           = 204;
  Image27.Width         = 44;
  Image27.Height        = 40;
  Image27.Picture_Name  = moon2_bmp;
  Image27.Picture_Type  = 7;
  Image27.Picture_Ratio = 1;
  Image27.Blend_Color   = 0xFFFFFF;
  Image27.Source        = -1L;
  Image27.RotationAngle = 0;
  Image27.RotationCenterLeft = 279;
  Image27.RotationCenterTop = 227;
  Image27.ScaleX        = 0.5000;
  Image27.ScaleY        = 0.5000;
  Image27.Active        = 0;
  Image27.OnUp          = 0;
  Image27.OnDown        = 0;
  Image27.OnClick       = 0;
  Image27.OnPress       = 0;

  Image25.OwnerScreen   = &Home;
  Image25.Order         = 48;
  Image25.Visible       = 0;
  Image25.Opacity       = 255;
  Image25.Tag           = 255;
  Image25.Left          = 269;
  Image25.Top           = 86;
  Image25.Width         = 41;
  Image25.Height        = 40;
  Image25.Picture_Name  = light2_bmp;
  Image25.Picture_Type  = 7;
  Image25.Picture_Ratio = 1;
  Image25.Blend_Color   = 0xFFFFFF;
  Image25.Source        = -1L;
  Image25.RotationAngle = 0;
  Image25.RotationCenterLeft = 283;
  Image25.RotationCenterTop = 98;
  Image25.ScaleX        = 0.5000;
  Image25.ScaleY        = 0.5000;
  Image25.Active        = 0;
  Image25.OnUp          = 0;
  Image25.OnDown        = 0;
  Image25.OnClick       = 0;
  Image25.OnPress       = 0;

  Image23.OwnerScreen   = &Home;
  Image23.Order         = 49;
  Image23.Visible       = 0;
  Image23.Opacity       = 255;
  Image23.Tag           = 255;
  Image23.Left          = 269;
  Image23.Top           = 47;
  Image23.Width         = 41;
  Image23.Height        = 40;
  Image23.Picture_Name  = light2_bmp;
  Image23.Picture_Type  = 7;
  Image23.Picture_Ratio = 1;
  Image23.Blend_Color   = 0xFFFFFF;
  Image23.Source        = -1L;
  Image23.RotationAngle = 0;
  Image23.RotationCenterLeft = 284;
  Image23.RotationCenterTop = 56;
  Image23.ScaleX        = 0.5000;
  Image23.ScaleY        = 0.5000;
  Image23.Active        = 0;
  Image23.OnUp          = 0;
  Image23.OnDown        = 0;
  Image23.OnClick       = 0;
  Image23.OnPress       = 0;

  Image21.OwnerScreen   = &Home;
  Image21.Order         = 50;
  Image21.Visible       = 0;
  Image21.Opacity       = 255;
  Image21.Tag           = 255;
  Image21.Left          = 269;
  Image21.Top           = 8;
  Image21.Width         = 41;
  Image21.Height        = 40;
  Image21.Picture_Name  = light2_bmp;
  Image21.Picture_Type  = 7;
  Image21.Picture_Ratio = 1;
  Image21.Blend_Color   = 0xFFFFFF;
  Image21.Source        = -1L;
  Image21.RotationAngle = 0;
  Image21.RotationCenterLeft = 283;
  Image21.RotationCenterTop = 18;
  Image21.ScaleX        = 0.5000;
  Image21.ScaleY        = 0.5000;
  Image21.Active        = 0;
  Image21.OnUp          = 0;
  Image21.OnDown        = 0;
  Image21.OnClick       = 0;
  Image21.OnPress       = 0;

  Label1.OwnerScreen = &Home;
  Label1.Order       = 51;
  Label1.Visible     = 1;
  Label1.Opacity     = 255;
  Label1.Tag         = 255;
  Label1.Left        = 141;
  Label1.Top         = 84;
  Label1.Width       = 29;
  Label1.Height      = 15;
  Label1.Caption     = Label1_Caption;
  Label1.FontName    = 26;
  Label1.Font_Color  = 0xFFFFFF;
  Label1.FontHandle  = 26;
  Label1.Source      = -1UL;
  Label1.Active      = 0;
  Label1.OnUp        = 0;
  Label1.OnDown      = 0;
  Label1.OnClick     = 0;
  Label1.OnPress     = 0;

  Label2.OwnerScreen = &Home;
  Label2.Order       = 52;
  Label2.Visible     = 1;
  Label2.Opacity     = 255;
  Label2.Tag         = 255;
  Label2.Left        = 140;
  Label2.Top         = 103;
  Label2.Width       = 32;
  Label2.Height      = 15;
  Label2.Caption     = Label2_Caption;
  Label2.FontName    = 26;
  Label2.Font_Color  = 0xFFFFFF;
  Label2.FontHandle  = 26;
  Label2.Source      = -1UL;
  Label2.Active      = 0;
  Label2.OnUp        = 0;
  Label2.OnDown      = 0;
  Label2.OnClick     = 0;
  Label2.OnPress     = 0;

  Label3.OwnerScreen = &Home;
  Label3.Order       = 53;
  Label3.Visible     = 1;
  Label3.Opacity     = 255;
  Label3.Tag         = 255;
  Label3.Left        = 262;
  Label3.Top         = 129;
  Label3.Width       = 29;
  Label3.Height      = 15;
  Label3.Caption     = Label3_Caption;
  Label3.FontName    = 26;
  Label3.Font_Color  = 0xFFFFFF;
  Label3.FontHandle  = 26;
  Label3.Source      = -1UL;
  Label3.Active      = 0;
  Label3.OnUp        = 0;
  Label3.OnDown      = 0;
  Label3.OnClick     = 0;
  Label3.OnPress     = 0;

  Label4.OwnerScreen = &Home;
  Label4.Order       = 54;
  Label4.Visible     = 1;
  Label4.Opacity     = 255;
  Label4.Tag         = 255;
  Label4.Left        = 259;
  Label4.Top         = 179;
  Label4.Width       = 34;
  Label4.Height      = 15;
  Label4.Caption     = Label4_Caption;
  Label4.FontName    = 26;
  Label4.Font_Color  = 0xFFFFFF;
  Label4.FontHandle  = 26;
  Label4.Source      = -1UL;
  Label4.Active      = 0;
  Label4.OnUp        = 0;
  Label4.OnDown      = 0;
  Label4.OnClick     = 0;
  Label4.OnPress     = 0;

  Label5.OwnerScreen = &Home;
  Label5.Order       = 55;
  Label5.Visible     = 1;
  Label5.Opacity     = 255;
  Label5.Tag         = 255;
  Label5.Left        = 261;
  Label5.Top         = 220;
  Label5.Width       = 28;
  Label5.Height      = 15;
  Label5.Caption     = Label5_Caption;
  Label5.FontName    = 26;
  Label5.Font_Color  = 0xFFFFFF;
  Label5.FontHandle  = 26;
  Label5.Source      = -1UL;
  Label5.Active      = 0;
  Label5.OnUp        = 0;
  Label5.OnDown      = 0;
  Label5.OnClick     = 0;
  Label5.OnPress     = 0;

  Label7.OwnerScreen = &Home;
  Label7.Order       = 56;
  Label7.Visible     = 1;
  Label7.Opacity     = 255;
  Label7.Tag         = 255;
  Label7.Left        = 15;
  Label7.Top         = 129;
  Label7.Width       = 42;
  Label7.Height      = 15;
  Label7.Caption     = Label7_Caption;
  Label7.FontName    = 26;
  Label7.Font_Color  = 0xFFFFFF;
  Label7.FontHandle  = 26;
  Label7.Source      = -1UL;
  Label7.Active      = 0;
  Label7.OnUp        = 0;
  Label7.OnDown      = 0;
  Label7.OnClick     = 0;
  Label7.OnPress     = 0;

  Label8.OwnerScreen = &Home;
  Label8.Order       = 57;
  Label8.Visible     = 1;
  Label8.Opacity     = 255;
  Label8.Tag         = 255;
  Label8.Left        = 10;
  Label8.Top         = 179;
  Label8.Width       = 56;
  Label8.Height      = 15;
  Label8.Caption     = Label8_Caption;
  Label8.FontName    = 26;
  Label8.Font_Color  = 0xFFFFFF;
  Label8.FontHandle  = 26;
  Label8.Source      = -1UL;
  Label8.Active      = 0;
  Label8.OnUp        = 0;
  Label8.OnDown      = 0;
  Label8.OnClick     = 0;
  Label8.OnPress     = 0;

  Label9.OwnerScreen = &Home;
  Label9.Order       = 58;
  Label9.Visible     = 1;
  Label9.Opacity     = 255;
  Label9.Tag         = 255;
  Label9.Left        = 16;
  Label9.Top         = 220;
  Label9.Width       = 45;
  Label9.Height      = 15;
  Label9.Caption     = Label9_Caption;
  Label9.FontName    = 26;
  Label9.Font_Color  = 0xFFFFFF;
  Label9.FontHandle  = 26;
  Label9.Source      = -1UL;
  Label9.Active      = 1;
  Label9.OnUp        = 0;
  Label9.OnDown      = 0;
  Label9.OnClick     = 0;
  Label9.OnPress     = 0;

  Label10.OwnerScreen = &Home;
  Label10.Order       = 59;
  Label10.Visible     = 1;
  Label10.Opacity     = 255;
  Label10.Tag         = 255;
  Label10.Left        = 99;
  Label10.Top         = 220;
  Label10.Width       = 37;
  Label10.Height      = 15;
  Label10.Caption     = Label10_Caption;
  Label10.FontName    = 26;
  Label10.Font_Color  = 0xFFFFFF;
  Label10.FontHandle  = 26;
  Label10.Source      = -1UL;
  Label10.Active      = 1;
  Label10.OnUp        = 0;
  Label10.OnDown      = 0;
  Label10.OnClick     = 0;
  Label10.OnPress     = 0;

  Label11.OwnerScreen = &Home;
  Label11.Order       = 60;
  Label11.Visible     = 1;
  Label11.Opacity     = 255;
  Label11.Tag         = 255;
  Label11.Left        = 180;
  Label11.Top         = 220;
  Label11.Width       = 36;
  Label11.Height      = 15;
  Label11.Caption     = Label11_Caption;
  Label11.FontName    = 26;
  Label11.Font_Color  = 0xFFFFFF;
  Label11.FontHandle  = 26;
  Label11.Source      = -1UL;
  Label11.Active      = 1;
  Label11.OnUp        = 0;
  Label11.OnDown      = 0;
  Label11.OnClick     = 0;
  Label11.OnPress     = 0;

  Label15.OwnerScreen = &Home;
  Label15.Order       = 61;
  Label15.Visible     = 1;
  Label15.Opacity     = 255;
  Label15.Tag         = 255;
  Label15.Left        = 84;
  Label15.Top         = 129;
  Label15.Width       = 60;
  Label15.Height      = 15;
  Label15.Caption     = Label15_Caption;
  Label15.FontName    = 26;
  Label15.Font_Color  = 0xFFFFFF;
  Label15.FontHandle  = 26;
  Label15.Source      = -1UL;
  Label15.Active      = 0;
  Label15.OnUp        = 0;
  Label15.OnDown      = 0;
  Label15.OnClick     = 0;
  Label15.OnPress     = 0;

  Label16.OwnerScreen = &Home;
  Label16.Order       = 62;
  Label16.Visible     = 1;
  Label16.Opacity     = 255;
  Label16.Tag         = 255;
  Label16.Left        = 92;
  Label16.Top         = 179;
  Label16.Width       = 51;
  Label16.Height      = 15;
  Label16.Caption     = Label16_Caption;
  Label16.FontName    = 26;
  Label16.Font_Color  = 0xFFFFFF;
  Label16.FontHandle  = 26;
  Label16.Source      = -1UL;
  Label16.Active      = 0;
  Label16.OnUp        = 0;
  Label16.OnDown      = 0;
  Label16.OnClick     = 0;
  Label16.OnPress     = 0;

  Label17.OwnerScreen = &Home;
  Label17.Order       = 63;
  Label17.Visible     = 1;
  Label17.Opacity     = 255;
  Label17.Tag         = 255;
  Label17.Left        = 180;
  Label17.Top         = 129;
  Label17.Width       = 32;
  Label17.Height      = 15;
  Label17.Caption     = Label17_Caption;
  Label17.FontName    = 26;
  Label17.Font_Color  = 0xFFFFFF;
  Label17.FontHandle  = 26;
  Label17.Source      = -1UL;
  Label17.Active      = 0;
  Label17.OnUp        = 0;
  Label17.OnDown      = 0;
  Label17.OnClick     = 0;
  Label17.OnPress     = 0;

  Label20.OwnerScreen = &Home;
  Label20.Order       = 64;
  Label20.Visible     = 1;
  Label20.Opacity     = 255;
  Label20.Tag         = 255;
  Label20.Left        = 170;
  Label20.Top         = 170;
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

  Label23.OwnerScreen = &Home;
  Label23.Order       = 65;
  Label23.Visible     = 1;
  Label23.Opacity     = 255;
  Label23.Tag         = 255;
  Label23.Left        = 167;
  Label23.Top         = 183;
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
  Box28.Order         = 66;
  Box28.Visible       = 1;
  Box28.Opacity       = 255;
  Box28.Tag           = 255;
  Box28.Left          = 4;
  Box28.Top           = 5;
  Box28.Width         = 77;
  Box28.Height        = 76;
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
  Label35.Order       = 67;
  Label35.Visible     = 1;
  Label35.Opacity     = 255;
  Label35.Tag         = 255;
  Label35.Left        = 24;
  Label35.Top         = 103;
  Label35.Width       = 31;
  Label35.Height      = 15;
  Label35.Caption     = Label35_Caption;
  Label35.FontName    = 26;
  Label35.Font_Color  = 0xFFFFFF;
  Label35.FontHandle  = 26;
  Label35.Source      = -1UL;
  Label35.Active      = 0;
  Label35.OnUp        = 0;
  Label35.OnDown      = 0;
  Label35.OnClick     = 0;
  Label35.OnPress     = 0;

  Label36.OwnerScreen = &Home;
  Label36.Order       = 68;
  Label36.Visible     = 1;
  Label36.Opacity     = 255;
  Label36.Tag         = 255;
  Label36.Left        = 259;
  Label36.Top         = 25;
  Label36.Width       = 33;
  Label36.Height      = 15;
  Label36.Caption     = Label36_Caption;
  Label36.FontName    = 26;
  Label36.Font_Color  = 0xFFFFFF;
  Label36.FontHandle  = 26;
  Label36.Source      = -1UL;
  Label36.Active      = 0;
  Label36.OnUp        = 0;
  Label36.OnDown      = 0;
  Label36.OnClick     = 0;
  Label36.OnPress     = 0;

  Label38.OwnerScreen = &Home;
  Label38.Order       = 69;
  Label38.Visible     = 1;
  Label38.Opacity     = 255;
  Label38.Tag         = 255;
  Label38.Left        = 263;
  Label38.Top         = 64;
  Label38.Width       = 23;
  Label38.Height      = 15;
  Label38.Caption     = Label38_Caption;
  Label38.FontName    = 26;
  Label38.Font_Color  = 0xFFFFFF;
  Label38.FontHandle  = 26;
  Label38.Source      = -1UL;
  Label38.Active      = 0;
  Label38.OnUp        = 0;
  Label38.OnDown      = 0;
  Label38.OnClick     = 0;
  Label38.OnPress     = 0;

  Label40.OwnerScreen = &Home;
  Label40.Order       = 70;
  Label40.Visible     = 1;
  Label40.Opacity     = 255;
  Label40.Tag         = 255;
  Label40.Left        = 250;
  Label40.Top         = 103;
  Label40.Width       = 46;
  Label40.Height      = 15;
  Label40.Caption     = Label40_Caption;
  Label40.FontName    = 26;
  Label40.Font_Color  = 0xFFFFFF;
  Label40.FontHandle  = 26;
  Label40.Source      = -1UL;
  Label40.Active      = 0;
  Label40.OnUp        = 0;
  Label40.OnDown      = 0;
  Label40.OnClick     = 0;
  Label40.OnPress     = 0;

  Label42.OwnerScreen = &Home;
  Label42.Order       = 71;
  Label42.Visible     = 1;
  Label42.Opacity     = 255;
  Label42.Tag         = 255;
  Label42.Left        = 209;
  Label42.Top         = 103;
  Label42.Width       = 17;
  Label42.Height      = 15;
  Label42.Caption     = Label42_Caption;
  Label42.FontName    = 26;
  Label42.Font_Color  = 0xFFFFFF;
  Label42.FontHandle  = 26;
  Label42.Source      = -1UL;
  Label42.Active      = 0;
  Label42.OnUp        = 0;
  Label42.OnDown      = 0;
  Label42.OnClick     = 0;
  Label42.OnPress     = 0;

  Label61.OwnerScreen = &Home;
  Label61.Order       = 72;
  Label61.Visible     = 1;
  Label61.Opacity     = 255;
  Label61.Tag         = 255;
  Label61.Left        = 132;
  Label61.Top         = 6;
  Label61.Width       = 46;
  Label61.Height      = 21;
  Label61.Caption     = Label61_Caption;
  Label61.FontName    = 28;
  Label61.Font_Color  = 0xFFFFFF;
  Label61.FontHandle  = 28;
  Label61.Source      = -1UL;
  Label61.Active      = 1;
  Label61.OnUp        = 0;
  Label61.OnDown      = 0;
  Label61.OnClick     = 0;
  Label61.OnPress     = 0;

  Label68.OwnerScreen = &Home;
  Label68.Order       = 73;
  Label68.Visible     = 1;
  Label68.Opacity     = 255;
  Label68.Tag         = 255;
  Label68.Left        = 101;
  Label68.Top         = 61;
  Label68.Width       = 88;
  Label68.Height      = 15;
  Label68.Caption     = Label68_Caption;
  Label68.FontName    = 26;
  Label68.Font_Color  = 0xFFFFFF;
  Label68.FontHandle  = 26;
  Label68.Source      = -1UL;
  Label68.Active      = 0;
  Label68.OnUp        = 0;
  Label68.OnDown      = 0;
  Label68.OnClick     = 0;
  Label68.OnPress     = 0;

  Label69.OwnerScreen = &Home;
  Label69.Order       = 74;
  Label69.Visible     = 1;
  Label69.Opacity     = 255;
  Label69.Tag         = 255;
  Label69.Left        = 216;
  Label69.Top         = 61;
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

  Label73.OwnerScreen = &Home;
  Label73.Order       = 75;
  Label73.Visible     = 1;
  Label73.Opacity     = 255;
  Label73.Tag         = 255;
  Label73.Left        = 18;
  Label73.Top         = 49;
  Label73.Width       = 16;
  Label73.Height      = 15;
  Label73.Caption     = Label73_Caption;
  Label73.FontName    = 26;
  Label73.Font_Color  = 0xFFFFFF;
  Label73.FontHandle  = 26;
  Label73.Source      = -1UL;
  Label73.Active      = 1;
  Label73.OnUp        = 0;
  Label73.OnDown      = 0;
  Label73.OnClick     = 0;
  Label73.OnPress     = 0;

  Label74.OwnerScreen = &Home;
  Label74.Order       = 76;
  Label74.Visible     = 1;
  Label74.Opacity     = 255;
  Label74.Tag         = 255;
  Label74.Left        = 40;
  Label74.Top         = 49;
  Label74.Width       = 3;
  Label74.Height      = 15;
  Label74.Caption     = Label74_Caption;
  Label74.FontName    = 26;
  Label74.Font_Color  = 0xFFFFFF;
  Label74.FontHandle  = 26;
  Label74.Source      = -1UL;
  Label74.Active      = 1;
  Label74.OnUp        = 0;
  Label74.OnDown      = 0;
  Label74.OnClick     = 0;
  Label74.OnPress     = 0;

  Label75.OwnerScreen = &Home;
  Label75.Order       = 77;
  Label75.Visible     = 1;
  Label75.Opacity     = 255;
  Label75.Tag         = 255;
  Label75.Left        = 44;
  Label75.Top         = 49;
  Label75.Width       = 16;
  Label75.Height      = 15;
  Label75.Caption     = Label75_Caption;
  Label75.FontName    = 26;
  Label75.Font_Color  = 0xFFFFFF;
  Label75.FontHandle  = 26;
  Label75.Source      = -1UL;
  Label75.Active      = 1;
  Label75.OnUp        = 0;
  Label75.OnDown      = 0;
  Label75.OnClick     = 0;
  Label75.OnPress     = 0;

  Label76.OwnerScreen = &Home;
  Label76.Order       = 78;
  Label76.Visible     = 1;
  Label76.Opacity     = 255;
  Label76.Tag         = 255;
  Label76.Left        = 4;
  Label76.Top         = 20;
  Label76.Width       = 19;
  Label76.Height      = 15;
  Label76.Caption     = Label76_Caption;
  Label76.FontName    = 26;
  Label76.Font_Color  = 0xFFFFFF;
  Label76.FontHandle  = 26;
  Label76.Source      = -1UL;
  Label76.Active      = 1;
  Label76.OnUp        = 0;
  Label76.OnDown      = 0;
  Label76.OnClick     = 0;
  Label76.OnPress     = 0;

  Label77.OwnerScreen = &Home;
  Label77.Order       = 79;
  Label77.Visible     = 1;
  Label77.Opacity     = 255;
  Label77.Tag         = 255;
  Label77.Left        = 19;
  Label77.Top         = 20;
  Label77.Width       = 18;
  Label77.Height      = 15;
  Label77.Caption     = Label77_Caption;
  Label77.FontName    = 26;
  Label77.Font_Color  = 0xFFFFFF;
  Label77.FontHandle  = 26;
  Label77.Source      = -1UL;
  Label77.Active      = 1;
  Label77.OnUp        = 0;
  Label77.OnDown      = 0;
  Label77.OnClick     = 0;
  Label77.OnPress     = 0;

  Label78.OwnerScreen = &Home;
  Label78.Order       = 80;
  Label78.Visible     = 1;
  Label78.Opacity     = 255;
  Label78.Tag         = 255;
  Label78.Left        = 37;
  Label78.Top         = 20;
  Label78.Width       = 34;
  Label78.Height      = 15;
  Label78.Caption     = Label78_Caption;
  Label78.FontName    = 26;
  Label78.Font_Color  = 0xFFFFFF;
  Label78.FontHandle  = 26;
  Label78.Source      = -1UL;
  Label78.Active      = 1;
  Label78.OnUp        = 0;
  Label78.OnDown      = 0;
  Label78.OnClick     = 0;
  Label78.OnPress     = 0;

  Label58.OwnerScreen = &Home;
  Label58.Order       = 81;
  Label58.Visible     = 1;
  Label58.Opacity     = 255;
  Label58.Tag         = 255;
  Label58.Left        = 180;
  Label58.Top         = 35;
  Label58.Width       = 19;
  Label58.Height      = 21;
  Label58.Caption     = Label58_Caption;
  Label58.FontName    = 28;
  Label58.Font_Color  = 0xFFFFFF;
  Label58.FontHandle  = 28;
  Label58.Source      = -1UL;
  Label58.Active      = 0;
  Label58.OnUp        = 0;
  Label58.OnDown      = 0;
  Label58.OnClick     = 0;
  Label58.OnPress     = 0;

  Label60.OwnerScreen = &Home;
  Label60.Order       = 82;
  Label60.Visible     = 1;
  Label60.Opacity     = 255;
  Label60.Tag         = 255;
  Label60.Left        = 209;
  Label60.Top         = 37;
  Label60.Width       = 8;
  Label60.Height      = 17;
  Label60.Caption     = Label60_Caption;
  Label60.FontName    = 27;
  Label60.Font_Color  = 0xFFFFFF;
  Label60.FontHandle  = 27;
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
  Box45.Left          = 139;
  Box45.Top           = 46;
  Box45.Width         = 178;
  Box45.Height        = 40;
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
  Image60.Left          = 289;
  Image60.Top           = 56;
  Image60.Width         = 50;
  Image60.Height        = 50;
  Image60.Picture_Name  = circle2_bmp;
  Image60.Picture_Type  = 7;
  Image60.Picture_Ratio = 1;
  Image60.Blend_Color   = 0xFFFFFF;
  Image60.Source        = -1L;
  Image60.RotationAngle = 0;
  Image60.RotationCenterLeft = 293;
  Image60.RotationCenterTop = 59;
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
  Box40.Left          = 4;
  Box40.Top           = 171;
  Box40.Width         = 133;
  Box40.Height        = 40;
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
  Box39.Left          = 4;
  Box39.Top           = 129;
  Box39.Width         = 133;
  Box39.Height        = 40;
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
  Box38.Left          = 4;
  Box38.Top           = 88;
  Box38.Width         = 133;
  Box38.Height        = 40;
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
  Box37.Left          = 4;
  Box37.Top           = 46;
  Box37.Width         = 133;
  Box37.Height        = 40;
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
  Image51.Left          = 18;
  Image51.Top           = 177;
  Image51.Width         = 69;
  Image51.Height        = 64;
  Image51.Picture_Name  = peo2_bmp;
  Image51.Picture_Type  = 7;
  Image51.Picture_Ratio = 1;
  Image51.Blend_Color   = 0xFFFFFF;
  Image51.Source        = -1L;
  Image51.RotationAngle = 0;
  Image51.RotationCenterLeft = 31;
  Image51.RotationCenterTop = 190;
  Image51.ScaleX        = 0.4000;
  Image51.ScaleY        = 0.4000;
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
  Image50.Left          = 18;
  Image50.Top           = 177;
  Image50.Width         = 69;
  Image50.Height        = 64;
  Image50.Picture_Name  = peo1_bmp;
  Image50.Picture_Type  = 7;
  Image50.Picture_Ratio = 1;
  Image50.Blend_Color   = 0xFFFFFF;
  Image50.Source        = -1L;
  Image50.RotationAngle = 0;
  Image50.RotationCenterLeft = 31;
  Image50.RotationCenterTop = 190;
  Image50.ScaleX        = 0.4000;
  Image50.ScaleY        = 0.4000;
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
  Image49.Left          = 19;
  Image49.Top           = 132;
  Image49.Width         = 63;
  Image49.Height        = 64;
  Image49.Picture_Name  = sun4_bmp;
  Image49.Picture_Type  = 7;
  Image49.Picture_Ratio = 1;
  Image49.Blend_Color   = 0xFFFFFF;
  Image49.Source        = -1L;
  Image49.RotationAngle = 0;
  Image49.RotationCenterLeft = 33;
  Image49.RotationCenterTop = 146;
  Image49.ScaleX        = 0.4400;
  Image49.ScaleY        = 0.4400;
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
  Image48.Left          = 19;
  Image48.Top           = 132;
  Image48.Width         = 63;
  Image48.Height        = 64;
  Image48.Picture_Name  = sun3_bmp;
  Image48.Picture_Type  = 7;
  Image48.Picture_Ratio = 1;
  Image48.Blend_Color   = 0xFFFFFF;
  Image48.Source        = -1L;
  Image48.RotationAngle = 0;
  Image48.RotationCenterLeft = 33;
  Image48.RotationCenterTop = 145;
  Image48.ScaleX        = 0.4400;
  Image48.ScaleY        = 0.4400;
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
  Image47.Left          = 20;
  Image47.Top           = 94;
  Image47.Width         = 58;
  Image47.Height        = 54;
  Image47.Picture_Name  = moon4_bmp;
  Image47.Picture_Type  = 7;
  Image47.Picture_Ratio = 1;
  Image47.Blend_Color   = 0xFFFFFF;
  Image47.Source        = -1L;
  Image47.RotationAngle = 0;
  Image47.RotationCenterLeft = 33;
  Image47.RotationCenterTop = 106;
  Image47.ScaleX        = 0.4400;
  Image47.ScaleY        = 0.4400;
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
  Image46.Left          = 20;
  Image46.Top           = 94;
  Image46.Width         = 58;
  Image46.Height        = 54;
  Image46.Picture_Name  = moon3_bmp;
  Image46.Picture_Type  = 7;
  Image46.Picture_Ratio = 1;
  Image46.Blend_Color   = 0xFFFFFF;
  Image46.Source        = -1L;
  Image46.RotationAngle = 0;
  Image46.RotationCenterLeft = 33;
  Image46.RotationCenterTop = 106;
  Image46.ScaleX        = 0.4400;
  Image46.ScaleY        = 0.4400;
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
  Image28.Left          = 20;
  Image28.Top           = 56;
  Image28.Width         = 59;
  Image28.Height        = 57;
  Image28.Picture_Name  = house2_bmp;
  Image28.Picture_Type  = 7;
  Image28.Picture_Ratio = 1;
  Image28.Blend_Color   = 0xFFFFFF;
  Image28.Source        = -1L;
  Image28.RotationAngle = 0;
  Image28.RotationCenterLeft = 34;
  Image28.RotationCenterTop = 68;
  Image28.ScaleX        = 0.4400;
  Image28.ScaleY        = 0.4400;
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
  Image2.Left          = 20;
  Image2.Top           = 56;
  Image2.Width         = 59;
  Image2.Height        = 57;
  Image2.Picture_Name  = house1_bmp;
  Image2.Picture_Type  = 7;
  Image2.Picture_Ratio = 1;
  Image2.Blend_Color   = 0xFFFFFF;
  Image2.Source        = -1L;
  Image2.RotationAngle = 0;
  Image2.RotationCenterLeft = 34;
  Image2.RotationCenterTop = 68;
  Image2.ScaleX        = 0.4400;
  Image2.ScaleY        = 0.4400;
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
  Button30.Left          = 4;
  Button30.Top           = 212;
  Button30.Width         = 25;
  Button30.Height        = 25;
  Button30.Pen_Width     = 1;
  Button30.Pen_Color     = 0x0000;
  Button30.Color         = 0x808080;
  Button30.Press_Color   = 0xFFFFFF;
  Button30.ColorTo       = 0xFFFFFF;
  Button30.Press_ColorTo = 0xC0C0C0;
  Button30.Gradient      = _FT812_BRUSH_GR_NONE;
  Button30.Caption       = Button30_Caption;
  Button30.TextAlign     = taCenter;
  Button30.FontName      = 26;
  Button30.Font_Color    = 0xFFFFFF;
  Button30.FontHandle    = 26;
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
  Box22.Left          = 31;
  Box22.Top           = 212;
  Box22.Width         = 286;
  Box22.Height        = 25;
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
  Button47.Left          = 4;
  Button47.Top           = 5;
  Button47.Width         = 313;
  Button47.Height        = 40;
  Button47.Pen_Width     = 1;
  Button47.Pen_Color     = 0x0000;
  Button47.Color         = 0x00FF;
  Button47.Press_Color   = 0xFFFFFF;
  Button47.ColorTo       = 0xFFFFFF;
  Button47.Press_ColorTo = 0xC0C0C0;
  Button47.Gradient      = _FT812_BRUSH_GR_NONE;
  Button47.Caption       = Button47_Caption;
  Button47.TextAlign     = taCenter;
  Button47.FontName      = 28;
  Button47.Font_Color    = 0xFFFFFF;
  Button47.FontHandle    = 28;
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
  Box41.Left          = 139;
  Box41.Top           = 88;
  Box41.Width         = 40;
  Box41.Height        = 40;
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
  Box42.Left          = 277;
  Box42.Top           = 88;
  Box42.Width         = 40;
  Box42.Height        = 40;
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
  Box43.Left          = 139;
  Box43.Top           = 171;
  Box43.Width         = 40;
  Box43.Height        = 40;
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
  Box44.Left          = 277;
  Box44.Top           = 171;
  Box44.Width         = 40;
  Box44.Height        = 40;
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
  Box46.Left          = 139;
  Box46.Top           = 129;
  Box46.Width         = 178;
  Box46.Height        = 40;
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
  Box47.Left          = 181;
  Box47.Top           = 88;
  Box47.Width         = 94;
  Box47.Height        = 40;
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
  Box48.Left          = 181;
  Box48.Top           = 171;
  Box48.Width         = 94;
  Box48.Height        = 40;
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
  CircleButton1.Left          = 143;
  CircleButton1.Top           = 92;
  CircleButton1.Radius        = 16;
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
  CircleButton2.Left          = 143;
  CircleButton2.Top           = 175;
  CircleButton2.Radius        = 16;
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
  CircleButton3.Left          = 281;
  CircleButton3.Top           = 92;
  CircleButton3.Radius        = 16;
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
  CircleButton4.Left          = 281;
  CircleButton4.Top           = 175;
  CircleButton4.Radius        = 16;
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
  Box23.Left          = 181;
  Box23.Top           = 88;
  Box23.Width         = 13;
  Box23.Height        = 40;
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
  Box49.Left          = 181;
  Box49.Top           = 171;
  Box49.Width         = 13;
  Box49.Height        = 40;
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
  Label29.Left        = 144;
  Label29.Top         = 58;
  Label29.Width       = 81;
  Label29.Height      = 17;
  Label29.Caption     = Label29_Caption;
  Label29.FontName    = 27;
  Label29.Font_Color  = 0xFFFFFF;
  Label29.FontHandle  = 27;
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
  Label31.Left        = 144;
  Label31.Top         = 141;
  Label31.Width       = 58;
  Label31.Height      = 17;
  Label31.Caption     = Label31_Caption;
  Label31.FontName    = 27;
  Label31.Font_Color  = 0xFFFFFF;
  Label31.FontHandle  = 27;
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
  Label32.Left        = 68;
  Label32.Top         = 58;
  Label32.Width       = 38;
  Label32.Height      = 17;
  Label32.Caption     = Label32_Caption;
  Label32.FontName    = 27;
  Label32.Font_Color  = 0xFFFF00;
  Label32.FontHandle  = 27;
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
  Label33.Left        = 67;
  Label33.Top         = 99;
  Label33.Width       = 40;
  Label33.Height      = 17;
  Label33.Caption     = Label33_Caption;
  Label33.FontName    = 27;
  Label33.Font_Color  = 0xFFFFFF;
  Label33.FontHandle  = 27;
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
  Label46.Left        = 73;
  Label46.Top         = 141;
  Label46.Width       = 26;
  Label46.Height      = 17;
  Label46.Caption     = Label46_Caption;
  Label46.FontName    = 27;
  Label46.Font_Color  = 0xFFFFFF;
  Label46.FontHandle  = 27;
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
  Label47.Left        = 65;
  Label47.Top         = 182;
  Label47.Width       = 42;
  Label47.Height      = 17;
  Label47.Caption     = Label47_Caption;
  Label47.FontName    = 27;
  Label47.Font_Color  = 0xFFFFFF;
  Label47.FontHandle  = 27;
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
  Label86.Left        = 300;
  Label86.Top         = 56;
  Label86.Width       = 10;
  Label86.Height      = 21;
  Label86.Caption     = Label86_Caption;
  Label86.FontName    = 28;
  Label86.Font_Color  = 0xFFFFFF;
  Label86.FontHandle  = 28;
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
  Label87.Left        = 298;
  Label87.Top         = 139;
  Label87.Width       = 12;
  Label87.Height      = 21;
  Label87.Caption     = Label87_Caption;
  Label87.FontName    = 28;
  Label87.Font_Color  = 0xFFFFFF;
  Label87.FontHandle  = 28;
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
  Label95.Left        = 240;
  Label95.Top         = 56;
  Label95.Width       = 35;
  Label95.Height      = 21;
  Label95.Caption     = Label95_Caption;
  Label95.FontName    = 28;
  Label95.Font_Color  = 0xFFFFFF;
  Label95.FontHandle  = 28;
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
  Label96.Left        = 240;
  Label96.Top         = 139;
  Label96.Width       = 35;
  Label96.Height      = 21;
  Label96.Caption     = Label96_Caption;
  Label96.FontName    = 28;
  Label96.Font_Color  = 0xFFFFFF;
  Label96.FontHandle  = 28;
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
  Box61.Left          = 139;
  Box61.Top           = 129;
  Box61.Width         = 178;
  Box61.Height        = 40;
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
  Image61.Left          = 289;
  Image61.Top           = 138;
  Image61.Width         = 50;
  Image61.Height        = 50;
  Image61.Picture_Name  = circle2_bmp;
  Image61.Picture_Type  = 7;
  Image61.Picture_Ratio = 1;
  Image61.Blend_Color   = 0xFFFFFF;
  Image61.Source        = -1L;
  Image61.RotationAngle = 0;
  Image61.RotationCenterLeft = 293;
  Image61.RotationCenterTop = 142;
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
  Box55.Left          = 4;
  Box55.Top           = 171;
  Box55.Width         = 133;
  Box55.Height        = 40;
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
  Box53.Left          = 4;
  Box53.Top           = 129;
  Box53.Width         = 133;
  Box53.Height        = 40;
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
  Box52.Left          = 4;
  Box52.Top           = 88;
  Box52.Width         = 133;
  Box52.Height        = 40;
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
  Box51.Left          = 4;
  Box51.Top           = 46;
  Box51.Width         = 133;
  Box51.Height        = 40;
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
  Image59.Left          = 18;
  Image59.Top           = 178;
  Image59.Width         = 69;
  Image59.Height        = 64;
  Image59.Picture_Name  = peo2_bmp;
  Image59.Picture_Type  = 7;
  Image59.Picture_Ratio = 1;
  Image59.Blend_Color   = 0xFFFFFF;
  Image59.Source        = -1L;
  Image59.RotationAngle = 0;
  Image59.RotationCenterLeft = 28;
  Image59.RotationCenterTop = 191;
  Image59.ScaleX        = 0.4000;
  Image59.ScaleY        = 0.4000;
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
  Image58.Left          = 18;
  Image58.Top           = 178;
  Image58.Width         = 69;
  Image58.Height        = 64;
  Image58.Picture_Name  = peo1_bmp;
  Image58.Picture_Type  = 7;
  Image58.Picture_Ratio = 1;
  Image58.Blend_Color   = 0xFFFFFF;
  Image58.Source        = -1L;
  Image58.RotationAngle = 0;
  Image58.RotationCenterLeft = 28;
  Image58.RotationCenterTop = 191;
  Image58.ScaleX        = 0.4000;
  Image58.ScaleY        = 0.4000;
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
  Image57.Left          = 20;
  Image57.Top           = 133;
  Image57.Width         = 56;
  Image57.Height        = 66;
  Image57.Picture_Name  = dish2_bmp;
  Image57.Picture_Type  = 7;
  Image57.Picture_Ratio = 1;
  Image57.Blend_Color   = 0xFFFFFF;
  Image57.Source        = -1L;
  Image57.RotationAngle = 0;
  Image57.RotationCenterLeft = 34;
  Image57.RotationCenterTop = 97;
  Image57.ScaleX        = 0.4400;
  Image57.ScaleY        = 0.4400;
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
  Image56.Left          = 20;
  Image56.Top           = 133;
  Image56.Width         = 56;
  Image56.Height        = 66;
  Image56.Picture_Name  = dish1_bmp;
  Image56.Picture_Type  = 7;
  Image56.Picture_Ratio = 1;
  Image56.Blend_Color   = 0xFFFFFF;
  Image56.Source        = -1L;
  Image56.RotationAngle = 0;
  Image56.RotationCenterLeft = 34;
  Image56.RotationCenterTop = 148;
  Image56.ScaleX        = 0.4400;
  Image56.ScaleY        = 0.4400;
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
  Image55.Left          = 18;
  Image55.Top           = 96;
  Image55.Width         = 62;
  Image55.Height        = 59;
  Image55.Picture_Name  = book2_bmp;
  Image55.Picture_Type  = 7;
  Image55.Picture_Ratio = 1;
  Image55.Blend_Color   = 0xFFFFFF;
  Image55.Source        = -1L;
  Image55.RotationAngle = 0;
  Image55.RotationCenterLeft = 28;
  Image55.RotationCenterTop = 110;
  Image55.ScaleX        = 0.4400;
  Image55.ScaleY        = 0.4400;
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
  Image54.Left          = 18;
  Image54.Top           = 96;
  Image54.Width         = 62;
  Image54.Height        = 59;
  Image54.Picture_Name  = book1_bmp;
  Image54.Picture_Type  = 7;
  Image54.Picture_Ratio = 1;
  Image54.Blend_Color   = 0xFFFFFF;
  Image54.Source        = -1L;
  Image54.RotationAngle = 0;
  Image54.RotationCenterLeft = 28;
  Image54.RotationCenterTop = 110;
  Image54.ScaleX        = 0.4400;
  Image54.ScaleY        = 0.4400;
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
  Image53.Left          = 17;
  Image53.Top           = 54;
  Image53.Width         = 67;
  Image53.Height        = 55;
  Image53.Picture_Name  = media2_bmp;
  Image53.Picture_Type  = 7;
  Image53.Picture_Ratio = 1;
  Image53.Blend_Color   = 0xFFFFFF;
  Image53.Source        = -1L;
  Image53.RotationAngle = 0;
  Image53.RotationCenterLeft = 28;
  Image53.RotationCenterTop = 66;
  Image53.ScaleX        = 0.4400;
  Image53.ScaleY        = 0.4400;
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
  Image52.Left          = 17;
  Image52.Top           = 54;
  Image52.Width         = 67;
  Image52.Height        = 55;
  Image52.Picture_Name  = media1_bmp;
  Image52.Picture_Type  = 7;
  Image52.Picture_Ratio = 1;
  Image52.Blend_Color   = 0xFFFFFF;
  Image52.Source        = -1L;
  Image52.RotationAngle = 0;
  Image52.RotationCenterLeft = 28;
  Image52.RotationCenterTop = 67;
  Image52.ScaleX        = 0.4400;
  Image52.ScaleY        = 0.4400;
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
  Button31.Left          = 4;
  Button31.Top           = 212;
  Button31.Width         = 25;
  Button31.Height        = 25;
  Button31.Pen_Width     = 1;
  Button31.Pen_Color     = 0x0000;
  Button31.Color         = 0x808080;
  Button31.Press_Color   = 0xFFFFFF;
  Button31.ColorTo       = 0xFFFFFF;
  Button31.Press_ColorTo = 0xC0C0C0;
  Button31.Gradient      = _FT812_BRUSH_GR_NONE;
  Button31.Caption       = Button31_Caption;
  Button31.TextAlign     = taCenter;
  Button31.FontName      = 28;
  Button31.Font_Color    = 0xFFFFFF;
  Button31.FontHandle    = 28;
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
  Box50.Left          = 31;
  Box50.Top           = 212;
  Box50.Width         = 286;
  Box50.Height        = 25;
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
  Button32.Left          = 4;
  Button32.Top           = 5;
  Button32.Width         = 313;
  Button32.Height        = 40;
  Button32.Pen_Width     = 1;
  Button32.Pen_Color     = 0x0000;
  Button32.Color         = 0x808080;
  Button32.Press_Color   = 0xFFFFFF;
  Button32.ColorTo       = 0xFFFFFF;
  Button32.Press_ColorTo = 0xC0C0C0;
  Button32.Gradient      = _FT812_BRUSH_GR_NONE;
  Button32.Caption       = Button32_Caption;
  Button32.TextAlign     = taCenter;
  Button32.FontName      = 28;
  Button32.Font_Color    = 0xFFFFFF;
  Button32.FontHandle    = 28;
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
  Box56.Left          = 139;
  Box56.Top           = 88;
  Box56.Width         = 40;
  Box56.Height        = 40;
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
  Box57.Left          = 277;
  Box57.Top           = 88;
  Box57.Width         = 40;
  Box57.Height        = 40;
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
  Box58.Left          = 139;
  Box58.Top           = 171;
  Box58.Width         = 40;
  Box58.Height        = 40;
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
  Box59.Left          = 277;
  Box59.Top           = 171;
  Box59.Width         = 40;
  Box59.Height        = 40;
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
  Box60.Left          = 139;
  Box60.Top           = 46;
  Box60.Width         = 178;
  Box60.Height        = 40;
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
  Box62.Left          = 181;
  Box62.Top           = 88;
  Box62.Width         = 94;
  Box62.Height        = 40;
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
  Box63.Left          = 181;
  Box63.Top           = 171;
  Box63.Width         = 94;
  Box63.Height        = 40;
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
  CircleButton5.Left          = 143;
  CircleButton5.Top           = 92;
  CircleButton5.Radius        = 16;
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
  CircleButton6.Left          = 143;
  CircleButton6.Top           = 175;
  CircleButton6.Radius        = 16;
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
  CircleButton7.Left          = 281;
  CircleButton7.Top           = 92;
  CircleButton7.Radius        = 16;
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
  CircleButton8.Left          = 281;
  CircleButton8.Top           = 175;
  CircleButton8.Radius        = 16;
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
  Box64.Left          = 181;
  Box64.Top           = 88;
  Box64.Width         = 13;
  Box64.Height        = 40;
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
  Box65.Left          = 181;
  Box65.Top           = 171;
  Box65.Width         = 13;
  Box65.Height        = 40;
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
  Label48.Left        = 66;
  Label48.Top         = 58;
  Label48.Width       = 40;
  Label48.Height      = 17;
  Label48.Caption     = Label48_Caption;
  Label48.FontName    = 27;
  Label48.Font_Color  = 0xFFFF00;
  Label48.FontHandle  = 27;
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
  Label49.Left        = 66;
  Label49.Top         = 99;
  Label49.Width       = 40;
  Label49.Height      = 17;
  Label49.Caption     = Label49_Caption;
  Label49.FontName    = 27;
  Label49.Font_Color  = 0xFFFFFF;
  Label49.FontHandle  = 27;
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
  Label50.Left        = 62;
  Label50.Top         = 141;
  Label50.Width       = 48;
  Label50.Height      = 17;
  Label50.Caption     = Label50_Caption;
  Label50.FontName    = 27;
  Label50.Font_Color  = 0xFFFFFF;
  Label50.FontHandle  = 27;
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
  Label51.Left        = 65;
  Label51.Top         = 182;
  Label51.Width       = 42;
  Label51.Height      = 17;
  Label51.Caption     = Label51_Caption;
  Label51.FontName    = 27;
  Label51.Font_Color  = 0xFFFFFF;
  Label51.FontHandle  = 27;
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
  Label52.Left        = 144;
  Label52.Top         = 58;
  Label52.Width       = 51;
  Label52.Height      = 17;
  Label52.Caption     = Label52_Caption;
  Label52.FontName    = 27;
  Label52.Font_Color  = 0xFFFFFF;
  Label52.FontHandle  = 27;
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
  Label53.Left        = 144;
  Label53.Top         = 141;
  Label53.Width       = 81;
  Label53.Height      = 17;
  Label53.Caption     = Label53_Caption;
  Label53.FontName    = 27;
  Label53.Font_Color  = 0xFFFFFF;
  Label53.FontHandle  = 27;
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
  Label88.Left        = 298;
  Label88.Top         = 56;
  Label88.Width       = 12;
  Label88.Height      = 21;
  Label88.Caption     = Label88_Caption;
  Label88.FontName    = 28;
  Label88.Font_Color  = 0xFFFFFF;
  Label88.FontHandle  = 28;
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
  Label89.Left        = 300;
  Label89.Top         = 139;
  Label89.Width       = 10;
  Label89.Height      = 21;
  Label89.Caption     = Label89_Caption;
  Label89.FontName    = 28;
  Label89.Font_Color  = 0xFFFFFF;
  Label89.FontHandle  = 28;
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
  Label97.Left        = 240;
  Label97.Top         = 56;
  Label97.Width       = 30;
  Label97.Height      = 21;
  Label97.Caption     = Label97_Caption;
  Label97.FontName    = 28;
  Label97.Font_Color  = 0xFFFFFF;
  Label97.FontHandle  = 28;
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
  Label98.Left        = 240;
  Label98.Top         = 139;
  Label98.Width       = 35;
  Label98.Height      = 21;
  Label98.Caption     = Label98_Caption;
  Label98.FontName    = 28;
  Label98.Font_Color  = 0xFFFFFF;
  Label98.FontHandle  = 28;
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
  Button37.Left          = 4;
  Button37.Top           = 5;
  Button37.Width         = 313;
  Button37.Height        = 40;
  Button37.Pen_Width     = 1;
  Button37.Pen_Color     = 0x0000;
  Button37.Color         = 0x80C0;
  Button37.Press_Color   = 0xFFFFFF;
  Button37.ColorTo       = 0xFFFFFF;
  Button37.Press_ColorTo = 0xC0C0C0;
  Button37.Gradient      = _FT812_BRUSH_GR_NONE;
  Button37.Caption       = Button37_Caption;
  Button37.TextAlign     = taCenter;
  Button37.FontName      = 28;
  Button37.Font_Color    = 0xFFFFFF;
  Button37.FontHandle    = 28;
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
  Button38.Left          = 4;
  Button38.Top           = 212;
  Button38.Width         = 25;
  Button38.Height        = 25;
  Button38.Pen_Width     = 1;
  Button38.Pen_Color     = 0x0000;
  Button38.Color         = 0x808080;
  Button38.Press_Color   = 0xFFFFFF;
  Button38.ColorTo       = 0xFFFFFF;
  Button38.Press_ColorTo = 0xC0C0C0;
  Button38.Gradient      = _FT812_BRUSH_GR_NONE;
  Button38.Caption       = Button38_Caption;
  Button38.TextAlign     = taCenter;
  Button38.FontName      = 28;
  Button38.Font_Color    = 0xFFFFFF;
  Button38.FontHandle    = 28;
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
  Box11.Left          = 31;
  Box11.Top           = 212;
  Box11.Width         = 286;
  Box11.Height        = 25;
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
  Box24.Left          = 4;
  Box24.Top           = 46;
  Box24.Width         = 313;
  Box24.Height        = 40;
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
  Box76.Left          = 4;
  Box76.Top           = 88;
  Box76.Width         = 313;
  Box76.Height        = 40;
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
  Box77.Left          = 4;
  Box77.Top           = 129;
  Box77.Width         = 313;
  Box77.Height        = 40;
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
  Box78.Left          = 4;
  Box78.Top           = 171;
  Box78.Width         = 313;
  Box78.Height        = 40;
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
  CheckBox1.Left             = 256;
  CheckBox1.Top              = 58;
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
  CheckBox2.Left             = 256;
  CheckBox2.Top              = 99;
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
  CheckBox3.Left             = 256;
  CheckBox3.Top              = 141;
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
  CheckBox4.Left             = 256;
  CheckBox4.Top              = 182;
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
  Label6.Left        = 23;
  Label6.Top         = 54;
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
  Label26.Left        = 23;
  Label26.Top         = 95;
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
  Label27.Left        = 23;
  Label27.Top         = 137;
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
  Label28.Left        = 23;
  Label28.Top         = 178;
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
  Box25.Left          = 79;
  Box25.Top           = 170;
  Box25.Width         = 73;
  Box25.Height        = 40;
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
  Image69.Left          = 81;
  Image69.Top           = 174;
  Image69.Width         = 63;
  Image69.Height        = 64;
  Image69.Picture_Name  = sun3_bmp;
  Image69.Picture_Type  = 7;
  Image69.Picture_Ratio = 1;
  Image69.Blend_Color   = 0xFFFFFF;
  Image69.Source        = -1L;
  Image69.RotationAngle = 0;
  Image69.RotationCenterLeft = 94;
  Image69.RotationCenterTop = 190;
  Image69.ScaleX        = 0.5000;
  Image69.ScaleY        = 0.5000;
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
  Image70.Left          = 81;
  Image70.Top           = 174;
  Image70.Width         = 63;
  Image70.Height        = 64;
  Image70.Picture_Name  = sun4_bmp;
  Image70.Picture_Type  = 7;
  Image70.Picture_Ratio = 1;
  Image70.Blend_Color   = 0xFFFFFF;
  Image70.Source        = -1L;
  Image70.RotationAngle = 0;
  Image70.RotationCenterLeft = 94;
  Image70.RotationCenterTop = 190;
  Image70.ScaleX        = 0.5000;
  Image70.ScaleY        = 0.5000;
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
  Box10.Left          = 4;
  Box10.Top           = 170;
  Box10.Width         = 73;
  Box10.Height        = 40;
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
  Image67.Left          = 7;
  Image67.Top           = 176;
  Image67.Width         = 58;
  Image67.Height        = 54;
  Image67.Picture_Name  = moon3_bmp;
  Image67.Picture_Type  = 7;
  Image67.Picture_Ratio = 1;
  Image67.Blend_Color   = 0xFFFFFF;
  Image67.Source        = -1L;
  Image67.RotationAngle = 0;
  Image67.RotationCenterLeft = 18;
  Image67.RotationCenterTop = 190;
  Image67.ScaleX        = 0.5000;
  Image67.ScaleY        = 0.5000;
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
  Image68.Left          = 7;
  Image68.Top           = 176;
  Image68.Width         = 58;
  Image68.Height        = 54;
  Image68.Picture_Name  = moon4_bmp;
  Image68.Picture_Type  = 7;
  Image68.Picture_Ratio = 1;
  Image68.Blend_Color   = 0xFFFFFF;
  Image68.Source        = -1L;
  Image68.RotationAngle = 0;
  Image68.RotationCenterLeft = 18;
  Image68.RotationCenterTop = 190;
  Image68.ScaleX        = 0.5000;
  Image68.ScaleY        = 0.5000;
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
  Box30.Left          = 4;
  Box30.Top           = 46;
  Box30.Width         = 148;
  Box30.Height        = 122;
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
  Image66.Left          = 64;
  Image66.Top           = 87;
  Image66.Width         = 58;
  Image66.Height        = 50;
  Image66.Picture_Name  = circle1_bmp;
  Image66.Picture_Type  = 7;
  Image66.Picture_Ratio = 1;
  Image66.Blend_Color   = 0xFFFFFF;
  Image66.Source        = -1L;
  Image66.RotationAngle = 0;
  Image66.RotationCenterLeft = 79;
  Image66.RotationCenterTop = 99;
  Image66.ScaleX        = 0.5000;
  Image66.ScaleY        = 0.5000;
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
  Image65.Left          = 64;
  Image65.Top           = 87;
  Image65.Width         = 50;
  Image65.Height        = 50;
  Image65.Picture_Name  = circle2_bmp;
  Image65.Picture_Type  = 7;
  Image65.Picture_Ratio = 1;
  Image65.Blend_Color   = 0xFFFFFF;
  Image65.Source        = -1L;
  Image65.RotationAngle = 0;
  Image65.RotationCenterLeft = 77;
  Image65.RotationCenterTop = 99;
  Image65.ScaleX        = 0.5000;
  Image65.ScaleY        = 0.5000;
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
  Button12.Left          = 4;
  Button12.Top           = 212;
  Button12.Width         = 25;
  Button12.Height        = 25;
  Button12.Pen_Width     = 1;
  Button12.Pen_Color     = 0x0000;
  Button12.Color         = 0x808080;
  Button12.Press_Color   = 0xFFFFFF;
  Button12.ColorTo       = 0xFFFFFF;
  Button12.Press_ColorTo = 0xC0C0C0;
  Button12.Gradient      = _FT812_BRUSH_GR_NONE;
  Button12.Caption       = Button12_Caption;
  Button12.TextAlign     = taCenter;
  Button12.FontName      = 28;
  Button12.Font_Color    = 0xFFFFFF;
  Button12.FontHandle    = 28;
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
  Box9.Left          = 31;
  Box9.Top           = 212;
  Box9.Width         = 286;
  Box9.Height        = 25;
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
  Button13.Left          = 154;
  Button13.Top           = 46;
  Button13.Width         = 40;
  Button13.Height        = 40;
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
  Button14.Left          = 195;
  Button14.Top           = 46;
  Button14.Width         = 40;
  Button14.Height        = 40;
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
  Button15.Left          = 236;
  Button15.Top           = 46;
  Button15.Width         = 40;
  Button15.Height        = 40;
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
  Button16.Left          = 154;
  Button16.Top           = 88;
  Button16.Width         = 40;
  Button16.Height        = 40;
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
  Button17.Left          = 195;
  Button17.Top           = 88;
  Button17.Width         = 40;
  Button17.Height        = 40;
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
  Button18.Left          = 236;
  Button18.Top           = 88;
  Button18.Width         = 40;
  Button18.Height        = 40;
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
  Button19.Left          = 154;
  Button19.Top           = 129;
  Button19.Width         = 40;
  Button19.Height        = 40;
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
  Button20.Left          = 195;
  Button20.Top           = 129;
  Button20.Width         = 40;
  Button20.Height        = 40;
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
  Button21.Left          = 236;
  Button21.Top           = 129;
  Button21.Width         = 40;
  Button21.Height        = 40;
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
  Button22.Left          = 195;
  Button22.Top           = 170;
  Button22.Width         = 40;
  Button22.Height        = 40;
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
  Button23.Left          = 277;
  Button23.Top           = 46;
  Button23.Width         = 40;
  Button23.Height        = 40;
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
  Button24.Left          = 277;
  Button24.Top           = 88;
  Button24.Width         = 40;
  Button24.Height        = 40;
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
  Button25.Left          = 277;
  Button25.Top           = 129;
  Button25.Width         = 40;
  Button25.Height        = 40;
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
  Button26.Left          = 277;
  Button26.Top           = 170;
  Button26.Width         = 40;
  Button26.Height        = 40;
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
  Button27.Left          = 154;
  Button27.Top           = 170;
  Button27.Width         = 40;
  Button27.Height        = 40;
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
  Button28.Left          = 236;
  Button28.Top           = 170;
  Button28.Width         = 40;
  Button28.Height        = 40;
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
  Button29.Left          = 4;
  Button29.Top           = 5;
  Button29.Width         = 313;
  Button29.Height        = 40;
  Button29.Pen_Width     = 1;
  Button29.Pen_Color     = 0x0000;
  Button29.Color         = 0x8000FF;
  Button29.Press_Color   = 0xFFFFFF;
  Button29.ColorTo       = 0xFFFFFF;
  Button29.Press_ColorTo = 0xC0C0C0;
  Button29.Gradient      = _FT812_BRUSH_GR_NONE;
  Button29.Caption       = Button29_Caption;
  Button29.TextAlign     = taCenter;
  Button29.FontName      = 28;
  Button29.Font_Color    = 0xFFFFFF;
  Button29.FontHandle    = 28;
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
  Box36.Left          = 28;
  Box36.Top           = 51;
  Box36.Width         = 100;
  Box36.Height        = 17;
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
  Label24.Left        = 51;
  Label24.Top         = 52;
  Label24.Width       = 0;
  Label24.Height      = 15;
  Label24.Caption     = Label24_Caption;
  Label24.FontName    = 26;
  Label24.Font_Color  = 0x0000;
  Label24.FontHandle  = 26;
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
  Label25.Left        = 22;
  Label25.Top         = 133;
  Label25.Width       = 97;
  Label25.Height      = 25;
  Label25.Caption     = Label25_Caption;
  Label25.FontName    = 29;
  Label25.Font_Color  = 0xFFFFFF;
  Label25.FontHandle  = 29;
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
  Label54.Left        = 34;
  Label54.Top         = 183;
  Label54.Width       = 34;
  Label54.Height      = 15;
  Label54.Caption     = Label54_Caption;
  Label54.FontName    = 26;
  Label54.Font_Color  = 0xFFFFFF;
  Label54.FontHandle  = 26;
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
  Label55.Left        = 119;
  Label55.Top         = 183;
  Label55.Width       = 22;
  Label55.Height      = 15;
  Label55.Caption     = Label55_Caption;
  Label55.FontName    = 26;
  Label55.Font_Color  = 0xFFFF00;
  Label55.FontHandle  = 26;
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
  Button33.Left          = 4;
  Button33.Top           = 5;
  Button33.Width         = 313;
  Button33.Height        = 40;
  Button33.Pen_Width     = 1;
  Button33.Pen_Color     = 0x0000;
  Button33.Color         = 0xFF8000;
  Button33.Press_Color   = 0xFFFFFF;
  Button33.ColorTo       = 0xFFFFFF;
  Button33.Press_ColorTo = 0xC0C0C0;
  Button33.Gradient      = _FT812_BRUSH_GR_NONE;
  Button33.Caption       = Button33_Caption;
  Button33.TextAlign     = taCenter;
  Button33.FontName      = 28;
  Button33.Font_Color    = 0xFFFFFF;
  Button33.FontHandle    = 28;
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
  Button34.Left          = 4;
  Button34.Top           = 212;
  Button34.Width         = 25;
  Button34.Height        = 25;
  Button34.Pen_Width     = 1;
  Button34.Pen_Color     = 0x0000;
  Button34.Color         = 0x808080;
  Button34.Press_Color   = 0xFFFFFF;
  Button34.ColorTo       = 0xFFFFFF;
  Button34.Press_ColorTo = 0xC0C0C0;
  Button34.Gradient      = _FT812_BRUSH_GR_NONE;
  Button34.Caption       = Button34_Caption;
  Button34.TextAlign     = taCenter;
  Button34.FontName      = 28;
  Button34.Font_Color    = 0xFFFFFF;
  Button34.FontHandle    = 28;
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
  Box26.Left          = 31;
  Box26.Top           = 212;
  Box26.Width         = 286;
  Box26.Height        = 25;
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
  Box66.Left          = 4;
  Box66.Top           = 46;
  Box66.Width         = 313;
  Box66.Height        = 164;
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
  Label62.Left        = 9;
  Label62.Top         = 52;
  Label62.Width       = 0;
  Label62.Height      = 15;
  Label62.Caption     = Label62_Caption;
  Label62.FontName    = 26;
  Label62.Font_Color  = 0xFFFFFF;
  Label62.FontHandle  = 26;
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
  Label63.Left        = 9;
  Label63.Top         = 72;
  Label63.Width       = 0;
  Label63.Height      = 15;
  Label63.Caption     = Label63_Caption;
  Label63.FontName    = 26;
  Label63.Font_Color  = 0xFFFFFF;
  Label63.FontHandle  = 26;
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
  Label64.Left        = 9;
  Label64.Top         = 91;
  Label64.Width       = 0;
  Label64.Height      = 15;
  Label64.Caption     = Label64_Caption;
  Label64.FontName    = 26;
  Label64.Font_Color  = 0xFFFFFF;
  Label64.FontHandle  = 26;
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
  Label65.Left        = 9;
  Label65.Top         = 111;
  Label65.Width       = 0;
  Label65.Height      = 15;
  Label65.Caption     = Label65_Caption;
  Label65.FontName    = 26;
  Label65.Font_Color  = 0xFFFFFF;
  Label65.FontHandle  = 26;
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
  Label66.Left        = 9;
  Label66.Top         = 130;
  Label66.Width       = 0;
  Label66.Height      = 15;
  Label66.Caption     = Label66_Caption;
  Label66.FontName    = 26;
  Label66.Font_Color  = 0xFFFFFF;
  Label66.FontHandle  = 26;
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
  Label67.Left        = 9;
  Label67.Top         = 150;
  Label67.Width       = 0;
  Label67.Height      = 15;
  Label67.Caption     = Label67_Caption;
  Label67.FontName    = 26;
  Label67.Font_Color  = 0xFFFFFF;
  Label67.FontHandle  = 26;
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
  Label104.Left        = 9;
  Label104.Top         = 170;
  Label104.Width       = 0;
  Label104.Height      = 15;
  Label104.Caption     = Label104_Caption;
  Label104.FontName    = 26;
  Label104.Font_Color  = 0xFFFFFF;
  Label104.FontHandle  = 26;
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
  Label105.Left        = 9;
  Label105.Top         = 189;
  Label105.Width       = 0;
  Label105.Height      = 15;
  Label105.Caption     = Label105_Caption;
  Label105.FontName    = 26;
  Label105.Font_Color  = 0xFFFFFF;
  Label105.FontHandle  = 26;
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
  Box67.Left          = 4;
  Box67.Top           = 134;
  Box67.Width         = 77;
  Box67.Height        = 76;
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
  Image63.Left          = 27;
  Image63.Top           = 158;
  Image63.Width         = 62;
  Image63.Height        = 52;
  Image63.Picture_Name  = leaf_bmp;
  Image63.Picture_Type  = 7;
  Image63.Picture_Ratio = 1;
  Image63.Blend_Color   = 0xFFFFFF;
  Image63.Source        = -1L;
  Image63.RotationAngle = 0;
  Image63.RotationCenterLeft = 42;
  Image63.RotationCenterTop = 171;
  Image63.ScaleX        = 0.5000;
  Image63.ScaleY        = 0.5000;
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
  Box74.Left          = 4;
  Box74.Top           = 46;
  Box74.Width         = 77;
  Box74.Height        = 43;
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
  Image62.Left          = 45;
  Image62.Top           = 64;
  Image62.Width         = 19;
  Image62.Height        = 22;
  Image62.Picture_Name  = eu_bmp;
  Image62.Picture_Type  = 7;
  Image62.Picture_Ratio = 1;
  Image62.Blend_Color   = 0xFFFFFF;
  Image62.Source        = -1L;
  Image62.RotationAngle = 0;
  Image62.RotationCenterLeft = 51;
  Image62.RotationCenterTop = 71;
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
  Button35.Left          = 4;
  Button35.Top           = 5;
  Button35.Width         = 313;
  Button35.Height        = 40;
  Button35.Pen_Width     = 1;
  Button35.Pen_Color     = 0x0000;
  Button35.Color         = 0x8080;
  Button35.Press_Color   = 0xFFFFFF;
  Button35.ColorTo       = 0xFFFFFF;
  Button35.Press_ColorTo = 0xC0C0C0;
  Button35.Gradient      = _FT812_BRUSH_GR_NONE;
  Button35.Caption       = Button35_Caption;
  Button35.TextAlign     = taCenter;
  Button35.FontName      = 28;
  Button35.Font_Color    = 0xFFFFFF;
  Button35.FontHandle    = 28;
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
  Button36.Left          = 4;
  Button36.Top           = 212;
  Button36.Width         = 25;
  Button36.Height        = 25;
  Button36.Pen_Width     = 1;
  Button36.Pen_Color     = 0x0000;
  Button36.Color         = 0x808080;
  Button36.Press_Color   = 0xFFFFFF;
  Button36.ColorTo       = 0xFFFFFF;
  Button36.Press_ColorTo = 0xC0C0C0;
  Button36.Gradient      = _FT812_BRUSH_GR_NONE;
  Button36.Caption       = Button36_Caption;
  Button36.TextAlign     = taCenter;
  Button36.FontName      = 28;
  Button36.Font_Color    = 0xFFFFFF;
  Button36.FontHandle    = 28;
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
  Box27.Left          = 31;
  Box27.Top           = 212;
  Box27.Width         = 286;
  Box27.Height        = 25;
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
  Box68.Left          = 83;
  Box68.Top           = 46;
  Box68.Width         = 77;
  Box68.Height        = 164;
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
  Box69.Left          = 161;
  Box69.Top           = 46;
  Box69.Width         = 77;
  Box69.Height        = 164;
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
  Box70.Left          = 240;
  Box70.Top           = 46;
  Box70.Width         = 77;
  Box70.Height        = 164;
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
  Box71.Left          = 83;
  Box71.Top           = 165;
  Box71.Width         = 77;
  Box71.Height        = 45;
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
  Box72.Left          = 161;
  Box72.Top           = 165;
  Box72.Width         = 77;
  Box72.Height        = 45;
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
  Box73.Left          = 240;
  Box73.Top           = 165;
  Box73.Width         = 77;
  Box73.Height        = 45;
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
  Box75.Left          = 4;
  Box75.Top           = 90;
  Box75.Width         = 77;
  Box75.Height        = 43;
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
  Label56.Left        = 87;
  Label56.Top         = 190;
  Label56.Width       = 54;
  Label56.Height      = 15;
  Label56.Caption     = Label56_Caption;
  Label56.FontName    = 26;
  Label56.Font_Color  = 0x0000;
  Label56.FontHandle  = 26;
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
  Label30.Left        = 177;
  Label30.Top         = 190;
  Label30.Width       = 35;
  Label30.Height      = 15;
  Label30.Caption     = Label30_Caption;
  Label30.FontName    = 26;
  Label30.Font_Color  = 0x0000;
  Label30.FontHandle  = 26;
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
  Label57.Left        = 246;
  Label57.Top         = 190;
  Label57.Width       = 49;
  Label57.Height      = 15;
  Label57.Caption     = Label57_Caption;
  Label57.FontName    = 26;
  Label57.Font_Color  = 0x0000;
  Label57.FontHandle  = 26;
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
  Label82.Left        = 11;
  Label82.Top         = 138;
  Label82.Width       = 50;
  Label82.Height      = 17;
  Label82.Caption     = Label82_Caption;
  Label82.FontName    = 27;
  Label82.Font_Color  = 0xFFFFFF;
  Label82.FontHandle  = 27;
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
  Label83.Left        = 17;
  Label83.Top         = 190;
  Label83.Width       = 35;
  Label83.Height      = 15;
  Label83.Caption     = Label83_Caption;
  Label83.FontName    = 26;
  Label83.Font_Color  = 0xFFFFFF;
  Label83.FontHandle  = 26;
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
  Label84.Left        = 6;
  Label84.Top         = 50;
  Label84.Width       = 61;
  Label84.Height      = 15;
  Label84.Caption     = Label84_Caption;
  Label84.FontName    = 26;
  Label84.Font_Color  = 0xFFFFFF;
  Label84.FontHandle  = 26;
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
  Label85.Left        = 16;
  Label85.Top         = 94;
  Label85.Width       = 41;
  Label85.Height      = 15;
  Label85.Caption     = Label85_Caption;
  Label85.FontName    = 26;
  Label85.Font_Color  = 0xFFFFFF;
  Label85.FontHandle  = 26;
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
  Label90.Left        = 56;
  Label90.Top         = 65;
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
  Label91.Left        = 44;
  Label91.Top         = 109;
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
  Label92.Left        = 14;
  Label92.Top         = 65;
  Label92.Width       = 25;
  Label92.Height      = 17;
  Label92.Caption     = Label92_Caption;
  Label92.FontName    = 27;
  Label92.Font_Color  = 0xFFFFFF;
  Label92.FontHandle  = 27;
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
  Label93.Left        = 8;
  Label93.Top         = 109;
  Label93.Width       = 25;
  Label93.Height      = 17;
  Label93.Caption     = Label93_Caption;
  Label93.FontName    = 27;
  Label93.Font_Color  = 0xFFFFFF;
  Label93.FontHandle  = 27;
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
  Label94.Left        = 68;
  Label94.Top         = 56;
  Label94.Width       = 58;
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
  Label99.Left        = 155;
  Label99.Top         = 56;
  Label99.Width       = 35;
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
  Label100.Left        = 234;
  Label100.Top         = 56;
  Label100.Width       = 35;
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
  Label79.Left        = 141;
  Label79.Top         = 56;
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
  Label80.Left        = 210;
  Label80.Top         = 56;
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
  Label81.Left        = 288;
  Label81.Top         = 56;
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
  Box33.Left          = 4;
  Box33.Top           = 46;
  Box33.Width         = 148;
  Box33.Height        = 164;
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
  Label34.Left        = 24;
  Label34.Top         = 58;
  Label34.Width       = 40;
  Label34.Height      = 17;
  Label34.Caption     = Label34_Caption;
  Label34.FontName    = 27;
  Label34.Font_Color  = 0x400000;
  Label34.FontHandle  = 27;
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
  Label44.Left        = 24;
  Label44.Top         = 89;
  Label44.Width       = 52;
  Label44.Height      = 17;
  Label44.Caption     = Label44_Caption;
  Label44.FontName    = 27;
  Label44.Font_Color  = 0xFFFFFF;
  Label44.FontHandle  = 27;
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
  Label45.Left        = 24;
  Label45.Top         = 120;
  Label45.Width       = 27;
  Label45.Height      = 17;
  Label45.Caption     = Label45_Caption;
  Label45.FontName    = 27;
  Label45.Font_Color  = 0xFFFFFF;
  Label45.FontHandle  = 27;
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
  Label71.Left        = 24;
  Label71.Top         = 151;
  Label71.Width       = 42;
  Label71.Height      = 17;
  Label71.Caption     = Label71_Caption;
  Label71.FontName    = 27;
  Label71.Font_Color  = 0xFFFFFF;
  Label71.FontHandle  = 27;
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
  Label72.Left        = 24;
  Label72.Top         = 181;
  Label72.Width       = 31;
  Label72.Height      = 17;
  Label72.Caption     = Label72_Caption;
  Label72.FontName    = 27;
  Label72.Font_Color  = 0xFFFFFF;
  Label72.FontHandle  = 27;
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
  Button48.Left          = 4;
  Button48.Top           = 212;
  Button48.Width         = 25;
  Button48.Height        = 25;
  Button48.Pen_Width     = 1;
  Button48.Pen_Color     = 0x0000;
  Button48.Color         = 0x808080;
  Button48.Press_Color   = 0xFFFFFF;
  Button48.ColorTo       = 0xFFFFFF;
  Button48.Press_ColorTo = 0xC0C0C0;
  Button48.Gradient      = _FT812_BRUSH_GR_NONE;
  Button48.Caption       = Button48_Caption;
  Button48.TextAlign     = taCenter;
  Button48.FontName      = 28;
  Button48.Font_Color    = 0xFFFFFF;
  Button48.FontHandle    = 28;
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
  Box79.Left          = 31;
  Box79.Top           = 212;
  Box79.Width         = 286;
  Box79.Height        = 25;
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
  Button1.Left          = 154;
  Button1.Top           = 46;
  Button1.Width         = 40;
  Button1.Height        = 40;
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
  Button2.Left          = 195;
  Button2.Top           = 46;
  Button2.Width         = 40;
  Button2.Height        = 40;
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
  Button3.Left          = 236;
  Button3.Top           = 46;
  Button3.Width         = 40;
  Button3.Height        = 40;
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
  Button4.Left          = 154;
  Button4.Top           = 87;
  Button4.Width         = 40;
  Button4.Height        = 40;
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
  Button5.Left          = 195;
  Button5.Top           = 87;
  Button5.Width         = 40;
  Button5.Height        = 40;
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
  Button6.Left          = 236;
  Button6.Top           = 87;
  Button6.Width         = 40;
  Button6.Height        = 40;
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
  Button7.Left          = 154;
  Button7.Top           = 129;
  Button7.Width         = 40;
  Button7.Height        = 40;
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
  Button8.Left          = 195;
  Button8.Top           = 129;
  Button8.Width         = 40;
  Button8.Height        = 40;
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
  Button9.Left          = 236;
  Button9.Top           = 129;
  Button9.Width         = 40;
  Button9.Height        = 40;
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
  Button10.Left          = 195;
  Button10.Top           = 170;
  Button10.Width         = 40;
  Button10.Height        = 40;
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
  Button39.Left          = 277;
  Button39.Top           = 46;
  Button39.Width         = 40;
  Button39.Height        = 32;
  Button39.Pen_Width     = 1;
  Button39.Pen_Color     = 0x0000;
  Button39.Color         = 0xFF0000;
  Button39.Press_Color   = 0xFFFFFF;
  Button39.ColorTo       = 0xFFFFFF;
  Button39.Press_ColorTo = 0xC0C0C0;
  Button39.Gradient      = _FT812_BRUSH_GR_NONE;
  Button39.Caption       = Button39_Caption;
  Button39.TextAlign     = taCenter;
  Button39.FontName      = 26;
  Button39.Font_Color    = 0xFFFFFF;
  Button39.FontHandle    = 26;
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
  Button40.Left          = 277;
  Button40.Top           = 79;
  Button40.Width         = 40;
  Button40.Height        = 32;
  Button40.Pen_Width     = 1;
  Button40.Pen_Color     = 0x0000;
  Button40.Color         = 0x800000;
  Button40.Press_Color   = 0xFFFFFF;
  Button40.ColorTo       = 0xFFFFFF;
  Button40.Press_ColorTo = 0xC0C0C0;
  Button40.Gradient      = _FT812_BRUSH_GR_NONE;
  Button40.Caption       = Button40_Caption;
  Button40.TextAlign     = taCenter;
  Button40.FontName      = 26;
  Button40.Font_Color    = 0xFFFFFF;
  Button40.FontHandle    = 26;
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
  Button41.Left          = 277;
  Button41.Top           = 112;
  Button41.Width         = 40;
  Button41.Height        = 32;
  Button41.Pen_Width     = 1;
  Button41.Pen_Color     = 0x0000;
  Button41.Color         = 0x800000;
  Button41.Press_Color   = 0xFFFFFF;
  Button41.ColorTo       = 0xFFFFFF;
  Button41.Press_ColorTo = 0xC0C0C0;
  Button41.Gradient      = _FT812_BRUSH_GR_NONE;
  Button41.Caption       = Button41_Caption;
  Button41.TextAlign     = taCenter;
  Button41.FontName      = 26;
  Button41.Font_Color    = 0xFFFFFF;
  Button41.FontHandle    = 26;
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
  Button42.Left          = 277;
  Button42.Top           = 145;
  Button42.Width         = 40;
  Button42.Height        = 32;
  Button42.Pen_Width     = 1;
  Button42.Pen_Color     = 0x0000;
  Button42.Color         = 0x800000;
  Button42.Press_Color   = 0xFFFFFF;
  Button42.ColorTo       = 0xFFFFFF;
  Button42.Press_ColorTo = 0xC0C0C0;
  Button42.Gradient      = _FT812_BRUSH_GR_NONE;
  Button42.Caption       = Button42_Caption;
  Button42.TextAlign     = taCenter;
  Button42.FontName      = 26;
  Button42.Font_Color    = 0xFFFFFF;
  Button42.FontHandle    = 26;
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
  Button43.Left          = 154;
  Button43.Top           = 170;
  Button43.Width         = 40;
  Button43.Height        = 40;
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
  Button44.Left          = 236;
  Button44.Top           = 170;
  Button44.Width         = 40;
  Button44.Height        = 40;
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
  Button45.Left          = 4;
  Button45.Top           = 5;
  Button45.Width         = 313;
  Button45.Height        = 40;
  Button45.Pen_Width     = 1;
  Button45.Pen_Color     = 0x0000;
  Button45.Color         = 0xFF0000;
  Button45.Press_Color   = 0xFFFFFF;
  Button45.ColorTo       = 0xFFFFFF;
  Button45.Press_ColorTo = 0xC0C0C0;
  Button45.Gradient      = _FT812_BRUSH_GR_NONE;
  Button45.Caption       = Button45_Caption;
  Button45.TextAlign     = taCenter;
  Button45.FontName      = 28;
  Button45.Font_Color    = 0xFFFFFF;
  Button45.FontHandle    = 28;
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
  Button46.Left          = 277;
  Button46.Top           = 178;
  Button46.Width         = 40;
  Button46.Height        = 32;
  Button46.Pen_Width     = 1;
  Button46.Pen_Color     = 0x0000;
  Button46.Color         = 0x800000;
  Button46.Press_Color   = 0xFFFFFF;
  Button46.ColorTo       = 0xFFFFFF;
  Button46.Press_ColorTo = 0xC0C0C0;
  Button46.Gradient      = _FT812_BRUSH_GR_NONE;
  Button46.Caption       = Button46_Caption;
  Button46.TextAlign     = taCenter;
  Button46.FontName      = 26;
  Button46.Font_Color    = 0xFFFFFF;
  Button46.FontHandle    = 26;
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
