#pragma once

#include<opencv\cv.h>


using namespace std;

class BackgroundRemover {
public:
	bool calibrated = false;
	BackgroundRemover(void);
	void calibrate(cv::Mat input);
	cv::Mat BackgroundRemover::getForeground(cv::Mat input);

private:
	cv::Mat background;

	cv::Mat getForegroundMask(cv::Mat input);
	void BackgroundRemover::removeBackground(cv::Mat input, cv::Mat background);
};