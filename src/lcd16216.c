///////////////////////////////////////////////////////////////////////////
////                 Driver for LCD module                             ////
////                                                                   ////
////  lcd_init()   Must be called before any other function.           ////
////                                                                   ////
////  lcd_gotoxy(x,y) Set write position on LCD (upper left is 1,1)    ////
////                                                                   ////
////  lcd_print( char *text ) Print string                             ////
////                                                                   ////
////  lcd_clear() Clear display, reset position to 1,1                 ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////


// As defined in the following structure the pin connection is as follows:
//     D0  not used
//     D1  not used
//     D2  rs        PIC pin 21
//     D3  enable    PIC pin 22
//     D4  D4        PIC pin 27
//     D5  D5        PIc pIn 28
//     D6  D6        PIC pin 29
//     D7  D7        PIC pin 30
//
//   LCD pins D0-D3 are not used and PIC D0 abd D1 are not used.


struct lcd_pin_map {                 // This structure is overlayed
           BOOLEAN unused1;          // on to an I/O port to gain
           BOOLEAN unused2;          // access to the LCD pins.
           BOOLEAN rs;               // The bits are allocated from
           BOOLEAN enable;           // low order up.
           int     data : 4;
        } lcd;


#byte lcd = 8                       // set lcd to port D (at address 8)

void lcd_send_nibble( BYTE n ) {
      lcd.data = n;
      delay_us(2);
      lcd.enable = 1;
      delay_us(2);
      lcd.enable = 0;
      delay_ms(1);
}


void lcd_send_byte( BYTE address, BYTE n ) {
      lcd.rs = address; // 0 for command, 1 for data
      lcd.enable = 0;
      lcd_send_nibble(n >> 4);
      lcd_send_nibble(n & 0xf);
}


void lcd_init() {
    set_tris_d(0);
    delay_ms(50);

    lcd_send_byte( 0, 0x20);
    lcd_send_byte( 0, 0x20);
    lcd_send_byte( 0, 0x28);
    delay_us(50);

    lcd_send_byte( 0, 0x00);
    lcd_send_byte( 0, 0xC0);
    delay_us(50);

    lcd_send_byte( 0, 0x00);
    lcd_send_byte( 0, 0x10);
    delay_ms(2);

    lcd_send_byte( 0, 0x00);
    lcd_send_byte( 0, 0x60);

    delay_ms(5);
}


void lcd_gotoxy( BYTE x, BYTE y) {
   BYTE address;

   if(y!=1)
     address=0x40;
   else
     address=0;
   address+=x-1;
   lcd_send_byte(0,0x80|address);
}


void lcd_print( char *text )
{
    int j=0;
    while( text[j] )
	 lcd_send_byte(1,text[j++]);
}

void lcd_clear(void)
{
    lcd_send_byte(0,1);
    delay_ms(5);
}

