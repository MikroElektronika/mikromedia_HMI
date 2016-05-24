#include "Mikromedia_HMI_35C_Demo_objects.h"
#include "Mikromedia_HMI_35C_Demo_resources.h"

//--------------------- User code ---------------------//



unsigned char e=0,a=0,i=0,j=0,k=0,kk=0,dat=0,ti=0,dat_ok=0,hom=1,gr_pow=0,choice_t=0,scr=0,ch_au=0,ch_ha=0,ch_bl=0,ch_de=0,vol=255;
unsigned char arm[6],date[12],tim[5]={0,0,0,0,0};
char xt[4],txt[4],tekst_1[7],tekst_2[7],tek_1[15],tek_2[15];
unsigned long adress;
int ti_le,ha,so,ba,num=0;
int temp_t[4]={181,181,181,181},hum_t[4]={181,181,181,181},dim_l[4]={181,181,181,181},hum_l[4]={181,181,181,181};
int t_t[4]={15,15,15,15},h_t[4]={20,20,20,20},d_l[4]={0,0,0,0},h_l[4]={15,15,15,15}; 
long ero=0;

typedef struct
{
    char hour;
    char min;
    char day;
    char mon;
    int year;
} datetime_t;

datetime_t datum,datum2;

extern char cSF_test_status;
extern void InitMCU();
extern void SF_Start();
extern void SF_mem();
extern void SF_rem();
extern void SF_Test(char *test);

//sbit Mmc_Chip_Select at GPIO_PIN33_bit;

//  Initialize date.
void Start_mcu() {
    GPIO_Pin_Digital_Input(_GPIO_PIN_NUM_53);
    GPIO_PIN53_bit = 0;
    datum2.hour=14;
    datum2.min=30;
    datum2.day=25;
    datum2.mon=4;
    datum2.year=2016;
}

//  Function is called by TimerA to enable Home screen change.        
void Enable_Home_Change() {
    if(j==0) j=1;
    else j=0;
    k=1;
    kk++;
}

//  Function is called by main to perform change on Home screen if Enable_Home_Change allowed it.
void Home_Change() {
    if(k==0) return;
    if(k==1) {
    
        ero+=TIMER_READ_LS;
        srand((unsigned)ero);
        num=rand()%20+1;
        
        switch(num) {
        case 1:
            Label9.Font_Color=0x0000FFFF;
            Label10.Font_Color=0x0000FFFF;
            Box14.Color=0x00400080;
            Box15.Color=0x00400080;
            Box14.Color=0x00400080;
            Box15.Color=0x00400080;
            Image9.Visible=0;
            Image16.Visible=0;
            Image29.Visible=1;
            Image30.Visible=1;
            if(hom==1) DrawScreen(&Home);
            break;
        case 2:
            Label11.Font_Color=0x0000FFFF;
            Box16.Color=0x00400080;
            Image17.Visible=0;
            Image31.Visible=1;            
            if(hom==1) DrawScreen(&Home);
            break;
        case 3:
            
            break;
        case 4:
            
            break;
        case 5:
            Box13.Color=0x00400080;
            Label7.Font_Color=0x0000FFFF;
            Label8.Font_Color=0x0000FFFF;
            Image6.Visible=0;
            Image35.Visible=1;
            if(hom==1) DrawScreen(&Home);
            break;            
        case 6:
            Box3.Color=0x00C0C0C0;
            Label36.Font_Color=0x00000000;
            Image10.Visible=0;
            Image21.Visible=1;
            if(hom==1) DrawScreen(&Home);
            break;
        case 7:
            
            break;
        case 8:
            Box5.Color=0x00C0C0C0;
            Label38.Font_Color=0x00000000;
            Image12.Visible=0;
            Image23.Visible=1;            
            if(hom==1) DrawScreen(&Home);
            break;
        case 9:
            
            break;
        case 10:
            Box7.Color=0x00C0C0C0;
            Label40.Font_Color=0x00000000;
            Image14.Visible=0;
            Image25.Visible=1;            
            if(hom==1) DrawScreen(&Home);
            break;
        case 11:
            
            break;
        case 12:
            Label5.Font_Color=0x00000000;
            Box12.Color=0x00C0C0C0;
            Image1.Visible=0;
            Image27.Visible=1;            
            if(hom==1) DrawScreen(&Home);
            break;    
        case 14:
            Box20.Color=0x00FF4000;
            Image7.Visible=0;
            Image36.Visible=1;            
            if(hom==1) DrawScreen(&Home);
            break;    
        case 15:
            Box21.Color=0x00008080;
            Image8.Visible=0;
            Image37.Visible=1;
            if(hom==1) DrawScreen(&Home);
            break;     
        case 16:
            Box28.Color=0x00800000;
            if(hom==1) DrawScreen(&Home);
            break;
        case 17:
            Box34.Color=0x00000080;
            Image3.Visible=0;
            Image38.Visible=1;
            Image41.Visible=0;
            Image75.Visible=1;
            Image44.Visible=0;
            Image76.Visible=1;            
            if(hom==1) DrawScreen(&Home);
            break;   
        default:
            Label5.Font_Color=0x00FFFFFF;
            Label7.Font_Color=0x00FFFFFF;
            Label8.Font_Color=0x00FFFFFF;
            Label9.Font_Color=0x00FFFFFF;
            Label10.Font_Color=0x00FFFFFF;
            Label11.Font_Color=0x00FFFFFF;
            Label36.Font_Color=0x00FFFFFF;
            Label38.Font_Color=0x00FFFFFF;
            Label40.Font_Color=0x00FFFFFF;                                                           
            Box3.Color=0x00808080;
            Box5.Color=0x00808080;
            Box7.Color=0x00808080;
            Box12.Color=0x00808080;
            Box13.Color=0x008000FF;
            Box14.Color=0x008000FF;
            Box15.Color=0x008000FF;
            Box16.Color=0x008000FF;
            Box20.Color=0x00FF8000;            
            Box21.Color=0x00008040;
            Box28.Color=0x00FF0000;
            Box34.Color=0x000000FF;
            Image1.Visible=1;
            Image3.Visible=1;
            Image6.Visible=1;
            Image7.Visible=1;
            Image8.Visible=1;
            Image9.Visible=1;
            Image10.Visible=1;
            Image12.Visible=1;
            Image14.Visible=1;
            Image16.Visible=1;
            Image17.Visible=1;
            Image41.Visible=1;
            Image44.Visible=1;
            Image21.Visible=0;
            Image23.Visible=0;
            Image25.Visible=0;
            Image27.Visible=0;
            Image29.Visible=0;
            Image30.Visible=0;
            Image31.Visible=0;
            Image35.Visible=0;
            Image36.Visible=0;
            Image37.Visible=0;
            Image38.Visible=0;            
            Image75.Visible=0;
            Image76.Visible=0;                                    
            if(hom==1) DrawScreen(&Home);
        }
        if(num<17) num++;
        else num=0;
        if(kk>=24) {
            datum2.min++;
            if(datum2.min==60) {
                datum2.min=0;
                datum2.hour++;
                if(datum2.hour==24) {
                    datum2.hour=0;
                    datum2.day++;
                    if((datum2.day==30 && datum2.mon==2 && datum2.year%4==0)||
                       (datum2.day==29 && datum2.mon==2 && datum2.year%4!=0)||
                       (datum2.day==31 && (datum2.mon==4 || datum2.mon==6 || datum2.mon==9 || datum2.mon==11))||
                       (datum2.day==32 && (datum2.mon==1 || datum2.mon==3 || datum2.mon==5 || datum2.mon==7 || datum2.mon==8 || datum2.mon==10 || datum2.mon==12))) {
                        datum2.day=1;
                        datum2.mon++;
                        if(datum2.mon>12) { datum2.mon=1; datum2.year++; }
                    }
                }
            }
            kk=0;
            ByteToStr(datum2.hour,Label73.Caption);
            ByteToStr(datum2.min,Label75.Caption);
            ByteToStr(datum2.day,Label76.Caption);
            strcat(Label76.Caption,".");
            ByteToStr(datum2.mon,Label77.Caption);
            strcat(Label77.Caption,".");
            IntToStr(datum2.year,Label78.Caption);
            strcat(Label78.Caption,".");
        }
        k=0;
    }
}

