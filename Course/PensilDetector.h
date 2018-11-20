#pragma once

#include "opencv\cv.h"

using namespace cv;
using namespace std;

class PensilDetector
{
	public:
		PensilDetector(void);
		void DrawPensil(Mat frame);
	private:
		int FindBiggestContour(vector<vector<Point>> contours);
};
