#pragma once

#include "opencv\cv.h"


using namespace std;

class PensilDetector
{
	public:
		PensilDetector(void);
		cv::Point DrawPensil(cv::Mat frame);
	private:
		int FindBiggestContour(vector<vector<cv::Point>> contours);
		cv::Point GetBottomPoint(vector<cv::Point> points);
		void DrawRotateRect(cv::Mat frame, cv::RotatedRect rRect);

		int counter = 0;
		std::vector<vector<cv::Point>> contoursContainer;

};
