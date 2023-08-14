#include<stdio.h>

/*
   Uncomment & Comment the library according to the microcontroller
*/

#include<reg51.h>
//#include<REGX52.h>

/* 
   Print function which send specific comands to mobile phone like when
   you turn off and on any device then it send the status of the device 
*/
#include"print.h" 
/*
  LCD function it prints the temp of the room and humidity and status of any device
*/
#include"lcd.h"    



// Defining pins of the micro controller 
sbit L1=P2^0;
sbit L2=P2^1;
sbit L3=P2^2;
sbit L4=P2^3;
sbit L5=P2^4;
sbit L6=P2^5;
sbit L7=P2^6;
sbit L8=P2^7;
sbit DHT11=P3^5;                  // Connect DHT11 Sensor Pin to P3.5 Pin 
char I_RH,D_RH,I_Temp,D_Temp,CheckSum,z,y; 
datt[11]={'0','1','2','3','4','5','6','7','8','9'};
int f,l,i,x=0,j=0;
char d[3],d1[5];
unsigned dat[20];
int tempi=20,tempm=25,tempii=25,tempmm=35;

void timer2(void) interrupt 5    // Timer 2 interrupt handler
{
	z--;
	L5=~L5;
	if(z==0)
	{
		y--;
		z=16;
		if(y==0)
		{
			TR2=0;
			L5=0;
		}
	}
  TF2=0;
}

void timer_delay20ms()		      // Timer0 delay function 
{
	TH0 = 0xB8;				      // Load higher 8-bit in TH0 
	TL0 = 0x0C;				      // Load lower 8-bit in TL0 
	TR0 = 1;				      // Start timer0 
	while(TF0 == 0);	          // Wait until timer0 flag set 
	TR0 = 0;				      // Stop timer0 
	TF0 = 0;				      // Clear timer0 flag 
}

void timer_delay30us()            //Timer0 delay function 
{
	TH0 = 0xFF;				      // Load higher 8-bit in TH0 
	TL0 = 0xF1;				      // Load lower 8-bit in TL0 
	TR0 = 1;				      // Start timer0 
	while(TF0 == 0);	          // Wait until timer0 flag set 
	TR0 = 0;				      // Stop timer0 
	TF0 = 0;				      // Clear timer0 flag 
}

void Request()		              // Microcontroller send start pulse or request 
{
	DHT11 = 0;		 		      // set to low pin 
	timer_delay20ms();            // wait for 20ms 
	DHT11 = 1;				      // set to high pin 
}

void Response()			          // receive response from DHT11 
{
	while(DHT11==1);
	while(DHT11==0);
	while(DHT11==1);
}

