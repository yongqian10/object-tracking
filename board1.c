#include<8051.h>
__sbit __at (0x90) rs; 
__sbit __at (0x91) rw;
__sbit __at (0x92) e; 
__sbit __at (0x93) trig1;
__sbit __at (0x94) echo1;
__sbit __at (0x95) trig2;
__sbit __at (0x96) echo2;
int i;			//i for for loop
void determinepositionofneighbour(int *symbol,unsigned int detected_distance);  //to determine position of neighbour
void delay(unsigned int msec);     //Function to provide time delay in msec.
void lcdcmd(unsigned char item);  // sent command code to lcd
void lcddata(unsigned char dat);  //sent data to display on lcd
void receiver1();		  //receiver for serial transmit
void transmitter1(unsigned int b);    //trasnmitter for serial transmit
void lcd_position(unsigned char lcd_point);     //display position of target on lcd
void lcd_position_predefine(unsigned char lcd_x, unsigned char lcd_y);    //stepper step to display position on lcd
void position_point_onlcd(unsigned int *xy);    //stepper step to display position on lcd
unsigned int *distance_determine_x_y();         //function to input x and y coordinate from sensor 
unsigned char point_y(unsigned int pointy);     //stepper step to display position on lcd
unsigned char point_x(unsigned int pointx);     //stepper step to display position on lcd
void direction_determine(unsigned int *xxyy);   //determine heading direction of target
void send_pulse1(void);           //sent trigger pulse to ultrasonic sensor_x
void send_pulse2(void);            //sent trigger pulse to ultrasonic sensor_y
void delay1();                     // delay 12us to activate ultrasonic wave

void main()
{                      
lcdcmd(0x01);           //clear lcd
lcdcmd(0x38);           //2line 5x8
lcdcmd(0x0C);           //cursor off lcd on 
while(1){               //infinite loop
lcdcmd(0x01);           //clear lcd
position_point_onlcd(distance_determine_x_y());       //call main function 
delay(25000);       //high delay to see value on lcd clearly
}
}

void delay1()      // delay 12us to activate ultrasonic wave
{TH1=0xFF;	
TL1=0xF2;           //delay ~12us
TR1=1;		    //start timer1
while(TF1==0);      //wait for timer flag =0 again 
TR1=0;              // off timer1
TF1=0;              //clear flag 
}

void send_pulse1(void)                  //sent trigger pulse to ultrasonic sensor_x
{
TH0=0x00;TL0=0x00; 			//reload timer0 
 trig1=1;                                //sent high signal to trig1 pin
delay1();				// delay for ~12us
 trig1=0;                               //sent low signal to trig1 pin
 return;				//return from subroutine
}

void send_pulse2(void)              //sent trigger pulse to ultrasonic sensor_y
{
TH0=0x00;TL0=0x00;                  //sent high signal to trig2 pin
 trig2=1;
delay1();
 trig2=0;			    //sent low signal to trig2 pin
 return;
}

void delay(unsigned int msec)     //Function to provide time delay in msec.
{int i,j,k ;
for(i=0;i<msec;i++);
for(k=0;k<25000;k++);
for(j=0;j<25000;j++);			
return;}

void lcdcmd(unsigned char item)    // sent command code to lcd
{P0 = item;
rs= 0;
rw=0;
e=1;
delay(100);
e=0;}

void lcddata(unsigned char dat)    //sent data to display on lcd
{P0=dat;
rs=1;
rw=0;
e=1;
delay(100);
e=0;}

void receiver1()                           //receiver for serial transmit
{unsigned int received_information;
TMOD=0x20;                                 //timer0 mode 2
TH1=0xFD;                                  // 9600 braud rate
SCON=0x50;                                 
TR1=1;					//start timer1
RI=0;					
received_information=SBUF;		//place value want to transmit in SBUF  register
while(RI==0)				//wait stop code and ready to receive				
RI=0;
transmitter1(received_information);
}

void transmitter1(unsigned int a)      //transmitter for serial transmit
{TMOD=0x20;
TH1=0xFD;
SCON=0x50;
TR1=1;
SBUF=a;
while(TI==0);
TI=0;
}

