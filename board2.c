#include<8051.h>
__sbit __at (0x90) rs; 
__sbit __at (0x91) rw;
__sbit __at (0x92) e; 
__sbit __at (0x93) trig1;
__sbit __at (0x94) echo1;
__sbit __at (0x95) trig2;
__sbit __at (0x96) echo2;
int i;
void determinepositionofneighbour(unsigned int *symbol,unsigned int detected_distance);    //use the gathered data from board 1 to calculate the position of neighbour
void delay(unsigned int msec);  
void lcdcmd(unsigned char item);  
void lcddata(unsigned char dat);
void lcd_position(unsigned char lcd_point);
void lcd_position_predefine(unsigned char lcd_x, unsigned char lcd_y);
void receiver2();								//use to receive transmitted data and store in array
unsigned char point_x(unsigned int pointx);
unsigned char point_y(unsigned int pointy);
unsigned int distance_determine_replacemnt();			// use for detect the distance between reference device and its neighbor in board 1 
unsigned int distance_determine();				// use for detect the distance between reference device and its neighbor in board 2  

void main()
{
lcdcmd(0x01);
lcdcmd(0x38);
lcdcmd(0x0C);
  while(1){
lcdcmd(0x01);
receiver2();
delay(25000);
}}

void delay1()
{TH1=0xFF;
TL1=0xF2;
TR1=1;
while(TF1==0);
TR1=0;
TF1=0;
}

void send_pulse1(void) //to generate 10 microseconds delay
{
TH0=0x00;TL0=0x00; 
 trig1=1;
delay1();
 trig1=0;
 return;
}

void send_pulse2(void) //to generate 10 microseconds delay
{
TH0=0x00;TL0=0x00; 
 trig2=1;
delay1();
 trig2=0;
 return;
}

void delay(unsigned int msec)    //Function to provide time delay in msec.
{int i,j,k ;
for(i=0;i<msec;i++);
for(k=0;k<25000;k++);
for(j=0;j<25000;j++);
return;}

void lcdcmd(unsigned char item)    //Function to send command to LCD
{P0 = item;
rs= 0;
rw=0;
e=1;
delay(100);
e=0;}

void lcddata(unsigned char dat)
{P0=dat;
rs=1;
rw=0;
e=1;
delay(100);
e=0;}

void transmitter1(unsigned int b)
{TMOD=0x20;
TH1=0xFD;
SCON=0x50;
TR1=1;
SBUF=b;
while(TI==0);
TI=0;
}

unsigned int distance_determine_replacemnt()			//this time we take 1 data value only because no comaparison between data is needed 
{unsigned int Count,Time,rfl;
 TMOD= 0x11;
send_pulse1();
while(echo1==0);
TH0=TL0=0;
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
	rfl= Time/58;
return rfl;}

unsigned int distance_determine()
{unsigned int Count,Time,rf2;
 TMOD= 0x11;
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
	rf2= Time/58;
return rf2;}

void lcd_position(unsigned char lcd_point)
{switch(lcd_point){
case 1:lcdcmd(0x80);
	lcddata('P');
	break;
case 2: lcdcmd(0x81);
	lcddata('O');
	break;
case 3: lcdcmd(0x82);
	lcddata('P');
	break;
case 4: lcdcmd(0x83);
	lcddata('O');
	break;
case 5: lcdcmd(0x84);
	lcddata('P');
	break;
case 6: lcdcmd(0x85);
	lcddata('O');
	break;			
case 7: lcdcmd(0x86);
	lcddata('P');
	break;
case 8: lcdcmd(0x87);
	lcddata('O');
	break;	
case 9: lcdcmd(0x88);
	lcddata('P');
	break;	
case 10:lcdcmd(0x89);
	lcddata('O');
	break;	
case 11:lcdcmd(0x8A);
	lcddata('P');
	break;	
case 12:lcdcmd(0x8B);
	lcddata('O');
	break;
case 13:lcdcmd(0x8C);
	lcddata('P');
	break;	
case 14:lcdcmd(0x8D);
	lcddata('O');
	break;	
case 15:lcdcmd(0x8E);
	lcddata('P');
	break;	
case 16:lcdcmd(0x8F);
	lcddata('O');
	break;	
case 17:lcdcmd(0xC0);
	lcddata('P');
	break;	
case 18:lcdcmd(0xC1);
	lcddata('O');
	break;	
case 19:lcdcmd(0xC2);
	lcddata('P');
	break;	
case 20:lcdcmd(0xC3);
	lcddata('O');
	break;
case 21:lcdcmd(0xC4);
	lcddata('O');
	break;					
case 22:lcdcmd(0xC5);
	lcddata('P');
	break;	
case 23:lcdcmd(0xC6);
	lcddata('O');
	break;	
case 24:lcdcmd(0xC7);
	lcddata('P');
	break;	
case 25:lcdcmd(0xC8);
	lcddata('O');
	break;	
case 26:lcdcmd(0xC9);
	lcddata('P');
	break;	
case 27:lcdcmd(0xCA);
	lcddata('O');
	break;	
case 28:lcdcmd(0xCB);
	lcddata('P');
	break;	
case 29:lcdcmd(0xCC);
	lcddata('O');
	break;	
case 30:lcdcmd(0xCD);
	lcddata('P');
	break;	
case 31:lcdcmd(0xCE);
	lcddata('O');
	break;	
case 32:lcdcmd(0xCF);
	lcddata('P');
	break;
default:lcdcmd(0x81);
	lcddata('N');}}

