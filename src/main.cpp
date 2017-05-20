#include "common.h"
#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

#include "include.h"
#include <wiringPi.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

struct _ctl ctl;

int main()
{
	ctl.flag_bzon = 1;
	ctl.flag_debug = true;

//
	//check+network here
	//connect server
	//update data
//


//
	itac_init();
//OLED_TEST();
//OLED_WORK();


	cam_init();

	for(;;)
	{

		//hole both key while switch mode
		if  ( !digitalRead(LK) && !digitalRead(RK) ) 
		{
			bz_notice();
			delay(700);
			bz_notice();
			delay(700);
			bz_notice();
			delay(1500);
			if  ( !digitalRead(LK) && !digitalRead(RK) ) 
			{
				bz_notice();
				bz_notice();
				//power off
				system("shutdown -h now");
			}
			
		}
		
		ctl.flag_mode = digitalRead(DIPS2);
		if ( ctl.flag_mode )
		{
			digital(13 ,10, 11 ,12);
			rec_init();
			bz_ready();
			while(1)
			{
				if ( ctl.flag_mode != digitalRead(DIPS2) )
				{ break;}
				shot();
			}
		}

	}

	return 0;
}