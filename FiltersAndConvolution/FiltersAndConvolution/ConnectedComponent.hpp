//
//  ConnectedComponent.hpp
//  FiltersAndConvolution
//
//  Created by Sid Zhang on 2022/9/17.
//

#ifndef ConnectedComponent_hpp
#define ConnectedComponent_hpp

#include <stdio.h>
#include <opencv2/core.hpp>

using namespace std;

class ConnectedComponent {
public:
    ConnectedComponent(cv::Mat& image);
    cv::Mat getLargestComponentImage();
private:
    cv::Mat image; // gray image
    bool processed;
    vector<cv::Point> largestConnectedComponent;
private:
    void beginIterate();
    vector<cv::Point> findConnectedPoints(cv::Point pt);
    int64 fromPoint(cv::Point &pt);
    void setPointsValue(vector<cv::Point>& points,  uint8_t value);
};
#endif /* ConnectedComponent_hpp */
