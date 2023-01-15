//
//  main.cpp
//  contour
//
//  Created by Sid Zhang on 2022/12/4.
//

#include <iostream>
#include <algorithm>
#include <random>
#include <functional>
#include <memory>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

using namespace std;

cv::Mat src_gray;
int thresh = 100;
cv::RNG rng(12345);

struct AreaCmp {
    AreaCmp(const vector<float>& _areas) : areas(&_areas) {}
    bool operator()(int a, int b) const { return (*areas)[a] > (*areas)[b]; }
    const vector<float>* areas;
};


void example1();
void thresh_callback1(int, void* );
void example2();
void example3();
void exercise04();
void exercise05();
void exercise08();

int main(int argc, const char * argv[]) {
//    example1();
//    example2();
//    example3();
//    exercise04();
//    exercise05();
    exercise08();
    return 0;
}

void example1() {
    cv::Mat src = cv::imread("/Users/zsg/Desktop/contour1.jpg");
    cv::cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);
    cv::blur(src_gray, src_gray, cv::Size(3, 3));
    
    const char* source_window = "Source";
    cv::imshow(source_window, src);
    const int max_thresh = 255;
    cv::createTrackbar("Canny thresh", source_window, &thresh, max_thresh, thresh_callback1);
    thresh_callback1(0, 0);
    
    cv::waitKey();
}

void thresh_callback1(int, void* )
{
    cv::Mat canny_output;
    cv::Canny( src_gray, canny_output, thresh, thresh*2 );
    vector<vector<cv::Point> > contours;
    vector<cv::Vec4i> hierarchy;
    cv::findContours( canny_output, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE );
    cv::Mat drawing = cv::Mat::zeros( canny_output.size(), CV_8UC3 );
    for( size_t i = 0; i< contours.size(); i++ )
    {
        cv::Scalar color = cv::Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
        cv::drawContours( drawing, contours, (int)i, color, 2, cv::LINE_8, hierarchy, 0 );
    }
    cv::imshow( "Contours", drawing );
}

