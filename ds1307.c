#include <ds1307.h>
#include <stdint.h>
#include <i2clcd1.h>
#include <dt1307.h>
#include <keypad.h>
#byte OPTION_REG =0x81
#byte TMR0 =0x01
#bit tmr0ie = 0x0b.5 
#bit gie = 0x0b.7 
#bit pie = 0x0b.6 
#bit tmr0if = 0x0b.2 
   int32_t gio=0;
   int32_t phut=0;
   int32_t giay=0;
   int32_t tictac=0;
   int MODE=0;
   char pad;
   int alarm;
   rtc_t p,p2;
   uint8_t light=0x00;
   int y=0;
 int mang[]={0,0,0,0,0,0,0,0,0};
 int baothuc[]={0,0,0,0,0,0,0,0,0};
 int limit[]={0,0,23,0,0,59,0,0,59};
 char day[7][4]={"Mon","Tue","Wed","Thu","Fri","Sat","Sun"};
 void checkalarm(rtc_t *rtc);
 int convert(uint8_t bien);
 void checkmode();
 void denlcd();
 int baonhieungay(int a,int b);
 uint8_t inttohex(int a);
#INT_TIMER0
void Timer0_isr(void)
{  

    if(pad=='1'&&pad!='2')
       {
           tictac++;
           
           if(tictac==999)
           {
               tictac=0;
               giay++;
               if(giay==60)
               {
                  giay=0;
                  phut++;
                  if(phut==60)
                  {
                     gio++;
                  }
               }
           }
           TMR0=100;  
       }
    
            
       
       
   }

void checkalarm1(rtc_t *rtc)
{
   char a;
   
   if(convert(rtc->hour)==baothuc[2])
      if(convert(rtc->min)==baothuc[5])
         if(convert(rtc->sec)==baothuc[8])
         {
         while(1)
         {
         
         I2C_LCD_BackLight(1);
         delay_ms(500);
         I2C_LCD_BackLight(0);
         delay_ms(500);
         if(checkpad()=='4')
            {
               alarm=0;
               while(checkpad()=='4'){};
               I2C_LCD_BackLight(1);
               break;
               
            }
         }
    
         }
    
        
}   
void checkalarm(rtc_t *rtc)
{
   char a;
   RTC_GetTime(rtc);
   if(convert(rtc->hour)==baothuc[2])
      if(convert(rtc->min)==baothuc[5])
         if(convert(rtc->sec)==baothuc[8])
         {
         while(1)
         {
         
         I2C_LCD_BackLight(1);
         delay_ms(500);
         I2C_LCD_BackLight(0);
         delay_ms(500);
         if(checkpad()=='4')
            {
               alarm=0;
               while(checkpad()=='4'){};
               I2C_LCD_BackLight(1);
               break;
               
            }
         }
    
         }
 delay_ms(100);     
        
}   
int convert(uint8_t bien)
{
   int a;
   a=(bien>>4)*10+(bien&0x0F);;
   return a;
}

void hienthigio3()
{  
    I2C_LCD_Clear();
    move_LCD(1,1);
    char c[]="Stop Watch";
    gio=0;
    phut=0;
    giay=0;
    tictac=0;
    pad='2';
    OPTION_REG = (1<<2);  // Timer0 with external freq and 32 as prescalar
    TMR0=100;       // Load the time value for 1ms delay
    TMR0IE=1;       //Enable timer interrupt bit in PIE1 register
    GIE=1;          //Enable Global Interrupt
    PIE=1; 
    I2C_LCD_Puts(c);
  while(1){
  
   move_LCD(2,1);
   tachso(gio);
   LCD_Write_Chr(':');
   tachso(phut);
   LCD_Write_Chr(':');
   tachso(giay);
   LCD_Write_Chr(':');
   tachso2(tictac);
   if(checkpad()!=0&&checkpad()<'5')
   {
      pad=checkpad();
      if(checkpad()!=0)
   {
      if(old==keyin){
      
      }
      else{
      if(pad=='1')
      {
      while(checkpad()=='1'){};
          y++;
          
          if(y%2==0) {pad='2';}
          
      }
      if(pad=='3')
      {
          gio=0;
          phut=0;
          giay=0;
          tictac=0;
      }
      if(pad=='4')
      {
          MODE=0;
          I2C_LCD_Clear();
          
          while(checkpad()=='4'){};
          break;
      }
   }
   }
   
   old=keyin;
   if(MODE!=3) break;
  }  
  }
    TMR0IE=0;       //Enable timer interrupt bit in PIE1 register
    GIE=0;          //Enable Global Interrupt
    PIE=0;    

}

