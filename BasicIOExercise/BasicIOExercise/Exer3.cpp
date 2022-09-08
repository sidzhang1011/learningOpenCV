//
//  Exer3.cpp
//  BasicIOExercise
//
//  Created by Sid Zhang on 2022/9/8.
//

#include "Exer3.hpp"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

Exer3::Exer3():
windowName("rectangle"){
    originalImage = cv::imread("/Users/zsg/Downloads/large.jpeg");
//    cv::cvtColor(image, originalImage, cv::COLOR_BGR2BGRA);

}

Exer3::~Exer3() {
    
}

void Exer3::myMouseCallback(int event, int x, int y, int flags, void* userdata) {
    static int firstX;
    static int firstY;
    static bool lbPressed = false;
    cv::MouseEventTypes eventType = (cv::MouseEventTypes)event;
    switch(eventType) {
        case cv::EVENT_LBUTTONDOWN:
            firstX = x;
            firstY = y;
            lbPressed = true;
            break;
        case cv::EVENT_LBUTTONUP:
            if (lbPressed) {
                lbPressed = false;
                Exer3 *exer3 = (Exer3 *)userdata;
                exer3->showHighlightedArea(firstX, firstY, x, y);
            }
            break;
        default:
            break;
    }
}

void Exer3::showWindow() {
    cv::imshow(windowName, originalImage);
    cv::setMouseCallback(windowName, Exer3::myMouseCallback, this);
    for(; ;) {
        char c = cv::waitKey(20);
        if (c == 27) {
            break;
        }
    }
    cv::destroyWindow(windowName);
}

void Exer3::showHighlightedArea(int x1, int y1, int x2, int y2) {
    originalImage.copyTo(highlightedImage);
    if (x1 < 0) x1 = 0;
    if (x1 > highlightedImage.cols) x1 = highlightedImage.cols;
    if (x2 < 0) x2 = 0;
    if (x2 > highlightedImage.cols) x2 = highlightedImage.cols;
    if (y1 < 0) y1 = 0;
    if (y1 > highlightedImage.rows) y1 = highlightedImage.rows;
    if (y2 < 0) y2 = 0;
    if (y2 > highlightedImage.rows) y2 = highlightedImage.rows;
    int x = cv::min(x1,x2);
    int y = cv::min(y1, y2);
    int w = cv::max(x1,x2) - x;
    int h = cv::max(y1,y2) - y;
    if(w == 0 || h == 0) return;

    cv::Mat ori1(highlightedImage, cv::Rect(x, y, w, h));
//    cv::Mat highImage;
//    highImage.create(w, h, CV_8UC3);
    cv::addWeighted(ori1, 0.35, 255, 0.65, 0.0, ori1);
    cv::imshow(windowName, highlightedImage);
}
