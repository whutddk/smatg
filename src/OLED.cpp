#include "common.h"
#include "OLED.h"
#include "oledfont.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include "stdio.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "include.h"

void OLED_WORK()
{
	OLED_Clear();  
	OLED_init();
	delay(10);

	OLED_ShowString(0,0,(uint8 *)("sys_init..."),2);

	if (ctl.flag_bzon == true)
	{
		OLED_ShowString(1,2,(uint8 *)("bz on..."),2);
	}
	else
	{
		OLED_ShowString(1,2,(uint8 *)("bz off..."),2);
	}
	if (ctl.flag_debug == true)
	{
		OLED_ShowString(1,3,(uint8 *)("debug on..."),2);
	}
	else
	{
		OLED_ShowString(1,3,(uint8 *)("debug off..."),2);
	}
	

}

void OLED_TEST()
{
	OLED_Clear();  
	OLED_init();
	delay(10);

	OLED_ShowString(0,0,(uint8 *)("abcdefgh1"),2);
	OLED_ShowString(0,1,(uint8 *)("abcdefgh1"),2);
	OLED_ShowString(0,2,(uint8 *)("abcdefgh1"),2);
	OLED_ShowString(0,3,(uint8 *)("abcdefgh1"),2);
OLED_ShowString(0,4,(uint8 *)("abcdefgh1"),2);
OLED_ShowString(0,5,(uint8 *)("abcdefgh1"),2);
OLED_ShowString(0,6,(uint8 *)("abcdefgh1"),2);
}

void I2C_start(void)

{    

  I2CCLK_H;  

  delayMicroseconds(1);

  I2CSDA_H;

  delayMicroseconds(1);      

  I2CSDA_L;

  delayMicroseconds(1);  

  I2CCLK_L;

  delayMicroseconds(1);

}



void I2C_stop(void)

{ 

  I2CCLK_L;   	   //时钟保持高，数据线从低到高一次跳变，I2C通信停止

  delayMicroseconds(1);      // 延时1us 

  I2CSDA_L;

  delayMicroseconds(1);

  I2CSDA_H;

  delayMicroseconds(1);

}

uint8 I2Ccheck_ACK(void)

{ 

  I2CCLK_H;

  delayMicroseconds(1);

  I2CCLK_L;

	return 0;
}

void I2CSendByte(uint8 ch)

{ 

  uint8 i;     
I2CCLK_L; 

  for (i=0; i < 8; i++)         

  {
	if ((( ch & 0x80 ) >> 7) )
	{
   	 I2CSDA_H;
	}
	else
	{
       	  I2CSDA_L;    
	}

    	    ch <<= 1;         

    	delayMicroseconds(1);       
	I2CCLK_H; 
        delayMicroseconds(1);         
	I2CCLK_L;
    	delayMicroseconds(1);          
  }

}

/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(uint8  IIC_Command)
{
	I2C_start();
	I2CSendByte(0x78);
	I2Ccheck_ACK();
	I2CSendByte(0x00);
	I2Ccheck_ACK();
	I2CSendByte(IIC_Command);
	I2Ccheck_ACK();
	I2C_stop();

}

/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(uint8 IIC_Data)
{
	I2C_start();
	I2CSendByte(0x78);
	I2Ccheck_ACK();
	I2CSendByte(0x40);
	I2Ccheck_ACK();
	I2CSendByte(IIC_Data);
	I2Ccheck_ACK();
	I2C_stop();
}

/********************************************
// fill_Picture
********************************************/
void fill_picture(uint8 fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		Write_IIC_Command(0xb0+m);		//page0-page1
		Write_IIC_Command(0x00);		//low column start address
		Write_IIC_Command(0x10);		//high column start address
		for(n=0;n<128;n++)
			{
				Write_IIC_Data(fill_Data);
			}
	}
}

void OLED_Set_Pos(uint8 x, uint8 y) 
{ 
	uint8 x0;
	x0 = x+32;
	Write_IIC_Command(0xb0+y);
	Write_IIC_Command (( ( x0 & 0xf0 )>>4 )|0x10);
	Write_IIC_Command(x0&0x0f); 
}   

void OLED_Display_On(void)
{
	Write_IIC_Command(0X8D);  //SET DCDC命令
	Write_IIC_Command(0X14);  //DCDC ON
	Write_IIC_Command(0XAF);  //DISPLAY ON
}
  
void OLED_Display_Off(void)
{
	Write_IIC_Command(0X8D);  //SET DC
	Write_IIC_Command(0X10);  //DCDC OFF
	Write_IIC_Command(0XAE);  //DISPLAY OFF
}
  
void OLED_Clear(void)  
{  
	uint8 i,n;		    
	for(i=0;i<6;i++)  
	{  
		Write_IIC_Command(0xb0+i);    
		Write_IIC_Command(0x00);      
		Write_IIC_Command(0x12);      
		for(n=32;n<96;n++)
			Write_IIC_Data(0); 
	} 
}

void OLED_On(void)  
{  
	uint8 i,n;		    
	for(i=0;i<1;i++)  
	{  
		Write_IIC_Command(0xb0+i);    
		Write_IIC_Command(0x00);      
		Write_IIC_Command(0x12);    
		for(n=32;n<128;n++)
			Write_IIC_Data(1); 
	} 
}


