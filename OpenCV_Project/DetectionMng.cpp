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
	/* �������������������������������������������������������������������������������������������������������������������� */
	// 8��Ʈ �Ǵ� 32��Ʈ �Ǽ��� 1ä�� ���󿡼� Ư¡���� �����Ѵ�.
	/* �������������������������������������������������������������������������������������������������������������������� */
	/* Variables for goodFeaturesToTrack() */
	static vector<Point2f> vCornerPoints;		// ����� Ư¡��
	static const int maxCorners = 300;			// ������ �ִ� Ư¡�� ��
	static double qualityLevel = 0.001;			// �ּ� Ư���� ���� �����ϴ� ��
	static double minDistance = 200.0;			// Ư¡�� ���� �ּ� �Ÿ�
	static InputArray mask = noArray();		// ����� ���� ����
	static int gBlockSize = 5;							// ��� ����� ũ��
	static bool useHarrisDetector = true;		// cornerHarris() ��� �� ���ΰ�?
	static double k = 0.04;								// cornerHarris()���� ���Ǵ� ���

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
	/* �������������������������������������������������������������������������������������������������������������������� */
	// 8��Ʈ 1ä�� �׷��� ������ ���󿡼� ���� �����Ѵ�.
	/* �������������������������������������������������������������������������������������������������������������������� */
	/* Variables for HoughCircles() */
	static vector<Vec3f> vHoughCircles;			// ���� �Ķ����(center_x, center_y, radius) ����
	static int method = HOUGH_GRADIENT;		// ���� �����ϴ� ���
	static double dp = 1.0;									// ��ť�ķ����� ���ݿ��� ���� ���������� �� ����
	static double minDist;									// ����� ���� �߽� ������ �ּ� �Ÿ�
	static double param1 = 100.0;						// Canny ���� ���� �Լ��� ���� �Ӱ谪
	static double param2 = 100.0;						// Canny ���� ���� �Լ��� ���� �Ӱ谪
	static int minRadius = 0;								// ���� �ּ� ������
	static int maxRadius = 0;								// ���� �ִ� ������

	/* Detecting */
	HoughCircles(frame, vHoughCircles, method, dp, minDist);

	/* Return Value */
	return vHoughCircles;
}
void DetectionMng::DrawMaskRect(Mat &frame)
{
	/* Variables for rectangle() */
	static const Scalar color = Scalar(0, 0, 0);	// ����
	static int thickness = 1;										// �β�
	static int lineType = CV_AA;								// ��
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
	// cvRound() : �Ǽ����� �ݿø��Ͽ� ���������� ��ȯ�Ѵ�.
	// contains() : ������ ���� �ȿ� �ش� ����Ʈ�� �����ϴ°�?
}
void DetectionMng::DrawDetectedCircles(vector<Vec3f> &points, Mat&frame)
{
	/* Variables for circle() */
	static int center_x, center_y;							// ���� �߽� X, Y ��ǥ
	static int radius;											// ���� ������
	static Scalar color = Scalar(0, 0, 255);			// ����
	static int thickness = 2;									// �β�
	static int lineType = CV_AA;							// ��
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