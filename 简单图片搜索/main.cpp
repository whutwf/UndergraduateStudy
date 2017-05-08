/*
特征s------深度------聚类
*/
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;
#include <opencv2/opencv.hpp>
#include "color.h"
#include "sort.h"
#include "showManyPicture.h"
using namespace cv;

int main(){
    double array[806][2];

    ofstream out("a.txt");
    ofstream out_1("ab.csv");

    ColorHistogram z;
        vector<double> w;

    cv::Mat image = cv::imread("queries/6.png");
        z.descripe(image, w);

        for(int i = 0; i  < 806; ++i){
                 vector<double> h;
                string name = "../dataset/";
                name = concatenate(name ,i);
                name += ".png";
                cv::Mat  img = cv::imread(name);
                 z.descripe(img, h);
                 double df = z.getDistance(w, h);
                 array[i][0] = i;
                 array[i][1] = df;

               vector<double>::iterator it;
                for(it = h.begin(); it != h.end(); ++it){
                    out_1 <<*it << ",";
                }
                out_1 << "\n";
                out << i << "\t" << df << "\n";

                 cout <<i << " \t"<<  df <<endl;

        }
        out.close();
        out_1.close();
 //vector<double>::iterator it;
//int     i =0;
   /* for(it = w.begin(); it != w.end(); ++it){
        cout << i++ << "\t"<< *it << endl;
        if(i > 300) break;
        }
        cout << w.size();*/
        BubbleSort(array, 806);
        for(int i = 0; i < 806; ++i){
            for(int j = 0; j < 2; ++j){
                cout << array[i][j] << "\t";
            }
            cout << endl;
        }
    vector<Mat>  imgs;
 for(int i = 0; i < 12; ++i){
         string name = "../dataset/";
        name = concatenate(name ,array[i][0]);
                name += ".png";
                cv::Mat  img = cv::imread(name);
             //    cv::imwrite("../mysql/name", image);
                imgs.push_back(img);
 }
 imshowMany("wangfe", imgs);
    cv::waitKey(0);
}