void lcd_position_predefine(unsigned char lcd_x, unsigned char lcd_y)
{unsigned char lcd_point;
if (lcd_y==1)	
{lcd_point=lcd_x;
lcd_position(lcd_point);}
else 
{lcd_point=33-lcd_x;
lcd_position(lcd_point);	
}}

unsigned char point_x(unsigned int pointx)
{
if (pointx>0 && pointx<=2)
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
return 16;}

unsigned char point_y(unsigned int pointy)
{
if (pointy<15)
return 1;
else 
return 2;
}

void determinepositionofneighbour(unsigned int *symbol,unsigned int detected_distance)  //use the gathered data from board 1 to calculate the position of neighbour
{unsigned int set_range1, set_range2;
unsigned int buffer1,buffer2,buffer3,buffer4;
unsigned int ncx,ncy,ncc;
buffer1=symbol[0];  //distance displacement of board 1
buffer2=symbol[1];  //x coordinate
buffer3=symbol[2];  //y coordinate
buffer4=symbol[3];  //direction
lcdcmd(0x8E);				//observe transmitted value on lcd for error checking 
lcddata((buffer4%10)+48);
lcdcmd(0x8D);
lcddata(((buffer4/10)%10)+48);
lcdcmd(0x8C);
lcddata((buffer4/100)+48);
lcdcmd(0xCE);
lcddata((buffer1%10)+48);		
lcdcmd(0xCD);
lcddata(((buffer1/10)%10)+48);
lcdcmd(0xCC);
lcddata((buffer1/100)+48);
lcdcmd(0xCB);
lcddata((detected_distance%10)+48);
lcdcmd(0xCA);
lcddata(((detected_distance/10)%10)+48);
lcdcmd(0xC9);
lcddata((detected_distance/100)+48);
set_range1=buffer1-2;				//setup range for distance matching between reference device and its neighbour
set_range2=buffer1+2;
if(detected_distance>set_range1 && detected_distance<set_range2)	//if distance match with each other
{ncc=buffer1/2;								//divide distance by 2 for the +-45' direction							
switch(buffer4)								//switch case :direction
{case 1:ncx=buffer2-buffer1;  //west
       ncy=buffer3;
       lcd_position_predefine(point_x(ncx), point_y(ncy));
       break;
case 2:ncx=buffer2+buffer1;  //east
       ncy=buffer3;
       lcd_position_predefine(point_x(ncx), point_y(ncy));
       break;
case 3:ncy=buffer3-buffer1;  //north
       ncx=buffer2;
       lcd_position_predefine(point_x(ncx), point_y(ncy));
       break; 
case 4:ncy=buffer3+buffer1;  //south
       ncx=buffer2;
        lcd_position_predefine(point_x(ncx), point_y(ncy));
       break;
case 5:ncx=buffer2-ncc; //north-west
       ncy=buffer3-ncc;
        lcd_position_predefine(point_x(ncx), point_y(ncy));
       break;
case 6:ncx=buffer2+ncc; //north-east
       ncy=buffer3-ncc;
        lcd_position_predefine(point_x(ncx), point_y(ncy));
       break;
case 7:ncx=buffer2-ncc; //south-west
       ncy=buffer3+ncc;
        lcd_position_predefine(point_x(ncx), point_y(ncy));
       break;
case 8:ncx=buffer2+ncc; //south-east
       ncy=buffer3+ncc;
        lcd_position_predefine(point_x(ncx), point_y(ncy));
       break;
default:lcd_position_predefine(point_x(buffer2), point_y(buffer3));
}}
else 
lcd_position_predefine(point_x(buffer2), point_y(buffer3));}

	
void receiver2()					//use to receive transmitted data and store in array
{unsigned int received_information[4];
unsigned int a;
TMOD=0x20;
TH1=0xFD;
SCON=0x50;
TR1=1;
a=SBUF;
while(RI==0);
delay(100);
RI=0;
received_information[0]=distance_determine_replacemnt();			//distance detected in board 1(reference device)
received_information[1]=2;							//x coordinate
received_information[2]=5;							//y coordinate
received_information[3]=a;						//direction
determinepositionofneighbour(received_information,distance_determine());}	//put them in next function

