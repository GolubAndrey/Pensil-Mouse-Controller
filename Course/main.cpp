#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

#include "BackgroundRemover.h"
#include "SkinDetector.h"
#include "FaceDetector.h"
#include "FingerCount.h"
#include "PensilDetector.h"

#pragma warning(disable:4996)

using namespace cv;
using namespace std;

int main(int, char**) {
	cv::VideoCapture videoCapture(1);
	videoCapture.set(CV_CAP_PROP_SETTINGS, 1);

	if (!videoCapture.isOpened()) {
		cout << "Can't find camera!" << endl;
		return -1;
	}

	cv::Mat frame, frameOut, handMask, foreground, fingerCountDebug;

	BackgroundRemover backgroundRemover;
	SkinDetector skinDetector;
	FaceDetector faceDetector;
	FingerCount fingerCount;
	PensilDetector pensilDetector;

	while (true) {
		videoCapture >> frame;
		frameOut = frame.clone();

		skinDetector.drawSkinColorSampler(frameOut);

		foreground = backgroundRemover.getForeground(frame);
		/*faceDetector.removeFaces(frame, foreground);
		handMask = skinDetector.getSkinMask(foreground);
		fingerCountDebug = fingerCount.findFingersCount(handMask, frameOut);*/

		//imshow("foreground", foreground);
		//skinDetector.getSkinMask(frame)
		imshow("window", frameOut);
		imshow("test", skinDetector.getSkinMask(foreground));
		//imshow("back", foreground);
		if (skinDetector.calibrate && backgroundRemover.calibrated)
			pensilDetector.DrawPensil(skinDetector.getSkinMask(foreground));
		//imshow("handMask", handMask);
		//imshow("handDetection", fingerCountDebug);

		int key = waitKey(1);

		if (key == 27) // esc
			break;
		else if (key == 98) // b
			backgroundRemover.calibrate(frame);
		else if (key >= int('1') && key <= int('6'))
			skinDetector.calibrateInPosition(frame, key - 49);
		/*else if (key == 115) // s
			skinDetector.calibrateInPosition(frame,1);
		if (key == 97)//a
			skinDetector.calibrateInPosition(frame, 0);
		if (key == 100)//d
			skinDetector.calibrateInPosition(frame, 2);*/
	}

	return 0;
}