#pragma once

#include "opencv\cv.h"


using namespace std;

class PensilDetector
{
	public:
		PensilDetector(void);
		void DrawPensil(cv::Mat frame);
		cv::Point GetBottomPoint(vector<cv::Point> points);

		bool GetLeftClick();
		bool GetRightClick();

		cv::Point GetMousePosition();

		void CalculatePositionAndClicks(cv::Mat frame);
	private:
		int FindBiggestContour(vector<vector<cv::Point>> contours);
		void DrawRotateRect(cv::Mat frame, cv::RotatedRect rRect);

		void CheckLeftAndRightClick(int angel);

		bool leftClickFlag = false;
		bool rightClickFlag = false;

		bool timerLeft = false;
		bool timerRight = false;

		int timer = 0;

		const int timerClick = 1000;
		
		cv::Point mousePosition;
		std::vector<vector<cv::Point>> tempVector;
		std::vector<vector<cv::Point>> contoursContainer;

};
