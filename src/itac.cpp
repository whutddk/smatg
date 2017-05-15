#include <wiringPi.h>
#include "common.h"
#include "include.h"


void itac_init()
{
	wiringPiSetup();

//OLED
	pinMode(3,OUTPUT);
	pinMode(5,OUTPUT);

	pullUpDnControl(3,PUD_UP);
	pullUpDnControl(5,PUD_UP);

//KEY
	pinMode(LK,INPUT);
	pinMode(RK,INPUT);

	pullUpDnControl(LK,PUD_UP);
	pullUpDnControl(RK,PUD_UP);
//switch
	pinMode(DIPS1,INPUT);
	pinMode(DIPS2,INPUT);

	pullUpDnControl(DIPS1,PUD_UP);
	pullUpDnControl(DIPS2,PUD_UP);

//bz
	pinMode(BUZZER,OUTPUT);
	digitalWrite(BUZZER,HIGH);

	bz_notice();
	
//LED
	pinMode(DS,OUTPUT);
	pinMode(SHCP,OUTPUT);
	pinMode(STCP,OUTPUT);

//digital
	pinMode(DI,OUTPUT);
	pinMode(CLK1,OUTPUT);
}


void bz_ready()
{
	if (ctl.flag_bzon == false)
	{
		return;
	}
	digitalWrite(BUZZER,LOW);
	delay(10);
	digitalWrite(BUZZER,HIGH);
	digitalWrite(BUZZER,LOW);
	delay(10);
	digitalWrite(BUZZER,HIGH);
	delay(70);
	digitalWrite(BUZZER,LOW);
	delay(10);
	digitalWrite(BUZZER,HIGH);
	digitalWrite(BUZZER,LOW);
	delay(10);
	digitalWrite(BUZZER,HIGH);
	delay(70);
	digitalWrite(BUZZER,LOW);
	delay(10);
	digitalWrite(BUZZER,HIGH);
	digitalWrite(BUZZER,LOW);
	delay(10);
	digitalWrite(BUZZER,HIGH);
	delay(70);
	digitalWrite(BUZZER,LOW);
	delay(10);
	digitalWrite(BUZZER,HIGH);
	digitalWrite(BUZZER,LOW);
	delay(10);
	digitalWrite(BUZZER,HIGH);
	delay(70);
}

void bz_notice()
{
	if (ctl.flag_bzon == false)
	{
		return;
	}
	digitalWrite(BUZZER,LOW);
	delay(10);
	digitalWrite(BUZZER,HIGH);
	delay(50);
	digitalWrite(BUZZER,LOW);
	delay(10);
	digitalWrite(BUZZER,HIGH);
}

void SH_EDGE()
{
	digitalWrite(SHCP,LOW);
	delayMicroseconds(1);
	digitalWrite(SHCP,HIGH);
	delayMicroseconds(1);
	digitalWrite(SHCP,LOW);
}

void ST_PLUSE()
{
	digitalWrite(STCP,LOW);
	delayMicroseconds(1);
	digitalWrite(STCP,HIGH);
	delayMicroseconds(1);
	digitalWrite(STCP,LOW);
}

void led_out(int data)
{
	int i = 0;
	for ( i = 0 ;i < 8 ;i ++)
	{
		if (data & 0x80)
		{
			digitalWrite(DS,HIGH);
		}
		else
		{
			digitalWrite(DS,LOW);
		}
		data <<= 1;
		SH_EDGE();
	}
	ST_PLUSE();

	return ;
}

uint8 SEGData[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x50,0x79,0x39,0x40,0X38};//1-9 r E C - L 

void digital(uint8 one ,uint8 two, uint8 three ,uint8 four)
{
	TM1637_writeCammand(0x44); 

	TM1637_writeData(0xc0,SEGData[one]);
	TM1637_writeData(0xc1,SEGData[two]);
	TM1637_writeData(0xc2,SEGData[three]);
	TM1637_writeData(0xc3,SEGData[four]);

	TM1637_writeCammand(0x8a);
}

void TM1637_start(void)
{
	digitalWrite(CLK1,HIGH);
	digitalWrite(DI,HIGH);
	delayMicroseconds(140);	
	digitalWrite(DI,LOW);
	delayMicroseconds(140);	
	digitalWrite(CLK1,LOW);
	delayMicroseconds(140);	
}


void TM1637_stop(void)
{
	digitalWrite(CLK1,LOW);
	delayMicroseconds(140);	
	digitalWrite(DI,LOW);
	delayMicroseconds(140);	
	digitalWrite(CLK1,HIGH);
	delayMicroseconds(140);	
	digitalWrite(DI,HIGH);
	delayMicroseconds(140);	
}


void TM1637_write1Bit(uint8 mBit) 
{
	digitalWrite(CLK1,LOW);
	delayMicroseconds(140);	

	if(mBit)
	{
	digitalWrite(DI,HIGH);;
	}
	else
	{
	digitalWrite(DI,LOW);
	}
	delayMicroseconds(140);	
	digitalWrite(CLK1,HIGH); 
	delayMicroseconds(140);	
}


void TM1637_write1Byte(uint8 mByte)
{
	uint8 loop = 0;

	for(loop=0;loop<8;loop++)
	{
		TM1637_write1Bit((mByte>>loop)&0x01);
	}

	digitalWrite(CLK1,LOW);
	delayMicroseconds(140);	
	digitalWrite(DI,HIGH);
	delayMicroseconds(140);	
	digitalWrite(CLK1,HIGH);
	delayMicroseconds(140);	

	pinMode(DI,INPUT);
	while(digitalRead(DI)==1);
	pinMode(DI,OUTPUT);
}

void TM1637_writeCammand(uint8 mData)
{ 
	TM1637_start();
	TM1637_write1Byte(mData);
	TM1637_stop();
}
 

void TM1637_writeData(uint8 addr,uint8 mData)
{
	TM1637_start(); 
	TM1637_write1Byte(addr);
	TM1637_write1Byte(mData);
	TM1637_stop();
}


