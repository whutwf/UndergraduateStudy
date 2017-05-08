
#if !defined MATCHER
#define MATCHER

#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include<opencv2/legacy/legacy.hpp>

class RobustMatcher {

  private:

	   // 特征检测器指针
	  cv::Ptr<cv::FeatureDetector> detector;
	  // 描述子提取器指针
	  cv::Ptr<cv::DescriptorExtractor> extractor;
	  float ratio; //两个最近邻的最大比率
	  bool refineF; //是否改善F矩阵
	  double distance; //到极线的距离
	  double confidence; // 置信概率

	  cv::Mat image1, image2;//输入图像
public:
	  std::vector<cv::DMatch> matches; // 输出匹配
	  std::vector<cv::KeyPoint> keypoints1;
	  std::vector<cv::KeyPoint> keypoints2;

  public:

	  RobustMatcher() : ratio(0.65f), refineF(true), confidence(0.99), distance(3.0) {	  

		   // SURF特征检测 器的默认特征
		  detector= new cv::SurfFeatureDetector();
		  extractor= new cv::SurfDescriptorExtractor();
	  }

	  void setImage(cv::Mat &image_1, cv::Mat &image_2)
	  {
		  image1 = image_1;
		  image2 = image_2;
	  }

	  void setFeatureDetector(cv::Ptr<cv::FeatureDetector>& detect) {

		  detector= detect;
	  }

	  void setDescriptorExtractor(cv::Ptr<cv::DescriptorExtractor>& desc) {

		  extractor= desc;
	  }

	  void setMinDistanceToEpipolar(double d) {

		  distance= d;
	  }

	  void setConfidenceLevel(double c) {

		  confidence= c;
	  }

	  void setRatio(float r) {

		  ratio= r;
	  }

	  void refineFundamental(bool flag) {

		  refineF= flag;
	  }

	  //移除大于阈值的匹配
	  int ratioTest(std::vector<std::vector<cv::DMatch>>& matches) {

		int removed=0;

		for (std::vector<std::vector<cv::DMatch>>::iterator matchIterator= matches.begin();
			 matchIterator!= matches.end(); ++matchIterator) {

				 if (matchIterator->size() > 1) {

					 if ((*matchIterator)[0].distance/(*matchIterator)[1].distance > ratio) {

						 matchIterator->clear(); 
						 removed++;
					 }

				 } else { 

					 matchIterator->clear(); 
					 removed++;
				 }
		}

		return removed;
	  }

	  // 插入对称匹配
	  void symmetryTest(const std::vector<std::vector<cv::DMatch>>& matches1,
		                const std::vector<std::vector<cv::DMatch>>& matches2,
					    std::vector<cv::DMatch>& symMatches) {
			
		// 图一到图二
		for (std::vector<std::vector<cv::DMatch>>::const_iterator matchIterator1= matches1.begin();
			 matchIterator1!= matches1.end(); ++matchIterator1) {

			if (matchIterator1->size() < 2) // 不计删除匹配
				continue;

			//  图而到图一
			for (std::vector<std::vector<cv::DMatch>>::const_iterator matchIterator2= matches2.begin();
				matchIterator2!= matches2.end(); ++matchIterator2) {

				if (matchIterator2->size() < 2) 
					continue;

				//匹配
				if ((*matchIterator1)[0].queryIdx == (*matchIterator2)[0].trainIdx  && 
					(*matchIterator2)[0].queryIdx == (*matchIterator1)[0].trainIdx) {

						symMatches.push_back(cv::DMatch((*matchIterator1)[0].queryIdx,
									  				    (*matchIterator1)[0].trainIdx,
													    (*matchIterator1)[0].distance));
						break; 
				}
			}
		}
	  }

