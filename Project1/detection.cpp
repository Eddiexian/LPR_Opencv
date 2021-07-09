#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


// 右 上 下 左
void drawLines(Mat& input, const std::vector<Vec2f>& lines) {
	for (int i = 0; i < lines.size(); i++) {
		float r = lines[i][0];
		float theta = lines[i][1];
		if (theta < CV_PI / 4.0 || theta>3 * CV_PI / 4.0) {
			Point pt1(r / cos(theta), 0);
			Point pt2((r - input.rows * sin(theta)) / cos(theta), input.rows);
			line(input, pt1, pt2, Scalar(255, 255, 255), 3);
		}
		else {
			Point pt1(0, r / sin(theta));
			Point pt2(input.cols, (r - input.cols * cos(theta)) / sin(theta));
			line(input, pt1, pt2, Scalar(255, 255, 255), 3);
		}
	}
}



vector<Point> findLinePoint(Vec2f& line,int rows,int cols)
{
	float r = line[0];
	float theta = line[1];
	Point pt1, pt2;
	if (theta < CV_PI / 4.0 || theta>3 * CV_PI / 4.0) {
		pt1.x = r / cos(theta);
		pt1.y = 0;
		pt2.x = (r - rows * sin(theta)) / cos(theta);
		pt2.y = rows;
	}
	else {
		pt1.x = 0;
		pt1.y = r / sin(theta);
		pt2.x = cols;
		pt2.y = (r - cols * cos(theta)) / sin(theta);
	}

	vector<Point> linepoint{ { pt1,pt2 } };

	return linepoint;
}


double getDistance(Point a, Point b)
{
	double distance;
	distance = powf((a.x - b.x), 2) + powf((a.y - b.y), 2);
	distance = sqrtf(distance);

	return distance;
}

Point2f getCrossPoint(Vec4i LineA, Vec4i LineB)
{
	double ka, kb;
	ka = (double)(LineA[3] - LineA[1]) / (double)(LineA[2] - LineA[0]); //求出LineA斜率
	kb = (double)(LineB[3] - LineB[1]) / (double)(LineB[2] - LineB[0]); //求出LineB斜率

	Point2f crossPoint;
	crossPoint.x = (ka * LineA[0] - LineA[1] - kb * LineB[0] + LineB[1]) / (ka - kb);
	crossPoint.y = (ka * kb * (LineA[0] - LineB[0]) + ka * LineB[1] - kb * LineA[1]) / (ka - kb);
	return crossPoint;
}


