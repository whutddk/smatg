#ifndef _INCLUDE_H_
#define _INCLUDE_H_

#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
using namespace cv;
using namespace std;

struct _ctl
{
	bool flag_mode;
	bool flag_attack;
	bool flag_bzon;
	bool flag_debug;
};


extern struct _ctl ctl;




extern CascadeClassifier haar_cascade;
extern Ptr<FaceRecognizer> model;

extern VideoCapture cap;
extern vector<Mat> images;
extern vector<int> labels;





extern void itac_init();
extern void SH_EDGE();
extern void ST_PLUSE();
extern void led_out(int data);
extern uint8 SEGData[];
extern void digital(uint8 one,uint8 two,uint8 three,uint8 four);
extern void TM1637_start();
extern void TM1637_stop();
extern void TM1637_write1Bit(uint8 mBit);
extern void TM1637_write1Byte(uint8 mByte);
extern void TM1637_writeCammand(uint8 mData);
extern void TM1637_writeData(uint8 addr,uint8 mData);
extern void bz_notice();
extern void bz_ready();

extern void rec_init();
extern void cam_init();
extern void learn_init();
extern void rec();
extern void learn();
extern void get_systime(char *systime);

extern void I2C_start(void);
extern void I2C_stop(void);
extern uint8 I2Ccheck_ACK(void);
extern void I2CSendByte(uint8 ch);

extern void OLED_WORK();
extern void OLED_TEST();
extern void OLED_init();				   
extern void Write_IIC_Command(uint8  IIC_Command);
extern void Write_IIC_Data(uint8 IIC_Data);
extern void fill_picture(uint8 fill_Data);
extern void OLED_Set_Pos(uint8 x, uint8 y);
extern void OLED_Display_On(void);
extern void OLED_Display_Off(void);
extern void OLED_Clear(void);
extern void OLED_On(void);
extern void OLED_ShowChar(uint8 x,uint8 y,uint8 chr,uint8 Char_Size);
extern uint32 oled_pow(uint8 m,uint8 n);
extern void OLED_DrawBMP(uint8 x0, uint8 y0,uint8 x1, uint8 y1,uint8 BMP[]);
extern void OLED_ShowString(uint8 x,uint8 y,uint8 *chr,uint8 Char_Size);
extern void OLED_ShowNum(uint8 x,uint8 y,uint32 num,uint8 len,uint8 size2);
extern void OLED_Rectangle(uint8 x1,uint8 y1,uint8 x2,uint8 y2);
extern void OLED_PutPixel(uint8 x,uint8 y);
#endif

