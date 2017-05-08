// Harris_1.cpp : 定义控制台应用程  b序的入口点。

#include "stdafx.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include "opencv2/opencv.hpp"
#include <iostream>

#include <iostream>
#include <vector>

const int PI = 3.1415926535;
//高斯滤波

float gauss(float x, float sigma)
{
	float x_1;
	if(sigma == 0)
		return 0.0;
	x_1 = (float)exp((-x * x) / (2 * sigma * sigma));
	return  x_1;
}

float meanGauss(float x, float sigma)
{
	float x_1;
	x_1 = (gauss(x, sigma) + gauss(x - 0.5, sigma) +
			gauss(x + 0.5, sigma)) / 3.0;
	x_1 = x_1 / (PI * 2.0 * sigma * sigma);
	return x_1;
}

float norm(float x, float y)
{
	return (float)sqrt(x * x + y * y);
}

float distance(float a, float b, float c, float d)
{
	return norm((a - c), (b - d));
}

void juanJiGauss(cv::Mat &result_1, cv::Mat &result_2, cv::Mat &result,
				 int rows, int cols, float sigma)
{
	int width = 3.35 * sigma + 0.33;

	for(int i = 0; i < rows; ++i)
	{
		uchar* output_1 = result_1.ptr<uchar>(i);
		uchar* output_2 = result_2.ptr<uchar>(i);
		uchar* output = result.ptr<uchar>(i);
		for(int j = 0; j < cols; ++j)
		{
			meanGauss(distance((float)i, float(j), (float)width, float(width)), sigma);
			meanGauss(distance((float)i, float(j), (float)width, float(width)), sigma);
			meanGauss(distance((float)i, float(j), (float)width, float(width)), sigma);
		}
	}
}


bool judgeLoopLocal(int _rows, int _cols, int size, int i, int j)
{
	if(i > (int)(size / 2) && i < _rows - (int)(size / 2)
						&& j > (int)(size / 2) && j < _cols - (int)(size / 2))
	{
		return true;					
	}
	else 
		return false;
}

/*非局部最大值抑制
算法比较低下，遍历整个图像，算法复杂度是O(n^4)
*/
void localMax(std::vector<cv::Point> &_localMax, int _rows, int _cols, int size, 
				std::vector<std::vector<double> > &minValue, std::vector<std::vector<double> > &tempLocalMax)
{
		for(int i = 0; i < _rows; ++i)
		{
				tempLocalMax.resize(_rows);
				for(int j = 0; j < _cols; ++j)
				{
					tempLocalMax[i].resize(_cols);
					double max = -1E6;
					if(judgeLoopLocal(_rows, _cols, size, i, j))
					{
							for(int m = 0; m < (int)(size / 2); ++m)
							{
								for(int n = 0; n < (int)(size / 2); ++n)
								{
									if(minValue[i + m - (int)(size / 2)][j + n - (int)(size / 2)] > max)
										max = minValue[i + m - (int)(size / 2)][j + n - (int)(size / 2)];
								}
							}
					}
					if(max > 0)
					{
						tempLocalMax[i][j] = max;
					}
					else
					{
						tempLocalMax[i][j] = 0;
					}
				}
		}
}


void _a(std::vector<cv::Point> &points, std::vector<std::vector<double> > &minValue, 
			//std::vector<std::vector<double> > &tempLocalMax ,
			int thread, int rows, int cols )
{
	for(int i = 0; i < rows; ++i)
	{
		for(int j =0; j < cols; ++j)
		{
			//if((minValue[i][j] == tempLocalMax[i][j]) && (tempLocalMax[i][j] > thread))
			if(minValue[i][j] > thread)
				points.push_back(cv::Point(j, i));
		}
	}
}

