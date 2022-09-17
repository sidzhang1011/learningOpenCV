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
string exerWindow = "exerWindow";

void sum_rgb(const cv::Mat& src, cv::Mat& dst);
void threshold();
void smoothing();
void morphology();
void exer();

int main(int argc, const char * argv[]) {
    threshold();
    smoothing();
    morphology();
    exer();
    // Idle until the user hits any key.
    //
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
//    cv::accumulate(b, s);
//    cv::Mat b1 = cv::Mat::ones(3, 3, CV_32F);
//    cv::Mat g1 = cv::Mat::ones(3, 3, CV_32F);
//    cv::Mat r1 = cv::Mat::eye(3, 3, CV_32F);
//    cv::Mat s1 = cv::Mat::zeros(3,3, CV_32F);
//    cv::accumulate(b1, s1);
//    cout << s1 << endl;
//    cv::accumulate(g1, s1);
//    cout << s1 << endl;
//    cv::accumulate(r1, s1);
//    cout << s1 << endl;

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
//    cv::imshow(fileName, dst );
//    cv::waitKey(0);
}

void  smoothing() {
    // boxfilter
    // medianblur
    // Gaussianblur
    // bilateralFilter
    //
    
    // Derivatives and gradients
    // sobel use scharr for more accurate results
    // Laplacian
    
}

void morphology() {
    // erode
    // dilate
    // morphologyEx: open, close, etc.
    // getStructuringElement
    // filter2D
    // sepFilter2D
    // getDerivKernels
    // getGaussianKernels
    
    cv::Mat kx, ky;
    cv::getDerivKernels(kx, ky, 0, 1, cv::FILTER_SCHARR);
    cout<< kx << endl << ky << endl;
}

void exer1() {
    string fileName = "/Users/zsg/Desktop/pengzuo.jpeg";
    cv::Mat image = cv::imread(fileName);
    cv::Mat smoothed1;
    cv::GaussianBlur(image, smoothed1, cv::Size(11,11), 2.15, 2.15);
//    cv::blur(image, smoothed1, cv::Size(11,11));
    
    cv::Mat smoothed2;
    cv::GaussianBlur(image, smoothed2, cv::Size(5,5), 1.25, 1.25);
    cv::GaussianBlur(smoothed2, smoothed2, cv::Size(5,5), 1.25, 1.25);
    
    cv::Mat smoothed = cv::Mat(image.rows, 2*image.cols, image.type());
    smoothed1.copyTo(smoothed.colRange(0, image.cols));
    smoothed2.copyTo(smoothed.colRange(image.cols, 2*image.cols));
    
    cv::imshow(exerWindow, smoothed);
}

void exer2() {
    cv::Mat image = cv::Mat::zeros(100, 100, CV_8UC1);
    image.at<uint8_t>(50,50) = 255;
    image.at<uint8_t>(49,49) = 255;
    image.at<uint8_t>(49,50) = 255;
    image.at<uint8_t>(50,49) = 255;
    cv::Mat blurred1;
    cv::GaussianBlur(image, blurred1, cv::Size(5,5), 1.25);
    cv::GaussianBlur(blurred1, blurred1, cv::Size(5,5), 1.25);
    cv::Mat blurred2;
    cv::GaussianBlur(image, blurred2, cv::Size(9,9), 1.85);
    
    cv::Mat blurred = cv::Mat(image.rows, 2*image.cols, image.type());
    blurred1.copyTo(blurred.colRange(0, image.cols));
    blurred2.copyTo(blurred.colRange(image.cols, 2*image.cols));
    cv::imshow(exerWindow, blurred);
}

void exer4() {
    string fileName1 = "/Users/zsg/Desktop/tree1.jpeg";
    cv::Mat img1 = cv::imread(fileName1);
    string fileName2 = "/Users/zsg/Desktop/tree2.jpeg";
    cv::Mat img2 = cv::imread(fileName2);
    
    cv::Mat diff12;
    cv::subtract(img1, img2, diff12);
    diff12 = cv::abs(diff12);
    
    cv::Mat cleandiff;
    cv::erode(diff12, cleandiff, cv::Mat());
    cv::dilate(cleandiff, cleandiff, cv::Mat());
    
    cv::Mat dirtydiff;
    cv::dilate(diff12, dirtydiff, cv::Mat());
    cv::erode(dirtydiff, dirtydiff, cv::Mat());

    cv::Mat both = cv::Mat(diff12.rows, 2*diff12.cols, diff12.type());
    cleandiff.copyTo(both.colRange(0, diff12.cols));
    dirtydiff.copyTo(both.colRange(diff12.cols, 2*diff12.cols));

    imshow(exerWindow, both);

}

void exer5() {
    string fileName1 = "/Users/zsg/Desktop/cup1.jpeg";
    cv::Mat img1 = cv::imread(fileName1);
    string fileName2 = "/Users/zsg/Desktop/cup2.jpeg";
    cv::Mat img2 = cv::imread(fileName2);

    cv::Mat diffs;
    cv::subtract(img1, img2, diffs);
    diffs = cv::abs(diffs);
    vector<cv::Mat> planes;
    cv::split(diffs, planes);
    
    cv::Mat diff12, t;
    cv::addWeighted(planes[0], 1./3., planes[1], 1./3., 0, t);
    cv::addWeighted(planes[2], 1./3., t, 1.0, 0, diff12);
    
    cv::Mat threshold12;
    cv::threshold(diff12, threshold12, 50, 250, cv::THRESH_BINARY);
    
    cv::morphologyEx(threshold12, threshold12, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3)));
    
    cv::Mat eroded;
    cv::erode(threshold12, eroded, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3)));
    cv::subtract(threshold12, eroded, threshold12);
    threshold12 = cv::abs(threshold12);

    

    imshow(exerWindow, threshold12);

}

void exer() {
//    exer1();
//    exer2();
//    exer4();
    exer5();
    
    
    cv::waitKey(0);
    cv::destroyWindow(exerWindow);
}