Mat preprocess_perceptive(Mat src,Mat edge)
{
	vector<vector<Point> > contours;

	findContours(edge, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	if (contours.empty())
		cout << "contours empty";

	double largest_area = 0;
	double area;
	vector<vector<Point> > largest_contours;


	//尋找最大contour區域
	for (int i = 0; i < contours.size(); i++) {  // get the largest contour
		area = fabs(contourArea(contours[i]));
		if (area >= largest_area) {
			largest_area = area;
			largest_contours.clear();
			largest_contours.push_back(contours[i]);
		}
	}


	//使用convexhull縮減候選Point
	vector<vector<Point> >hull(largest_contours.size());
	for (size_t i = 0; i < largest_contours.size(); i++)
	{
		convexHull(largest_contours[i], hull[i]);
	}

	Mat drawing = Mat::zeros(src.size(), CV_8UC1);
	for (size_t i = 0; i < largest_contours.size(); i++)
	{

		drawContours(drawing, hull, (int)i, Scalar(255, 255, 255));
	}
	//imshow("Hull demo", drawing);

	//cout << "ConvexHull: " << hull[0] << endl;


	//霍夫找線 嘗試取出四角落Point

	vector< Vec2f > lines;

	HoughLines(drawing, lines, 1, CV_PI / 50, 50);
	//cout << "霍夫找線" << endl;

	Mat line_test = Mat::zeros(src.rows, src.cols, CV_8UC3);

	drawLines(drawing, lines);

	//imshow("找線", drawing);


	int rowsForline = src.rows;
	int colsForline = src.cols;

	vector<vector<Point>> all_lines;
	for (auto a : lines)
	{
		all_lines.push_back(findLinePoint(a, rowsForline, colsForline));
	}

	Vec4i RLine(all_lines[0][0].x, all_lines[0][0].y, all_lines[0][1].x, all_lines[0][1].y);
	Vec4i TLine(all_lines[1][0].x, all_lines[1][0].y, all_lines[1][1].x, all_lines[1][1].y);
	Vec4i BLine(all_lines[2][0].x, all_lines[2][0].y, all_lines[2][1].x, all_lines[2][1].y);
	Vec4i LLine(all_lines[3][0].x, all_lines[3][0].y, all_lines[3][1].x, all_lines[3][1].y);

	Point RT = getCrossPoint(RLine, TLine);
	Point RB = getCrossPoint(RLine, BLine);
	Point LT = getCrossPoint(LLine, TLine);
	Point LB = getCrossPoint(LLine, BLine);

	//circle(src, RT, 2, Scalar(0, 0, 255), 1);
	//circle(src, RB, 2, Scalar(0, 0, 255), 1);
	//circle(src, LT, 2, Scalar(0, 0, 255), 1);
	//circle(src, LB, 2, Scalar(0, 0, 255), 1);

	Point2f srcrect[4] = {
		LB,LT,RT,RB
	};

	RotatedRect pos = minAreaRect(hull[0]);


	//dstrect[0] = LB
	//dstrect[1] = LT
	//dstrect[2] = RT
	//dstrect[3] = RB


	Point2f dstrect[4];

	pos.points(dstrect);
	//cout << "POINT" << endl;
	//cout << dstrect[0] << endl;
	//cout << dstrect[1] << endl;
	//cout << dstrect[2] << endl;
	//cout << dstrect[3] << endl;


	//line(src, dstrect[0], dstrect[1], Scalar(255, 0, 0), 2);
	//line(src, dstrect[1], dstrect[2], Scalar(255, 0, 0), 2);
	//line(src, dstrect[2], dstrect[3], Scalar(255, 0, 0), 2);
	//line(src, dstrect[0], dstrect[3], Scalar(255, 0, 0), 2);


	//計算透視轉換矩陣
	Mat transformMartix = getPerspectiveTransform(srcrect, dstrect);

	Mat perspective;
	warpPerspective(src, perspective, transformMartix, src.size(), cv::INTER_LINEAR);

	//imshow("perpective", perspective);

	return perspective;

}

Mat AffineTransform(Mat src,Mat edge)
{
	vector<vector<Point> > contours;

	findContours(edge, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	if (contours.empty())
		cout << "contours empty";

	double largest_area = 0;
	double area;
	vector<vector<Point> > largest_contours;


	//尋找最大contour區域
	for (int i = 0; i < contours.size(); i++) {  // get the largest contour
		area = fabs(contourArea(contours[i]));
		if (area >= largest_area) {
			largest_area = area;
			largest_contours.clear();
			largest_contours.push_back(contours[i]);
		}
	}


	//使用convexhull縮減候選Point
	vector<vector<Point> >hull(largest_contours.size());
	for (size_t i = 0; i < largest_contours.size(); i++)
	{
		convexHull(largest_contours[i], hull[i]);
	}

	Mat drawing = Mat::zeros(src.size(), CV_8UC1);
	for (size_t i = 0; i < largest_contours.size(); i++)
	{

		drawContours(drawing, hull, (int)i, Scalar(255, 255, 255));
	}


	RotatedRect pos = minAreaRect(hull[0]);


	//dstrect[0] = LB
	//dstrect[1] = LT
	//dstrect[2] = RT
	//dstrect[3] = RB


	Point2f dstrect[4];

	pos.points(dstrect);

	Point2f srcTri[3];
	Point2f dstTri[3];

	

	srcTri[0] = dstrect[1];//LT,LB,RB
	srcTri[1] = dstrect[0];
	srcTri[2] = dstrect[3];
	



	//todo
	dstTri[0] = dstrect[1];
	dstTri[1] = Point2f(dstrect[1].x, dstrect[0].y);
	dstTri[2] = Point2f(dstrect[3].x, dstrect[0].y);
	

	Mat warp_mat(2, 3, CV_32FC1);
	warp_mat = getAffineTransform(srcTri, dstTri);

	Mat warp_dst;
	warp_dst = Mat::zeros(src.rows, src.cols, src.type());
	
	warpAffine(src, warp_dst, warp_mat, warp_dst.size());

	Point center = Point(warp_dst.cols / 2, warp_dst.rows / 2);
	double angle = -50.0;
	double scale = 0.6;
	Mat rot_mat(2, 3, CV_32FC1);
	Mat warp_rotate_dst;

	
	rot_mat = getRotationMatrix2D(center, angle, scale);

	
	warpAffine(src, warp_rotate_dst, rot_mat, warp_dst.size());
	

	//imshow("warp_window", warp_dst);



	//imshow("warp_rotate", warp_rotate_dst);


	/*circle(src, dstrect[0], 1, Scalar(255, 255, 0),3,-1);
	circle(src, dstrect[1], 1, Scalar(0, 255, 0),3,-1);
	circle(src, dstrect[2], 1, Scalar(0, 0, 255),3,-1);
	circle(src, dstrect[3], 1, Scalar(255, 0, 0),3,-1);*/

	return warp_dst;
}

int main()
{
	Mat src = imread("D://ALJ_0000.jpg");

	if (src.empty())
	{
		cout << "Can not find the image..." << endl;
		system("pause");
	}
	//imshow("Input image", src);

	Mat gray_img, bin_img;
	cvtColor(src, gray_img, COLOR_BGR2GRAY);
	threshold(gray_img, bin_img, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

	imshow("Bin img", bin_img);
	Mat edge;
	Canny(bin_img, edge, 200, 100);

	imshow("Canny img", edge);

	Mat processed = preprocess_perceptive(src, edge);

	imshow("test", processed);
	
	Mat gray_img_processed, bin_img_processed;
	cvtColor(processed, gray_img_processed, COLOR_BGR2GRAY);
	threshold(gray_img_processed, bin_img_processed, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

	imshow("Bin img", bin_img_processed);
	Mat edge_processed;
	Canny(bin_img, edge_processed, 200, 100);

	Mat last = AffineTransform(processed, edge_processed);

	imshow("after transform", last);



	waitKey(0);

	return 0;



}