//  Function is called when Temperature or Lighting mode(value of scr) 
//  is changed to draw the values of temperature/humidity/dimmer in the choasen mode 
//  and color the mode button's caption and image yellow.
void Draw_state(unsigned char ch) {
  if(scr==0) {
    switch(ch) {
        case 0:
            Label32.Font_Color=0x00FFFF00;
            Label33.Font_Color=0x00FFFFFF;
            Label46.Font_Color=0x00FFFFFF;
            Label47.Font_Color=0x00FFFFFF;
            Image2.Visible=0;
            Image28.Visible=1;
            Image46.Visible=1;
            Image47.Visible=0;
            Image48.Visible=1;
            Image49.Visible=0;
            Image50.Visible=1;
            Image51.Visible=0;
            Image5.Visible=1;
            Image42.Visible=0;
            Image40.Visible=0;
            Image43.Visible=0;
            Label2.Caption="HOME";
            break;
        case 1:
            Label32.Font_Color=0x00FFFFFF;
            Label33.Font_Color=0x00FFFF00;
            Label46.Font_Color=0x00FFFFFF;
            Label47.Font_Color=0x00FFFFFF;
            Image2.Visible=1;
            Image28.Visible=0;
            Image46.Visible=0;
            Image47.Visible=1;
            Image48.Visible=1;
            Image49.Visible=0;
            Image50.Visible=1;
            Image51.Visible=0;
            Image5.Visible=0;
            Image42.Visible=1;
            Image40.Visible=0;
            Image43.Visible=0;
            Label2.Caption="NIGHT";
            break;
        case 2:
            Label32.Font_Color=0x00FFFFFF;
            Label33.Font_Color=0x00FFFFFF;
            Label46.Font_Color=0x00FFFF00;
            Label47.Font_Color=0x00FFFFFF;
            Image2.Visible=1;
            Image28.Visible=0;
            Image46.Visible=1;
            Image47.Visible=0;
            Image48.Visible=0;
            Image49.Visible=1;
            Image50.Visible=1;
            Image51.Visible=0;
            Image5.Visible=0;
            Image42.Visible=0;
            Image40.Visible=1;
            Image43.Visible=0;
            Label2.Caption="DAY";
            break;
        case 3:
            Label32.Font_Color=0x00FFFFFF;
            Label33.Font_Color=0x00FFFFFF;
            Label46.Font_Color=0x00FFFFFF;
            Label47.Font_Color=0x00FFFF00;
            Image2.Visible=1;
            Image28.Visible=0;
            Image46.Visible=1;
            Image47.Visible=0;
            Image48.Visible=1;
            Image49.Visible=0;
            Image50.Visible=0;
            Image51.Visible=1;
            Image5.Visible=0;
            Image42.Visible=0;
            Image40.Visible=0;
            Image43.Visible=1;
            Label2.Caption="PARTY";
            break;
    }
    Box23.Left=temp_t[ch];
    Box49.Left=hum_t[ch];
    IntToStr(t_t[ch],Label95.Caption);
    IntToStr(h_t[ch],Label96.Caption);
    DrawScreen(&Temperature_Mode);
  }
  else {
    switch(ch) {
        case 0:
            Label48.Font_Color=0x00FFFF00;
            Label49.Font_Color=0x00FFFFFF;
            Label50.Font_Color=0x00FFFFFF;
            Label51.Font_Color=0x00FFFFFF;
            Image52.Visible=0;
            Image53.Visible=1;
            Image54.Visible=1;
            Image55.Visible=0;
            Image56.Visible=1;
            Image57.Visible=0;
            Image58.Visible=1;
            Image59.Visible=0;
            Image71.Visible=1;
            Image72.Visible=0;
            Image73.Visible=0;
            Image74.Visible=0;
            Label4.Caption="MEDIA";
            break;
        case 1:
            Label48.Font_Color=0x00FFFFFF;
            Label49.Font_Color=0x00FFFF00;
            Label50.Font_Color=0x00FFFFFF;
            Label51.Font_Color=0x00FFFFFF;
            Image52.Visible=1;
            Image53.Visible=0;
            Image54.Visible=0;
            Image55.Visible=1;
            Image56.Visible=1;
            Image57.Visible=0;
            Image58.Visible=1;
            Image59.Visible=0;
            Image71.Visible=0;
            Image72.Visible=1;
            Image73.Visible=0;
            Image74.Visible=0;
            Label4.Caption="RELAX";
            break;
        case 2:
            Label48.Font_Color=0x00FFFFFF;
            Label49.Font_Color=0x00FFFFFF;
            Label50.Font_Color=0x00FFFF00;
            Label51.Font_Color=0x00FFFFFF;
            Image52.Visible=1;
            Image53.Visible=0;
            Image54.Visible=1;
            Image55.Visible=0;
            Image56.Visible=0;
            Image57.Visible=1;
            Image58.Visible=1;
            Image59.Visible=0;
            Image71.Visible=0;
            Image72.Visible=0;
            Image73.Visible=1;
            Image74.Visible=0;
            Label4.Caption="DINNER";
            break;
        case 3:
            Label48.Font_Color=0x00FFFFFF;
            Label49.Font_Color=0x00FFFFFF;
            Label50.Font_Color=0x00FFFFFF;
            Label51.Font_Color=0x00FFFF00;
            Image52.Visible=1;
            Image53.Visible=0;
            Image54.Visible=1;
            Image55.Visible=0;
            Image56.Visible=1;
            Image57.Visible=0;
            Image58.Visible=0;
            Image59.Visible=1;
            Image71.Visible=0;
            Image72.Visible=0;
            Image73.Visible=0;
            Image74.Visible=1;
            Label4.Caption="PARTY";
            break;
    }
    Box64.Left=dim_l[ch];
    Box65.Left=hum_l[ch];
    IntToStr(d_l[ch],Label97.Caption);
    IntToStr(h_l[ch],Label98.Caption);
    DrawScreen(&Light_Mode);
  }
}

