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


CascadeClassifier haar_cascade;
Ptr<FaceRecognizer> model;

VideoCapture cap(-1);


void cam_init()
{
	if (!cap.isOpened())
	{
		cerr << "Capture Device ID" << "1" << "cannot be opened." << endl;
		return ;
	}
	else
	{
		cout << "done!" << endl;
		cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
		cap.set(CV_CAP_PROP_FRAME_HEIGHT,480);	
cout << "out_set"<<endl;
	}
}

void rec_init()
{
   	string fn_haar = "haarcascade_frontalface_alt.xml";
	haar_cascade.load(fn_haar);				
    	model = createLBPHFaceRecognizer();
    	model->load("facerec.yml");

	model->set("threshold",10000.0);
}



void rec()
{
	vector< Rect_<int> > faces;
	Mat gray;
	Mat frame;
	cap >> frame;
	OLED_Clear(); 
	Mat original;
	original = frame.clone();
         					// Convert the current frame to grayscale:

//shot
	ctl.flag_attack = digitalRead(DIPS1);
	if ( ( !digitalRead(LK) && !digitalRead(RK) ) || ctl.flag_attack )  //click LK comfirm
	{
		char time[15];
		get_systime(time);
		
		if ( ctl.flag_attack == true )
		{
			OLED_ShowString(1,0,(uint8 *)("attack"),2);
		}
		else
		{
			OLED_ShowString(1,0,(uint8 *)("shot"),2);
		}
		imwrite(format("./shot/%s.JPG",time), original);
		delay(1000);
		return;
	}



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
            	Mat face = gray(face_i);
            					// Resizing the face is necessary for Eigenfaces and Fisherfaces. You can easily
            					// verify this, by reading through the face recognition tutorial coming with OpenCV.
            					// Resizing IS NOT NEEDED for Local Binary Patterns Histograms, so preparing the
            					// input data really depends on the algorithm used.
            					// I strongly encourage you to play around with the algorithms. See which work best
            					// in your scenario, LBPH should always be a contender for robust face recognition.
            					//
            					// Since I am showing the Fisherfaces algorithm here, I also show how to resize the
            					// face you have just found:
            	Mat face_resized;
            	cv::resize(face, face_resized, Size(100,100 ), 1.0, 1.0, INTER_CUBIC);
            					// Now perform the prediction, see how easy that is:

		

            	int prediction = -1;
		double confidence = 0.0;
		model->predict(face_resized,prediction,confidence);


            					// And finally write all we've found out to the original image!
            					// First of all draw a green rectangle around the detected face:
            	rectangle(original, face_i, CV_RGB(0, 255,0), 1);
           					// Create the text we will annotate the box with:

OLED_Rectangle(face_i.x / 10, ( face_i.y  )/15,(face_i.x + face_i.width)/10,(face_i.y + face_i.height)/15);


           	string box_text = format("Prediction = %d", prediction);
            					// Calculate the position for annotated text (make sure we don't
            					// put illegal values in there):
            	int pos_x = std::max(face_i.tl().x - 10, 0);
            	int pos_y = std::max(face_i.tl().y - 10, 0);
            					// And now put it into the image:
            	putText(original, box_text, Point(pos_x, pos_y), FONT_HERSHEY_PLAIN, 1.5, CV_RGB(255,0,255), 2.0);

		cout << "prediction = "<< prediction << ";   confidence = "<<  confidence << endl;
  	     }
        					// Show the result:
	if (ctl.flag_debug == true)
	{     
       	 imshow("face_recognizer", original);
	}

	waitKey(1);
    	return ;
}



