#define I2C_LCD_ADDR 0x4E

#define I2C_LCD_Delay_Ms(u16DelayMs) delay_ms(u16DelayMs)

#define LCD_EN 2
#define LCD_RW 1
#define LCD_RS 0
#define LCD_D4 4
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7
#define LCD_BL 3


#bit SSPM3 =0x14.3 
#byte SSPADD =0x93 
#byte SSPBUF =0x13 
#bit SSPEN =0x14.5 
#bit SEN =0x91.0 
#bit PEN =0x91.2
#bit SSPIF =0x0C.3 
#bit ACKSTAT =0x91.6
#bit ACKDT =0x91.5 
#bit RCEN =0x91.3 
#bit ACKEN =0x91.4 
#bit RSEN =0x91.1 
#byte TRISC =0x87 
void i2c_init1(void)
{
 
    SSPM3 = 1;//SSPM3:SSPM0: Synchronous Serial Port Mode Select bits
    SSPADD = 0x09;// clock = FOSC / (4 * (SSPADD+1) ) 
    SSPEN = 1; //1 = Enables the serial port and configures the SDA and SCL pins as the source of the serial port pins
}
void i2c_start1(void)
{

    SEN = 1;//1 = Initiate Start condition on SDA and SCL pins. Automatically cleared by hardware.
    while (SEN) {

    }
}

void i2c_stop1(void)
{
    
    PEN = 1;
    while (PEN) {
        
    }
}

uint8_t i2c_write1(uint8_t u8Data)
{

    SSPIF = 0;
    SSPBUF = u8Data;
    while (SSPIF == 0) {

    }

    return ACKSTAT;
}

uint8_t i2c_read1(uint8_t u8Ack)
{
    uint8_t tmp;

    RCEN = 1;
    while (RCEN) {

    }
    tmp = SSPBUF;
    RCEN = 0;
    if (u8Ack) {
        ACKDT = 1;
    } else {
        ACKDT = 0;
    }
    
    ACKEN = 1;
    while (ACKEN) {

    }

    return tmp;
}

void i2c_repeat_start1(void)
{
    
    RSEN = 1;
    while (RSEN) {

    }
}




void My_I2C_Init(void)
{

   i2c_init1();
}

void I2C_Write1(uint8_t u8Address, uint8_t *pu8Data, uint8_t u8Length)
{
   uint8_t i;

   i2c_start1();
   i2c_write1(u8Address);
   for (i = 0; i < u8Length; ++i) {
      i2c_write1(pu8Data[i]);
   }
   i2c_stop1();
}

void I2C_Read1(uint8_t u8Address, uint8_t *pu8Data, uint8_t u8Length)
{
   uint8_t i;

   i2c_start1();
   i2c_write1(u8Address);
   for (i = 0; i < u8Length - 1; ++i) {
      pu8Data[i] = i2c_read1(0);
   }
   pu8Data[i] = i2c_read1(1);
   i2c_stop1();
}

static uint8_t u8LCD_Buff[8];//bo nho dem luu lai toan bo
static uint8_t u8LcdTmp;

#define   MODE_4_BIT      0x28
#define   CLR_SCR         0x01
#define   DISP_ON         0x0C
#define   CURSOR_ON      0x0E
#define   CURSOR_HOME      0x80

static void I2C_LCD_Write_4bit(uint8_t u8Data);
static void I2C_LCD_FlushVal(void);
static void I2C_LCD_WriteCmd(uint8_t u8Cmd);

static void I2C_LCD_FlushVal(void)
{
   uint8_t i;
   
   for (i = 0; i < 8; ++i) {
      u8LcdTmp >>= 1;
      if(u8LCD_Buff[i]) {
         u8LcdTmp |= 0x80;
      }
   }
   I2C_Write1(I2C_LCD_ADDR, &u8LcdTmp, 1);
}

void I2C_LCD_Init(void)
{
   uint8_t i;
   
  
   
   My_I2C_Init();
   
   for (i = 0; i < 8; ++i) {
      u8LCD_Buff[i] = 0;
   }
   
   I2C_LCD_FlushVal();
   delay_ms(30);
   u8LCD_Buff[LCD_RS] = 0;
   I2C_LCD_FlushVal();
   
   u8LCD_Buff[LCD_RW] = 0;
   I2C_LCD_FlushVal();
   
   I2C_LCD_Write_4bit(0x03);
   I2C_LCD_Delay_Ms(5);
   
   I2C_LCD_Write_4bit(0x03);
   I2C_LCD_Delay_Ms(5);
   
   I2C_LCD_Write_4bit(0x03);
   I2C_LCD_Delay_Ms(5);
   
   I2C_LCD_Write_4bit(MODE_4_BIT >> 4);
   I2C_LCD_Delay_Ms(1);
   
   I2C_LCD_WriteCmd(MODE_4_BIT);
   I2C_LCD_WriteCmd(DISP_ON);
   I2C_LCD_WriteCmd(CURSOR_ON);
   I2C_LCD_WriteCmd(CLR_SCR);
}
static void I2C_LCD_Write_4bit(uint8_t u8Data)
{
   //4 bit can ghi chinh la 4 5 6 7
   //dau tien gan LCD_E=1
   //ghi du lieu
   //sau do gan LCD_E=0

   if(u8Data & 0x08) {
      u8LCD_Buff[LCD_D7] = 1;
   } else {
      u8LCD_Buff[LCD_D7] = 0;
   }
   if(u8Data & 0x04) {
      u8LCD_Buff[LCD_D6] = 1;
   } else {
      u8LCD_Buff[LCD_D6] = 0;
   }
   if(u8Data & 0x02) {
      u8LCD_Buff[LCD_D5] = 1;
   } else {
      u8LCD_Buff[LCD_D5] = 0;
   }
   if(u8Data & 0x01) {
      u8LCD_Buff[LCD_D4] = 1;
   } else {
      u8LCD_Buff[LCD_D4] = 0;
   }
   
   u8LCD_Buff[LCD_EN] = 1;
   I2C_LCD_FlushVal();   
   
   u8LCD_Buff[LCD_EN] = 0;
   I2C_LCD_FlushVal();
   
}

