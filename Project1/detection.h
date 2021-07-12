#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void drawLines(Mat& input, const std::vector<Vec2f>& lines);

vector<Point> findLinePoint(Vec2f& line, int rows, int cols);

double getDistance(Point a, Point b);

Point2f getCrossPoint(Vec4i LineA, Vec4i LineB);

Mat preprocess_perceptive(Mat src, Mat edge);

Mat AffineTransform(Mat src, Mat edge);

Mat preprocess_all(Mat src);