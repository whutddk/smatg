#ifndef __OLED_H
#define __OLED_H			  	 
  
#include "common.h"

#define I2CSDA_H digitalWrite(3,HIGH)

#define I2CSDA_L digitalWrite(3,LOW)

#define I2CCLK_H digitalWrite(5,HIGH)

#define I2CCLK_L digitalWrite(5,LOW)

#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  



#endif  
	 



