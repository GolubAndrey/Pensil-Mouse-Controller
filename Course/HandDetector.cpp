// FingerTips.cpp : Defines the entry point for the console application.
//

/*************************************************************************
*******************Author : P.W.G. Dilan Shaminda*************************
*******************Email : dlnshaminda@gmail.com**************************
**************************************************************************/

/*#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
using namespace cv;
const int max_value_H = 360 / 2;
const int max_value = 255;
const String window_capture_name = "Video Capture";
const String window_detection_name = "Object Detection";
int low_H = 0, low_S = 0, low_V = 0;
int high_H = max_value_H, high_S = max_value, high_V = max_value;
static void on_low_H_thresh_trackbar(int, void *)
{
	low_H = min(high_H - 1, low_H);
	setTrackbarPos("Low H", window_detection_name, low_H);
}
static void on_high_H_thresh_trackbar(int, void *)
{
	high_H = max(high_H, low_H + 1);
	setTrackbarPos("High H", window_detection_name, high_H);
}
static void on_low_S_thresh_trackbar(int, void *)
{
	low_S = min(high_S - 1, low_S);
	setTrackbarPos("Low S", window_detection_name, low_S);
}
static void on_high_S_thresh_trackbar(int, void *)
{
	high_S = max(high_S, low_S + 1);
	setTrackbarPos("High S", window_detection_name, high_S);
}
static void on_low_V_thresh_trackbar(int, void *)
{
	low_V = min(high_V - 1, low_V);
	setTrackbarPos("Low V", window_detection_name, low_V);
}
static void on_high_V_thresh_trackbar(int, void *)
{
	high_V = max(high_V, low_V + 1);
	setTrackbarPos("High V", window_detection_name, high_V);
}
int main(int argc, char* argv[])
{
	VideoCapture cap(1);
	namedWindow(window_capture_name);
	namedWindow(window_detection_name);
	// Trackbars to set thresholds for HSV values
	createTrackbar("Low H", window_detection_name, &low_H, max_value_H, on_low_H_thresh_trackbar);
	createTrackbar("High H", window_detection_name, &high_H, max_value_H, on_high_H_thresh_trackbar);
	createTrackbar("Low S", window_detection_name, &low_S, max_value, on_low_S_thresh_trackbar);
	createTrackbar("High S", window_detection_name, &high_S, max_value, on_high_S_thresh_trackbar);
	createTrackbar("Low V", window_detection_name, &low_V, max_value, on_low_V_thresh_trackbar);
	createTrackbar("High V", window_detection_name, &high_V, max_value, on_high_V_thresh_trackbar);
	Mat frame, frame_HSV, frame_threshold;
	while (true) {
		cap >> frame;
		if (frame.empty())
		{
			break;
		}
		// Convert from BGR to HSV colorspace
		cvtColor(frame, frame_HSV, COLOR_BGR2HSV);
		// Detect the object based on HSV Range Values
		inRange(frame_HSV, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), frame_threshold);
		// Show the frames
		imshow(window_capture_name, frame);
		imshow(window_detection_name, frame_threshold);
		char key = (char)waitKey(30);
		if (key == 'q' || key == 27)
		{
			break;
		}
	}
	return 0;
}*/

/*int findBiggestContour(vector<vector<Point> > contours)
{
	int indexOfBiggestContour = -1;
	int sizeOfBiggestContour = 0;

	for (int i = 0; i < contours.size(); i++) {
		if (contours[i].size() > sizeOfBiggestContour) {
			sizeOfBiggestContour = contours[i].size();
			indexOfBiggestContour = i;
		}
	}
	return indexOfBiggestContour;
}*/

/*#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\video\background_segm.hpp>
#include <opencv2\opencv.hpp>
#include <stdio.h>
#include <iostream>
#pragma warning(disable : 4996)

using namespace cv;
using namespace std;



int main(int argc, char* argv[])
{   
	Ptr < BackgroundSubtractor >  pMOG2;
	Mat fgMaskMOG2;
	Mat fKNN;

	pMOG2 = createBackgroundSubtractorMOG2();

	Ptr<BackgroundSubtractorKNN> KNN = createBackgroundSubtractorKNN();

	VideoCapture cap(1);  // open the default camera
	Mat frame(Size(640, 420), CV_8UC3);
	Mat frame2(Size(640, 420), CV_8UC3);
	Mat frame_threshold;
	//Mat frame, frame2;
	//createTrackbars(); // create trackbars
	if (!cap.isOpened())
		return -1;

	int ncomp;

	while (true) 
	{
		cap >> frame; // new frame from camera

		pMOG2->apply(frame, fgMaskMOG2);

		KNN->apply(frame, fKNN);

		cv::Mat bw;

		cv::cvtColor(frame, bw, CV_RGB2GRAY);



		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;

		Canny(bw, bw, 100, 200, 3);

		findContours(bw, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

		Mat drawing = Mat::zeros(bw.size(), CV_8UC3);
		char text[200] = "";
		CvFont font = cvFont(2, 2);

		for (int i = 0; i< contours.size(); i++)
		{
			cv::Rect brect = cv::boundingRect(contours[i]);
			if (brect.area() < 20000)
				continue;
			drawContours(drawing, contours, i, CV_RGB(255, 0, 0), 2, 8, hierarchy, 0, Point());
			sprintf(text, "S = %d", brect.area());
			putText(drawing, text, cvPoint(brect.x + 20, brect.y + 20), 1, 1, CV_RGB(0, 255, 0));
			rectangle(drawing, brect, CV_RGB(0, 0, 255), 3);
		}
		
		imshow("Original Video", fKNN);
		if (cvWaitKey(15) == 27)  break;
	}
	return 0;
}*/