//  Function is called by main to animate the bars of the Green Power screen
//  if the demo is in that screen(indicated by gr_pow).
void Green_power_bars() {
    if(gr_pow==1) {
        if(Box71.Top>=207) {
            Box71.Top=46;
            Box71.Height=164;
            strcpy(Label94.Caption,"12:00");
        }
        else {
            Box71.Top+=1;
            Box71.Height-=1;
            ha=12-(int)floor((Box71.Top-46)*12/164);
            IntToStr(ha,Label94.Caption);
            strcat(Label94.Caption,":00");
        }
        if(Box72.Top>=207) {
            Box72.Top=46;
            Box72.Height=164;
            so=100;
            IntToStr(so,tekst_1);
            strcpy(Label99.Caption,tekst_1);           
        }
        else {
            Box72.Top+=3;
            Box72.Height-=3;
            strcpy(Label99.Caption,"");
            so=100-(int)floor((Box72.Top-46)*100/164);
            IntToStr(so,tekst_1);
            strcpy(Label99.Caption,tekst_1);
        }
        if(Box73.Top>=207) {
            Box73.Top=46;
            Box73.Height=164;
            ba=100;
            IntToStr(ba,tekst_2);
            strcpy(Label100.Caption,tekst_2);
        }
        else {
            Box73.Top+=2;
            Box73.Height-=2;
            strcpy(Label100.Caption,"");
            ba=100-(int)floor((Box73.Top-46)*100/164);
            IntToStr(ba,tekst_2);
            strcpy(Label100.Caption,tekst_2);
        }
        DrawScreen(&Time_Screen);
    }
}

//  Function is called to write the chosen text on the System Log.
//  When a new text is written to the first row,all the old one is lowered down by a row.
void Write_Log(char *event) {
    char hour[4],min[4],day[4],mon[4],year[7];    
    strcpy(Label105.Caption,Label104.Caption);
    strcpy(Label104.Caption,Label67.Caption);
    strcpy(Label67.Caption,Label66.Caption);
    strcpy(Label66.Caption,Label65.Caption);
    strcpy(Label65.Caption,Label64.Caption);
    strcpy(Label64.Caption,Label63.Caption);
    strcpy(Label63.Caption,Label62.Caption);
    ByteToStr(datum2.hour,hour);
    strcpy(Label62.Caption,hour);
    strcat(Label62.Caption,"  :");
    ByteToStr(datum2.min,min);
    strcat(Label62.Caption,min);
    strcat(Label62.Caption,"  ");
    ByteToStr(datum2.day,day);
    strcat(Label62.Caption,day);
    strcat(Label62.Caption,".");
    ByteToStr(datum2.mon,mon);
    strcat(Label62.Caption,mon);
    strcat(Label62.Caption,".");
    IntToStr(datum2.year,year);
    strcat(Label62.Caption,year);
    strcat(Label62.Caption,".    ");
    strcat(Label62.Caption,event);
    strcpy(Label16.Caption,"Events Up!");
}


//----------------- End of User code ------------------//

// Event Handlers

void Box1OnClick() {
    if(CheckBox2.Checked==1) {
        GPIO_PIN53_bit = 1;
        Delay_ms(80);
        GPIO_PIN53_bit = 0;
    }
    DrawScreen(&Temperature_Mode);
    scr=0;
    choice_t=0;
    hom=0;
}

void Box2OnClick() {
    if(CheckBox2.Checked==1) {
        GPIO_PIN53_bit = 1;
        Delay_ms(80);
        GPIO_PIN53_bit = 0;
    } 
    DrawScreen(&Light_Mode);
    scr=1;
    choice_t=0;
    hom=0;
}

void Box13OnClick() {
    if(CheckBox2.Checked==1) {
        GPIO_PIN53_bit = 1;
        Delay_ms(80);
        GPIO_PIN53_bit = 0;
    }
    DrawScreen(&System_Log_Screen);
    hom=0;    
}

void Box20OnClick() {
    if(CheckBox2.Checked==1) {
        GPIO_PIN53_bit = 1;
        Delay_ms(80);
        GPIO_PIN53_bit = 0;
    }
    DrawScreen(&Green_Power_Screen);
    hom=0;
}

void Box21OnClick() {
    if(CheckBox2.Checked==1) {
        GPIO_PIN53_bit = 1;
        Delay_ms(80);
        GPIO_PIN53_bit = 0;
    }
    DrawScreen(&Time_Screen);
    gr_pow=1;
    hom=0;
}

void Box31OnClick() {
    if(CheckBox1.Checked==1) {
        FT812_Sound_SetAndPlay(_FT812_SOUND_XYLOPHONE,_FT812_SOUND_PITCH_B3,vol);
        Delay_ms(1000);
        FT812_Sound_SetAndPlay(_FT812_SOUND_TUBA,_FT812_SOUND_PITCH_B3,vol);
        Delay_ms(1000);
        FT812_Sound_SetAndPlay(_FT812_SOUND_GLOCKENSPIEL,_FT812_SOUND_PITCH_B3,vol);
    }
}

void Box28OnClick() {
    char i;
    datum.hour=0;
    datum.min=0;
    datum.day=0;
    datum.mon=0;
    datum.year=0;
    Button39.Color=0x00FF0000;
    Button40.Color=0x00800000;
    Button41.Color=0x00800000;
    Button42.Color=0x00800000;
    Button46.Color=0x00800000;
    Label34.Font_Color=0x00400000;
    Label44.Font_Color=0x00FFFFFF;
    Label45.Font_Color=0x00FFFFFF;
    Label71.Font_Color=0x00FFFFFF;
    Label72.Font_Color=0x00FFFFFF;                    
    ti=0;
    for(i=0;i<4;i++) {
        tim[i]=0;
    }
    hom=0;
    DrawScreen(&Settings_Screen);               
}