void OLED_ShowChar(uint8 x,uint8 y,uint8 chr,uint8 Char_Size)
{      	
	uint8 c=0,i=0;	
		c=chr-' ';			
		if( x > Max_Column-1 )
		{
			x=0;
			y=y+2;
		}
		if( Char_Size == 16 )
			{
				OLED_Set_Pos(56-x,y);	
				for(i=7;i>0;i--)
					Write_IIC_Data(F8X16[c*16+i-1]);
				OLED_Set_Pos(56-x,y+1);
				for(i=7;i>0;i--)
					Write_IIC_Data(F8X16[c*16+i+8-1]);
			}
			else 
			{	
				OLED_Set_Pos(56-x,y);
				for(i=7;i>0;i--)
					Write_IIC_Data(F6x8[c][i-1]);
				
			}
}

uint32 oled_pow(uint8 m,uint8 n)
{
	uint32 result=1;	 
	while(n--)
		result*=m;    
	return result;
}	


void OLED_init()
{	
	Write_IIC_Command(0xAE);//--display off

	Write_IIC_Command(0x00);//---set low column address
	Write_IIC_Command(0x12);//---set high column address

	Write_IIC_Command(0x00);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)

	Write_IIC_Command(0xB0);//--set page address

	Write_IIC_Command(0x81); // contract control
	Write_IIC_Command(0x2F);//--128   

	Write_IIC_Command(0xA1);//set segment remap 

	Write_IIC_Command(0xA6);//--normal / reverse

	Write_IIC_Command(0xA8);//--set multiplex ratio(1 to 64)
	Write_IIC_Command(0x2F);//--1/32 duty

	Write_IIC_Command(0xC8);//Com scan direction

	Write_IIC_Command(0xD3);//-set display offset
	Write_IIC_Command(0x00);//

	Write_IIC_Command(0xD5);//set osc division
	Write_IIC_Command(0x80);//

	Write_IIC_Command(0xD9);//set pre-charge period
	Write_IIC_Command(0x1F);//

	Write_IIC_Command(0xDA);//set COM pins
	Write_IIC_Command(0x12);//

	//Write_IIC_Command(0x23);//set COM pins
	//Write_IIC_Command(0x0);

	Write_IIC_Command(0xDB);//set vcomh
	Write_IIC_Command(0x40);//

	Write_IIC_Command(0x8D);//set charge pump enable
	Write_IIC_Command(0x14);//

	Write_IIC_Command(0xAF);//--turn on oled panel
}


void OLED_DrawBMP(uint8 x0, uint8 y0,uint8 x1, uint8 y1,uint8 BMP[])
{ 	
 	uint16 j=0;
 	uint8 x,y;
  
  	if( y1 % 8 == 0) 
		y = y1 / 8;      
  	else 
		y = y1 / 8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    		for( x = x0; x < x1; x++)
	    	{      
	    		Write_IIC_Data(BMP[j++]);	    	
	   	}
	}
} 


void OLED_ShowString(uint8 x,uint8 y,uint8 *chr,uint8 Char_Size)
{
	uint8 j = 0;
	while ( chr[j] != '\0' )
	{		
		OLED_ShowChar(x,y,chr[j],Char_Size);
		x+=8;
		if( x > 120 )
		{
			x = 0;
			y += 2;
		}
			j++;
	}
}

 		  
void OLED_ShowNum(uint8 x,uint8 y,uint32 num,uint8 len,uint8 size2)
{         	
	uint8 t,temp;
	uint8 enshow = 0;						   
	for( t = 0; t < len; t++)
	{
		temp = ( num / oled_pow(10,len-t-1)) % 10;
		if( enshow == 0 && t < ( len-1 ) )
		{
			if( temp == 0 )
			{
				OLED_ShowChar( x + ( size2 / 2 ) * t, y,' ',size2 );
				continue;
			}
			else 
				enshow=1; 	 
		}
	 	OLED_ShowChar ( x + ( size2 / 2 ) * t,y,temp + '0',size2 ); 
	}
}

void OLED_PutPixel(uint8 x,uint8 y)//x:0-63,y:0-31
{
	uint8 data1;
	OLED_Set_Pos(x,y);
	data1 = 0x01 << ( y % 8);
	Write_IIC_Command(0xb0+(y>>3));
	Write_IIC_Command(((x&0xf0)>>4)|0x10);
	Write_IIC_Command((x&0x0f)|0x00);
	Write_IIC_Data(data1);
} 

void OLED_Rectangle(uint8 x1,uint8 y1,uint8 x2,uint8 y2)
{
	uint8 n;

if (x1>62)x1 = 62;
if (y1>30)y1 = 30;
if (x2>63)x2 = 62;
if (y2>31)y2 = 30;

if(x1>x2||y1>y2)
{return;}

	OLED_Set_Pos(63 -x2,y1>>3);
	for( n = 63 - x2;n <= 63-x1;n++)
	{
		Write_IIC_Data(0x01 << (y1 % 8));
	}
	OLED_Set_Pos(63-x2,y2>>3);
	for( n = 63-x2; n <= 63-x1;n++)
	{
		Write_IIC_Data(0x01<<(y2 % 8));
	}
printf("%d,%d",x1,y1);
printf("%d,%d",x2,y2);
}
