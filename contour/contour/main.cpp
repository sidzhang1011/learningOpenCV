//
//  main.cpp
//  contour
//
//  Created by Sid Zhang on 2022/12/4.
//

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

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

int main(int argc, const char * argv[]) {
//    example1();
//    example2();
    example3();
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