void Box54OnClick() {
    unsigned char i;
    for(i=0;i<12;i++) {
        date[i]=0;
    }
    for(i=0;i<5;i++) {
        tim[i]=0;
    }
    datum.hour=0;
    datum.min=0;
    datum.day=0;
    datum.mon=0;
    datum.year=0;
    strcpy(Label34.Caption,"Hours:  ");
    strcpy(Label44.Caption,"Minutes:  ");
    strcpy(Label45.Caption,"Day:  ");
    strcpy(Label71.Caption,"Month:  ");
    strcpy(Label72.Caption,"Year:  ");    
    dat=0;
    hom=1;
    DrawScreen(&Home);
}

void Button1OnClick() {
    if(i<=11){
        date[dat]=1;
        if(dat>=0 && dat<=1) { if(tim[0]!=1) { datum.hour=datum.hour*10+date[dat]; strcat(Label34.Caption,"1"); } if(dat==1) tim[0]=1; if(dat==0) dat++; }
        if(dat>=2 && dat<=3) { if(tim[1]!=1) { datum.min=datum.min*10+date[dat]; strcat(Label44.Caption,"1"); } if(dat==3) tim[1]=1; if(dat==2) dat++; }
        if(dat>=4 && dat<=5) { if(tim[2]!=1) { datum.day=datum.day*10+date[dat]; strcat(Label45.Caption,"1"); } if(dat==5) tim[2]=1; if(dat==4) dat++; }
        if(dat>=6 && dat<=7) { if(tim[3]!=1) { datum.mon=datum.mon*10+date[dat]; strcat(Label71.Caption,"1"); } if(dat==7) tim[3]=1; if(dat==6) dat++; }
        if(dat>=8 && dat<=11) { if(tim[4]!=1) { datum.year=datum.year*10+date[dat]; strcat(Label72.Caption,"1"); } if(dat==11) tim[4]=1; dat++; }
        //dat++;
        DrawScreen(&Settings_Screen);
    }
}

void Button2OnClick() {
    if(i<=11){
        date[dat]=2;
        if(dat>=0 && dat<=1) { if(tim[0]!=1) { datum.hour=datum.hour*10+date[dat]; strcat(Label34.Caption,"2"); } if(dat==1) tim[0]=1; if(dat==0) dat++; }
        if(dat>=2 && dat<=3) { if(tim[1]!=1) { datum.min=datum.min*10+date[dat]; strcat(Label44.Caption,"2"); } if(dat==3) tim[1]=1; if(dat==2) dat++; }
        if(dat>=4 && dat<=5) { if(tim[2]!=1) { datum.day=datum.day*10+date[dat]; strcat(Label45.Caption,"2"); } if(dat==5) tim[2]=1; if(dat==4) dat++; }
        if(dat>=6 && dat<=7) { if(tim[3]!=1) { datum.mon=datum.mon*10+date[dat]; strcat(Label71.Caption,"2"); } if(dat==7) tim[3]=1; if(dat==6) dat++; }
        if(dat>=8 && dat<=11) { if(tim[4]!=1) { datum.year=datum.year*10+date[dat]; strcat(Label72.Caption,"2"); } if(dat==11) tim[4]=1; dat++; }
        //dat++;
        DrawScreen(&Settings_Screen);
    }
}

void Button3OnClick() {
    if(i<=11){
        date[dat]=3;
        if(dat>=0 && dat<=1) { if(tim[0]!=1) { datum.hour=datum.hour*10+date[dat]; strcat(Label34.Caption,"3"); } if(dat==1) tim[0]=1; if(dat==0) dat++; }
        if(dat>=2 && dat<=3) { if(tim[1]!=1) { datum.min=datum.min*10+date[dat]; strcat(Label44.Caption,"3"); } if(dat==3) tim[1]=1; if(dat==2) dat++; }
        if(dat>=4 && dat<=5) { if(tim[2]!=1) { datum.day=datum.day*10+date[dat]; strcat(Label45.Caption,"3"); } if(dat==5) tim[2]=1; if(dat==4) dat++; }
        if(dat>=6 && dat<=7) { if(tim[3]!=1) { datum.mon=datum.mon*10+date[dat]; strcat(Label71.Caption,"3"); } if(dat==7) tim[3]=1; if(dat==6) dat++; }
        if(dat>=8 && dat<=11) { if(tim[4]!=1) { datum.year=datum.year*10+date[dat]; strcat(Label72.Caption,"3"); } if(dat==11) tim[4]=1; dat++; }
        //dat++;
        DrawScreen(&Settings_Screen);
    }
}

void Button4OnClick() {
    if(i<=11){
        date[dat]=4;
        if(dat>=0 && dat<=1) { if(tim[0]!=1) { datum.hour=datum.hour*10+date[dat]; strcat(Label34.Caption,"4"); } if(dat==1) tim[0]=1; if(dat==0) dat++; }
        if(dat>=2 && dat<=3) { if(tim[1]!=1) { datum.min=datum.min*10+date[dat]; strcat(Label44.Caption,"4"); } if(dat==3) tim[1]=1; if(dat==2) dat++; }
        if(dat>=4 && dat<=5) { if(tim[2]!=1) { datum.day=datum.day*10+date[dat]; strcat(Label45.Caption,"4"); } if(dat==5) tim[2]=1; if(dat==4) dat++; }
        if(dat>=6 && dat<=7) { if(tim[3]!=1) { datum.mon=datum.mon*10+date[dat]; strcat(Label71.Caption,"4"); } if(dat==7) tim[3]=1; if(dat==6) dat++; }
        if(dat>=8 && dat<=11) { if(tim[4]!=1) { datum.year=datum.year*10+date[dat]; strcat(Label72.Caption,"4"); } if(dat==11) tim[4]=1; dat++; }
        //dat++;
        DrawScreen(&Settings_Screen);
    }
}