void XYPixelCoe(const cv::Mat &image, cv::Mat &result_1, 
				cv::Mat &result_2, cv::Mat &result, int rows, int cols)
{
	result_1.create(image.size(), image.type());
	result_2.create(image.size(), image.type());
	result.create(image.size(), image.type());
	for(int j = 1; j < rows - 1; j++)
	{
		const uchar* precious = 
			image.ptr<const uchar>(j - 1);
		const uchar* current = 
			image.ptr<const uchar>(j);
		const uchar* next = 
			image.ptr<const uchar>(j + 1);

		uchar* output_1 = result_1.ptr<uchar>(j);
		uchar* output_2 = result_2.ptr<uchar>(j);
		uchar* output = result.ptr<uchar>(j);
		for(int i = 1; i < cols - 1; ++i)
		{
			*output_1 = cv::saturate_cast<uchar>
							(precious[i + 1] + 2*current[i + 1] + next[i + 1]
							- precious[i - 1] - 2*current[i - 1] - next[i - 1]);
			*output_2 = cv::saturate_cast<uchar>
						(precious[i - 1] + 2*precious[i] + precious[i + 1]
							- next[i - 1] - 2*next[i] - next[i + 1]);
			*output = (*output_1) * (*output_2);

			output_1++;
			output_2++;
			output++;
		}
	}
}
/*template<typename T>
void  createTwoDArray(int rows, int cols, T **_array)
{
	_array = new T*[rows];
	for(int i = 0; i < rows; ++i)
	{
		_array[i] = new T[cols];
	}
}

template<typename T>
void deleteTwoDArray(int rows, int cols, T **_array)
{
	for(int i = 0; i < rows; ++i)
	{
		delete [cols]_array[i];
		_array[i] = NULL;
	}
	delete [rows]_array;
	_array = NULL;
}*/


void harrisResponse( const cv::Mat &result_1, const cv::Mat &result_2, const cv::Mat &result, 
						int rows, int cols, int k, std::vector<std::vector<double> > &minValue)
{
	minValue.resize(rows);
	for(int j = 0; j < rows; ++j)
	{
		minValue[j].resize(cols);
		const uchar* pixel_1 = result_1.ptr<const uchar>(j);
		const uchar* pixel_2 = result_2.ptr<const uchar>(j);
		const uchar* pixel = result.ptr<const uchar>(j);
		for(int i = 0; i < cols; ++i)
		{
			int xPixel = pixel_1[i] * pixel[i];
			int yPixel = pixel_2[i] * pixel_2[i];
			int xyPixel = pixel[i] * pixel[i];

			minValue[j][i] = (xPixel * yPixel- xyPixel) 
							 / (xPixel + yPixel + 0.0000001);
		}
	}
}

void drawOnImage(cv::Mat &image, const std::vector<cv::Point> &points,
				cv::Scalar color = cv::Scalar(255, 255, 255), 
				int radius = 3, int thickness = 2)
{
	std::vector<cv::Point>::const_iterator it = points.begin();
	while(it != points.end())
	{
		cv::circle(image, *it, radius, color, thickness);
		++it;
	}
}

void changeGrayImage(cv::Mat &cimage, cv::Mat &gimage)
{
	cv::cvtColor(cimage, gimage, CV_BGR2GRAY);
}

/*void changeGrayImage(cv::Mat &cimage, cv::Mat &gimage, 
					double a = 0.114, double b = 0.587, double c = 0.299)
{
	int m = cimage.rows;
	int n = cimage.cols * cimage.channels();
	for(int i = 0; i < m; ++i)
	{
		for(int j = 0; j < n; ++j)
		{

		}
	}
}*/

int _tmain(int argc, _TCHAR* argv[])
{
	float s;
	std::cin >> s;
	cv::Mat image = cv::imread("1.bmp");
	cv::Mat gimage;
	cv::Mat result[3];

	int _rows = image.rows;
	int _cols = image.cols ;
	std::cout << _rows << _cols;

	std::vector<cv::Point> points;

	std::vector<std::vector<double> > minValue;
	std::vector<std::vector<double> > tempLocalMax;

	changeGrayImage(image, gimage);
	XYPixelCoe(gimage, result[1], result[2], result[0], _rows, _cols);
	juanJiGauss(result[1], result[2], result[0], _rows, _cols, s);
	harrisResponse(result[1], result[2], result[0], _rows, _cols, 0.04, minValue);

	//localMax(points, _rows, _cols, 5, minValue, tempLocalMax);
	//_a(points, minValue, tempLocalMax, 8, _rows, _cols );
	_a(points, minValue, 5000, _rows, _cols );

	drawOnImage(gimage, points);

	cv::namedWindow("wangfei");
	cv::imshow("result", result[1]);
	cv::waitKey(500);
	cv::imshow("result", result[2]);
	cv::waitKey(500);
	cv::imshow("result", result[0]);
	cv::waitKey(500);
	cv::imshow("image", gimage);   
	cv::waitKey(5000);
	
	system("pause");
	return 0;
}
