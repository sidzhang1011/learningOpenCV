//
//  main.cpp
//  FiltersAndConvolution
//
//  Created by Sid Zhang on 2022/9/13.
//

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
using namespace std;

void sum_rgb(const cv::Mat& src, cv::Mat& dst);
void threshold();

int main(int argc, const char * argv[]) {
    threshold();
    // Idle until the user hits any key.
    //
    cv::waitKey(0);
    return 0;
}

void sum_rgb( const cv::Mat& src, cv::Mat& dst ) {
    // Split image onto the color planes.
    //
    vector< cv::Mat> planes;
    cv::split(src, planes);
    cv::Mat b = planes[0], g = planes[1], r = planes[2], s;
    // Add equally weighted rgb values.
    //
    cv::addWeighted( r, 1./3., g, 1./3., 0.0, s );
    cv::addWeighted( s, 1., b, 1./3., 0.0, s );
    
    // Truncate values above 150.
    //
    cv::threshold( s, dst, 150, 150, cv::THRESH_TRUNC );
}

void threshold() {
    // Load the image from the given file name.
    //
    string fileName = "/Users/zsg/Desktop/平时学习.jpeg";
    cv::Mat src = cv::imread(fileName), dst;
    if( src.empty() ) {
        cout << "can not load " << fileName << endl;
    }
    sum_rgb( src, dst);
    // Create a named window with the name of the file and
    // show the image in the window
    //
    cv::imshow(fileName, dst );
}
