//
//  NumericTypeWriter.cpp
//  LargeArray
//
//  Created by Sid Zhang on 2022/8/25.
//

#include "NumericTypeWriter.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <map>
#include <vector>
//#include <opencv2/photo/photo.hpp>

NumericTypeWriter::NumericTypeWriter(const cv::Size2i& writerSize, const cv::Size2i& numericSize)
:cursorPos(0,0),
lastCursorPos(0,0) {
    this->writerSize = writerSize;
    this->numericSize = numericSize;
    maxCursorPos.x = writerSize.width / numericSize.width - 1;
    maxCursorPos.y = writerSize.height/ numericSize.height - 1;
}

bool NumericTypeWriter::createTypeWriter() {
//    std::unique_ptr<cv::Mat> tw(new cv::Mat(this->writerSize, CV_8UC3));
//    this->typeWriter = std::move(tw);
    this->typeWriter = std::unique_ptr<cv::Mat>(new cv::Mat(this->writerSize, CV_8UC3));
    if(this->typeWriter)
        return true;
    else
        return false;
}

bool NumericTypeWriter::showTypeWriter() {
    bool result = createTypeWriter();
    if (!result) {
        return result;
    }
    this->windowName = "Numeric Type Writer";
    cv::namedWindow(this->windowName, cv::WINDOW_GUI_EXPANDED);
    cv::imshow(this->windowName, *this->typeWriter);

    bool changed = false;
    bool quit = false;
    for (; ;) {
        if (changed) {
            cv::imshow(this->windowName, *this->typeWriter);
        }
        char c = (char)cv::waitKey(30);
        if (c != 27) {
            if (maxCursorPos.x < 0 || maxCursorPos.y < 0)
                c = 0;
        }
        switch (c) {
            case 27:
                quit = true;
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                changed = handleNumber(c - '0');
                break;
            case '\r':
                handleEnter();
                break;
            // backspace
            case 127:
                handleBackspace();
                break;
                // up
            case 0x0:
                if (cursorPos.x > 0) {
                    --cursorPos.x;
                } else if (cursorPos.x == 0) {
                    cursorPos.y = 0;
                }
                break;
                // down
            case 0x01:
                if (cursorPos.x < maxCursorPos.x) {
                    ++cursorPos.x;
                }
                break;
                // left
            case 0x02:
                if (cursorPos.y > 0) {
                    --cursorPos.y;
                }
                break;
                // right
            case 0x03:
                if (cursorPos.y < maxCursorPos.y+1) {
                    ++cursorPos.y;
                }
                break;
            default:
                break;
        }
        
        if (quit) break;
    }
    cv::destroyWindow(this->windowName);

    return result;
}


void NumericTypeWriter::handleBackspace() {
    // line start, return
    if (cursorPos.x == 0 && cursorPos.y == 0) return;
    if (allTyped.size() == 0) return;
    int pos = calculatePosForCursorInText();
    if (pos < 1) return;
    allTyped.erase(allTyped.begin() + pos - 1);
    if (cursorPos.y > 0) {
        --cursorPos.y;
    } else {
        --cursorPos.x;
        cursorPos.y = maxCursorPos.y;
                
        do {
            int pos = calculatePosForCursorInText();
            int value = allTyped[pos];
            if (value != '\r' && value != 10)
                break;
            --cursorPos.y;
            if (0 == cursorPos.y) {
                break;
            }
        } while(1);
    }
    
    cv::Point2i tempLast = lastCursorPos;
    if(tempLast.y > 0) {
        --tempLast.y;
    }
    int clearPixelX = tempLast.x * numericSize.height;
    for (int y = tempLast.y; y < maxCursorPos.y; ++y) {
        displayDigit(clearPixelX, y * numericSize.width, 100);
    }
    
    displayRemainingContents(pos - 1);
}

void NumericTypeWriter::handleEnter() {
    if (cursorPos.x > maxCursorPos.x)
        return;
    addAndDisplayContents('\n');
    ++cursorPos.x;
    cursorPos.y = 0;
}


cv::Vec3b getColor(int num) {
    switch(num) {
        case 0:
            return cv::Vec3b(255, 255, 255);
        case 1:
            return cv::Vec3b(128, 255, 128);
        case 2:
            return cv::Vec3b(0, 255, 0);
        case 3:
            return cv::Vec3b(0, 0, 255);
        case 4:
            return cv::Vec3b(255, 255, 0);
        case 5:
            return cv::Vec3b(255, 0, 255);
        case 6:
            return cv::Vec3b(0, 255, 255);
        case 7:
            return cv::Vec3b(128, 0, 255);
        case 8:
            return cv::Vec3b(0, 255, 128);
        case 9:
            return cv::Vec3b(0, 128, 255);

    }
    return cv::Vec3b(255, 255, 255);
}

bool NumericTypeWriter::handleNumber(int num) {
    if (cursorPos.x > maxCursorPos.x)
        return false;
    if (num < 0 || num > 9) return false;
    
    bool result = addAndDisplayContents(num);
    if (!result) return false;

    ++cursorPos.y;
    if (cursorPos.y > maxCursorPos.y) {
        cursorPos.y = 0;
        ++ cursorPos.x;
    }
    return true;
}