void example2() {
    cv::Mat img, img_edge, img_color;
    
    // load image or show help if no image was provided
    //
    img = cv::imread("/Users/zsg/Desktop/contour1.jpg", cv::IMREAD_GRAYSCALE);
    cv::threshold(img, img_edge, 100, 255, cv::THRESH_BINARY);
    cv::imshow("Image after threshold", img_edge);
    vector< vector< cv::Point > > contours;
    vector< cv::Vec4i > hierarchy;
    
    cout << img_edge.depth() << endl;
    cout << img_edge.channels() << endl;
    cv::findContours(img_edge, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
    cout << "\n\nHit any key to draw the next contour, ESC to quit\n\n";
    cout << "Total Contours Detected: " << contours.size() << endl;

    vector<int> sortIdx(contours.size());
    vector<float> areas(contours.size());
    for( int n = 0; n < (int)contours.size(); n++ ) {
        sortIdx[n] = n;
        areas[n] = contourArea(contours[n], false);
    }
    // sort contours so that the largest contours go first
    //
    std::sort( sortIdx.begin(), sortIdx.end(), AreaCmp(areas ));
    
     for( int n = 0; n < (int)sortIdx.size(); n++ ) {
         int idx = sortIdx[n];
         cv::cvtColor( img, img_color, cv::COLOR_GRAY2BGR );
         cv::drawContours(
                          img_color, contours, idx,
                          cv::Scalar(0,0,255), 2, 8, hierarchy,
                          0                 // Try different values of max_level, and see what happens
                          );
         cout << "Contour #" << idx << ": area=" << areas[idx] << ", nvertices=" << contours[idx].size() << endl;
         cv::imshow("contours", img_color);
         int k;
         if( (k = cv::waitKey()&255) == 27 )
             break;
    
    }
      cout << "Finished all contours\n";
}

void example3() {
    cv::Mat img, img_edge, labels, img_color, stats, centroids, img_gray;
    img = cv::imread("/Users/zsg/Desktop/cc2.jpg", cv::IMREAD_GRAYSCALE);
    cv::threshold(img, img_edge, 128, 255, cv::THRESH_BINARY);
    cv::imshow("Image after threshold", img_edge);
// /*
    int i, nccomps = cv::connectedComponentsWithStats(img_edge, labels, stats, centroids);
    cout << "Total connected Components Detected: " << nccomps << endl;
    
    vector< cv::Vec3b> colors(nccomps + 1);
    colors[0] = cv::Vec3b(0,0,0); // background pixels remain black.
    for( i = 1; i <= nccomps; i++ ) {
        if( stats.at<int>(i, cv::CC_STAT_AREA) < 200 )
            colors[i] = cv::Vec3b(0,0,0); // small regions are painted with black too.
        else
            colors[i] = cv::Vec3b(rand()%256, rand()%256, rand()%256);
    }
    img_color = cv::Mat::zeros(img.size(), CV_8UC3);
    for( int y = 0; y < img_color.rows; y++ )
        for( int x = 0; x < img_color.cols; x++ )
        {
            int label = labels.at<int>(y, x);
            CV_Assert(0 <= label && label <= nccomps);
            img_color.at<cv::Vec3b>(y, x) = colors[label];
        }
    
    cv::cvtColor(img_color, img_gray, cv::COLOR_BGR2GRAY);
    cv::threshold(img_gray, img_gray, 1, 255, cv::THRESH_BINARY);
    nccomps = cv::connectedComponentsWithStats(img_gray, labels, stats, centroids);
    
    cv::imshow("Labeled map", img_color);
  //*/
    cv::waitKey();
}

static vector<cv::Point> sampleContour( const cv::Mat& image, int n=300) {
    vector<vector<cv::Point>> _contours;
    vector<cv::Point> all_points;
    cv::findContours(image, _contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);
    for (size_t i=0; i < _contours.size(); i++) {
        for (size_t j=0; j < _contours[i].size(); j++) {
            all_points.push_back( _contours[i][j]);
        }
    }
    
    // If too little points, replicate them
    //
    int dummy = 0;
    for (int add=(int)all_points.size(); add<n; add++) {
        all_points.push_back(all_points[dummy++]);
    }
    
    // Sample uniformly
//    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
//    shuffle(all_points.begin(), all_points.end(), default_random_engine(seed));
    random_device rd;
    mt19937 g(rd());
    shuffle(all_points.begin(), all_points.end(), g);
    
    vector<cv::Point> sampled;
    for (int i=0; i<n; i++) {
        sampled.push_back(all_points[i]);
    }
    return sampled;
}

void example04() {
    string path = "../data/shape_sample/";
    int indexQuery = 1;
//    shared_ptr<cv::ShapeContextDistanceExtractor> mysc = cv::createShape
    
}

double crossProduct(cv::Point2d p1, cv::Point2d p2, cv::Point2d p3) {
    return (p3.x- p1.x)*(p2.y - p1.y) - (p3.y - p1.y) * (p2.x - p1.x);
}

bool isInside(cv::Point2d p1, cv::Point2d p2, cv::Point2d p3, cv::Point2d p) {
    // if p1, p2, p3 is not counterclockwise, exchange p2&p3
    if (crossProduct(p1, p2, p3) < 0) {
        cv::Point2d tempP(p2);
        p2 = p3;
        p3 = tempP;
    }
    if(crossProduct(p1, p, p2) >= 0) return false;
    if(crossProduct(p2, p, p3) >= 0) return false;
    if(crossProduct(p3, p, p1) >= 0) return false;

    return true;
}

void exercise04() {
    cv::Point2d p1(2,1), p2(1, 1), p3(1, 2), p(1.2, 1.5);
    if (isInside(p1, p2, p3, p)) {
        cout << "inside\n";
    } else {
        cout << "outSide\n";
    }
}

void exercise05() {
    cv::Mat src = cv::Mat::zeros(80, 80, CV_8UC3);
    cv::ellipse(src, cv::Point(40,40), cv::Size(20,20), 0, 0, 360, cv::Scalar(255,255,255), 1);
    cv::Mat img;
    cv::cvtColor(src, img, cv::COLOR_BGR2GRAY);
    cout << src.channels() <<endl;
    cout << img.channels() << endl;
    cout << img.type() <<endl;
//    cout << img <<endl;
    cv::threshold(img, img, 100, 255, cv::THRESH_BINARY);
    vector<vector<cv::Point>> _contours;
    cv::findContours(img, _contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);
    double contourLen;
    for (int i = 0; i <_contours.size(); ++i) {
          double area = cv::contourArea(_contours[i]);
          double length = cv::arcLength(_contours[i], true);
          cout <<"area: " <<area <<" " <<"length: " <<length <<endl;
          contourLen = length;
      }
    cout << contourLen << endl; // 131.882 > 125.7
    
    double epsilonRates[] = {0.9, 0.66, 0.33, 0.1, 0.05, 0.01};
    int count = sizeof(epsilonRates)/sizeof(double);
    vector<vector<cv::Point>> contourPloy(count);
    for(int i = 0; i< count; i++) {
        cv::Mat dstImg(img.size(), CV_8UC3, cv::Scalar::all(0));
        cv::approxPolyDP(_contours[1], contourPloy[i], epsilonRates[i] * 125.7, true);
        cv::drawContours(dstImg, contourPloy, i, cv::Scalar(0, 255, 255));
        string winName = "poly";
        winName.append(to_string(i));
        cv::imshow(winName, dstImg);
    }
    
    cv::waitKey();
    
    
}

void exercise08() {
    cv::Mat img = cv::imread("/Users/zsg/Documents/images/letterA_tilted.tif", cv::IMREAD_GRAYSCALE);
    double hu_original[7];
    double hu[7];
    cv::Moments mo = cv::moments(img);
    cv::HuMoments(mo, hu_original);
    for(int i=0; i<7; i++) {
        cout << hu_original[i] << ",";
    }
    cout << endl;
    
    cout << endl << "deviation rate:" << endl<<endl;

    // rotate 90
    cv::Mat img_rot90;
    cv::rotate(img, img_rot90, cv::ROTATE_90_CLOCKWISE);
    mo = cv::moments(img_rot90);
    cv::HuMoments(mo, hu);
    for(int i=0; i<7; i++) {
        printf("%.4f, ", (hu[i] - hu_original[i])/hu_original[i]);
    }
    cout << endl;
    
    // scale 2 times
    cv::Mat img_scale2;
    cv::pyrUp(img, img_scale2);
    mo = cv::moments(img_scale2);
    cv::HuMoments(mo, hu);
    for(int i=0; i<7; i++) {
//        cout << (hu[i] - hu_original[i])/hu_original[i] << ",";
        printf("%.4f, ", (hu[i] - hu_original[i])/hu_original[i]);
    }
    cout << endl;

    // rotate 90 and scale 2 times
    cv::Mat img_rs;
    cv::pyrUp(img_rot90, img_rs);
    mo = cv::moments(img_rs);
    cv::HuMoments(mo, hu);
    for(int i=0; i<7; i++) {
        printf("%.4f, ", (hu[i] - hu_original[i])/hu_original[i]);
    }
    cout << endl;
}
