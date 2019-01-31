#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <Windows.h>

#include "BackgroundRemover.h"
#include "SkinDetector.h"
#include "FaceDetector.h"
#include "FingerCount.h"
#include "PensilDetector.h"

#pragma warning(disable:4996)

using namespace cv;
using namespace std;

const int cursorOffsetRange = 3;

/*int main(int, char**) {
	cv::VideoCapture videoCapture(1);
	videoCapture.set(CV_CAP_PROP_SETTINGS, 1);

	if (!videoCapture.isOpened()) {
		cout << "Can't find camera!" << endl;
		return -1;
	}

	cv::Mat frame, frameOut, handMask, foreground, fingerCountDebug;

	BackgroundRemover backgroundRemover;
	SkinDetector skinDetector;
	FaceDetector faceDetector;
	FingerCount fingerCount;
	PensilDetector pensilDetector;

	while (true) {
		videoCapture >> frame;
		frameOut = frame.clone();

		skinDetector.drawSkinColorSampler(frameOut);

		foreground = backgroundRemover.getForeground(frame);
		imshow("window", frameOut);
		imshow("test", skinDetector.getSkinMask(foreground));
		if (skinDetector.calibrate && backgroundRemover.calibrated)
			pensilDetector.DrawPensil(skinDetector.getSkinMask(foreground));

		int key = waitKey(1);

		if (key == 27) // esc
			break;
		else if (key == 98) // b
			backgroundRemover.calibrate(frame);
		else if (key >= int('1') && key <= int('6'))
			skinDetector.calibrateInPosition(frame, key - 49);
	}

	return 0;
}*/

int main(int argc, char** argv)
{
	bool cursorFlag = false;
	cv::VideoCapture videoCapture(1);
	videoCapture.set(CV_CAP_PROP_SETTINGS, 1);

	if (!videoCapture.isOpened()) {
		cout << "Can't find camera!" << endl;
		return -1;
	}

	SkinDetector skinDetector;
	BackgroundRemover backgroundRemover;
	PensilDetector pensilDetector;

	cv::Mat frame, frameOut, foreground;

	VideoCapture cap(1); //capture the video from web cam

	if (!cap.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the web cam" << endl;
		return -1;
	}

	namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

	int iLowH = 0;
	int iHighH = 179;

	int iLowS = 0;
	int iHighS = 255;

	int iLowV = 0;
	int iHighV = 255;

	//Create trackbars in "Control" window
	cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
	cvCreateTrackbar("HighH", "Control", &iHighH, 179);

	cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
	cvCreateTrackbar("HighS", "Control", &iHighS, 255);

	cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
	cvCreateTrackbar("HighV", "Control", &iHighV, 255);

	cv::Point point;
	point.x = 0;
	point.y = 0;

	iLowH = 75;
	iLowS = 113;
	iLowV = 23;
	iHighH = 125;

	while (true)
	{
		videoCapture >> frame;
		frameOut = frame.clone();

		Mat imgHSV;

		//foreground = backgroundRemover.getForeground(frame);

		frameOut = skinDetector.getSkinMask(frame, iLowH, iLowS, iLowV, iHighH, iHighS, iHighV);

		
		if (backgroundRemover.calibrated)
		{
			//pensilDetector.DrawPensil(frameOut);
			pensilDetector.CalculatePositionAndClicks(frameOut);
			cv::Point point2 = pensilDetector.GetMousePosition();
			if (!((point.x < point2.x + cursorOffsetRange) && (point.x > point2.x - cursorOffsetRange) && (point.y < point2.y + cursorOffsetRange) && (point.y > point2.y - cursorOffsetRange)))
			{
				point.x = (point.x + point2.x) / 2;
				point.y = (point.y + point2.y) / 2;
			}
			cv::Size size=frame.size();
			if (cursorFlag)
			{
				int x;
				int y;
				POINT cursorPoint;
				if (point.x < 100 || point.x>size.width-100 || point.y<100 || point.y>size.height-50)
				{
					GetCursorPos(&cursorPoint);
					x = cursorPoint.x;
					y = cursorPoint.y;
				}
				else
				{
					int temp = GetSystemMetrics(SM_CXSCREEN);
					x = temp*(point.x-100)/(size.width-200);
					y = GetSystemMetrics(SM_CYSCREEN)*(point.y-100) / (size.height-150);
				}
				SetCursorPos(x, y);
				if (pensilDetector.GetLeftClick())
				{
					//mouse_event(MOUSEEVENTF_LEFTDOWN, x, y, 0, 0);
					puts("Left click");
				}

				if (pensilDetector.GetRightClick())
				{
					//mouse_event(MOUSEEVENTF_RIGHTDOWN, x, y, 0, 0);
					puts("Right click");
				}
			}
			
			
		}

		imshow("Thresholded Image", frameOut); //show the thresholded image
		imshow("Original", frame); //show the original image

		int key = waitKey(1);
		if (key == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
		else if (key == 98) // b
			backgroundRemover.calibrate(frame);
		else
			if (key == 32)
				if (!cursorFlag)
					cursorFlag = true;
				else
					cursorFlag = false;

		
	}

	return 0;

}