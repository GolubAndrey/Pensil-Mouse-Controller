#pragma once

#include<opencv\cv.h>


using namespace cv;
using namespace std;

class BackgroundRemover {
public:
	bool calibrated = false;
	BackgroundRemover(void);
	void calibrate(Mat input);
	Mat BackgroundRemover::getForeground(Mat input);

private:
	Mat background;

	Mat getForegroundMask(Mat input);
	void BackgroundRemover::removeBackground(Mat input, Mat background);
};