void LCD_WaitBusy(void)
{
   
 /* delay_ms(1);*/
 char temp;
   
   //dau tien ghi tat ca 4 bit thap bang 1
   u8LCD_Buff[LCD_D4] = 1;
   u8LCD_Buff[LCD_D5] = 1;
   u8LCD_Buff[LCD_D6] = 1;
   u8LCD_Buff[LCD_D7] = 1;
   I2C_LCD_FlushVal();
   
   u8LCD_Buff[LCD_RS] = 0;
   I2C_LCD_FlushVal();
   
   u8LCD_Buff[LCD_RW] = 1;
   I2C_LCD_FlushVal();
   
   do {
      u8LCD_Buff[LCD_EN] = 1;
      I2C_LCD_FlushVal();
      I2C_Read1(I2C_LCD_ADDR + 1, &temp, 1);
      
      u8LCD_Buff[LCD_EN] = 0;
      I2C_LCD_FlushVal();
      u8LCD_Buff[LCD_EN] = 1;
      I2C_LCD_FlushVal();
      u8LCD_Buff[LCD_EN] = 0;
      I2C_LCD_FlushVal();
   } while (temp & 0x08);
   
}


static void I2C_LCD_WriteCmd(uint8_t u8Cmd)
{
   
   LCD_WaitBusy();

   u8LCD_Buff[LCD_RS] = 0;
   I2C_LCD_FlushVal();
   
   u8LCD_Buff[LCD_RW] = 0;
   I2C_LCD_FlushVal();
   
   I2C_LCD_Write_4bit(u8Cmd >> 4);
   I2C_LCD_Write_4bit(u8Cmd);
}

void LCD_Write_Chr(char chr)
{
   
   LCD_WaitBusy();
   u8LCD_Buff[LCD_RS] = 1;
   I2C_LCD_FlushVal();
   u8LCD_Buff[LCD_RW] = 0;
   I2C_LCD_FlushVal();
   I2C_LCD_Write_4bit(chr >> 4);
   I2C_LCD_Write_4bit(chr);
   
}

void I2C_LCD_Puts(char *sz)
{
   
   while (1) {
      if (*sz) {
         LCD_Write_Chr(*sz++);
      } else {
         break;
      }
   }
}

void I2C_LCD_Clear(void)
{
   
   I2C_LCD_WriteCmd(CLR_SCR);
}

void I2C_LCD_NewLine(void)
{
   
   I2C_LCD_WriteCmd(0xc0);
}

void I2C_LCD_BackLight(uint8_t u8BackLight)
{
   
   if(u8BackLight) {
      u8LCD_Buff[LCD_BL] = 1;
   } else {
      u8LCD_Buff[LCD_BL] = 0;
   }
   I2C_LCD_FlushVal();
}
void move_LCD(uint8_t y,uint8_t x){
       uint8_t Ad;
       Ad=64*(y-1)+(x-1)+0x80; // tính mã l?nh
       u8LCD_Buff[LCD_RS] = 0; // the following data is COMMAND
       I2C_LCD_FlushVal();
       I2C_LCD_Write_4bit(Ad >> 4);
       I2C_LCD_Write_4bit(Ad);
       LCD_WaitBusy();
       I2C_LCD_WriteCmd(CURSOR_ON);
       }
int32_t luythua(int32_t a){
   int32_t b,i;
   b=1;
   for(i=1;i<=a;i++){
   b=10*b;
   }
   return b;
}
void tachso(int32_t a)
{
   int32_t b,d,e;
   int32_t c;
   char tempt[]="a";
   c=0;
   d=0;
   
   b=a;
   
   while(b>9){
       b=b/10;
       c++;
  
}
   if(a<10)
   {
      LCD_Write_Chr('0');
   }
   while(c>=0){
   
       e=a/(luythua(c));
       tempt[0]='0'+e;
       I2C_LCD_Puts(tempt);
       d++;
       
       a=a-e*luythua(c);
       c--;   
}
   
}
void tachso2(int32_t a)
{
   int32_t b,d,e;
   int32_t c;
   char tempt[]="a";
   c=0;
   d=0;
   
   b=a;
   
   while(b>9){
       b=b/10;
       c++;
  
}
   
   if(a<10)
   {
      LCD_Write_Chr('0');
      LCD_Write_Chr('0');
   }
   else if(a<100)
   {
      LCD_Write_Chr('0');
   }
   while(c>=0){
   
       e=a/(luythua(c));
       tempt[0]='0'+e;
       I2C_LCD_Puts(tempt);
       d++;
       
       a=a-e*luythua(c);
       c--;   
}
   
}
