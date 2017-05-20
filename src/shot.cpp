/*************************************************************************
    > File Name: shot.cpp
    > Author: DDK
    > Mail: 295054118@QQ.com 
    > Created Time: Sat 20 May 2017 23:50:48 CST
 ************************************************************************/

#include "common.h"
#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include <wiringPi.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <time.h>

#include "include.h"
#include <wiringPi.h>

using namespace cv;
using namespace std;




void shot()
{
	vector< Rect_<int> > faces;
	Mat gray;
	Mat frame;
	cap >> frame;
	Mat original;
	original = frame.clone();
         					// Convert the current frame to grayscale:

	cvtColor(original, gray, CV_BGR2GRAY);
        					// Find the faces in the frame:
        haar_cascade.detectMultiScale(gray, faces,1.2,2,0|CV_HAAR_SCALE_IMAGE,Size(100,100),Size(1000,1000));
        					// At this point you have the position of the faces in
        					// faces. Now we'll get the faces, make a prediction and

// there are faces in the camera	        // annotate it in the video. Cool or what?
	if (faces.size() > 0 )
//shot a picture
	{
		char time[15];
		get_systime(time);
		bz_notice();
		imwrite(format("./shot/%s.JPG",time), original);
		delay(1000);
	}
	return;

}