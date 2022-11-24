//
//  main.cpp
//  histogram
//
//  Created by Sid Zhang on 2022/11/23.
//

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/photo.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace std;

void testcalc();
void example_1301();

int main(int argc, const char * argv[]) {
//    testcalc();
    example_1301();
    return 0;
}

void testcalc() {
//    cv::Mat src = cv::Mat::ones(4, 4, CV_8UC3);
//    cv::imshow("test1", src);
//    cout << "src:" << src << endl << endl;
//
//    cv::Mat hist;
//    int histSize[] = {4, 4};
//    float ranges1[] = {0,2};
//    float ranges2[] = {0,2};
    cv::Mat src = cv::imread("/Users/zsg/Desktop/刘涛.jpeg");
    cv::imshow("test1", src);
    
    cv::Mat hist;
    int histSize[] = {8, 8};
    float ranges1[] = {0,256};
    float ranges2[] = {0,256};
    const float *ranges[] = {ranges1, ranges2};
    int ch[] = {0, 1};
    cv::calcHist(&src, 1, ch, cv::noArray(), hist, 2, histSize, ranges);
    cout << "hist:" << hist.size() << endl <<hist << endl;
    
    cv::waitKey();

}

void example_1301() {
    cv::Mat src = cv::imread("/Users/zsg/Desktop/刘涛.jpeg");
    // Compute the HSV image, and decompose it into separate planes.
    //
    cv::Mat hsv;
    cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);
    float h_ranges[]      = {0, 180}; // hue is [0, 180]
    float s_ranges[]      = {0, 256};
    const float* ranges[] = {h_ranges, s_ranges};
    int histSize[]        = {60, 32}, ch[] = {0, 1};
    cv::Mat hist;
    // Compute the histogram
    //
    cv::calcHist(&hsv, 1, ch, cv::noArray(), hist, 2, histSize, ranges, true);
    cv::normalize(hist, hist, 0, 255, cv::NORM_MINMAX);
    int scale = 10;
    cv::Mat hist_img(histSize[0]*scale, histSize[1]*scale, CV_8UC3);
    // Draw our histogram.
    //
    cv::rectangle(
                  hist_img,
                  cv::Rect(0, 0, histSize[1]*scale, histSize[0]*scale),
                  cv::Scalar(0, 0, 255),
                  -1
                  );
    for( int h = 0; h < histSize[0]; h++ ) {
        for( int s = 0; s < histSize[1]; s++ ){
            float hval = hist.at<float>(h, s);
            cv::rectangle(
                          hist_img,
                          cv::Rect(s*scale,h*scale,scale,scale),
                          cv::Scalar::all(hval),
                          -1
                          );
        }
    }
    cv::imshow("image", src);
    cv::imshow("histogram", hist_img);
    cv::waitKey();
}
