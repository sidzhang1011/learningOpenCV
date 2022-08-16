//
//  main.cpp
//  test
//
//  Created by Sid Zhang on 2020/10/19.
//

#include <iostream>
#include <fstream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>
//#include <opencv2/videoio/videoio.hpp>
//#include "opencv2/videoio/videoio_c.h"

int g_slider_position = 0;
int g_run = 1, g_dontset = 0;
cv::VideoCapture g_cap;

void simpleTransform();
void otherTransform();
void showImage();
void showVideo();
bool writeRealToAVI();

int main(int argc, const char * argv[]) {
//    showImage();
//    showVideo();
//    simpleTransform();
    otherTransform();
//    writeRealToAVI();
    return 0;
}

void onTrackbarSlide(int pos, void *) {
    g_cap.set(cv::CAP_PROP_POS_FRAMES, pos);
    
    if(!g_dontset) {
        g_run = 1;
    }
    g_dontset = 0;
}

void showVideo() {
    std::string path = "/Users/zsg/Downloads/江青赵丹.mp4";
    std::string windowName = "Example2_4";
    cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);
    g_cap.open(path);
    
    int frames = (int)g_cap.get(cv::CAP_PROP_FRAME_COUNT);
    int tmpw = (int)g_cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int tmph = (int)g_cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    std::cout << "Video has " <<frames << " frames of dimensions(" << tmpw << "," << tmph << ")." << std::endl;
    
    cv::createTrackbar("Position", windowName, &g_slider_position, frames, onTrackbarSlide);
    
    cv::Mat frame;
    int last_changed_pos = 0;
    for(;;) {
        
        if(g_run != 0) {
            g_cap >> frame;
            if (frame.empty()) {
                break;
            }
            int current_pos = (int)g_cap.get(cv::CAP_PROP_POS_FRAMES);
            g_dontset = 1;
            
            if(abs(current_pos - last_changed_pos) >= 20) {
                last_changed_pos = current_pos;
//                cv::setTrackbarPos("Position", windowName, current_pos); 影响性能
            }
            cv::imshow(windowName, frame);
            
            g_run -= 1;
        }
        
        char c = (char)cv::waitKey(10);
        if (c == 's') {
            g_run = 1;
        } else if(c == 'r') {
            g_run = -1;
        } else if(c == 27) {
            break;
        }
    }

    cv::destroyWindow(windowName);
}


void showImage() {
    std::string path = "/Users/zsg/Desktop/pengzuo.jpg";
    cv::Mat image = cv::imread(path);
    cv::namedWindow("origin", cv::WINDOW_AUTOSIZE);
    cv::imshow("origin", image);
    cv::waitKey(4000);
    cv::destroyWindow("origin");
}

void simpleTransform() {
    std::string path = "/Users/zsg/Desktop/pengzuo.jpg";
    std::string winName1 = "Example2_5_in";
    std::string winName2 = "Example2_5_out";
    cv::namedWindow(winName1, cv::WINDOW_AUTOSIZE);
    cv::namedWindow(winName2, cv::WINDOW_AUTOSIZE);
    
    cv::Mat image = cv::imread(path);
    // create a window to show our input image
    cv::imshow(winName1, image);
    
    // create an image to hold the smoothed output
    cv::Mat out;
    
    cv::GaussianBlur(image, out, cv::Size(5,5), 3, 3);
    cv::GaussianBlur(out, out, cv::Size(5, 5), 3, 3);
    cv::imshow(winName2, out);
    
    for(; ;) {
        char c = (char)cv::waitKey(1000);
        if(c == 27) {
            break;
        }
    }
    
    cv::destroyWindow(winName1);
    cv::destroyWindow(winName2);
}

void otherTransform() {
    cv::Mat img_rgb, img_gry, img_cny, img_pyr, img_pyr2;
    std::string path = "/Users/zsg/Desktop/pengzuo.jpeg";
    
    std::string winName0 = "Example origin";
    std::string winName1 = "Example Gray";
    std::string winName2 = "Example Canny";
    cv::namedWindow(winName1, cv::WINDOW_AUTOSIZE);
    cv::namedWindow(winName2, cv::WINDOW_AUTOSIZE);
    
    img_rgb = cv::imread(path);
    if (img_rgb.empty()) {
        std::cout << "error file name? quit" << std::endl;
        return;
    }
    
    cv::imshow(winName0, img_rgb);

    cv::cvtColor(img_rgb, img_gry, cv::COLOR_BGR2GRAY);
    cv::imshow(winName1, img_gry);
    
    cv::pyrDown(img_gry, img_pyr);
    cv::pyrDown(img_pyr, img_pyr2);

    int x = 16, y = 32;
    cv::Vec3b intensity = img_rgb.at< cv::Vec3b>(y, x);
    uchar blue = intensity[0];
    uchar green = intensity[1];
    uchar red = intensity[2];
    std::cout << "At (x,y) = (" << x <<", " << y <<
        "): (blue, green, red) = (" << (unsigned int)blue << ", " <<(unsigned int)green << ", "
        << (unsigned int)red << ")" << std::endl;
    
    std::cout << "Gray pixel there is: "<< (unsigned int)img_gry.at<uchar>(y,x) << std::endl;
    
    x /= 4; y /= 4;
    std::cout << "Pyramid2 pixel there is: " << (unsigned int)img_pyr2.at<uchar>(y, x) << std::endl;


    cv::Canny(img_gry, img_cny, 10, 100, 3, true);
    cv::imshow(winName2, img_cny);
    
    for(; ;) {
        char c = (char)cv::waitKey(1000);
        if(c == 27) {
            break;
        }
    }
    
    cv::destroyWindow(winName1);
    cv::destroyWindow(winName2);
}

bool writeRealToAVI() {
    cv::Mat img_rgb;
    std::string path = "/Users/zsg/Desktop/camera1.avi";
//    std::string path = "camera1.avi";

    std::string winName1 = "Example2 11";
    std::string winName2 = "Log_Polar";
    cv::namedWindow(winName1, cv::WINDOW_AUTOSIZE);
    cv::namedWindow(winName2, cv::WINDOW_AUTOSIZE);
    cv::VideoCapture cap;
    
    cap.open(0);
    if (!cap.isOpened()) {
        std::cerr << "Couldn't open capture. " << std::endl;
        return false;
    }
    
    double fps = cap.get( cv::CAP_PROP_FPS) ;
    cv::Size size((int)cap.get( cv::CAP_PROP_FRAME_WIDTH ), (int)cap.get( cv::CAP_PROP_FRAME_HEIGHT ) );
    
    cv::VideoWriter writer;
    writer.open(path, cv::CAP_OPENCV_MJPEG, fps, size);
    if (!writer.isOpened()) {
        std::cerr << "Couldn't open file. " << std::endl;
        return false;

    }
    
    cv::Mat logpolar_frame, bgr_frame;
    for(; ;) {
        cap >> bgr_frame;
        if (bgr_frame.empty()) break;
        
        cv::imshow(winName1, bgr_frame);
        
        cv::logPolar(bgr_frame, logpolar_frame, cv::Point2f(bgr_frame.cols/2, bgr_frame.rows/2),
                     40, cv::WARP_FILL_OUTLIERS);
        cv::imshow(winName2, logpolar_frame);
        writer << logpolar_frame;
        
        
//        writer << bgr_frame;
        
        
        char c = (char)cv::waitKey(1000);
        if(c == 27) {
            break;
        }
    }
    
    cv::destroyWindow(winName1);
    cv::destroyWindow(winName2);
    return true;
}