	 //基于RANSAC匹配
	  cv::Mat ransacTest(const std::vector<cv::DMatch>& matches,
		                 const std::vector<cv::KeyPoint>& keypoints1, 
						 const std::vector<cv::KeyPoint>& keypoints2,
					     std::vector<cv::DMatch>& outMatches) {

		//转换Keypoints到points2f
		std::vector<cv::Point2f> points1, points2;	
		for (std::vector<cv::DMatch>::const_iterator it= matches.begin();
			 it!= matches.end(); ++it) {

			 //左边特征点的坐标
			 float x= keypoints1[it->queryIdx].pt.x;
			 float y= keypoints1[it->queryIdx].pt.y;
			 points1.push_back(cv::Point2f(x,y));
			 //右边特征点的值
			 x= keypoints2[it->trainIdx].pt.x;
			 y= keypoints2[it->trainIdx].pt.y;
			 points2.push_back(cv::Point2f(x,y));
	    }

		std::vector<uchar> inliers(points1.size(),0);
		cv::Mat fundemental= cv::findFundamentalMat(
			cv::Mat(points1),cv::Mat(points2), // 匹配点
		    inliers,      //匹配状态(inlier ou outlier)  
		    CV_FM_RANSAC, 
		    distance,     
		    confidence); 
	
		std::vector<uchar>::const_iterator itIn= inliers.begin();
		std::vector<cv::DMatch>::const_iterator itM= matches.begin();

		for ( ;itIn!= inliers.end(); ++itIn, ++itM) {

			if (*itIn) { 

				outMatches.push_back(*itM);
			}
		}

		std::cout << "Number of matched points (after cleaning): " << outMatches.size() << std::endl;

		if (refineF) {
			
			points1.clear();
			points2.clear();
	
			for (std::vector<cv::DMatch>::const_iterator it= outMatches.begin();
				 it!= outMatches.end(); ++it) {

				
				 float x= keypoints1[it->queryIdx].pt.x;
				 float y= keypoints1[it->queryIdx].pt.y;
				 points1.push_back(cv::Point2f(x,y));
				 
				 x= keypoints2[it->trainIdx].pt.x;
				 y= keypoints2[it->trainIdx].pt.y;
				 points2.push_back(cv::Point2f(x,y));
			}

			
			fundemental= cv::findFundamentalMat(
				cv::Mat(points1),cv::Mat(points2),
				CV_FM_8POINT);
		}

		return fundemental;
	  }

	  //对称在两张图上进行RANSAC匹配特征点
	  //返回基础矩阵
	  cv::Mat match() {

		// 测试SURF特征
		detector->detect(image1,keypoints1);
		detector->detect(image2,keypoints2);

		std::cout << "Number of SURF points (1): " << keypoints1.size() << std::endl;
		std::cout << "Number of SURF points (2): " << keypoints2.size() << std::endl;

		//取SURF描述子
		cv::Mat descriptors1, descriptors2;
		extractor->compute(image1,keypoints1,descriptors1);
		extractor->compute(image2,keypoints2,descriptors2);

		std::cout << "descriptor matrix size: " << descriptors1.rows << " by " << descriptors1.cols << std::endl;

		//创建匹配器 
		cv::BruteForceMatcher<cv::L2<float>> matcher;

		// 图一到图2的最近邻（k = 2）
		std::vector<std::vector<cv::DMatch>> matches1;
		matcher.knnMatch(descriptors1,descriptors2, 
			matches1, // 匹配结果的向量（每项有两个值）
			2);		  
	
		//图2到图一
		std::vector<std::vector<cv::DMatch>> matches2;
		matcher.knnMatch(descriptors2,descriptors1, 
			matches2, 
			2);		 

		std::cout << "Number of matched points 1->2: " << matches1.size() << std::endl;
		std::cout << "Number of matched points 2->1: " << matches2.size() << std::endl;

		//移除大于阈值的匹配
		//清理图一到图2
		int removed= ratioTest(matches1);
		std::cout << "Number of matched points 1->2 (ratio test) : " << matches1.size()-removed << std::endl;
		//清理图二到图一
		removed= ratioTest(matches2);
		std::cout << "Number of matched points 1->2 (ratio test) : " << matches2.size()-removed << std::endl;

		// 移除非对称匹配
	    std::vector<cv::DMatch> symMatches;
		symmetryTest(matches1,matches2,symMatches);

		std::cout << "Number of matched points (symmetry test): " << symMatches.size() << std::endl;

		cv::Mat fundemental= ransacTest(symMatches, keypoints1, keypoints2, matches);

		return fundemental;
	}
};

#endif
