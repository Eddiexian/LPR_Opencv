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
//Mat hist; //�N�n��o�������
//	int imgNum = 1;//�v�H��
//	int histDim = 1;//����Ϻ���
//	int histSize = 256; //����ϨC�@����bin�Ӽ�
//	float range[] = { 0, 256 };//�C�@���ת��έp�d��
//	const float* histRange = { range };//�]���ڭ̭p��1������ϩҥH �u���@��rang
//	bool uniform = true;//
//	bool accumulate = false;//
//	calcHist(&gray_img, imgNum, 0, Mat(), hist, histDim, &histSize, &histRange, uniform, accumulate);
//
//	int scale = 2;
//	Mat histImg(cv::Size(histSize * scale, histSize), CV_8UC1);//�Ω���ܪ����
//
//
//	uchar* pImg = nullptr;
//	for (size_t i = 0; i < histImg.rows; i++) //��l�Ƽv�H������
//	{
//		pImg = histImg.ptr<uchar>(i);
//		for (size_t j = 0; j < histImg.cols; j++)
//		{
//			pImg[j] = 0;
//		}
//	}
//
//	double maxValue = 0; //����Ϥ��̤j��bin����
//	minMaxLoc(hist, 0, &maxValue, 0, 0); //minMaxLoc�i�H�p��̤j�ȳ̤p�ȥH�Ψ��������m �o�̨D�̤j��
//
//	int histHeight = 256; //�nø�s����Ϫ��̤j����
//
//	float* p = hist.ptr<float>(0);
//	for (size_t i = 0; i < histSize; i++)//�i�檽��Ϫ�ø�s
//	{
//		float bin_val = p[i];
//		int intensity = cvRound(bin_val * histHeight / maxValue);  //�nø�s������ 
//		for (size_t j = 0; j < scale; j++) //ø�s���u �o�̥ΨCscale�����u�N��@��bin
//		{
//			cv::line(histImg, cv::Point(i * scale + j, histHeight - intensity), cv::Point(i * scale + j, histHeight - 1), 255);
//		}
//		
//	}
//	cv::namedWindow("�����");
//	cv::imshow("�����", histImg);
