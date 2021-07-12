#include <iostream>
#include <io.h>
#include <opencv2/opencv.hpp>
#include "detection.h"

using namespace cv;
using namespace std;
using namespace cv::ml;



char a[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9','A','B','C','D','E','F','G','H','J','K','L','M','N','P','Q','R','S','T','U','V','W','X','Y','Z' };

int find(char ar[], int n, char element)
{
	int i = 0;
	int index = -1;
	for (i = 0; i < n; i++)
	{
		if (element == ar[i])
		{
			index = i;
		}
	}
	return index;

};

vector<Mat> verticalProject(Mat srcimg)
{
	//row,colss
	int width_src = srcimg.cols;
	int height_src = srcimg.rows;

	vector<int> vector_num; //cols white pix
	for (int i = 0; i < width_src; i++)
	{
		int white_num = 0;
		for (int j = 0; j < height_src; j++)
		{
			int prePixel = srcimg.at<uchar>(j, i); //row,col
			if (prePixel == 255)                   //while
				white_num++;
		}

		vector_num.push_back(white_num);
	}

	cout << "Number" << vector_num.size() << endl;
	cout << "Cols" << width_src << endl;
	cout << "Rows" << height_src << endl;

	Mat draw_img(height_src, width_src, CV_8UC1, Scalar(0)); //init,type,black
	for (int i = 0; i < width_src; i++)
	{
		line(draw_img, Point(i, height_src), Point(i, height_src - vector_num[i]), Scalar(255), 1, 8);
	}
	imshow("Draw_imgV", draw_img);

	bool alreadystart = false;
	int start_index = 0;
	int end_index = 0;
	vector<Mat> roi_img; //seg

	for (int i = 0; i < width_src; i++)
	{
		if (!alreadystart && vector_num[i] != 0) //if this cols white pix != 1 then set this to start point
		{
			alreadystart = true;
			start_index = i;
		}
		else if (alreadystart && vector_num[i] <= 3) //fuzzy range
		{
			alreadystart = false;
			end_index = i;
			if (start_index > 3 && end_index < width_src - 4)
			{
				start_index -= 3;
				end_index += 3;
			}
			Mat roi = srcimg(Range(0, height_src), Range(start_index, end_index)); //segimg
			if (roi.cols >= 10)
			{
				roi_img.push_back(roi);
			}
		}
	}
	cout << "Roi size" << roi_img.size() << endl;
	return roi_img;
}

vector<Mat> horizontalProject(Mat srcimg)
{
	//row,cols
	int width_src = srcimg.cols;
	int height_src = srcimg.rows;

	vector<int> vector_num; //cols white pix
	for (int i = 0; i < height_src; i++)
	{
		int white_num = 0;
		for (int j = 0; j < width_src; j++)
		{
			int prePixel = srcimg.at<uchar>(i, j); //row,col
			if (prePixel == 255)                   //while
				white_num++;
		}

		vector_num.push_back(white_num);
	}

	cout << "Number" << vector_num.size() << endl;
	cout << "Cols" << width_src << endl;
	cout << "Rows" << height_src << endl;

	Mat draw_img(height_src, width_src, CV_8UC1, Scalar(0)); //init,type,black
	for (int i = 0; i < height_src; i++)
	{
		line(draw_img, Point(width_src, i), Point(width_src - vector_num[i], i), Scalar(255), 1, 8);
	}
	imshow("Draw_imgH", draw_img);

	bool alreadystart = false;
	int start_index = 0;
	int end_index = 0;
	vector<Mat> roi_img; //seg

	for (int i = 0; i < height_src; i++)
	{
		if (!alreadystart && vector_num[i] != 0) //if this cols white pix != 1 then set this to start point
		{
			alreadystart = true;
			start_index = i;
		}
		else if (alreadystart && vector_num[i] <= 20) //fuzzy range
		{
			alreadystart = false;
			end_index = i;
			if (start_index > 3 && end_index < height_src - 4)
			{
				start_index -= 3;
				end_index += 3;
			}
			Mat roi = srcimg(Range(start_index, end_index), Range(0, width_src)); //segimg
			if (roi.rows >= 20)
			{
				roi_img.push_back(roi);
			}
		}
	}
	cout << "Roi size" << roi_img.size() << endl;
	return roi_img;
}

vector<float> hog_feature_extractor(Mat img)
{



	// HOG特徵描述子
	Mat dst;
	resize(img, dst, Size(20, 20)); 

	HOGDescriptor hog(Size(20, 20), Size(4, 4), Size(2, 2), Size(4, 4), 4); // HOG特徵描述子提
	vector<float> descriptors;
	vector<Point> locations;
	hog.compute(dst, descriptors, Size(1, 1), Size(0, 0), locations);

	cout << descriptors.size();
	return descriptors;

}

vector<string> data_dir_list(string type)
{

	cout << "get data: " + type << endl;
	vector<string> path_list;
	string inPath = "D:\\data\\";
	type = type + "\\";
	string datatype = "*.jpg";

	



	long handle;
	struct _finddata_t fileinfo;

	handle = _findfirst((inPath+type+datatype).c_str(), &fileinfo);

	if (handle == -1)
	{
		return path_list;
	}
	do
	{
		//找到的檔案黨名
		
		//cout << fileinfo.name << endl;
		path_list.push_back(inPath+type+fileinfo.name);
	} while (!_findnext(handle, &fileinfo));

	_findclose(handle);
	
	return path_list;

}

void SVM_train()
{
	

	vector<string> path;

	
	Mat sampleFeatureMat = cv::Mat::zeros(13164, 1296, CV_32FC1);
	
	Mat sampleLabelMat = cv::Mat::zeros(13164, 1, CV_32SC1);
	int num = 0;
	int totalsize = 0;
	for (auto c : a)
	{
		string type;
		type.push_back(c);
		path = data_dir_list(type);
		totalsize += path.size();
		cout << totalsize << endl;
		for (int i = 0; i < path.size(); i++)
		{
			num += 1;
			Mat src = imread(path[i]);
			cout << "Image Read:  " << path[i] << endl;
			if (src.empty())
			{
				cout << "Can not find the image..." << endl;
				system("pause");
			}

			vector<float> temp = hog_feature_extractor(src);
			cout << "Hog Feature size : " << temp.size()<< endl;
			cout << num << endl;
			for (int j = 0 ; j < 1296; j++)
			{
				sampleFeatureMat.at<float>(num, j) = temp[j];
			}
			
			int label = find(a,34,c);
			
			sampleLabelMat.at<int>(num, 0) = label;

			cout << "label:" << sampleLabelMat.at<int>(num, 0) << endl;

		}
		
	}



	

	
	
	cv::Ptr<cv::ml::TrainData> td = cv::ml::TrainData::create(sampleFeatureMat, cv::ml::SampleTypes::ROW_SAMPLE, sampleLabelMat);


	


	
	Ptr<SVM> svm = SVM::create();
  //以下設定 SVM 參數
	svm->setType(SVM::C_SVC);
	svm->setKernel(SVM::RBF); //Kernel Function 推薦使用 RBF
	svm->setDegree(10.0);
	svm->setGamma(0.09);
	svm->setCoef0(1.0);
	svm->setC(10.0);
	svm->setNu(0.5);
	svm->setP(1.0);
	svm->setTermCriteria(TermCriteria(TermCriteria::EPS, 100,  FLT_EPSILON));

	cout << "SVM start train!" << endl;
	//10.0, 0.09, 1.0, 10.0, 0.5, 1.0
	svm->train(td);
	
	std::cout << "SVM分類器訓練完成" << std::endl;

	//將訓練好的SVM模型儲存為xml檔案
	svm->save("SVM_HOG.xml");


}

void KNN_train()
{

	vector<string> path;

	//13164 = numbers of sample
	//324 = hog feature size
	Mat sampleFeatureMat = cv::Mat::zeros(13164, 324, CV_32FC1);

	Mat sampleLabelMat = cv::Mat::zeros(13164, 1, CV_32SC1);
	int num = 0;
	int totalsize = 0;
	for (auto c : a)
	{
		string type;
		type.push_back(c);
		path = data_dir_list(type);
		totalsize += path.size();
		cout << totalsize << endl;
		for (int i = 0; i < path.size(); i++)
		{
			num += 1;
			Mat src = imread(path[i]);
			cout << "Image Read:  " << path[i] << endl;
			if (src.empty())
			{
				cout << "Can not find the image..." << endl;
				system("pause");
			}

			vector<float> temp = hog_feature_extractor(src);
			cout << "Hog Feature size : " << temp.size() << endl;
			cout << num << endl;
			for (int j = 0; j < 324; j++)
			{
				sampleFeatureMat.at<float>(num, j) = temp[j];
			}

			int label = find(a, 34, c);
			
			sampleLabelMat.at<int>(num, 0) = label;

			cout << "label:" << sampleLabelMat.at<int>(num, 0) << endl;

		}

	}



	cv::Ptr<cv::ml::TrainData> td = cv::ml::TrainData::create(sampleFeatureMat, cv::ml::SampleTypes::ROW_SAMPLE, sampleLabelMat);

	Ptr<KNearest> model = KNearest::create();
	
	model->setDefaultK(5);
	model->setIsClassifier(true);
	model->train(td);

	model->save("KNN_model.xml");
}

int KNN_predict(Mat src,Ptr<KNearest> model)
{

	
	vector<float> descriptors;
	descriptors = hog_feature_extractor(src);

	cout << "開始識別..." << std::endl;
	cv::Mat testDescriptor = cv::Mat::zeros(1, descriptors.size(), CV_32FC1);
	for (int i = 0; i < descriptors.size(); i++)
	{
		testDescriptor.at<float>(0, i) = descriptors[i];
	}
	int ans;
	ans = model->predict(testDescriptor);

	return ans;
}

int SVM_predict(Mat src, Ptr<SVM> svm)
{
	

	vector<float> descriptors;
	descriptors = hog_feature_extractor(src);

	cout << "開始識別..." << std::endl;
	cv::Mat testDescriptor = cv::Mat::zeros(1, descriptors.size(), CV_32FC1);
	for (int i = 0; i < descriptors.size(); i++)
	{
		testDescriptor.at<float>(0, i) = descriptors[i];
	}
	int ans;
	ans = svm->predict(testDescriptor);
	std::cout << "這張圖屬於：" << ans <<std::endl;
	return  ans;
}


int main(int argc, char** argv)
{

	/* SVM Model Loading
	
	Ptr<SVM> svm = cv::ml::SVM::load("SVM_HOG.xml ");
	if (svm->empty())
	{
		std::cout << "load svm detector failed!!!" << std::endl;
		return 0;
	}

	*/
	
	//KNN_train();
	//SVM_train();
	

	cout << "Loading KNN model..." << endl;
	Ptr<KNearest> model = StatModel::load<KNearest>("KNN_model.xml");
	cout << "Success loading." << endl;


	
	Mat src = imread("D://ALJ_0000.jpg");
	
	if (src.empty())
	{
		cout << "Can not find the image..." << endl;
		system("pause");
	}
	imshow("Input image", src);


	Mat pic = preprocess_all(src);

	//imshow("processed", pic);
	Rect roi;
	roi = selectROI(pic);
	pic = pic(roi);
	imshow("ROI", pic);
	Mat gray_img, bin_img;
	cvtColor(pic, gray_img, COLOR_BGR2GRAY);
	threshold(gray_img, bin_img, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

	imshow("Bin img", bin_img);

	//先進行水平投影
	vector<Mat> roi_first = horizontalProject(bin_img);

	for (int i = 0; i < roi_first.size(); i++)
	{
		vector<Mat> roi_Final = verticalProject(roi_first[i]); //垂直投影
		for (int j = 0; j < roi_Final.size(); j++)
		{
			string k = to_string(j);
			imshow(k, roi_Final[j]);
			int ans = KNN_predict(roi_Final[j],model); //KNN分類
			//int ans =  SVM_predict(roi_Final[j],svm);
			cout << "分類: " << a[ans] << endl;
		}
	}
	

	waitKey(0);
	
	
	return 0;
}
