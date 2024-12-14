#bit rb3 =0x06.3
#bit rb2 =0x06.2
#bit rb1 =0x06.1
#bit rb0 =0x06.0 
#byte TRISB =0x86 
#byte PORTB =0x06 
#bit RBPU =0x81.7 
uint8_t scan_code[4]={0x0E, 0x0D, 0x0B, 0x07};
uint8_t keyin,old;
char ascii_code[4][4]={ '7', '8', '9', '/',
'4', '5', '6', '*',
'1', '2', '3', '-',
'N', '0','=', '+'};
char key;
char checkpad();
char checkpad(){
  
//doan chuong trinh con doc keypad----- uint8_t checkpad(){
   uint8_t i,j; 

   for(i=0;i<4;i++){
         PORTB=0xFF-(1<<(i+4));
         delay_us(10);
         keyin=(rb0<<0)|(rb1<<1)|(rb2<<2)|(rb3<<3);
         keyin=keyin&0x0F;
         if(keyin!=0x0F)
               for(j=0;j<4;j++)
               {
                     if(keyin==scan_code[j]) 
                     {
                     
                     return ascii_code[j][i];
                     }
               }
}

return 0;
}

