//
//  main.cpp
//  BasicIOExercise
//
//  Created by Sid Zhang on 2022/9/8.
//

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "Exer3.hpp"

using namespace std;

void exer1();
void exer2();
void exer3();

int main(int argc, const char * argv[]) {
//    exer1();
    exer2();
    exer3();
    return 0;
}

void exer1() {
    string fileName = "/Users/zsg/Downloads/thirdvideos/江青赵丹.mp4";
    string windowName1 = "video frame";
    string windowName2 = "gray image";
    string windowName3 = "canny edge";
    cv::VideoCapture cap;
    cap.open(fileName);
    if (!cap.isOpened()) {
        cout << "open file " << fileName <<" failed!" <<endl;
        return;
    } else {
    }
    
    cv::Mat frame, grayImage, edgeImage;
    cv::namedWindow(windowName1, cv::WINDOW_AUTOSIZE);
    cap >> frame;
    cout << "type: " << frame.type() << "depth: " << frame.depth() << "channel: " << frame.channels() <<endl;
//    cv::imshow(windowName1, frame);
    
    cv::namedWindow(windowName2, cv::WINDOW_AUTOSIZE);
    cv::cvtColor(frame, grayImage, cv::COLOR_BGR2GRAY);
//    cv::imshow(windowName2, grayImage);

    cv::namedWindow(windowName3, cv::WINDOW_AUTOSIZE);
    cv::Canny(grayImage, edgeImage, 0.05, 0.15);
//    cv::imshow(windowName3, edgeImage);
    
    string windowName4 = "origin_to_processed";
    cv::Mat allImages;
    allImages.create(frame.rows, 3*frame.cols, frame.type());
    frame.copyTo(allImages.colRange(0, frame.cols));
    cv::Mat tempImage;
    cv::cvtColor(grayImage, tempImage, cv::COLOR_GRAY2BGR);
    tempImage.copyTo(allImages.colRange(frame.cols, 2*frame.cols));
    cv::cvtColor(edgeImage, tempImage, cv::COLOR_GRAY2BGR);
    tempImage.copyTo(allImages.colRange(2*frame.cols, 3*frame.cols));
    cv::imshow(windowName4, allImages);


    for(;;) {
        char c = (char)cv::waitKey(20);
        if (c == 27)
            break;
    }

//    cv::destroyWindow(windowName1);
//    cv::destroyWindow(windowName2);
//    cv::destroyWindow(windowName3);
    cv::destroyWindow(windowName4);
}

void myMouseCallback(int event, int x, int y, int flags, void* userdata) {
    
}

void exer2() {
    string fileName = "/Users/zsg/Downloads/large.jpeg";
    string windowName1 = "mouse event";
    cv::Mat image;
    cv::setMouseCallback(windowName1, myMouseCallback, &image);
//    cv::MouseEventTypes type;
    // the best way is to create a class e.g. Exer2, provide a updateUI() function, it's easy, skip
}

void exer3() {
    Exer3 exer3;
    exer3.showWindow();
}
