#define C_Ds1307ReadMode_U8   0xD1u // DS1307 ID in read mode
#define C_Ds1307WriteMode_U8  0xD0
#define C_Ds1307SecondRegAddress_U8   0x00   // Address to access Ds1307 SEC register
#define C_Ds1307DateRegAddress_U8     0x04   // Address to access Ds1307 DATE register
#define C_Ds1307ControlRegAddress_U8  0x07 

typedef struct
{
  uint8_t sec;
  uint8_t min;
  uint8_t hour;
  uint8_t weekDay;
  uint8_t date;
  uint8_t month;
  uint8_t year;  
}rtc_t;

void RTC_Init()
{
    My_I2C_Init();                             // Initialize the I2c module.
    i2c_start1();                          // Start I2C communication
 
    i2c_write1(C_Ds1307WriteMode_U8);        // Connect to DS1307 by sending its ID on I2c Bus
    i2c_write1(C_Ds1307ControlRegAddress_U8);// Select the Ds1307 ControlRegister to configure Ds1307
 
    i2c_write1(0x00);                        // Write 0x00 to Control register to disable SQW-Out
 
    i2c_stop1();                             // Stop I2C communication after initializing DS1307
}
void RTC_GetTime(rtc_t *rtc)
{
    i2c_start1();                             // Start I2C communication
 
    i2c_write1(C_Ds1307WriteMode_U8);        // connect to DS1307 by sending its ID on I2c Bus
    i2c_write1(C_Ds1307SecondRegAddress_U8); // Request Sec RAM address at 00H
 
    i2c_stop1();                                // Stop I2C communication after selecting Sec Register
 
    i2c_start1();                               // Start I2C communication
    i2c_write1(C_Ds1307ReadMode_U8);            // connect to DS1307(Read mode) by sending its ID
 
    rtc->sec = i2c_read1(0);// read second and return Positive ACK
    
    
    rtc->min = i2c_read1(0);                 // read minute and return Positive ACK
    rtc->hour= i2c_read1(0);               // read hour and return Positive ACK
    rtc->weekDay = i2c_read1(1);           // read weekDay and return Positive ACK
    /*rtc->date= i2c_read1(0);             // read Date and return Positive ACK
    rtc->month=i2c_read1(0);            // read Month and return Positive ACK
    rtc->year =i2c_read1(1);  */           // read Year and return Negative/No ACK
 
    i2c_stop1();                             // Stop I2C communication after reading the Date
}
void RTC_GetDate(rtc_t *rtc)
{
    i2c_start1();                             // Start I2C communication
 
    i2c_write1(C_Ds1307WriteMode_U8);        // connect to DS1307 by sending its ID on I2c Bus
    i2c_write1(C_Ds1307DateRegAddress_U8); // Request Sec RAM address at 00H
 
    i2c_stop1();                                // Stop I2C communication after selecting Sec Register
 
    i2c_start1();                               // Start I2C communication
    i2c_write1(C_Ds1307ReadMode_U8);            // connect to DS1307(Read mode) by sending its ID
 
    
             
    rtc->date= i2c_read1(0);             // read Date and return Positive ACK
    rtc->month=i2c_read1(0);            // read Month and return Positive ACK
    rtc->year =i2c_read1(1);             // read Year and return Negative/No ACK
 
    i2c_stop1();                             // Stop I2C communication after reading the Date
}
void RTC_SetDateTime(rtc_t *rtc)
{
    i2c_start1();                            // Start I2C communication
 
    i2c_write1(C_Ds1307WriteMode_U8);      // connect to DS1307 by sending its ID on I2c Bus
    i2c_write1(C_Ds1307SecondRegAddress_U8); // Request sec RAM address at 00H
 
    i2c_write1(rtc->sec);                    // Write sec from RAM address 00H
    i2c_write1(rtc->min);                    // Write min from RAM address 01H
    i2c_write1(rtc->hour);                    // Write hour from RAM address 02H
    i2c_write1(rtc->weekDay);                // Write weekDay on RAM address 03H
    i2c_write1(rtc->date);                    // Write date on RAM address 04H
    i2c_write1(rtc->month);                    // Write month on RAM address 05H
    i2c_write1(rtc->year);                    // Write year on RAM address 06h
 
    i2c_stop1();                            // Stop I2C communication after Setting the Date
}