void hengio2()
{  
   I2C_LCD_Clear();
   uint32_t count=0;
   uint32_t a;
   old=0;
   int vitri=8;
   pad='m';
   move_LCD(1,1);
   char c[]="Set Alarm";
   char d[]="00:00:00";
   I2C_LCD_Puts(c);
   move_LCD(2,1);
   I2C_LCD_Puts(d);
   move_LCD(2,8);
   
   
   RTC_GetTime(&p);
   
   while(1){
   
   if((baothuc[2]-convert(p.hour))!=0)
      a=50000;
   else if((baothuc[5]-convert(p.min))<=1)
      a=2000;
   else a=8000;
   if(checkpad()!=0)
   {
      if(old==keyin){
      
      }
      else{
         pad=checkpad();
         if(checkpad()=='4')
         {  
            vitri=vitri+3;
            if(vitri>8)
            {
               vitri=2;
            }
            move_LCD(2,vitri);
         }
         if(checkpad()=='1')
         {  
         if(mang[vitri]<=limit[vitri])
         {
             move_LCD(2,vitri-1);
             mang[vitri]++;
             
             tachso(mang[vitri]);
         }
         }
         if(checkpad()=='1')
         {  
         if(mang[vitri]>limit[vitri])
         {
             
             mang[vitri]=limit[vitri-1];
             move_LCD(2,vitri-1);
             tachso(mang[vitri]);
         }
         }
         if(checkpad()=='3')
         {
            alarm=1;
            baothuc[2]=mang[2];
            baothuc[5]=mang[5];
            baothuc[8]=mang[8];
            mang[2]=0;
            mang[5]=0;
            mang[8]=0;
            MODE=0;
            I2C_LCD_Clear();
            break;
         }
          
            
         
      }
   
   }
   count++;
   if(count>=a&&alarm==1)
   {
   checkalarm(&p); 
   count=0;
   }
   old=keyin;
   
   if(MODE!=2) break;
}
}
void hienthi()
{



   
   while(checkpad()==4){};
   RTC_GetTime(&p);
   RTC_GetDate(&p);
   move_LCD(1,1);
   tachso(convert(p.hour));
   LCD_Write_Chr(':');
   tachso(convert((p.min)));
   LCD_Write_Chr(':');
   tachso(convert(p.sec));
   
   move_LCD(2,1);
   I2C_LCD_Puts(day[convert(p.Weekday)-2]);
   LCD_Write_Chr('/');
   tachso(convert(p.date));
   LCD_Write_Chr('/');
   tachso(convert(p.month));
   
   if(MODE!=1) denlcd();
  
   if (alarm==1) checkalarm(&p);
  
  delay_ms(450);
   

}
void checkmode()
{
   uint16_t count=0;
   while(checkpad()=='3')
   {
      count++;
      delay_ms(5);
      
   }
   if(count>=50) MODE=1;
   if(count>=150) MODE=2;
   if(count>=300) MODE=3;
}
void denlcd(){
int a;
   if(checkpad()=='4')
   {
      a=(light&0x01);
      I2C_LCD_BackLight(a);
      light=~light;
      while(checkpad()=='4'){};
   }
   
}
void chinhgio1()
{
   I2C_LCD_Clear();
   hienthi();
   int vitri=8;
   uint8_t dong=0x00;
   int dong1=1;
   int chinhgio[]={0,0,0,0,0,0,0,0,0,0};
   int limitday[]={0,2,8,0,1,28,0,1,12};
   chinhgio[3]=convert(p.Weekday);
   chinhgio[2]=convert(p.hour);
   chinhgio[5]=convert(p.min);
   chinhgio[8]=convert(p.sec);
   chinhgio[6]=convert(p.date);
   chinhgio[9]=convert(p.month);
   chinhgio[0]=convert(p.year);
   move_LCD(1,8);
   
   while(1){
   
   if(checkpad()!=0)
   {
   
      if(old==keyin){
      
      }
      else{
      limitday[5]=baonhieungay(chinhgio[9],chinhgio[0]);
      if(chinhgio[6]>limitday[5]) 
      {
      chinhgio[6]=limitday[5];
      move_LCD(2,5);
      tachso(chinhgio[6]);
      move_LCD(1,8);
      vitri=8;
      }
         pad=checkpad();
         if(checkpad()=='4')
         {  
            vitri=vitri+3;
            if(vitri>9)
            {
               
               if (dong1==1)vitri=3;
               else vitri=2;
               dong=~dong;
               dong1=(dong&0x01)+1;
   
            }
            
             move_LCD(dong1,vitri);
         }
         if(checkpad()=='1')
            {
            if(dong1==1)
               {
               if(chinhgio[vitri]<=limit[vitri])
               {
                   move_LCD(dong1,vitri-1);
                   chinhgio[vitri]++;
                   
                   tachso(chinhgio[vitri]);
               }
               }
             if(dong1==2)
               {
               if((vitri==3)&&(chinhgio[3]<=limitday[2])) 
               {
                  chinhgio[3]++;
                  move_LCD(2,1);
                  I2C_LCD_Puts(day[chinhgio[3]-2]);
               }
               else if(chinhgio[vitri]<=limitday[vitri-1])
               {
                   move_LCD(dong1,vitri-1);
                   chinhgio[vitri]++;
                   
                   tachso(chinhgio[vitri]);
               }
               }
            }
         if(checkpad()=='1')
         {  
            if(dong1==1)
            {
               if(chinhgio[vitri]>limit[vitri])
               {
                   
                   chinhgio[vitri]=limit[vitri-1];
                   move_LCD(dong1,vitri-1);
                   tachso(chinhgio[vitri]);
               }
            }
            if(dong1==2)
               {
               if((vitri==3)&&(chinhgio[3]>limitday[2])) 
               {
                  chinhgio[3]= limitday[1];
                  move_LCD(2,1);
                  I2C_LCD_Puts(day[chinhgio[3]-2]);
               }
               else if(chinhgio[vitri]>limitday[vitri-1])
               {
                   move_LCD(dong1,vitri-1);
                   chinhgio[vitri]=limitday[vitri-1];
                   
                   tachso(chinhgio[vitri]);
               }
               }
               
          }
            
         
         if(checkpad()=='3')
         {
               p2.hour=inttohex(chinhgio[2]);
               p2.min=inttohex(chinhgio[5]);
               p2.sec=inttohex(chinhgio[8]);
               p2.date=inttohex(chinhgio[6]);
               p2.month=inttohex(chinhgio[9]);
               p2.Weekday=inttohex(chinhgio[3]);
               RTC_SetDateTime(&p2);
               MODE=0;
               I2C_LCD_Clear();
               break;
         }
          
            
         
      }
   
   }
   old=keyin;
   
   if(MODE!=1) break;
}


}
uint8_t inttohex(int a)
{
   uint8_t d;
   int b,c;
   b=a/10;
   c=a-b*10;
   d=(b<<4)+c;
   return d;
}
int baonhieungay(int a,int b)
{
   int c;
   if(a==4||a==6||a==9||a==11)
      {
         c=30;
      }
   else c=31;
   if(a==2) c=28;
   if((a==2)&&(b!=0)&&(b%4==0)) c=29;
   return c;
      
      
}
void main()
{
     //Enable the Peripheral Interrupt
   TRISB=0x0F;
   PORTB=0xF0;
   
   rbpu=0;
   I2C_LCD_Init();
   I2C_LCD_Clear();
   I2C_LCD_BackLight(1);
   RTC_Init();
   rtc_t p1;
   p1.hour=0x00;
   p1.min=0x00;
   p1.sec=0x00;
   p1.Weekday=0x03;
   p1.date=0x25;
   p1.month=0x4;
   p1.year=0x24;
   RTC_SetDateTime(&p1);
   MODE=0;
   
  
  
   while(TRUE)
   {
      checkmode();
      if (MODE==1) chinhgio1();
      if(MODE==2) hengio2();
      if (MODE==3) hienthigio3();
      hienthi();
      
      
      
   }
}