void Button5OnClick() {
if(i<=11){
        date[dat]=5;
        if(dat>=0 && dat<=1) { if(tim[0]!=1) { datum.hour=datum.hour*10+date[dat]; strcat(Label34.Caption,"5"); } if(dat==1) tim[0]=1; if(dat==0) dat++; }
        if(dat>=2 && dat<=3) { if(tim[1]!=1) { datum.min=datum.min*10+date[dat]; strcat(Label44.Caption,"5"); } if(dat==3) tim[1]=1; if(dat==2) dat++; }
        if(dat>=4 && dat<=5) { if(tim[2]!=1) { datum.day=datum.day*10+date[dat]; strcat(Label45.Caption,"5"); } if(dat==5) tim[2]=1; if(dat==4) dat++; }
        if(dat>=6 && dat<=7) { if(tim[3]!=1) { datum.mon=datum.mon*10+date[dat]; strcat(Label71.Caption,"5"); } if(dat==7) tim[3]=1; if(dat==6) dat++; }
        if(dat>=8 && dat<=11) { if(tim[4]!=1) { datum.year=datum.year*10+date[dat]; strcat(Label72.Caption,"5"); } if(dat==11) tim[4]=1; dat++; }
        //dat++;
        DrawScreen(&Settings_Screen);
    }
}

void Button6OnClick() {
    if(i<=11){
        date[dat]=6;
        if(dat>=0 && dat<=1) { if(tim[0]!=1) { datum.hour=datum.hour*10+date[dat]; strcat(Label34.Caption,"6"); } if(dat==1) tim[0]=1; if(dat==0) dat++; }
        if(dat>=2 && dat<=3) { if(tim[1]!=1) { datum.min=datum.min*10+date[dat]; strcat(Label44.Caption,"6"); } if(dat==3) tim[1]=1; if(dat==2) dat++; }
        if(dat>=4 && dat<=5) { if(tim[2]!=1) { datum.day=datum.day*10+date[dat]; strcat(Label45.Caption,"6"); } if(dat==5) tim[2]=1; if(dat==4) dat++; }
        if(dat>=6 && dat<=7) { if(tim[3]!=1) { datum.mon=datum.mon*10+date[dat]; strcat(Label71.Caption,"6"); } if(dat==7) tim[3]=1; if(dat==6) dat++; }
        if(dat>=8 && dat<=11) { if(tim[4]!=1) { datum.year=datum.year*10+date[dat]; strcat(Label72.Caption,"6"); } if(dat==11) tim[4]=1; dat++; }
        //dat++;
        DrawScreen(&Settings_Screen);
    }
}

void Button7OnClick() {
    if(i<=11){
        date[dat]=7;
        if(dat>=0 && dat<=1) { if(tim[0]!=1) { datum.hour=datum.hour*10+date[dat]; strcat(Label34.Caption,"7"); } if(dat==1) tim[0]=1; if(dat==0) dat++; }
        if(dat>=2 && dat<=3) { if(tim[1]!=1) { datum.min=datum.min*10+date[dat]; strcat(Label44.Caption,"7"); } if(dat==3) tim[1]=1; if(dat==2) dat++; }
        if(dat>=4 && dat<=5) { if(tim[2]!=1) { datum.day=datum.day*10+date[dat]; strcat(Label45.Caption,"7"); } if(dat==5) tim[2]=1; if(dat==4) dat++; }
        if(dat>=6 && dat<=7) { if(tim[3]!=1) { datum.mon=datum.mon*10+date[dat]; strcat(Label71.Caption,"7"); } if(dat==7) tim[3]=1; if(dat==6) dat++; }
        if(dat>=8 && dat<=11) { if(tim[4]!=1) { datum.year=datum.year*10+date[dat]; strcat(Label72.Caption,"7"); } if(dat==11) tim[4]=1; dat++; }
        //dat++;
        DrawScreen(&Settings_Screen);
    }
}

void Button8OnClick() {
    if(i<=11){
        date[dat]=8;
        if(dat>=0 && dat<=1) { if(tim[0]!=1) { datum.hour=datum.hour*10+date[dat]; strcat(Label34.Caption,"8"); } if(dat==1) tim[0]=1; if(dat==0) dat++; }
        if(dat>=2 && dat<=3) { if(tim[1]!=1) { datum.min=datum.min*10+date[dat]; strcat(Label44.Caption,"8"); } if(dat==3) tim[1]=1; if(dat==2) dat++; }
        if(dat>=4 && dat<=5) { if(tim[2]!=1) { datum.day=datum.day*10+date[dat]; strcat(Label45.Caption,"8"); } if(dat==5) tim[2]=1; if(dat==4) dat++; }
        if(dat>=6 && dat<=7) { if(tim[3]!=1) { datum.mon=datum.mon*10+date[dat]; strcat(Label71.Caption,"8"); } if(dat==7) tim[3]=1; if(dat==6) dat++; }
        if(dat>=8 && dat<=11) { if(tim[4]!=1) { datum.year=datum.year*10+date[dat]; strcat(Label72.Caption,"8"); } if(dat==11) tim[4]=1; dat++; }
        //dat++;
        DrawScreen(&Settings_Screen);
    }
}

void Button9OnClick() {
    if(i<=11){
        date[dat]=9;
        if(dat>=0 && dat<=1) { if(tim[0]!=1) { datum.hour=datum.hour*10+date[dat]; strcat(Label34.Caption,"9"); } if(dat==1) tim[0]=1; if(dat==0) dat++; }
        if(dat>=2 && dat<=3) { if(tim[1]!=1) { datum.min=datum.min*10+date[dat]; strcat(Label44.Caption,"9"); } if(dat==3) tim[1]=1; if(dat==2) dat++; }
        if(dat>=4 && dat<=5) { if(tim[2]!=1) { datum.day=datum.day*10+date[dat]; strcat(Label45.Caption,"9"); } if(dat==5) tim[2]=1; if(dat==4) dat++; }
        if(dat>=6 && dat<=7) { if(tim[3]!=1) { datum.mon=datum.mon*10+date[dat]; strcat(Label71.Caption,"9"); } if(dat==7) tim[3]=1; if(dat==6) dat++; }
        if(dat>=8 && dat<=11) { if(tim[4]!=1) { datum.year=datum.year*10+date[dat]; strcat(Label72.Caption,"9"); } if(dat==11) tim[4]=1; dat++; }
        //dat++;
        DrawScreen(&Settings_Screen);
    }
}

