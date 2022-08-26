//
//  NumericTypeWriter.hpp
//  LargeArray
//
//  Created by Sid Zhang on 2022/8/25.
//

#ifndef NumericTypeWriter_hpp
#define NumericTypeWriter_hpp

#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <map>
#include <list>

class NumericTypeWriter {
public:
    NumericTypeWriter(const cv::Size2i& writerSize = {500,500}, const cv::Size2i &numericSize = {10, 20});
    bool showTypeWriter();

private:
    cv::Size2i writerSize;
    cv::Size2i numericSize;
    cv::Point2i cursorPos;
    cv::Point2i lastCursorPos;
    cv::Point2i maxCursorPos;
    std::unique_ptr<cv::Mat> typeWriter;
    std::vector<int> allTyped;
    std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> digitNumPixels;
    std::string windowName;
    
private:
    bool createTypeWriter();
    void showNumber(int number, int grayIntensity);
    void displayDigit(int startPixelX, int startPixelY, int number);
    bool handleNumber(int num);
    void handleBackspace();
    void handleEnter();
    std::shared_ptr<cv::Mat> getPixels(int num);
    int calculatePosForCursorInText();
    void displayRemainingContents(int startPos);
    bool addAndDisplayContents(int num);
    
    
};

#endif /* NumericTypeWriter_hpp */