unsigned  int *distance_determine_x_y()     //function to input x and y coordinate from sensor 
{unsigned int Count,Time,rfl[4]; 
TMOD=0x11;              //state out array limit rfl					//timer0 mode 1 , timer1 mode 1
for(i=0;i<2;i++){				//take 2 input for each x and y coordinate  
send_pulse1();					//send pulse to trigger ultrasonic wave
while(echo1==0);				//wait for ultrasonic sensor to start sending wave 
TH0=TL0=0;					//timer count from 0
TR0=1;   
while(echo1 == 1)//Wait for Falling edge at Echo pin
	{
		if(TF0 == 1)//timer over if no obstacle is detected
			break;
  }
	TR0=0;//Stop Timer.
	TF0 = 0;//clear Timer Over Flow Flag
	Count = TL0 + TH0*256;//Calculate number of count
	Time = Count*1.085;//Calculate total time in uS.
	rfl[i] = Time/58;
delay(1000);}

for(i=2;i<4;i++){
send_pulse2();
while(echo2==0);
TH0=TL0=0;
TR0=1;   
while(echo2 == 1)//Wait for Falling edge at Echo pin
	{
		if(TF0 == 1)//timer over if no obstacle is detected
			break;
  }
	TR0=0;//Stop Timer.
	TF0 = 0;//clear Timer Over Flow Flag
	Count = TL0 + TH0*256;//Calculate number of count
	Time = Count*1.085;//Calculate total time in uS.
	rfl[i] = Time/58;
	delay(1000);}
direction_determine(rfl);
return rfl;}

void lcd_position(unsigned char lcd_point)	//display position of target on lcd
{switch(lcd_point){				//display correct position of target on lcd 
case 1:lcdcmd(0x80);				//line1 column1		
	lcddata('P');					
	break;
case 2: lcdcmd(0x81);				//line1 column2	
	lcddata('O');
	break;
case 3: lcdcmd(0x82);				//line1 column3	
	lcddata('P');
	break;
case 4: lcdcmd(0x83);				//line1 column4	
	lcddata('O');
	break;
case 5: lcdcmd(0x84);				//line1 column5
	lcddata('P');
	break;
case 6: lcdcmd(0x85);				//line1 column6	
	lcddata('O');
	break;			
case 7: lcdcmd(0x86);				//line1 column7	
	lcddata('P');
	break;
case 8: lcdcmd(0x87);				//line1 column8	
	lcddata('O');
	break;	
case 9: lcdcmd(0x88);				//line1 column9	
	lcddata('P');
	break;	
case 10:lcdcmd(0x89);				//line1 column10	
	lcddata('O');
	break;	
case 11:lcdcmd(0x8A);				//line1 column11	
	lcddata('P');
	break;	
case 12:lcdcmd(0x8B);				//line1 column12	
	lcddata('O');
	break;
case 13:lcdcmd(0x8C);				//line1 column13	
	lcddata('P');
	break;	
case 14:lcdcmd(0x8D);				//line1 column14	
	lcddata('O');
	break;	
case 15:lcdcmd(0x8E);				//line1 column15	
	lcddata('P');
	break;	
case 16:lcdcmd(0x8F);				//line1 column16	
	lcddata('O');
	break;	
case 17:lcdcmd(0xC0);				//line2 column1	
	lcddata('P');
	break;	
case 18:lcdcmd(0xC1);				//line2 column2	
	lcddata('O');
	break;	
case 19:lcdcmd(0xC2);				//line2 column3	
	lcddata('P');
	break;	
case 20:lcdcmd(0xC3);				//line2 column4	
	lcddata('O');
	break;
case 21:lcdcmd(0xC4);				//line2 column5	
	lcddata('O');
	break;					
case 22:lcdcmd(0xC5);				//line2 column6	
	lcddata('P');
	break;	
case 23:lcdcmd(0xC6);				//line2 column7	
	lcddata('O');
	break;	
case 24:lcdcmd(0xC7);				//line2 column8	
	lcddata('P');
	break;	
case 25:lcdcmd(0xC8);				//line2 column9	
	lcddata('O');
	break;	
case 26:lcdcmd(0xC9);				//line2 column10	
	lcddata('P');
	break;	
case 27:lcdcmd(0xCA);				//line2 column11	
	lcddata('O');
	break;	
case 28:lcdcmd(0xCB);				//line2 column12	
	lcddata('P');
	break;	
case 29:lcdcmd(0xCC);				//line2 column13	
	lcddata('O');
	break;	
case 30:lcdcmd(0xCD);				//line2 column14	
	lcddata('P');
	break;	
case 31:lcdcmd(0xCE);				//line2 column15	
	lcddata('O');
	break;	
case 32:lcdcmd(0xCF);				//line2 column16	
	lcddata('P');
	break;
default:lcdcmd(0x81);				//line1 column2	
	lcddata('N');}}


