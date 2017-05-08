
#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include "matcher.h"

int main()
{
	cv::Mat image1= cv::imread("1.jpg",0);
	cv::Mat image2= cv::imread("2.jpg",0);
	if (!image1.data || !image2.data)
		return 0; 

	cv::namedWindow("Image 1");
	cv::imshow("Image 1",image1);
	cv::namedWindow("Image 2");
	cv::imshow("Image 2",image2);

	RobustMatcher rmatcher;
	rmatcher.setImage(image1, image2);
	rmatcher.setConfidenceLevel(0.98);
	rmatcher.setMinDistanceToEpipolar(1.0);
	rmatcher.setRatio(0.65f);
	cv::Ptr<cv::FeatureDetector> pfd= new cv::SurfFeatureDetector(10); 
	rmatcher.setFeatureDetector(pfd);

	cv::Mat fundemental= rmatcher.match();

	
	cv::Mat imageMatches;
	cv::drawMatches(image1,rmatcher.keypoints1, 
		            image2,rmatcher.keypoints2,  
					rmatcher.matches,			
					imageMatches,		
					cv::Scalar(255,255,255)); 
	cv::namedWindow("Matches");
	cv::imshow("Matches",imageMatches);
	
	
	std::vector<cv::Point2f> points1, points2;
	for (std::vector<cv::DMatch>::const_iterator it= rmatcher.matches.begin();
		 it!= rmatcher.matches.end(); ++it) {

			
			 float x= rmatcher.keypoints1[it->queryIdx].pt.x;
			 float y= rmatcher.keypoints1[it->queryIdx].pt.y;
			 points1.push_back(cv::Point2f(x,y));
			 
			 x= rmatcher.keypoints2[it->trainIdx].pt.x;
			 y= rmatcher.keypoints2[it->trainIdx].pt.y;
			 points2.push_back(cv::Point2f(x,y));
	}

	std::cout << points1.size() << " " << points2.size() << std::endl; 

	
	std::vector<uchar> inliers(points1.size(),0);
	cv::Mat homography= cv::findHomography(
		cv::Mat(points1),cv::Mat(points2),
		inliers,	
		CV_RANSAC,
		1.);	    

	
	std::vector<cv::Point2f>::const_iterator itPts= points1.begin();
	std::vector<uchar>::const_iterator itIn= inliers.begin();
	while (itPts!=points1.end()) {

		
		if (*itIn) 
 			cv::circle(image1,*itPts,3,cv::Scalar(255,255,255),2);
		
		++itPts;
		++itIn;
	}

	itPts= points2.begin();
	itIn= inliers.begin();
	while (itPts!=points2.end()) {

		
		if (*itIn) 
			cv::circle(image2,*itPts,3,cv::Scalar(255,255,255),2);
		
		++itPts;
		++itIn;
	}

    
	cv::namedWindow("Image 1 Homography Points");
	cv::imshow("Image 1 Homography Points",image1);
	cv::namedWindow("Image 2 Homography Points");
	cv::imshow("Image 2 Homography Points",image2);

	
	cv::Mat result;
	cv::warpPerspective(image1,
		result,			
		homography,		
		cv::Size(2*image1.cols,image1.rows));
	cv::imshow("result", result);
	
	cv::Mat half(result,cv::Rect(0,0,image2.cols,image2.rows));
	image2.copyTo(half);

    
	cv::namedWindow("After warping");
	cv::imshow("After warping",result);

	cv::waitKey();
	return 0;
}