void NumericTypeWriter::displayRemainingContents(int startPos) {
    lastCursorPos = cursorPos;
    int startPixelX = cursorPos.x * numericSize.height;
    int startPixelY = cursorPos.y * numericSize.width;
    for (int j = startPos; j < allTyped.size(); ++j) {
        const int &currentNum = allTyped[j];
        if(currentNum == '\n') {
            ++lastCursorPos.x;
        } else {
            displayDigit(startPixelX, startPixelY, currentNum);
            if (lastCursorPos.y < maxCursorPos.y) {
                ++lastCursorPos.y;
                startPixelY += numericSize.width;
            } else {
                lastCursorPos.y = 0;
                ++lastCursorPos.x;
                startPixelX += numericSize.height;
                startPixelY = 0;
            }
        }
    }
}

bool NumericTypeWriter::addAndDisplayContents(int num) {
    int pos = calculatePosForCursorInText();
    if (pos == -1) return false;
    if (pos == allTyped.size()) {
        allTyped.push_back(num);
    } else {
        allTyped.insert(allTyped.begin() + pos, num);
    }

    displayRemainingContents(pos);
    return true;
}

int NumericTypeWriter::calculatePosForCursorInText() {
    int x = cursorPos.x;
    if (x >= maxCursorPos.x + 1) {
        return -1;
    }
    if (x < 0) {
        x = 0;
    }

    int y = cursorPos.y;
    if (y < 0) {
        y = 0;
    } else if (y > maxCursorPos.y) {
        y = maxCursorPos.y;
    }
        
    int pos = 0;
    int calcX = 0;
    int calcY = 0;
    for (pos = 0; pos < allTyped.size(); pos++) {
        if (allTyped[pos] == '\n' || allTyped[pos] == 10) {
            ++calcX;
            calcY = 0;
        } else {
            if (calcY < maxCursorPos.y) {
                ++calcY;
            } else {
                ++calcX;
                calcY = 0;
            }
        }
        if ((calcX > x) || (calcX == x && calcY > y)) {
            break;
        }
    }
    return pos;
}

