#pragma once

#include<opencv\cv.h>

using namespace std;

class ColorDetector
{
public:
	ColorDetector(void);
	void getPensilMask(cv::Mat &pensil,cv::Mat &wheelUp,cv::Mat &wheelDown);


	void CreateTrackbars();

private:
	int iLowPensilH = 0;
	int iHighPensilH = 179;

	int iLowPensilS = 0;
	int iHighPensilS = 255;

	int iLowPensilV = 0;
	int iHighPensilV = 255;

	int iLowWUH = 0;
	int iHighWUH = 179;

	int iLowWUS = 0;
	int iHighWUS = 255;

	int iLowWUV = 0;
	int iHighWUV = 255;

	int iLowWDH = 0;
	int iHighWDH = 179;

	int iLowWDS = 0;
	int iHighWDS = 255;

	int iLowWDV = 0;
	int iHighWDV = 255;



	void MorphOperations(cv::Mat &threshold);
};