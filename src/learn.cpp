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


void get_systime(char *systime)
{
	struct tm *curTime;
	time_t now;	
	now = time(NULL);
	curTime = localtime(&now);
	sprintf(systime,"%04d%02d%02d_%02d%02d%02d",curTime->tm_year+1900,curTime->tm_mon+1,curTime->tm_mday,curTime->tm_hour,curTime->tm_min,curTime->tm_sec);
	return ;

}


Ptr<FaceRecognizer> train_model;


void learn_init()
{
   	string fn_haar = "haarcascade_frontalface_alt.xml";
	haar_cascade.load(fn_haar);
	train_model = createLBPHFaceRecognizer();
	train_model -> load("facerec.yml");
}

void learn()
{
		vector< Rect_<int> > faces;
		Mat gray;
		Mat gray_face;
		Mat frame;
		cap >> frame;

		OLED_Clear(); 

		Mat original;
		original = frame.clone();
         					// Convert the current frame to grayscale:
     		cvtColor(original, gray, CV_BGR2GRAY);
        					// Find the faces in the frame:
        
        	haar_cascade.detectMultiScale(gray, faces,1.2,2,0|CV_HAAR_SCALE_IMAGE,Size(100,100),Size(1000,1000));
        					// At this point you have the position of the faces in
        					// faces. Now we'll get the faces, make a prediction and
	        					// annotate it in the video. Cool or what?



	Mat face_resized;
        for(int i = 0; i < faces.size(); i++) 
	{
            					// Process face by face:
            	Rect face_i = faces[i];
            					// Crop the face from the image. So simple with OpenCV C++:
            	Mat face = original(face_i);
            					// Resizing the face is necessary for Eigenfaces and Fisherfaces. You can easily
            					// verify this, by reading through the face recognition tutorial coming with OpenCV.
            					// Resizing IS NOT NEEDED for Local Binary Patterns Histograms, so preparing the
            					// input data really depends on the algorithm used.
            					// I strongly encourage you to play around with the algorithms. See which work best
            					// in your scenario, LBPH should always be a contender for robust face recognition.
            					//
            					// Since I am showing the Fisherfaces algorithm here, I also show how to resize the
            					// face you have just found:
            	
            	cv::resize(face, face_resized, Size(100,100), 1.0, 1.0, INTER_CUBIC);
            					// change size
		rectangle(original, face_i, CV_RGB(0, 255,0), 1);

       		OLED_Rectangle(face_i.x / 10, ( face_i.y  )/15,(face_i.x + face_i.width)/10,(face_i.y + face_i.height)/15);
		if (ctl.flag_debug == true)
		{
         		imshow("face", face_resized);
		
			waitKey(1);
		}
		while(digitalRead(LK)&&digitalRead(RK));
		
		delay(10);    //fifter
		if (!digitalRead(LK))
		{ 
			delay(1000);

			if (digitalRead(LK))  //click LK comfirm
			{
				char time[15];
				get_systime(time);
				imwrite(format("./sample/%s.JPG",time), face_resized);

				cvtColor(face_resized, gray_face, CV_BGR2GRAY);

				int prediction = -1;
				double confidence = 0.0;
				train_model->predict(gray_face,prediction,confidence);
				cout << "prediction = "<< prediction << ";   confidence = "<<  confidence << endl;

				vector<Mat> images;
				vector<int> labels;
				images.push_back(gray_face);
				labels.push_back(9);
				train_model ->update(images,labels);
				cout << "model_update!" << endl;

				bz_notice();
			}
			else //hold LK pass
			{
				while(!digitalRead(LK));//wait until KEY release
				continue;
			}	
		}

		if (!digitalRead(RK))
		{
			delay(1000);
			if (digitalRead(RK)) //cilck RK
			{
				train_model -> save("./facerec.yml");
				cout << "model_save!" << endl;
			}
			else
			{
				;
			}
		}
		
        }
        					// Show the result:
        
		if (ctl.flag_debug == true)
		{	
			imshow("face_recognizer", original);
		
			waitKey(1);
		}
    	return;
}