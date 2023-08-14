#ifndef LCD_H
#define LCD_H

// Defining pins of the micro controller

#define dis P1   
sbit rs = P3^4;  
sbit rw = P3^3;  
sbit e =  P3^2; 

// Defining functions

void msdelay(unsigned int time) 
{
    unsigned i,j ;
    for(i=0;i<time;i++)    
    for(j=0;j<1275;j++);
}
void lcd_cmd(unsigned char command)         //Function to send command instruction to LCD
{
    dis = (dis & 0x0F) | (command & 0x0F);
    rs= 0;
    rw=0;
    e=1;
    msdelay(1);
    e=0;
	msdelay(2);
  	dis = (dis & 0x0F) | (command << 4);
    e=1;
    msdelay(1);
    e=0;
}
void lcd_data(unsigned char disp_data)      //Function to send display data to LCD
{
    dis = (dis & 0x0F) | (disp_data&0xF0);
    rs= 1;
    rw=0;
    e=1;
    msdelay(1);
    e=0;
	msdelay(2);
	dis = (dis & 0x0F) | (disp_data << 4);
    e=1;
    msdelay(1);
    e=0;  
}
 void lcd_init()      // Function to prepare the LCD  and get it ready
{
    lcd_cmd(0x02);	  // for using 2 lines and 5X7 matrix of LCD
    lcd_cmd(0x28);    // turn display ON, cursor blinking
    lcd_cmd(0x0C);    // clear screen
    lcd_cmd(0x06);    // bring cursor to position 1 of line 1
	lcd_cmd(0x01);    // bring cursor to position 1 of line 1
    msdelay(3);
}
void ll(char *d)
{ 
	int i;
	for(i=0;d[i]!=0;i++)	// Send each char of string till the NULL 
	{
		lcd_data (d[i]);	// Call LCD data write 
	}
	msdelay(200);
	lcd_cmd(0x01);
    
}
void lll(char *d)
{ 
	int i;
	for(i=0;d[i]!=0;i++)				
	{
		lcd_data (d[i]);					
	}   
}
void lcd_xy (char row, char pos, char *str)	
{
	if (row == 0)
	lcd_cmd((pos & 0x0F)|0x80);	
	else if (row == 1)
	lcd_cmd((pos & 0x0F)|0xC0);
	lll(str);								
}

#endif