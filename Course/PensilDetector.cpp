#include "PensilDetector.h"
#include "opencv2\opencv.hpp"

#pragma warning(disable:4996)


PensilDetector::PensilDetector(void)
{
}

cv::Point PensilDetector::GetBottomPoint(vector<cv::Point> points)
{
	cv::Point tempPoint = cv::Point(0, 0);
	for (int i = 0; i < points.size(); i++)
	{
		if (points[i].y > tempPoint.y)
		{	
			tempPoint = points[i];
		}
	}
	return tempPoint;
}

void PensilDetector::CalculatePositionAndClicks(cv::Mat frame,cv::Mat wheelUp,cv::Mat wheelDown)
{
	vector<vector<cv::Point> > contours;
	vector<cv::Vec4i> hierarchy;

	findContours(frame, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	if (contours.size() == 0)
		return;

	int big = FindBiggestContour(contours);
	if (cv::contourArea(contours[big]) < 500)
	{
		return;
	}

	vector<int> convex;
	cv::convexHull(cv::Mat(contours[big]), convex, false);
	vector<cv::Vec4i> offsets;
	cv::convexityDefects(cv::Mat(contours[big]), convex, offsets);

	if (offsets.size() != 0)
	{
		for (int i = 0; i < offsets.size(); i++)
		{
			cv::Point tempPoint = cv::Point((contours[big][offsets[i].val[0]].x + contours[big][offsets[i].val[1]].x) / 2, (contours[big][offsets[i].val[0]].y + contours[big][offsets[i].val[1]].y) / 2);
			
			if (sqrt(pow(abs(tempPoint.x - contours[big][offsets[i].val[2]].x), 2) + pow(abs(tempPoint.y - contours[big][offsets[i].val[2]].y), 2))>50)
			{
				return;
			}
		}	
		 
		angel = cv::fitEllipse(contours[big]).angle - 90;
		mousePosition = GetBottomPoint(contours[big]);
		CheckWheels(contours[big], wheelUp, wheelDown);
	}
}

void PensilDetector::CheckWheels(vector<cv::Point> pensil,cv::Mat wheelUp, cv::Mat wheelDown)
{
	vector<vector<cv::Point> > contours;
	vector<cv::Vec4i> hierarchy;

	findContours(wheelUp, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	for (int i = 0; i < contours.size(); i++)
	{
		int j;
		for (j = 0; j < contours[i].size(); j++)
		{
			int temp = cv::pointPolygonTest(pensil, contours[i][j], false);
			if (temp == -1 || temp == 0)
			{
				break;
			}
		}

		if (j == contours[i].size())
		{
			if (!wheelDownFlag)
			{
				wheelUpFlag = true;
			}

			break;
		}
		else
		{
			if (wheelUpFlag)
			{
				wheelUpFlag = false;
			}
		}
	}

	findContours(wheelDown, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	for (int i = 0; i < contours.size(); i++)
	{
		int j;
		for (j = 0; j < contours[i].size(); j++)
		{
			if (cv::pointPolygonTest(pensil, contours[i][j], false) == -1)
			{
				break;
			}
		}

		if (j == contours[i].size())
		{
			if (!wheelDownFlag)
			{
				wheelDownFlag = true;
			}

			break;
		}
		else
		{
			if (wheelDownFlag)
			{
				wheelDownFlag = false;
			}
		}
	}
}

int PensilDetector::FindBiggestContour(vector<vector<cv::Point> > contours)
{
	int indexOfBiggestContour = -1;
	int sizeOfBiggestContour = 0;

	for (int i = 0; i < contours.size(); i++) 
	{
		if (contours[i].size() > sizeOfBiggestContour) 
		{
			sizeOfBiggestContour = contours[i].size();
			indexOfBiggestContour = i;
		}
	}

	return indexOfBiggestContour;
}

void PensilDetector::CheckClick()
{
	if ((angel > 1 && angel < 83) || (angel > -83 && angel < -1))
	{
		if (!clickPreparationFlag)
		{
			clickPreparationFlag = true;
		}
	}
	else
	{
		if (clickPreparationFlag)
		{
			clickPreparationFlag = false;
		}
	}

	if (angel > 1 && angel<75)
	{
		if (leftClickFlag)
		{
			leftClickFlag = false;
		}

		if (timerRight)
		{
			rightClickFlag = false;
		}
		else
		{
			rightClickFlag = true;
		}

		if (timer == 0)
		{
			timerRight = true;
		}

		if (timerRight)
		{
			timer++;
		}

		if (timerRight > timerClick)
		{
			timer = 0;
			timerRight = false;
		}
	}
	else
	{
		if (angel > -75 && angel<-1)
		{
			if (rightClickFlag)
			{
				rightClickFlag = false;
			}

			if (timerLeft)
			{
				leftClickFlag = false;
			}
			else
			{
				leftClickFlag = true;
			}

			if (timer == 0)
			{
				timerLeft = true;
			}

			if (timerLeft)
			{
				timer++;
			}

			if (timerLeft > timerClick)
			{
				timer = 0;
				timerLeft = false;
			}
		}
		else
		{
				rightClickFlag = false;
				timerRight = false;
				timer = 0;
				leftClickFlag = false;
				timerLeft = false;
				timer = 0;
		}
	}
}

bool PensilDetector::GetLeftClick()
{
	return leftClickFlag;
}

bool PensilDetector::GetRightClick()
{
	return rightClickFlag;
}

bool PensilDetector::GetClickPreparation()
{
	return clickPreparationFlag;
}

bool PensilDetector::GetWheelUp()
{
	return wheelUpFlag;
}

bool PensilDetector::GetWheelDown()
{
	return wheelDownFlag;
}

cv::Point PensilDetector::GetMousePosition()
{
	return mousePosition;
}