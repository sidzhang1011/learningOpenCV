//
//  main.cpp
//  GeometryTransform
//
//  Created by Sid Zhang on 2022/9/23.
//

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/photo.hpp>
using namespace std;

void affine();
void perspective();
void logPolar();
void inPaint();
void exercise1();
void exercise2();

int main(int argc, const char * argv[]) {
//    affine();
//    perspective();
//    logPolar();
//    inPaint();
//    exercise1();
    exercise2();
    return 0;
}

void exercise2() {
    cv::Mat src1 = cv::Mat::zeros(400, 600, CV_8UC1);
    cv::Point center(300, 200);
    int width = 200, height = 200;
    for (int i = -height/2; i<height/2; ++i ) {
        for (int j = -width/2; j< width/2; ++j) {
            src1.at<uint8_t>(center.y + j, center.x + i) = 255;
        }
    }
    cv::Mat dst1;
    cv::Point c1(200, 100);
    cv::logPolar(src1, dst1, c1, 20, 0);
    cv::imshow("test1", dst1);
    
    cv::Mat src2 = cv::Mat::zeros(400, 600, CV_8UC1);
    cv::circle(src2, center, 100, cv::Scalar(255,255,255), cv::FILLED);
    cv::Mat dst2;
    cv::Point c2(300, 110);
    cv::logPolar(src2, dst2, c2, 20, 0);
    cv::imshow("test2", dst2);
    cv::moveWindow("test2", 600, 0);
//    cv::resizeWindow("test2", 600, 800);

    cv::Mat dst3;
    cv::Point c3(400, 400);
    cv::logPolar(src2, dst3, c3, 20, 0);
    cv::imshow("test3", dst3);
    cv::moveWindow("test3", 0, 400);

    
    cv::waitKey();
}

void exercise1() {
    cv::Mat src = cv::imread("/Users/zsg/Desktop/eye.jpeg", cv::IMREAD_GRAYSCALE);
    cv::threshold(src, src, 60, 255, cv::THRESH_BINARY_INV);
    cv::Mat dst;
    cv::Laplacian(src, dst, CV_8UC1);
    dst = 255 - dst;
    cv::imshow("test", dst);
    cv::waitKey(0);

}

void inPaint() {
    cv::Mat src = cv::imread("/Users/zsg/Desktop/tree-ruined.jpg", cv::IMREAD_COLOR);
    cout << src.size() << endl;
    cv::Mat inpaintMask = cv::Mat::zeros(src.size(), CV_8UC1);
    cv::cvtColor(src, inpaintMask, cv::COLOR_BGR2GRAY);
    cv::threshold(inpaintMask, inpaintMask, 0, 255, cv::THRESH_BINARY_INV);
    for(int i = 955; i< 988; i++) {
        for (int j=830; j< 907; j++) {
            inpaintMask.at<uint8_t>(i, j) = 255;
//            src.at<cv::Vec<uint8_t, 3>>(i, j) = {255, 255, 255};
        }
    }
    cv::Mat dst;
    cv::inpaint(src, inpaintMask, dst, 5, cv::INPAINT_TELEA);
    
    cv::imshow("test", dst);
    cv::waitKey(0);
    

}

void logPolar() {
    cv::Mat src = cv::imread("/Users/zsg/Desktop/pengzuo.jpeg", cv::IMREAD_GRAYSCALE);
//    cv::cvtColor(temp, src, cv::COLOR_BGR2GRAY);
    double M = 100;
    cv::Mat dst(src.size(), src.type()), src2(src.size(), src.type());
    cv::logPolar(
                 src,
                 dst,
                 cv::Point2f(src.cols*0.5f, src.rows*0.5f),
                 M,
                 cv::INTER_LINEAR | cv::WARP_FILL_OUTLIERS
                 );
//    cout << src << endl;
//    cout << dst << endl;
    cv::logPolar(
                 dst,
                 src2,
                 cv::Point2f(src.cols*0.5f, src.rows*0.5f),
                 M,
                 cv::INTER_LINEAR | cv::WARP_INVERSE_MAP
                 );
    cv::imshow( "log-polar", dst );
    cv::imshow( "inverse log-polar", src2 );
    cv::waitKey();

}

