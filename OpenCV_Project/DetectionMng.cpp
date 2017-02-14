#include "stdafx.h"
#include "DetectionMng.h"

DetectionMng::DetectionMng()
{

}
DetectionMng::~DetectionMng()
{

}

vector<Point2f> DetectionMng::DetectFeaturePoints(Mat &frame)
{
	/* ────────────────────────────────────────────────────────── */
	// 8비트 또는 32비트 실수의 1채널 영상에서 특징점을 검출한다.
	/* ────────────────────────────────────────────────────────── */
	/* Variables for goodFeaturesToTrack() */
	static vector<Point2f> vCornerPoints;		// 검출된 특징점
	static const int maxCorners = 300;			// 검출할 최대 특징점 수
	static double qualityLevel = 0.001;			// 최소 특정의 질을 결정하는 값
	static double minDistance = 200.0;			// 특징점 간의 최소 거리
	static InputArray mask = noArray();		// 검출될 영역 지정
	static int gBlockSize = 5;							// 평균 블록의 크기
	static bool useHarrisDetector = true;		// cornerHarris() 사용 할 것인가?
	static double k = 0.04;								// cornerHarris()에서 사용되는 상수

	/* Detecting */
	//vCornerPoints.clear();
	goodFeaturesToTrack(
		frame,
		vCornerPoints,
		maxCorners,
		qualityLevel,
		minDistance,
		noArray(),
		gBlockSize,
		useHarrisDetector,
		k);

	/* Return Value */
	return vCornerPoints;
}
vector<Vec3f> DetectionMng::DetectCircles(Mat &frame)
{
	/* ────────────────────────────────────────────────────────── */
	// 8비트 1채널 그레이 스케일 영상에서 원을 검출한다.
	/* ────────────────────────────────────────────────────────── */
	/* Variables for HoughCircles() */
	static vector<Vec3f> vHoughCircles;			// 원의 파라미터(center_x, center_y, radius) 저장
	static int method = HOUGH_GRADIENT;		// 원을 검출하는 방법
	static double dp = 1.0;									// 어큐뮬레이터 간격에서 영상 간격으로의 역 비율
	static double minDist;									// 검출된 원의 중심 사이의 최소 거리
	static double param1 = 100.0;						// Canny 에지 검출 함수의 높은 임계값
	static double param2 = 100.0;						// Canny 에지 검출 함수의 낮은 임계값
	static int minRadius = 0;								// 원의 최소 반지츰
	static int maxRadius = 0;								// 원의 최대 반지름

	/* Detecting */
	HoughCircles(frame, vHoughCircles, method, dp, minDist);

	/* Return Value */
	return vHoughCircles;
}
void DetectionMng::DrawMaskRect(Mat &frame)
{
	/* Variables for rectangle() */
	static const Scalar color = Scalar(0, 0, 0);	// 색깔
	static int thickness = 1;										// 두께
	static int lineType = CV_AA;								// 선
	//static int shift;												//

	/* Drawing */
	rectangle(frame, m_MaskRect, color, thickness, lineType);
}
void DetectionMng::DrawCircleOnPoints(vector<Point2f> &points, Mat &frame)
{
	/* Drawing FeaturePoints on video */
	for (int i = 0; i < points.size(); i++)
	{
		int x = cvRound(points[i].x);
		int y = cvRound(points[i].y);
		if (m_MaskRect.contains(Point(x, y)))
			circle(frame, Point(x, y), 3, Scalar(0, 0, 255), -1, CV_AA);
	}
	// cvRound() : 실수형을 반올림하여 정수형으로 변환한다.
	// contains() : 정해진 범위 안에 해당 포인트가 존재하는가?
}
void DetectionMng::DrawDetectedCircles(vector<Vec3f> &points, Mat&frame)
{
	/* Variables for circle() */
	static int center_x, center_y;							// 원의 중심 X, Y 좌표
	static int radius;											// 원의 반지름
	static Scalar color = Scalar(0, 0, 255);			// 색깔
	static int thickness = 2;									// 두께
	static int lineType = CV_AA;							// 선
	static int shift = 0;										//

	/* Drawing */
	for (int i = 0; i < points.size(); i++)
	{
		center_x = cvRound(points[i][0]);
		center_y = cvRound(points[i][1]);
		radius = cvRound(points[i][2]);

		Point pt(center_x + radius, center_y + radius);
		if (m_MaskRect.contains(pt))
			circle(frame, Point(center_x, center_y), radius, color, thickness, lineType);
	}
}