#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;


//
//int main(int argc, char** argv)
//{
//
//
//
//	Mat src = imread("D://2.jpg");
//	if (src.empty())
//	{
//		cout << "Can not find the image..." << endl;
//		system("pause");
//	}
//
//	imshow("Input image", src);
//
//	Mat gray_img, bin_img, bin_img1;
//	cvtColor(src, gray_img, COLOR_BGR2GRAY);
//
//	imshow("Gray Image", gray_img);
//
//	Mat dst;
//	
//	threshold(gray_img, bin_img, 0 , 255, THRESH_BINARY_INV | THRESH_OTSU);
//	equalizeHist(bin_img, dst);
//	
//	
//	imshow("Bin img", bin_img);
//	imshow("EquHist Image",dst);
//
//	waitKey(0);
//	return 0;
//}

//
//Mat hist; //將要獲得的直方圖
//	int imgNum = 1;//影象數
//	int histDim = 1;//直方圖維度
//	int histSize = 256; //直方圖每一維度bin個數
//	float range[] = { 0, 256 };//每一維度的統計範圍
//	const float* histRange = { range };//因為我們計算1維直方圖所以 只有一個rang
//	bool uniform = true;//
//	bool accumulate = false;//
//	calcHist(&gray_img, imgNum, 0, Mat(), hist, histDim, &histSize, &histRange, uniform, accumulate);
//
//	int scale = 2;
//	Mat histImg(cv::Size(histSize * scale, histSize), CV_8UC1);//用於顯示直方圖
//
//
//	uchar* pImg = nullptr;
//	for (size_t i = 0; i < histImg.rows; i++) //初始化影象為全黑
//	{
//		pImg = histImg.ptr<uchar>(i);
//		for (size_t j = 0; j < histImg.cols; j++)
//		{
//			pImg[j] = 0;
//		}
//	}
//
//	double maxValue = 0; //直方圖中最大的bin的值
//	minMaxLoc(hist, 0, &maxValue, 0, 0); //minMaxLoc可以計算最大值最小值以及其對應的位置 這裡求最大值
//
//	int histHeight = 256; //要繪製直方圖的最大高度
//
//	float* p = hist.ptr<float>(0);
//	for (size_t i = 0; i < histSize; i++)//進行直方圖的繪製
//	{
//		float bin_val = p[i];
//		int intensity = cvRound(bin_val * histHeight / maxValue);  //要繪製的高度 
//		for (size_t j = 0; j < scale; j++) //繪製直線 這裡用每scale條直線代表一個bin
//		{
//			cv::line(histImg, cv::Point(i * scale + j, histHeight - intensity), cv::Point(i * scale + j, histHeight - 1), 255);
//		}
//		
//	}
//	cv::namedWindow("直方圖");
//	cv::imshow("直方圖", histImg);