void position_point_onlcd(unsigned int *xy)      //stepper step to display position on lcd
{unsigned int buffer1;
unsigned int buffer2;
unsigned int buffer3;
unsigned int buffer4;
buffer1=xy[0];					//input data from array 
buffer2=xy[1];
buffer3=xy[2];
buffer4=xy[3];
lcd_position_predefine(point_x(buffer2),point_y(buffer4));	//sent to next stepper step
}

unsigned char point_x(unsigned int pointx)			//stepper step to display position on lcd: range vs coordinate_x
{
if (pointx>0 && pointx<=2)					//range can change according to the area ratio of 'sensor area'
return 1;
else if(pointx>2 && pointx<=4)
return 2;
else if(pointx>4 && pointx<=6)
return 3;
else if (pointx>6 && pointx<=8)
return 4;
else if(pointx>8 && pointx<=10)
return 5;
else if (pointx>10 && pointx<=12)
return 6;
else if (pointx>12 && pointx<=14)
return 7;
else if(pointx>14 && pointx<=16)
return 8;
else if (pointx>16 && pointx<=18)
return 9;
else if (pointx>18 && pointx<=20)
return 10;
else if (pointx>20 && pointx<=22)
return 11;
else if (pointx>22 && pointx<=24)
return 12;
else if (pointx>24 && pointx<=26)
return 13;
else if (pointx>26 && pointx<=28)
return 14;
else if (pointx>28 && pointx<=30)
return 15;
else 
return 16;
}

unsigned char point_y(unsigned int pointy)      //stepper step to display position on lcd: range vs coordinate_y
{
if (pointy<15)					//range can change according to the area ratio of 'sensor area'
return 1;
else 
return 2;
}

void lcd_position_predefine(unsigned char lcd_x, unsigned char lcd_y)			//sent to next stepper step
{unsigned char lcd_point;
if (lcd_y==1)	
{lcd_point=lcd_x;
lcd_position(lcd_point);}
else 
{lcd_point=33-lcd_x;							//total column - number of columnn
lcd_position(lcd_point);						
}}

void direction_determine(unsigned int *xxyy)				//determine heading direction of target
{unsigned int direction;
unsigned int buffer5;
unsigned int buffer6;
unsigned int buffer7;
unsigned int buffer8;
buffer5=xxyy[0];							//coordinate_x1
buffer6=xxyy[1];							//coordinate_x2
buffer7=xxyy[2];							//coordinate_y1
buffer8=xxyy[3];							//coordiante_y2
lcdcmd(0x8E);								//observe value of coordinate x and y on lcd 
lcddata((buffer6%10)+48);
lcdcmd(0x8D);
lcddata(((buffer6/10)%10)+48);
lcdcmd(0x8C);
lcddata((buffer6/100)+48);
lcdcmd(0xCE);
lcddata((buffer8%10)+48);
lcdcmd(0xCD);
lcddata(((buffer8/10)%10)+48);
lcdcmd(0xCC);
lcddata((buffer8/100)+48);	
						//transmit coordinate y to board 2
if (buffer5-buffer6<5 || buffer6-buffer5<5 )
	if (buffer7-buffer8<5)
	`	if(buffer5>buffer6)
		{direction=1;
		lcdcmd(0xCF);
		lcddata((direction%10)+48);
		transmitter1(direction);
		}
		else 
		{direction=2;
		lcdcmd(0xCF);
		lcddata((direction%10)+48);
		transmitter1(direction);
		}
	else
		if(buffer7>buffer8)
		{direction=3;
		lcdcmd(0xCF);
		lcddata((direction%10)+48);
		transmitter1(direction);		}
		else
		{direction=4;
		lcdcmd(0xCF);
		lcddata((direction%10)+48);
		transmitter1(direction);
		}
else
	if (buffer7-buffer8<5||buffer8-buffer7<5)
	`	if(buffer5>buffer6)
		{direction=1;
		lcdcmd(0xCF);
		lcddata((direction%10)+48);
		transmitter1(direction);
		}
		else 
		{direction=2;
		 lcdcmd(0xCF);
		lcddata((direction%10)+48);
		transmitter1(direction);
		}
	else
		if(buffer7>buffer8)
			if(buffer5>buffer6)
			{direction=5;
			lcdcmd(0xCF);
			lcddata((direction%10)+48);
			transmitter1(direction);
			}
			else
			{direction=6;
			lcdcmd(0xCF);
			lcddata((direction%10)+48);
		transmitter1(direction);
			} 
		else
			if(buffer5>buffer6)
			{direction=7;
			lcdcmd(0xCF);
			lcddata((direction%10)+48);
		transmitter1(direction);
			}
			else
			{direction=8;
			lcdcmd(0xCF);
			lcddata((direction%10)+48);
			transmitter1(direction);
			}

		
}
