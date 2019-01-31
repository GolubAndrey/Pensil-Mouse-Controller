#pragma once

#include "opencv\cv.h"


using namespace std;

class PensilDetector
{
	public:
		PensilDetector(void);
		
		bool GetLeftClick();
		bool GetRightClick();
		bool GetWheelUp();
		bool GetWheelDown();
		bool GetClickPreparation();
		cv::Point GetMousePosition();
		
		void CheckClick();

		void CalculatePositionAndClicks(cv::Mat frame, cv::Mat wheelUp, cv::Mat wheelDown);
	private:
		int FindBiggestContour(vector<vector<cv::Point>> contours);
		void CheckWheels(vector<cv::Point> pensil,cv::Mat wheelUp, cv::Mat wheelDown);
		cv::Point GetBottomPoint(vector<cv::Point> points);

		bool leftClickFlag = false;
		bool rightClickFlag = false;
		bool wheelUpFlag = false;
		bool wheelDownFlag = false;
		bool clickPreparationFlag = false;

		bool timerLeft = false;
		bool timerRight = false;

		int timer = 0;
		int angel = 0;

		const int timerClick = 1000;
		
		cv::Point mousePosition;

};