void Button10OnClick() {
    if(i<=11){
        date[dat]=0;
        if(dat>=0 && dat<=1) { if(tim[0]!=1) { datum.hour=datum.hour*10+date[dat]; strcat(Label34.Caption,"0"); } if(dat==1) tim[0]=1; if(dat==0) dat++; }
        if(dat>=2 && dat<=3) { if(tim[1]!=1) { datum.min=datum.min*10+date[dat]; strcat(Label44.Caption,"0"); } if(dat==3) tim[1]=1; if(dat==2) dat++; }
        if(dat>=4 && dat<=5) { if(tim[2]!=1) { datum.day=datum.day*10+date[dat]; strcat(Label45.Caption,"0"); } if(dat==5) tim[2]=1; if(dat==4) dat++; }
        if(dat>=6 && dat<=7) { if(tim[3]!=1) { datum.mon=datum.mon*10+date[dat]; strcat(Label71.Caption,"0"); } if(dat==7) tim[3]=1; if(dat==6) dat++; }
        if(dat>=8 && dat<=11) { if(tim[4]!=1) { datum.year=datum.year*10+date[dat]; strcat(Label72.Caption,"0"); } if(dat==11) tim[4]=1; dat++; }
        //dat++;
        DrawScreen(&Settings_Screen);
    }
}

void Button11OnClick() {
    unsigned char i;
    strcpy(Label73.Caption,"");
    strcpy(Label75.Caption,"");
    strcpy(Label76.Caption,"");
    strcpy(Label77.Caption,"");
    strcpy(Label78.Caption,"");
    strcpy(Label34.Caption,"Hours:  ");
    strcpy(Label44.Caption,"Minutes:  ");
    strcpy(Label45.Caption,"Day:  ");
    strcpy(Label71.Caption,"Month:  ");
    strcpy(Label72.Caption,"Year:  ");
    for(i=0;i<12;i++) {
        date[i]=0;
    } 
    for(i=0;i<5;i++) {
        tim[i]=0;
    }
    dat=0;
    datum.hour=0;
    datum.min=0;
    datum.day=0;
    datum.mon=0;
    datum.year=0;
    Button39OnClick();
    DrawScreen(&Settings_Screen);    
}

void Button44OnClick() {
    if(tim[0]==1 && tim[1]==1 && tim[2]==1 && tim[3]==1 && tim[4]==1 && datum.hour<24 && datum.min<60 && datum.day<32 && datum.mon<13){
        datum2=datum;
        ByteToStr(datum2.hour,Label73.Caption);
        ByteToStr(datum2.min,Label75.Caption);
        ByteToStr(datum2.day,Label76.Caption);
        strcat(Label76.Caption,".");
        ByteToStr(datum2.mon,Label77.Caption);
        strcat(Label77.Caption,".");
        IntToStr(datum2.year,Label78.Caption);
        strcat(Label78.Caption,".");
    }
}

void Button39OnClick() {
    char i;
    if(ti!=0 && tim[0]!=1) {
        ti=0;
        dat=0;
        Button39.Color=0x00FF0000;
        Button40.Color=0x00800000;
        Button41.Color=0x00800000;
        Button42.Color=0x00800000;
        Button46.Color=0x00800000;
        Label34.Font_Color=0x00400000;
        Label44.Font_Color=0x00FFFFFF;
        Label45.Font_Color=0x00FFFFFF;
        Label71.Font_Color=0x00FFFFFF;
        Label72.Font_Color=0x00FFFFFF;
        
        strcpy(Label34.Caption,"Hour:  ");
        datum.hour=0;
        for(i=0;i<2;i++) {
            date[i]=0;
        }
        
        DrawScreen(&Settings_Screen);
    }
}

void Button40OnClick() {
    char i;
    if(ti!=1 && tim[1]!=1) {
        ti=1;
        dat=2;
        Button39.Color=0x00800000;
        Button40.Color=0x00FF0000;
        Button41.Color=0x00800000;
        Button42.Color=0x00800000;
        Button46.Color=0x00800000;
        Label34.Font_Color=0x00FFFFFF;
        Label44.Font_Color=0x00400000;
        Label45.Font_Color=0x00FFFFFF;
        Label71.Font_Color=0x00FFFFFF;
        Label72.Font_Color=0x00FFFFFF;
        
        strcpy(Label44.Caption,"Minutes:  ");
        datum.min=0;
        for(i=2;i<4;i++) {
            date[i]=0;
        }
        
        DrawScreen(&Settings_Screen);
    }
}

void Button41OnClick() {
    char i;
    if(ti!=2 && tim[2]!=1) {
        ti=2;
        dat=4;
        Button39.Color=0x00800000;
        Button40.Color=0x00800000;
        Button41.Color=0x00FF0000;
        Button42.Color=0x00800000;
        Button46.Color=0x00800000;
        Label34.Font_Color=0x00FFFFFF;
        Label44.Font_Color=0x00FFFFFF;
        Label45.Font_Color=0x00400000;
        Label71.Font_Color=0x00FFFFFF;
        Label72.Font_Color=0x00FFFFFF;
        
        strcpy(Label45.Caption,"Day:  ");
        datum.day=0;
        for(i=4;i<6;i++) {
            date[i]=0;
        }
        
        DrawScreen(&Settings_Screen);
    }
}

void Button42OnClick() {
    char i;
    if(ti!=3 && tim[3]!=1) {
        ti=3;
        dat=6;
        Button39.Color=0x00800000;
        Button40.Color=0x00800000;
        Button41.Color=0x00800000;
        Button42.Color=0x00FF0000;
        Button46.Color=0x00800000;
        Label34.Font_Color=0x00FFFFFF;
        Label44.Font_Color=0x00FFFFFF;
        Label45.Font_Color=0x00FFFFFF;
        Label71.Font_Color=0x00400000;
        Label72.Font_Color=0x00FFFFFF;
        
        strcpy(Label71.Caption,"Month:  ");
        datum.mon=0;
        for(i=6;i<8;i++) {
            date[i]=0;
        }
        
        DrawScreen(&Settings_Screen);    
    }
}

void Button46OnClick() {
    char i;
    if(ti!=4 && tim[4]!=1) {
        ti=4;
        dat=8;
        Button39.Color=0x00800000;
        Button40.Color=0x00800000;
        Button41.Color=0x00800000;
        Button42.Color=0x00800000;
        Button46.Color=0x00FF0000;
        Label34.Font_Color=0x00FFFFFF;
        Label44.Font_Color=0x00FFFFFF;
        Label45.Font_Color=0x00FFFFFF;
        Label71.Font_Color=0x00FFFFFF;
        Label72.Font_Color=0x00400000;
        
        strcpy(Label72.Caption,"Year:  ");
        datum.year=0;
        for(i=8;i<12;i++) {
            date[i]=0;
        }
                
        DrawScreen(&Settings_Screen);
    }
}

void Button12OnClick() {
    strcpy(Label23.Caption,Label94.Caption);
    strcat(Label23.Caption,"h");
    gr_pow=0;
    hom=1;
    DrawScreen(&Home);    
}

void Box35OnClick() {
    DrawScreen(&Keyboard_Screen);
    hom=0;
}