void NumericTypeWriter::displayDigit(int startPixelX, int startPixelY, int num) {
    for (int y = startPixelY; y < startPixelY + numericSize.width; y++) {
        for (int x = startPixelX; x < startPixelX + numericSize.height; x++) {
            typeWriter->at<cv::Vec3b>(x, y) = cv::Vec3b(0,0,0);
        }
    }
    cv::Vec3b color = getColor(num);
    int offsetY = 3;
    int offsetX = 7;
    switch(num) {
        case 0:
            for (int y = startPixelY + offsetY; y < startPixelY + numericSize.width - offsetY; y++ ) {
                typeWriter->at<cv::Vec3b>(startPixelX + offsetX, y) = color;
                typeWriter->at<cv::Vec3b>(startPixelX + numericSize.height - offsetX, y) = color;
            }
            for (int x = startPixelX + offsetX; x < startPixelX + numericSize.height - offsetX; x++ ) {
                typeWriter->at<cv::Vec3b>(x, startPixelY + offsetY) = color;
                typeWriter->at<cv::Vec3b>(x, startPixelY + numericSize.width - offsetY) = color;
            }
            break;
        case 1:
            for (int x = startPixelX + 5; x < startPixelX + numericSize.height - 5; x++ ) {
                typeWriter->at<cv::Vec3b>(x, startPixelY + numericSize.width/2) = color;
            }
            break;
        case 2:
            for (int y = startPixelY + offsetY; y < startPixelY + numericSize.width - offsetY; y++ ) {
                typeWriter->at<cv::Vec3b>(startPixelX + offsetX - 2, y) = color;
                typeWriter->at<cv::Vec3b>(startPixelX + offsetX + 3, y) = color;
                typeWriter->at<cv::Vec3b>(startPixelX + numericSize.height - offsetX + 2, y) = color;
            }
            for (int x = startPixelX + offsetX - 2; x < startPixelX + offsetX + 3; x++ ) {
                typeWriter->at<cv::Vec3b>(x, startPixelY + numericSize.width - offsetY) = color;
            }
            for (int x = startPixelX + offsetX + 3; x < startPixelX + numericSize.height - offsetX + 2; x++ ) {
                typeWriter->at<cv::Vec3b>(x, startPixelY + offsetY) = color;
            }

            break;
        case 3:
            for (int y = startPixelY + offsetY; y < startPixelY + numericSize.width - offsetY; y++ ) {
                typeWriter->at<cv::Vec3b>(startPixelX + offsetX - 2, y) = color;
                typeWriter->at<cv::Vec3b>(startPixelX + offsetX + 3, y) = color;
                typeWriter->at<cv::Vec3b>(startPixelX + numericSize.height - offsetX + 2, y) = color;
            }
            for (int x = startPixelX + offsetX - 2; x < startPixelX + numericSize.height - offsetX + 2; x++ ) {
                typeWriter->at<cv::Vec3b>(x, startPixelY + numericSize.width - offsetY) = color;
            }

            break;
        case 4:
            for (int y = startPixelY + offsetY; y < startPixelY + numericSize.width - offsetY + 3; y++ ) {
                typeWriter->at<cv::Vec3b>(startPixelX + offsetX + 3, y) = color;
            }
            for (int x = startPixelX + offsetX -1; x < startPixelX + offsetX + 3; x++ ) {
                typeWriter->at<cv::Vec3b>(x, startPixelY + offsetY) = color;
            }
            for (int x = startPixelX + offsetX - 2 ; x < startPixelX + numericSize.height - offsetX + 2 ; x++ ) {
                typeWriter->at<cv::Vec3b>(x, startPixelY + 0.5*numericSize.width + 2) = color;
            }
            break;
        case 5:
            for (int y = startPixelY + offsetY; y < startPixelY + numericSize.width - offsetY; y++ ) {
                typeWriter->at<cv::Vec3b>(startPixelX + offsetX - 2, y) = color;
                typeWriter->at<cv::Vec3b>(startPixelX + offsetX + 3, y) = color;
                typeWriter->at<cv::Vec3b>(startPixelX + numericSize.height - offsetX + 2, y) = color;
            }
            for (int x = startPixelX + offsetX - 2; x < startPixelX + offsetX + 3; x++ ) {
                typeWriter->at<cv::Vec3b>(x, startPixelY + offsetY) = color;
            }
            for (int x = startPixelX + offsetX + 3; x < startPixelX + numericSize.height - offsetX + 2; x++ ) {
                typeWriter->at<cv::Vec3b>(x, startPixelY + numericSize.width - offsetY) = color;
            }
            break;
        case 6:
            for (int y = startPixelY + offsetY; y < startPixelY + numericSize.width - offsetY; y++ ) {
                typeWriter->at<cv::Vec3b>(startPixelX + offsetX - 2, y) = color;
                typeWriter->at<cv::Vec3b>(startPixelX + offsetX + 3, y) = color;
                typeWriter->at<cv::Vec3b>(startPixelX + numericSize.height - offsetX + 2, y) = color;
            }
            for (int x = startPixelX + offsetX - 2; x < startPixelX + numericSize.height - offsetX + 2; x++ ) {
                typeWriter->at<cv::Vec3b>(x, startPixelY + offsetY) = color;
            }
            for (int x = startPixelX + offsetX + 3; x < startPixelX + numericSize.height - offsetX + 2; x++ ) {
                typeWriter->at<cv::Vec3b>(x, startPixelY + numericSize.width - offsetY) = color;
            }

            break;
        case 7:
            for (int y = startPixelY + offsetY; y < startPixelY + numericSize.width - offsetY; y++ ) {
                typeWriter->at<cv::Vec3b>(startPixelX + offsetX, y) = color;
            }
            for (int x = startPixelX + offsetX; x < startPixelX + numericSize.height - offsetX + 2; x++ ) {
                typeWriter->at<cv::Vec3b>(x, startPixelY + numericSize.width - offsetY) = color;
            }
            break;
        case 8:
            for (int y = startPixelY + offsetY; y < startPixelY + numericSize.width - offsetY; y++ ) {
                typeWriter->at<cv::Vec3b>(startPixelX + offsetX - 2, y) = color;
                typeWriter->at<cv::Vec3b>(startPixelX + offsetX + 3, y) = color;
                typeWriter->at<cv::Vec3b>(startPixelX + numericSize.height - offsetX + 2, y) = color;
            }
            for (int x = startPixelX + offsetX - 2; x < startPixelX + numericSize.height - offsetX + 2; x++ ) {
                typeWriter->at<cv::Vec3b>(x, startPixelY + offsetY) = color;
                typeWriter->at<cv::Vec3b>(x, startPixelY + numericSize.width - offsetY) = color;
            }
            break;
        case 9:
            for (int y = startPixelY + offsetY; y < startPixelY + numericSize.width - offsetY; y++ ) {
                typeWriter->at<cv::Vec3b>(startPixelX + offsetX - 2, y) = color;
                typeWriter->at<cv::Vec3b>(startPixelX + offsetX + 3, y) = color;
            }
            for (int x = startPixelX + offsetX - 2; x < startPixelX + offsetX + 3; x++ ) {
                typeWriter->at<cv::Vec3b>(x, startPixelY + offsetY) = color;
            }
            for (int x = startPixelX + offsetX - 2; x < startPixelX + numericSize.height - offsetX + 2; x++ ) {
                typeWriter->at<cv::Vec3b>(x, startPixelY + numericSize.width - offsetY) = color;
            }
            break;

        default:
            break;
    }
 
}

std::shared_ptr<cv::Mat> NumericTypeWriter::getPixels(int num) {
    if (digitNumPixels == nullptr) {
        digitNumPixels = std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>>(new std::vector<std::shared_ptr<cv::Mat>>());
        
    }
    return (*digitNumPixels)[num];
}
