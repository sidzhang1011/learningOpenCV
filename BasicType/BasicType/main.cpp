//
//  main.cpp
//  BasicType
//
//  Created by Sid Zhang on 2022/8/18.
//

#include <iostream>
#include <opencv2/core/core.hpp>

int main(int argc, const char * argv[]) {
    cv::Point3f pt1(1,2,3);
    cv::Point3f pt2 = pt1;
    cv::Point3f pt = pt1.cross(pt2);
    std::cout << pt << std::endl;
    return 0;
}