int Receive_data()			      // receive data 
{
	uint8_t q,c=0;	
	for (q=0; q<8; q++)
	{
		while(DHT11==0);		  // check received bit 0 or 1 
		timer_delay30us();
		if(DHT11 == 1)			  // if high pulse is greater than 30ms 
		c = (c<<1)|(0x01);	      // then its logic HIGH 
		
		else					  // otherwise its logic LOW 
		c = (c<<1);
		
		while(DHT11==1);
	}
	return c;
}
void dht()
	{		
		Request();				  // send start pulse 
		Response();				  // receive response 
		I_RH=Receive_data();	  // store first eight bit in I_RH 	
		D_RH=Receive_data();	  // store next eight bit in D_RH 
		I_Temp=Receive_data();	  // store next eight bit in I_Temp 
		D_Temp=Receive_data();	  // store next eight bit in D_Temp 
		CheckSum=Receive_data();  // store next eight bit in CheckSum 
		f=l=I_RH;
		f=I_RH/10;
		f=f+48;
		l=I_RH%10;
		l=l+48;
		d[0]=f;
		d[1]=l;
		d[2]='%';
		f=l=I_Temp;
		f=I_Temp/10;
		f=f+48;
		l=I_Temp%10;
		l=l+48;
		d1[0]=f;
		d1[1]=l;
		d1[2]=223;
		d1[3]='c';
		d1[4]=0;
		sprintf(dat,"HUM= ",d);
		lcd_xy(0,0,dat);
		lll(d);
		msdelay(100);
		lcd_cmd(0x01);
		sprintf(dat,"TEMP= ");
		lcd_xy(0,0,dat);
		lll(d1);
		msdelay(100);
		lcd_cmd(0x01);
}
void main()
{
	char dd;         // FOR STORING DATA 
	P2=0x00;         // TURNING ALL LOADS OFF
	TMOD=0x21;       // TIMER MODE 8 BIT TIMER
	TH1=0xFD;        // SETING BAUD RATE 9600
	SCON=0x50;       // SERIAL COMUNICATION MODE 1 10 BIT,1 START BIT,8 DATA BIT,1 STOP BIT
	IE=0x00100000b;  // ENABLING TIMER 2 INTRUPT
	TI=0;            // TRANSMIT INTRUPT
	RI=0;            // RECIVE INTRUPT
	RCLK=TCLK=0;     // CONFIGURING TIMER 2 FLAGS AND VALUES
	CP_RL2=0;
	TH2=0x00;
	TL2=0x00;
	TR2=0;
	TF2=0;
	TR1=1;           // START THE TIMER
	transmit("Connected");
	lcd_init();
	while(1)
	{
		while(RI==0)
		{
			dht();
			if(x==3)
			{
				if(I_Temp<=tempi)
				{
					if(j==0)
					{
						transmit("turning on heater / temp is below room temp");
						j=1;
						L6=1;
					}
				}
				if(I_Temp>=tempm )
				{
					if(j==1)
					{
						j=0;
						transmit("turning off heater/ temp is back to normal room temp");
						L6=0;
					}
				}
			}
			if(x==1)
			{
				if(I_Temp<=tempii)
				{
					if(j==1)
					{
						transmit("turning off ac/ temp is back to normal room temp");
						L7=0;
						j=0;
					}
				}
				if(I_Temp>=tempmm)
				{
					if(j==0)
					{
						transmit("turning on ac/ temp is above room temp");
						L7=1;
						j=1;
					}
				}
			}
			
		}
		dd=SBUF;
		RI=0;
    switch(dd)      // CASES TO PERFORM ACTION
		{
			case 'A':
				L1=1;
				transmit("load 1 on");
				ll("load 1 on");
				delay(200);
				break;
			case 'a':
				L1=0;
				transmit("load 1 off");
				ll("load 1 off");
				delay(200);
				break;
			case 'B':
				L2=1;
				transmit("load 2 on");
				ll("load 2 on");
				delay(200);
				break;
		  case 'b':
				L2=0;
				transmit("load 2 off");
				ll("load 2 off");
				delay(200);
				break;
			case 'G':
				L3=1;
				transmit("load 3 on");
				ll("load 3 on");
				delay(200);
				break;
			case 'g':
				L3=0;
				transmit("load 3 off");
				ll("load 3 off");
				delay(200);
				break;
			case 'D':
				L4=1;
				transmit("load 4 on");
				ll("load 4 on");
				delay(200);
				break;
			case 'd':
				L4=0;
				transmit("load 4 off");
				ll("load 4 off");
				delay(200);
				break;
			case 'E':
				L5=1;
				transmit("load 5 on");
				ll("load 5 on");
				delay(200);
				break;
			case 'e':
				L5=0;
				transmit("load 5 off");
				ll("load 5 off");
				delay(200);
				break;
			case 'F':
				L8=1;
				transmit("load 6 on");
				ll("load 6 on");
				delay(200);
				break;
			case 'f':
				L8=0;
				transmit("load 6 off");
				ll("load 6 off");
				delay(200);
				break;
			case 'T':
				transmit("TEMP = ");
				d1[2]=176;
				transmit(d1);
				transmit("HUM = ");
				transmit(d);
				break;
			case 'S':
				transmit("TEMP = ");
				d1[2]=176;
				transmit(d1);
				transmit("HUM = ");
				transmit(d);
				transmit("enter lower limit");
				while(RI==0)
				{
					
				}
				i=0;
				while(1)
				{
					if(SBUF==datt[i])
					{
						tempi=i*10;
						break;
					}
					i++;
				}
				RI=0;
				while(RI==0)
				{
					
				}
				i=0;
				while(1)
				{
					if(SBUF==datt[i])
					{
						tempi=tempi+i;
						break;
					}
					i++;
				}
				RI=0;
				transmit("enter upper limit");
				while(RI==0)
				{
				
				}
				i=0;
				while(1)
				{
					if(SBUF==datt[i])
					{
						tempm=i*10;
						break;
					}
					i++;
				}
				RI=0;
				while(RI==0)
				{
				
				}
				i=0;
				while(1)
				{
					if(SBUF==datt[i])
					{
						tempm=tempm+i;
						break;
					}
					i++;
				}
				RI=0;
				break;
		    case 'K':
				transmit("TEMP = ");
				d1[2]=176;
				transmit(d1);
				transmit("HUM = ");
				transmit(d);
				transmit("enter lower limit");
				while(RI==0)
				{
				
				}
				i=0;
				while(1)
				{
					if(SBUF==datt[i])
					{
						tempii=i*10;
						break;
					}
					i++;
				}
				RI=0;
				while(RI==0)
				{
				
				}
				i=0;
				while(1)
				{
					if(SBUF==datt[i])
					{
						tempii=tempii+i;
						break;
					}
					i++;
				}
				RI=0;
				transmit("enter upper limit");
				while(RI==0)
				{
				
				}
				i=0;
				while(1)
				{
					if(SBUF==datt[i])
					{
						tempmm=i*10;
						break;
					}
					i++;
				}
				RI=0;
				while(RI==0)
				{
				
				}
				i=0;
				while(1)
				{
					if(SBUF==datt[i])
					{
						tempmm=tempmm+i;
						break;
					}
					i++;
				}
				RI=0;
				break;
            case 'H':
				transmit("auto heater on");
				x=3;
				ll("auto heater on");
				j=0;
				delay(200);
				break;
			case 'h':
				transmit("auto heater off");
				x=2;
				ll("auto heater off");
				j=0;
				delay(200);
				break;
			case 'J':
				transmit("auto ac on");
				x=1;
				ll("auto ac on");
				j=0;
				delay(200);
				break;
			case 'j':
				transmit("auto ac off");
				x=0;
				ll("auto ac off");
				j=0;
				delay(200);
				break;
			case 'C':
				transmit("Connected to device");
				delay(200);
				break;
		    case 't':
				transmit("You entered in timer mode");
				delay(200);
				transmit("Enter the value of minutes");
				while(RI==0)
				{
				
				}
				i=0;
				while(1)
				{
					if(SBUF==datt[i])
					{
						y=i*10;
						break;
					}
					i++;
				}
				RI=0;
				while(RI==0)
				{
				
				}
				i=0;
				while(1)
				{
					if(SBUF==datt[i])
					{
						y=y+i;
						break;
					}
					i++;
				}
				y*2;
				TR2=1;
				L5=1;
				RI=0;
				break;
			default:
				transmit("wrong command");
				ll("ERROR 404 ");
				delay(200);
				break;
		}
	}
}