void Box37OnClick() {
    choice_t=0;
    Write_Log("Heating mode: Home");
    Draw_state(choice_t);
}

void Box38OnClick() {
    choice_t=1;
    Write_Log("Heating mode: Night");
    Draw_state(choice_t);
}

void Box39OnClick() {
    choice_t=2;
    Write_Log("Heating mode: Day");
    Draw_state(choice_t);
}

void Box40OnClick() {
    choice_t=3;
    Write_Log("Heating mode: Party");
    Draw_state(choice_t);    
}

void CircleButton1OnPress() {
    if(Box47.Left<Box23.Left) Box23.Left-=1;
    t_t[choice_t]=15+(int)floor((Box23.Left-181)*15/81);
    temp_t[choice_t]=Box23.Left;
    IntToStr(t_t[choice_t],Label95.Caption);
    DrawScreen(&Temperature_Mode);
}

void CircleButton2OnPress() {
    if(Box48.Left<Box49.Left) Box49.Left-=1;
    h_t[choice_t]=20+(int)floor((Box49.Left-181)*60/81);
    hum_t[choice_t]=Box49.Left;
    IntToStr(h_t[choice_t],Label96.Caption);
    DrawScreen(&Temperature_Mode);
}

void CircleButton3OnPress() {
    if((Box47.Left+Box47.Width)>(Box23.Left+Box23.Width)) Box23.Left+=1;
    t_t[choice_t]=15+(int)floor((Box23.Left-181)*15/81);
    temp_t[choice_t]=Box23.Left;
    IntToStr(t_t[choice_t],Label95.Caption);
    DrawScreen(&Temperature_Mode);
}

void CircleButton4OnPress() {
    if((Box48.Left+Box48.Width)>(Box49.Left+Box49.Width)) Box49.Left+=1;
    h_t[choice_t]=20+(int)floor((Box49.Left-181)*60/81);
    hum_t[choice_t]=Box49.Left;
    IntToStr(h_t[choice_t],Label96.Caption);
    DrawScreen(&Temperature_Mode);
}

void Box51OnClick() {
    choice_t=0;
    Write_Log("Lighting mode: Media");
    Draw_state(choice_t);
}

void Box52OnClick() {
    choice_t=1;
    Write_Log("Lighting mode: Relax");
    Draw_state(choice_t);
}

void Box53OnClick() {
    choice_t=2;
    Write_Log("Lighting mode: Dinner");
    Draw_state(choice_t);
}

void Box55OnClick() {
    choice_t=3;
    Write_Log("Lighting mode: Party");
    Draw_state(choice_t);
}

void CircleButton5OnPress() {
    if(Box62.Left<Box64.Left) Box64.Left-=1;
    d_l[choice_t]=0+(int)floor((Box64.Left-181)*100/81);
    dim_l[choice_t]=Box64.Left;
    IntToStr(d_l[choice_t],Label97.Caption);
    DrawScreen(&Light_Mode);
}

void CircleButton6OnPress() {
    if(Box63.Left<Box65.Left) Box65.Left-=1;
    h_l[choice_t]=15+(int)floor((Box65.Left-181)*15/81);
    hum_l[choice_t]=Box65.Left;
    IntToStr(h_l[choice_t],Label98.Caption);
    DrawScreen(&Light_Mode);
}

void CircleButton7OnPress() {
    if((Box62.Left+Box62.Width)>(Box64.Left+Box64.Width)) Box64.Left+=1;
    d_l[choice_t]=0+(int)floor((Box64.Left-181)*100/81);
    dim_l[choice_t]=Box64.Left;
    IntToStr(d_l[choice_t],Label97.Caption);
    DrawScreen(&Light_Mode);
}

void CircleButton8OnPress() {
    if((Box63.Left+Box63.Width)>(Box65.Left+Box65.Width)) Box65.Left+=1;
    h_l[choice_t]=15+(int)floor((Box65.Left-181)*15/81);
    hum_l[choice_t]=Box65.Left;
    IntToStr(h_l[choice_t],Label98.Caption);
    DrawScreen(&Light_Mode);
}

void Button13OnClick() {
    if(i<=5){
        arm[i]=1;
        i++;
        strcat(Label24.Caption,"1");
        DrawScreen(&System_Log_Screen);
    }
}

void Button14OnClick() {
    if(i<=5){
        arm[i]=1;
        i++;
        strcat(Label24.Caption,"2");
        DrawScreen(&System_Log_Screen);
    }
}

void Button15OnClick() {
    if(i<=5){
        arm[i]=1;
        i++;
        strcat(Label24.Caption,"3");
        DrawScreen(&System_Log_Screen);
    }
}

void Button16OnClick() {
    if(i<=5){
        arm[i]=1;
        i++;
        strcat(Label24.Caption,"4");
        DrawScreen(&System_Log_Screen);
    }
}

void Button17OnClick() {
    if(i<=5){
        arm[i]=1;
        i++;
        strcat(Label24.Caption,"5");
        DrawScreen(&System_Log_Screen);
    }
}

void Button18OnClick() {
    if(i<=5){
        arm[i]=1;
        i++;
        strcat(Label24.Caption,"6");
        DrawScreen(&System_Log_Screen);
    }
}

void Button19OnClick() {
    if(i<=5){
        arm[i]=1;
        i++;
        strcat(Label24.Caption,"7");
        DrawScreen(&System_Log_Screen);
    }
}

void Button20OnClick() {
    if(i<=5){
        arm[i]=1;
        i++;
        strcat(Label24.Caption,"8");
        DrawScreen(&System_Log_Screen);
    }
}

void Button21OnClick() {
    if(i<=5){
        arm[i]=1;
        i++;
        strcat(Label24.Caption,"9");
        DrawScreen(&System_Log_Screen);
    }
}

void Button22OnClick() {
    if(i<=5){
        arm[i]=1;
        i++;
        strcat(Label24.Caption,"0");
        DrawScreen(&System_Log_Screen);
    }
}

void Button23OnClick() {
    if(i<=5){
        arm[i]=0x0A;
        i++;
        strcat(Label24.Caption,"A");
        DrawScreen(&System_Log_Screen);
    }
}

void Button24OnClick() {
    if(i<=5){
        arm[i]=0x0B;
        i++;
        strcat(Label24.Caption,"B");
        DrawScreen(&System_Log_Screen);
    }
}

void Button25OnClick() {
    if(i<=5){
        arm[i]=0x0C;
        i++;
        strcat(Label24.Caption,"C");
        DrawScreen(&System_Log_Screen);
    }
}

