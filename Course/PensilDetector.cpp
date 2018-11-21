#include "PensilDetector.h"
#include "opencv2\opencv.hpp"

#pragma warning(disable:4996)

PensilDetector::PensilDetector(void)
{
}

void PensilDetector::DrawPensil(Mat frame)
{
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	findContours(frame, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	if (contours.size() == 0)
		return;
	Mat drawing = Mat::zeros(frame.size(), CV_8UC3);
	char text[200] = "";
	CvFont font = cvFont(2, 2);

	/*for (int i = 0; i< contours.size(); i++)
	{
		cv::Rect brect = cv::boundingRect(contours[i]);
		if (brect.area() < 1500)
			continue;
		drawContours(drawing, contours, i, CV_RGB(255, 0, 0), 2, 8, hierarchy, 0, Point());
		sprintf(text, "S = %d", brect.area());
		putText(drawing, text, cvPoint(brect.x + 20, brect.y + 20), 1, 1, CV_RGB(0, 255, 0));
		rectangle(drawing, brect, CV_RGB(0, 0, 255), 3);
	}*/
	int big = FindBiggestContour(contours);
	double epsilon = 0.1*arcLength(contours[big], true);
	approxPolyDP(contours[big],contours[big], epsilon, true);
	drawContours(drawing, contours, big, CV_RGB(255, 0, 0), 2, 8, hierarchy, 0, Point());
	//Rect brect = boundingRect(contours[big]);
	//rectangle(drawing, brect, CV_RGB(0, 0, 255), 3);
	imshow("Conturs",drawing);
}

int PensilDetector::FindBiggestContour(vector<vector<Point> > contours)
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
}