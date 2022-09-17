//
//  ConnectedComponent.cpp
//  FiltersAndConvolution
//
//  Created by Sid Zhang on 2022/9/17.
//

#include "ConnectedComponent.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <set>
#include <iostream>
using namespace std;

ConnectedComponent::ConnectedComponent(cv::Mat& image) {
    image.copyTo(this->image);
    processed = false;
}

cv::Mat ConnectedComponent::getLargestComponentImage() {
    if(!processed) {
        beginIterate();
        processed = true;
    }
    return image;
}

void ConnectedComponent::beginIterate() {
    uint8_t *input = (uint8_t *)(image.data);

    int gray;
    for(int i = 0; i < image.cols; i++){
        for(int j = 0; j < image.rows; j++){
            gray = input[j * image.cols + i];
            if (gray == 255) {
                vector<cv::Point> allConnectedPts = findConnectedPoints(cv::Point(i,j));
                if(allConnectedPts.size() > 1)
                    cout << allConnectedPts.size() << endl;
                
                if(allConnectedPts.size() > largestConnectedComponent.size()) {
                    setPointsValue(largestConnectedComponent, 0);
                    largestConnectedComponent = allConnectedPts;
                    setPointsValue(largestConnectedComponent, 200);
                } else {
                    setPointsValue(allConnectedPts, 0);
                }
            }
        }
    }
    setPointsValue(largestConnectedComponent, 255);
    cout<< largestConnectedComponent.size() << endl;
}


vector<cv::Point> ConnectedComponent::findConnectedPoints(cv::Point pt) {
    set<int64> cpSet;
    vector<cv::Point> cpVec;
    int checkingPos = 0;
    cpSet.insert(fromPoint(pt));
    cpVec.push_back(pt);
    int size = 1;
    while(checkingPos < size) {
        int x = cpVec[checkingPos].x;
        int y = cpVec[checkingPos].y;
        ++ checkingPos;
        for (int i=-1; i<=1; i++) {
            if (x+i < 0 || x+i >= image.cols) {
                continue;
            }
            for (int j=-1; j<=1; j++) {
                if(i==0 && j == 0) continue;
                if (y+j < 0 || y+j >= image.rows) continue;
                if(image.at<uint8_t>(y+j, x+i) == 255) {
                    cv::Point tempPt(x+i, y+j);
                    int64 fpt = fromPoint(tempPt);
                    if(cpSet.find(fpt) == cpSet.end()) {
                        cpVec.push_back(tempPt);
                        cpSet.insert(fpt);
                        ++size;
                    }
                }
            }
        }
    }
    
    return cpVec;
}

int64 ConnectedComponent::fromPoint(cv::Point &pt) {
    int64 result = (int64)pt.x;
    result = result << 32;
    result += pt.y;
    return result;
}

void ConnectedComponent::setPointsValue(vector<cv::Point> &points, uint8_t value) {
    for(auto iter = points.begin(); iter != points.end(); iter++) {
        cv::Point pt = *iter;
        image.at<uint8_t>(pt.y, pt.x) = value;
    }
}
