#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <Windows.h>

#include "ColorDetector.h"
#include "PensilDetector.h"

#pragma warning(disable:4996)

using namespace cv;
using namespace std;


bool cursorFlag = false;

const int cursorOffsetRange = 5;


int x;
int y;
bool clickFlag = false;



int main(int argc, char** argv)
{
	cv::VideoCapture videoCapture(1);
	videoCapture.set(CV_CAP_PROP_SETTINGS, 1);

	if (!videoCapture.isOpened()) 
	{
		cout << "Can't find camera!" << endl;
		return -1;
	}

	ColorDetector skinDetector;
	PensilDetector pensilDetector;

	cv::Mat frame,pensilFrame,wheelUpFrame,wheelDownFrame;

	VideoCapture cap(1);

	if (!cap.isOpened())
	{
		cout << "Cannot open the web cam" << endl;
		return -1;
	}

	cv::Point point;
	cv::Point prevPoint;
	point.x = 0;
	point.y = 0;
	
	while (true)
	{
		videoCapture >> frame;
		pensilFrame = frame.clone();
		wheelUpFrame = frame.clone();
		wheelDownFrame = frame.clone();

		skinDetector.getPensilMask(pensilFrame,wheelUpFrame,wheelDownFrame);
		cv::imshow("pensil", pensilFrame);
		cv::imshow("wheel down", wheelDownFrame);
		cv::imshow("wheel up", wheelUpFrame);

		if (cursorFlag)
		{
			pensilDetector.CalculatePositionAndClicks(pensilFrame, wheelUpFrame, wheelDownFrame);
			pensilDetector.CheckClick();
			if (!pensilDetector.GetClickPreparation())
			{
				prevPoint = point;
				point = pensilDetector.GetMousePosition();
				if (((point.x < prevPoint.x + cursorOffsetRange) && (point.x > prevPoint.x - cursorOffsetRange) && (point.y < prevPoint.y + cursorOffsetRange) && (point.y > prevPoint.y - cursorOffsetRange)))
				{
					cv::Size size = frame.size();
					POINT cursorPoint;
					if (point.x < 150 || point.x>size.width - 150 || point.y<150 || point.y>size.height - 50)
					{
						GetCursorPos(&cursorPoint);
						x = cursorPoint.x;
						y = cursorPoint.y;
					}
					else
					{
						int temp = GetSystemMetrics(SM_CXSCREEN);
						x = temp - temp*(point.x - 150) / (size.width - 300);
						y = GetSystemMetrics(SM_CYSCREEN)*(point.y - 150) / (size.height - 200);
					}
					SetCursorPos(x, y);
				}
			}

			if (pensilDetector.GetLeftClick())
			{
				mouse_event(MOUSEEVENTF_LEFTDOWN, x, y, 0, 0);
				clickFlag = true;
				puts("Left click");
				continue;
			}
			else
			{
				if (clickFlag)
				{
					mouse_event(MOUSEEVENTF_LEFTUP, x, y, 0, 0);
				}
				clickFlag = false;
			}

			if (pensilDetector.GetRightClick())
			{
				mouse_event(MOUSEEVENTF_RIGHTDOWN, x, y, 0, 0);
				mouse_event(MOUSEEVENTF_RIGHTUP, x, y, 0, 0);
				puts("Right click");
			}

			if (pensilDetector.GetWheelUp())
			{
				mouse_event(MOUSEEVENTF_WHEEL, 0, 0, DWORD(WHEEL_DELTA), 0);
				puts("wheel up");
			}

			if (pensilDetector.GetWheelDown())
			{
				mouse_event(MOUSEEVENTF_WHEEL, 0, 0, DWORD(-WHEEL_DELTA), 0);
				puts("wheel down");
			}
		}
			
		imshow("Original", frame);

		int key = waitKey(1);
		if (key == 27)
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
		else
		{
			if (key == 32)
			{
				if (!cursorFlag)
				{
					cursorFlag = true;
				}
				else
				{
					cursorFlag = false;
				}
			}
		}
	}

	return 0;
}