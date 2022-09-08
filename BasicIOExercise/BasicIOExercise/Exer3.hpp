//
//  Exer3.hpp
//  BasicIOExercise
//
//  Created by Sid Zhang on 2022/9/8.
//

#ifndef Exer3_hpp
#define Exer3_hpp

#include <stdio.h>
#include <iostream>
#include <opencv2/core.hpp>

class Exer3 {
public:
    Exer3();
    ~Exer3();
    
private:
    std::string windowName;
    cv::Mat originalImage;
    cv::Mat highlightedImage;
    
public:
    void showWindow();
    void showHighlightedArea(int x1, int y1, int x2, int y2);
    static void myMouseCallback(int event, int x, int y, int flags, void* userdata);
};
#endif /* Exer3_hpp */