void Button26OnClick() {
    if(i<=5){
        arm[i]=0x0D;
        i++;
        strcat(Label24.Caption,"D");
        DrawScreen(&System_Log_Screen);
    }
}

void Button27OnClick() {
    char cnt;
    for(cnt=0;cnt<=5;cnt++) arm[cnt]=0;
    i=0;
    strcpy(Label24.Caption,"");
    DrawScreen(&System_Log_Screen);
}

void Button28OnClick() {
    unsigned char l,cnt[4];
    if(i==6) {
        if(a==0) {  
            a=1; 
            i=0; 
            InitMCU();
            SF_Start();
            SF_mem();
//            Write_Log("Security System armed");             
        }
        else {  
            i=0;
            InitMCU();            
            SF_Start();
            SF_rem();
//            Write_Log("Security System disarmed");   
        }
        DrawScreen(&System_Log_Screen);
        Delay_ms(2000);
        strcpy(Label24.Caption," ");
        hom=1;
        DrawScreen(&Home);
    }
}

void Box10OnClick() {
    Image67.Visible=0;
    Image68.Visible=1;
    Image69.Visible=1;
    Image70.Visible=0;
    Label54.Font_Color=0x00FFFF00;
    Label55.Font_Color=0x00FFFFFF;
    DrawScreen(&System_Log_Screen);
}

void Box25OnClick() {
    Image67.Visible=1;
    Image68.Visible=0;
    Image69.Visible=0;
    Image70.Visible=1;
    Label54.Font_Color=0x00FFFFFF;
    Label55.Font_Color=0x00FFFF00;
    DrawScreen(&System_Log_Screen);
}

void Box24OnClick() {
    CheckBox1.Checked=1-CheckBox1.Checked;
    DrawScreen(&Keyboard_Screen);
    vol=255-vol;
    FT812_Sound_SetAndPlay(_FT812_SOUND_XYLOPHONE,_FT812_SOUND_PITCH_B3,vol);
    Delay_ms(1000);
}

void Box76OnClick() {
    CheckBox2.Checked=1-CheckBox2.Checked;
    DrawScreen(&Keyboard_Screen);
    if(CheckBox2.Checked==1) {
        GPIO_Pin_Digital_Output(_GPIO_PIN_NUM_53);
        GPIO_PIN53_bit = 1;
        Delay_ms(444);
        GPIO_PIN53_bit = 0;
    }
    else {
        GPIO_Pin_Digital_Input(_GPIO_PIN_NUM_53);
        GPIO_PIN53_bit = 0;
    }
}

void Box77OnClick() {
    CheckBox3.Checked=1-CheckBox3.Checked;
    if(CheckBox3.Checked==1) FT812_PWM_Duty(64); 
    else FT812_PWM_Duty(128);
    DrawScreen(&Keyboard_Screen);
}

void Box78OnClick() {
    CheckBox4.Checked=1-CheckBox4.Checked;
    DrawScreen(&Keyboard_Screen);
    Delay_ms(1000);
    Button12OnClick();
    Delay_ms(4000);
    Box1OnClick();
    Delay_ms(2500);
    Box38OnClick();
    Delay_ms(2500);
    Box39OnClick();
    Delay_ms(2500);
    Box40OnClick();
    Delay_ms(2500);
    Button12OnClick();
    Delay_ms(4000);
    Box2OnClick();
    Delay_ms(2500);
    Box52OnClick();
    Delay_ms(2500);
    Box53OnClick();
    Delay_ms(2500);
    Box55OnClick();
    Delay_ms(2500);
    Button12OnClick();
    Delay_ms(4000);
    Box13OnClick();
    Delay_ms(2500);
    Box10OnClick();
    Delay_ms(2500);
    Box25OnClick();
    Delay_ms(2500);
    Button13OnClick();
    Delay_ms(500);
    Button14OnClick();
    Delay_ms(500);
    Button15OnClick();
    Delay_ms(500);
    Button16OnClick();
    Delay_ms(500);
    Button17OnClick();
    Delay_ms(500);
    Button18OnClick();
    Delay_ms(500);
    Button28OnClick();
    Delay_ms(4000);
    Box13OnClick();
    Delay_ms(2500);
    Button13OnClick();
    Delay_ms(500);
    Button14OnClick();
    Delay_ms(500);
    Button15OnClick();
    Delay_ms(500);
    Button16OnClick();
    Delay_ms(500);
    Button17OnClick();
    Delay_ms(500);
    Button18OnClick();
    Delay_ms(500);
    Button28OnClick();
    Delay_ms(4000);
    Box20OnClick();
    Delay_ms(10000);
    Button12OnClick();
    Delay_ms(4000);
    Box21OnClick();
    Delay_ms(10000);
    Button12OnClick();
    CheckBox4.Checked=1-CheckBox4.Checked;
}

void Box12OnClick() {
    Box3.Color=0x00808080;
    Box5.Color=0x00808080;
    Box7.Color=0x00808080;
    Image21.Visible=0;
    Image23.Visible=0;
    Image25.Visible=0;
    Label36.Font_Color=0x00FFFFFF;
    Label38.Font_Color=0x00FFFFFF;
    Label40.Font_Color=0x00FFFFFF;
    Image10.Visible=1;
    Image12.Visible=1;
    Image14.Visible=1;
    DrawScreen(&Home);
}

void Box3OnClick() {
    if(Box3.Color==0x00808080) {
        Box3.Color=0x00C0C0C0;
        Label36.Font_Color=0x00000000;
        Image10.Visible=0;
        Image21.Visible=1;
    }
    else {
        Box3.Color=0x00808080;
        Label36.Font_Color=0x00FFFFFF;
        Image10.Visible=1;
        Image21.Visible=0;
    }
    DrawScreen(&Home);
}

void Box5OnClick() {
    if(Box5.Color==0x00808080) {
        Box5.Color=0x00C0C0C0;
        Label38.Font_Color=0x00000000;
        Image12.Visible=0;
        Image23.Visible=1;
    }
    else {
        Box5.Color=0x00808080;
        Label38.Font_Color=0x00FFFFFF;
        Image12.Visible=1;
        Image23.Visible=0;
    }
    DrawScreen(&Home);
}

void Box7OnClick() {
    if(Box7.Color==0x00808080) {
        Box7.Color=0x00C0C0C0;
        Label40.Font_Color=0x00000000;
        Image14.Visible=0;
        Image25.Visible=1;
    }
    else {
        Box7.Color=0x00808080;
        Label40.Font_Color=0x00FFFFFF;
        Image14.Visible=1;
        Image25.Visible=0;
    }
    DrawScreen(&Home);
}