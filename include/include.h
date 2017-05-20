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

extern void shot();
#endif

