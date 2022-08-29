//
//  main.cpp
//  ArrayOperation
//
//  Created by Sid Zhang on 2022/8/29.
//

#include <iostream>
#include <opencv2/core/core.hpp>
void arrayop();

int main(int argc, const char * argv[]) {
    arrayop();
    return 0;
}

void arrayop() {
    cv::Mat m1 = cv::Mat::zeros(4, 4, CV_32FC1);
    m1.at<float>(1, 1) = 10;
    std::cout << m1 << std::endl;
    
    cv::Mat m2 = cv::Mat::eye(4, 4, CV_32FC1);
    cv::Mat mdiff = m1 - m2;
    std::cout << m2 <<std::endl;
    std::cout << mdiff <<std::endl;
    

}
