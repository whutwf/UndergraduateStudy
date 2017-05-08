#if !defined COLHISTOGRAM
#define COLHISTOGRAM

#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
using namespace cv;

class ColorHistogram {

  private:

    int histSize[3];
	float hranges[3][2];
    const float* ranges[3];
    int channels[3];

  public:

	ColorHistogram() {

		// Prepare arguments for a color histogram
		histSize[0]=  8;
		histSize[1]=  12;
		histSize[2]= 3;

		hranges[0][0]= 0.0;    // BRG range
		hranges[0][1]= 180.0;
		ranges[0]= hranges[0]; // all channels have the same ra

		hranges[1][0]= 0.0;    // BRG range
		hranges[1][1] = 256.0;
		ranges[1]= hranges[1];

			hranges[2][0]= 0.0;    // BRG range
		hranges[2][1] = 256.0;
		ranges[2]= hranges[2];

		channels[0]= 0;		// the three channels
		channels[1]= 1;
		channels[2]= 2;
	}

	// Computes the histogram.
	cv::MatND getHistogram(const cv::Mat &image, cv::Mat mask = cv::Mat()) {

		cv::MatND hist;
        GaussianBlur(image, image, Size(5,5), 0,0 );
		cv::cvtColor(image, image, CV_BGR2HSV);
		// Compute histogram
		cv::calcHist(&image,
			1,			// histogram of  1 image only
			channels,	// the channel used
			mask,	// no mask is used
			hist,		// the resulting histogram
			3,			// it is a 3D histogram
			histSize,	// number of bins
			ranges		// pixel value range
		);

		return hist;
	}

	void descripe(const cv::Mat &image, vector<double> &features){
    ColorHistogram h;
    cv::MatND d;
    int w_center = int(image.cols * 0.5);
    int h_center = int(image.rows * 0.5);

     //定义画椭圆形中心位置
    cv::Point center = cv::Point(w_center, h_center);
    //定义椭圆的长短轴
    cv::Size ellipseSize = cv::Size(int(image.cols * 0.75) / 2, int(image.rows * 0.75) / 2 );
    //定义画矩形的四角起始终止位置
    int x_start[4] = {0, w_center, 0, w_center};
    int y_start[4] = {0, 0, h_center, h_center};
    int x_end[4] = {w_center, image.cols, w_center, image.cols};
    int y_end[4] = {h_center, h_center, image.rows, image.rows};

    cv::Mat ellipseMask = cv::Mat::zeros(image.rows, image.cols,CV_8UC1);
    ellipse(ellipseMask, center, ellipseSize, 0, 0, 360, Scalar(255,255,255),  -1, CV_AA,0);

   //imshow("", ellipseMask);
    for(int i = 0; i < 4; ++i) {
        cv::Mat cornerMask = cv::Mat::zeros(image.rows, image.cols,CV_8UC1);
        rectangle(cornerMask, Point(x_start[i], y_start[i]), Point(x_end[i], y_end[i]), Scalar(255,255,255), -1, CV_AA, 0);
        subtract(cornerMask, ellipseMask, cornerMask);
        d = h.getHistogram(image, cornerMask);
        cv::normalize(d,d, 1 );
        getFeatures(features, d);
      //  imshow(i+"", cornerMask);
    }
    d = h.getHistogram(image, ellipseMask);
    cv::normalize(d, d, 1 );
     getFeatures(features, d);
}

void getFeatures(vector<double>&features, const cv::MatND &h){
    for(int i = 0; i < 8;++i)
    {
        for(int j = 0; j < 12;++j)
        {
            for(int k = 0; k < 3;++k)
            {
                features.push_back(h.at<float>(i,j,k));
            }
        }
    }
}

    double getDistance(vector<double> w, vector<double> h, double eps = 1e-10){
      double  semblance = 0;
      vector<double>::iterator it;
      vector<double>::iterator item;
      for(it = w.begin(), item = h.begin(); it != w.end(), item != h.end() ; ++it, ++item){
         double pow = ((*it) - (*item))*((*it) - (*item));
         double div = pow / ((*it) + (*item) + eps);
          semblance += div;
      }
       semblance = 0.5 * semblance;
      return  semblance;
  }

};


#endif