void affine(){
    cv::Mat src = cv::imread("/Users/zsg/Desktop/tree2.jpeg", 1);
    cv::Point2f srcTri[] = {
        cv::Point2f(0, 0),          // src Top left
        cv::Point2f(src.cols-1, 0), // src Top right
        cv::Point2f(0, src.rows-1)  // src Bottom left
    };
    cv::Point2f dstTri[] = {
        cv::Point2f(src.cols*0.f, src.rows*0.33f),      // dst Top left
        cv::Point2f(src.cols*0.85f, src.rows*0.25f),    // dst Top right
        cv::Point2f(src.cols*0.15f, src.rows*0.7f)      // dst Bottom left
    };
    
    // COMPUTE AFFINE MATRIX
    //
    cv::Mat warp_mat = cv::getAffineTransform(srcTri, dstTri);
    cv::Mat dst, dst2;
    cv::warpAffine(
                   src,
                   dst,
                   warp_mat,
                   src.size(),
                   cv::INTER_LINEAR,
                   cv::BORDER_CONSTANT,
                   cv::Scalar()
                   );
    for( int i = 0; i < 3; ++i ) {
        cv::circle(dst, dstTri[i], 5, cv::Scalar(255, 0, 255), -1, cv::LINE_AA);
    }
    string win1 = "Affine Transform Test";
    string win2 = "Rotated Image";
    cv::imshow(win1, dst);
    cv::waitKey();
    for(int frame=0;;++frame) {
        // COMPUTE ROTATION MATRIX
        cv::Point2f center(src.cols*0.5f, src.rows*0.5f);
        double angle = frame*3 % 360, scale = (cos((angle - 60)* CV_PI/180) + 1.05)*0.8;
        cv::Mat rot_mat = cv::getRotationMatrix2D(center, angle, scale);
        cv::warpAffine(
                       src,
                       dst,
                       rot_mat,
                       src.size(),
                       cv::INTER_LINEAR,
                       cv::BORDER_CONSTANT,
                       cv::Scalar()
                       );
        cv::imshow(win2, dst);
        if(cv::waitKey(30) >= 0 )
            break;
    }
    
    cv::destroyWindow(win1);
    cv::destroyWindow(win2);
}

void perspective(){
    string fileName = "/Users/zsg/Desktop/tree1.jpeg";
    cv::Mat src = cv::imread(fileName, 1);
      
    cv::Point2f srcQuad[] = {
        cv::Point2f(0, 0),              // src Top left
        cv::Point2f(src.cols-1, 0),     // src Top right
        cv::Point2f(src.cols-1, src.rows-1),    // src Bottom right
        cv::Point2f(0, src.rows-1)      // src Bottom left
    };
    
    cv::Point2f dstQuad[] = {
        cv::Point2f(src.cols*0.05f, src.rows*0.33f),
        cv::Point2f(src.cols*0.9f,  src.rows*0.25f),
        cv::Point2f(src.cols*0.8f,  src.rows*0.9f),
        cv::Point2f(src.cols*0.2f,  src.rows*0.7f)
    };
    // COMPUTE PERSPECTIVE MATRIX
    //
    cv::Mat warp_mat = cv::getPerspectiveTransform(srcQuad, dstQuad);
    cv::Mat dst;
    cv::warpPerspective(src, dst, warp_mat, src.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar());
    for( int i = 0; i < 4; i++ )
        cv::circle(dst, dstQuad[i], 5, cv::Scalar(255, 0, 255), -1, cv::LINE_AA);
    string windowName = "Perspective Transform Test";
    cv::imshow(windowName, dst);
    cv::waitKey();
    cv::destroyWindow(windowName);
}
