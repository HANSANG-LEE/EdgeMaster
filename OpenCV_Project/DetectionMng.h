#pragma once

#include "opencv.hpp"
#include "cv.h"
#include "highgui.hpp"

using namespace std;
using namespace cv;

class DetectionMng
{
public:
	DetectionMng();
	virtual ~DetectionMng();

private:
	Rect m_MaskRect = Rect(Point(30, 30), Point(290, 210));
	VideoCapture m_Capture;

public:
	vector<Point2f> DetectFeaturePoints(Mat &frame);
	vector<Vec3f> DetectCircles(Mat &frame);
	void DrawMaskRect(Mat &frame);
	void DrawCircleOnPoints(vector<Point2f> &points, Mat &frame);
	void DrawDetectedCircles(vector<Vec3f> &points, Mat&frame);
};