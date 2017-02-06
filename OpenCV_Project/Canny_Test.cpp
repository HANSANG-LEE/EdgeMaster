#include "opencv.hpp"

using namespace std;
using namespace cv;

int main()
{
	// 원본 이미지
	Mat srcImage;
	srcImage = imread("Replacement_Part03.jpg");
	if (srcImage.empty()) return -1;
	imshow("srcImage", srcImage);

	// Canny
	Mat cannyImage;
	cvtColor(srcImage, cannyImage, CV_RGB2GRAY);
	Canny(srcImage, cannyImage, 85, 170);
	imshow("cannyImage", cannyImage);

	// Sobel
	Mat sobelImage;
	Size ksize = Size(3, 3);
	int scale = 2.5;
	int delta = 0;
	int ddepth = CV_8U;
	GaussianBlur(srcImage, sobelImage, ksize, 0, 0, BORDER_DEFAULT);
	cvtColor(sobelImage, sobelImage, CV_RGB2GRAY);
	Sobel(srcImage, sobelImage, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	Sobel(srcImage, sobelImage, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	imshow("sobelImage", sobelImage);

	// Scharr
	Mat scharrImage;
	GaussianBlur(srcImage, scharrImage, ksize, 0, 0, BORDER_DEFAULT);
	cvtColor(sobelImage, scharrImage, CV_RGB2GRAY);
	Sobel(srcImage, scharrImage, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	Sobel(srcImage, scharrImage, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	imshow("scharrImage", scharrImage);
	
	waitKey(0);
	return 0;
}