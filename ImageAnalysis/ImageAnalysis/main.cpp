//
//  main.cpp
//  ImageAnalysis
//
//  Created by Sid Zhang on 2022/9/30.
//

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/photo.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
//#include <opencv2


using namespace std;
cv::Rect fgRect;
cv::Mat src_img, roi_img, dst_img;
string win1 = "grabcut_image_original";

void fourier();
void dft_example();
void grab_cut();
void exer01();
void exer02();
void exer04();
void exer05();
void exer08();
void exer09();
void exer10();
void exer11();
void exer12();

void dft_color(cv::Mat srcImg, cv::Mat filter, cv::Mat& dstImg);

int main(int argc, const char * argv[]) {
//    fourier();
//    dft_example();
//    grab_cut();
//    exer01();
//    exer02();
//    exer04();
//    exer05();
//    exer08();
    exer09();
    exer10();
    exer11();
    exer12();

    return 0;
}

void fourier() {
    cv::Mat src = cv::Mat::eye(4, 4, CV_32FC1);
    src.at<float>(2,1) = 0.5;
    src.at<float>(2,3) = 0.2;
    src.at<float>(0,2) = 0.25;
    src.at<float>(1,2) = 0.5;
    src.at<float>(1,3) = 0.2;
    src.at<float>(0,1) = 0.25;
    cv::Mat dst, dst1, dst2;
    cv::dft(src, dst, cv::DFT_SCALE);
    cv::dft(src, dst1, cv::DFT_SCALE);
    cv::dft(src, dst2, cv::DFT_SCALE | cv::DFT_COMPLEX_OUTPUT);
    cout << dst << endl << dst.type() << endl << dst.channels() << endl;
    cout << dst1 << endl << dst1.type() << endl;
//    cout << dst2 << endl << dst2.type() << endl;
    cv::idft(dst, src);//, cv::DFT_COMPLEX_INPUT);
    cout << src << endl << src.channels() << endl;
    
    int size = cv::getOptimalDFTSize(89);
    cout << size << endl;
}

void dft_example() {
    string fileName = "/Users/zsg/Desktop/tree1.jpeg";
    cv::Mat A = cv::imread(fileName, cv::IMREAD_GRAYSCALE);
    if( A.empty() ) {
        cout << "Cannot load " << fileName << endl;
        return;
    }
    cv::Size patchSize( 100, 100 );
    cv::Point topleft( A.cols/2, A.rows/2 );
    cv::Rect roi(topleft.x, topleft.y, patchSize.width, patchSize.height);
    cv::Mat B = A( roi );

    int dft_M = cv::getOptimalDFTSize( A.rows+B.rows-1 );
    int dft_N = cv::getOptimalDFTSize( A.cols+B.cols-1 );
    cv::Mat dft_A = cv::Mat::zeros( dft_M, dft_N, CV_32FC1 );
    cv::Mat dft_B = cv::Mat::zeros( dft_M, dft_N, CV_32FC1 );
    cv::Mat dft_A_part = dft_A( cv::Rect(0, 0, A.cols,A.rows) );
    cv::Mat dft_B_part = dft_B( cv::Rect(0, 0, B.cols,B.rows) );
    A.convertTo( dft_A_part, dft_A_part.type(), 1, -mean(A)[0] );
    B.convertTo( dft_B_part, dft_B_part.type(), 1, -mean(B)[0] );
    cout <<"mean(A):\n" << mean(A) <<endl;
    cout << mean(B) << endl;
    cv::dft( dft_A, dft_A, 0, A.rows );
    cv::dft( dft_B, dft_B, 0, B.rows );
    // set the last parameter to false to compute convolution instead of correlation
    //
    cv::mulSpectrums( dft_A, dft_B, dft_A, 0, true );
    cv::idft( dft_A, dft_A, cv::DFT_SCALE, A.rows + B.rows - 1 );
//    cv::imshow( "Image1", dft_A );
//    cv::imshow( "Image2", dft_B_part );
    cv::Mat corr = dft_A( cv::Rect(0, 0, A.cols + B.cols - 1, A.rows + B.rows - 1) );
    cv::normalize( corr, corr, 0, 1, cv::NORM_MINMAX, corr.type() );
//    cv::imshow( "Correlation1", corr );
    cv::pow( corr, 3., corr );
    

    B ^= cv::Scalar::all( 255 );
    cv::imshow( "Image", A );
    cv::imshow( "Correlation", corr );
    cv::moveWindow("Correlation", 800, 0);
    cv::waitKey();
}

void onMouse(int event, int x, int y, int flags, void *userData);
void showImg();

void grab_cut() {
    src_img = cv::imread("/Users/zsg/Desktop/figure.jpeg");
    cv::namedWindow(win1, cv::WINDOW_AUTOSIZE);
    cv::imshow(win1, src_img);
    cv::setMouseCallback(win1, onMouse);
    
    cv::Mat fgModel, bgModel;
    while(1 ){
        char c = cv::waitKey(0);
        if (c == 'o') {
            dst_img = cv::Mat::zeros(src_img.size(), CV_8UC1);
            cv::grabCut(src_img, dst_img, fgRect, bgModel, fgModel, 1, cv::GC_INIT_WITH_RECT);
            cv::compare(dst_img, cv::GC_PR_FGD, dst_img, cv::CMP_EQ);
            
            cv::Mat fg = cv::Mat(src_img.size(), CV_8UC3, cv::Scalar(255,255,255));
            src_img.copyTo(fg, dst_img);
            cv::imshow(win1, fg);
        } else if ( c == 27) {
            break;
        }
    }
    cv::waitKey();
    cv::destroyWindow(win1);
}

void onMouse(int event, int x, int y, int flags, void *userdata) {
    switch(event) {
        case cv::EVENT_LBUTTONDOWN: {
            fgRect.x = x;
            fgRect.y = y;
            fgRect.width = 1;
            fgRect.height = 1;
            break;
        }
        case cv::EVENT_MOUSEMOVE: {
            if (flags & cv::EVENT_FLAG_LBUTTON) {
                fgRect = cv::Rect(cv::Point(fgRect.x, fgRect.y), cv::Point(x, y));
                showImg();
                break;
            }
        }
        case cv::EVENT_LBUTTONUP: {
            showImg();
            break;
        }
        default:
            break;
    }
}

void showImg() {
    src_img.copyTo(roi_img);
    cv::rectangle(roi_img, fgRect, cv::Scalar(0, 0, 255), 2);
    cv::imshow(win1, roi_img);
}

void exer01() {
    string imageFileName = "/Users/zsg/Desktop/刘涛.jpeg";
    cv::Mat srcImg = cv::imread(imageFileName, cv::IMREAD_COLOR);
    cv::namedWindow("original", cv::WINDOW_AUTOSIZE);
    cv::imshow("original", srcImg);
    cv::Mat dstImg1, dstImg2, dstImg3;
    int t1 = 25;
    
    for (int i = 0; i<5; ++i) {
        cv::Canny(srcImg, dstImg1, t1, t1*1.5);
        cv::namedWindow("win1", cv::WINDOW_AUTOSIZE);
        cv::moveWindow("win1", 100, 200);
        cv::imshow("win1", dstImg1);

        cv::Canny(srcImg, dstImg2, t1, t1*2.75);
        cv::namedWindow("win2", cv::WINDOW_AUTOSIZE);
        cv::moveWindow("win2", 200, 100);
        cv::imshow("win2", dstImg2);

        cv::Canny(srcImg, dstImg3, t1, t1*4);
        cv::namedWindow("win3", cv::WINDOW_AUTOSIZE);
        cv::moveWindow("win3", 300, 0);
        cv::imshow("win3", dstImg3);

        int c = cv::waitKey(0);
        if (c == 27) break;
        t1 += 50;
    }

    cv::destroyWindow("original");
    cv::destroyWindow("win1");
    cv::destroyWindow("win2");
    cv::destroyWindow("win3");
}

void exer02() {
    string imageFileName = "/Users/zsg/Desktop/bicycle.jpeg";
    cv::Mat srcImg = cv::imread(imageFileName, cv::IMREAD_COLOR);
    cv::namedWindow("original", cv::WINDOW_AUTOSIZE);
    cv::imshow("original", srcImg);
    cv::Mat gray;
    cv::cvtColor(srcImg, gray, cv::COLOR_BGR2GRAY);
    cv::Mat edges;
    cv::Canny(gray, edges, 100, 175);
    cv::namedWindow("edges", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("edges", 100, 50);
    cv::imshow("edges", edges);
    
    vector<cv::Vec4s> lines;
//    cv::Mat lines;
    cv::HoughLinesP(edges, lines, 1, CV_PI/180.0, 200);
    cv::Mat result1 = cv::Mat::zeros(srcImg.rows, srcImg.cols, srcImg.type());

    srcImg.copyTo(result1);
    for (size_t i=0; i<lines.size(); ++i) {
        cv::line(result1, cv::Point(lines[i][0], lines[i][1]), cv::Point(lines[i][2], lines[i][3]), cv::Scalar(255,255,255), 2);
    }
    cv::namedWindow("win1", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("win1", 200, 100);
    cv::imshow("win1", result1);


    vector<cv::Vec3f> circles;
    cv::GaussianBlur(gray, gray, cv::Size(5,5), 0, 0);
    cv::Canny(gray, edges, 100, 175);
    cv::HoughCircles(edges, circles, cv::HOUGH_GRADIENT, 1, srcImg.cols/5, 100, 100, 20, 500);
    cv::Mat result2;
    srcImg.copyTo(result2);
    for (size_t i=0; i<circles.size(); ++i) {
        cv::Vec3f c = circles[i];
        cout << c[2] << endl;
        cv::circle(result2, cv::Point(cvRound(c[0]), cvRound(c[1])), cvRound(c[2]), cv::Scalar(0,255,0), 2, cv::LINE_AA);
    }
    cv::namedWindow("win2", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("win2", 300, 200);
    cv::imshow("win2", result2);

    cv::waitKey();
    cv::destroyWindow("original");
    cv::destroyWindow("win1");
    cv::destroyWindow("win2");
}

void exer04() {
    int kSize = 3;
    
    string imageFileName = "/Users/zsg/Desktop/bicycle.jpeg";
    cv::Mat srcImg = cv::imread(imageFileName, cv::IMREAD_COLOR);
    vector<cv::Mat> imgParts;
    cv::split(srcImg, imgParts);
    
    cv::namedWindow("original", cv::WINDOW_AUTOSIZE);
    cv::imshow("original", srcImg);

    for (int j = 0; j < 6; j++) {
        int64 prevtick = cv::getTickCount();
        kSize = 3 + 8 * j;
        cv::Mat k1 = cv::getGaussianKernel(kSize, 0);
        cv::Mat k2 = cv::getGaussianKernel(kSize, 0);
        cv::Mat kernel = k1 * k2.t();
        
        int dft_M = cv::getOptimalDFTSize(kernel.rows + srcImg.rows - 1);
        int dft_N = cv::getOptimalDFTSize(kernel.cols + srcImg.cols - 1);
        cv::Mat kernel_dft = cv::Mat::zeros(dft_M, dft_N, CV_32F);
        cv::Mat kernel_dft_part = kernel_dft(cv::Rect(0, 0, kernel.cols, kernel.rows));
        kernel.convertTo(kernel_dft_part, kernel_dft.type());
        cv::dft(kernel_dft, kernel_dft);
        vector<cv::Mat> imgFreqs;
        for (size_t i=0; i<imgParts.size(); ++i) {
            cv::Mat imgTemp = cv::Mat::zeros(dft_M, dft_N, CV_32F);
            cv::Mat imgTemp_part = imgTemp(cv::Rect(0, 0, srcImg.cols, srcImg.rows));
            imgParts[i].convertTo(imgTemp_part, imgTemp.type());
            cv::dft(imgTemp, imgTemp);
            imgFreqs.push_back(imgTemp);
        }
        
        vector<cv::Mat> freqMuls;
        for (size_t i=0; i<imgParts.size(); ++i) {
            cv::Mat mulTemp;
            cv::mulSpectrums(imgFreqs[i], kernel_dft, mulTemp, 0);
            cv::idft(mulTemp, mulTemp, cv::DFT_SCALE);
            cv::Mat mulTemp_part = mulTemp(cv::Rect(0, 0, srcImg.cols, srcImg.rows));
            freqMuls.push_back(mulTemp_part);
        }
        
        cv::Mat result1;
        cv::merge(freqMuls, result1);
        cv::normalize(result1, result1, 0, 1, cv::NORM_MINMAX, result1.type());
        int64 seconds = cv::getTickCount() - prevtick;
        float sec = (float)seconds/1000000000;
        cout << "kernel size:" << kSize <<" convolution time:" << sec << endl;
        
        string winName = "result" + to_string(j);
        
        cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);
        cv::imshow(winName, result1);
        
        cv::waitKey();
    }
    
}

float getRectSum(cv::Mat igrMat, int x, int y, int width, int height) {
    float sum = igrMat.at<float>(y+height, x+width) - igrMat.at<float>(y+height, x) - igrMat.at<float>(y, x+width) + igrMat.at<float>(y, x);
    return sum;
}

void fillHorzLine(cv::Mat src, cv::Vec4s &line, int x, int y, int width) {
    while(x > 0 && src.at<uint8_t>(y, x) == 255) {
        --x;
    }
    int end = x + width;
    while(end < src.cols - 1 && src.at<uint8_t>(y, end) == 255) {
        ++end;
    }
    line[0] = x;
    line[1] = y;
    line[2] = end;
    line[3] = y;
}

void exer05() {
    string imageFileName = "/Users/zsg/Desktop/lines.png";
    cv::Mat srcImg = cv::imread(imageFileName, cv::IMREAD_GRAYSCALE);
    srcImg = 255 - srcImg;
//    cv::Mat edges;
//    cv::Canny(srcImg, edges, 20, 10);
    cv::threshold(srcImg, srcImg, 50, 255, cv::THRESH_BINARY);
    
    float reachFence = 0.8;         // continuous extent of edge
    float minEdgePoints = 100;       // minimum number of edget points
//    float minEdgetLenScale = 0.1;   // minimum edge length scale to image dimension
    cv::Mat igrMat;
    vector<cv::Vec4s> foundEdges;
    cv::integral(srcImg, igrMat, CV_32FC1);
    float sumFence = 255 * minEdgePoints * reachFence;
    
    // only for horizontal edge, vertical edge is similar
    int x = 0, y= 0;
    int count = 0;
    while (y < srcImg.rows) {
        float cmpFence = sumFence;
        while(x < srcImg.cols) {
            float sum_line = getRectSum(igrMat, x, y, minEdgePoints, 1);
            if (sum_line > 10) {
                cout << count << ": (" << x << ","<<y << ") " << sum_line << " " << endl;
                ++ count;
            }
            if (sum_line >= sumFence) {
                cv::Vec4s line;
                if (y == srcImg.rows - 1 || y == 0) {
                    fillHorzLine(srcImg, line, x, y, minEdgePoints);
                    foundEdges.push_back(line);
                    x = line[2] + 1;
                    cmpFence = sumFence;
                } else {
                    float sum_prev = getRectSum(igrMat, x, y-1, minEdgePoints, 1);
                    float sum_next = getRectSum(igrMat, x, y+1, minEdgePoints, 1);
                    if (sum_next < 0.2*sumFence || sum_prev < 0.2 *sumFence) {
                        fillHorzLine(srcImg, line, x, y, minEdgePoints);
                        foundEdges.push_back(line);
                        x = line[2] + 1;
                        cmpFence = sumFence;
                    } else {
                        cmpFence = sumFence;
                        x += int(cmpFence)/255;
                    }
                }
            } else {
                cmpFence = sumFence - sum_line;
                x += int(cmpFence)/255;
            }
        }
        ++y;
        x = 0;
    }
    
    cv::cvtColor(srcImg, srcImg, cv::COLOR_GRAY2BGR);
    for (int i = 0; i < foundEdges.size(); ++i) {
        cv::Vec4s line = foundEdges[i];
        cv::line(srcImg, cv::Point(line[0], line[1]), cv::Point(line[2], line[3]), cv::Scalar(0,0,255), 2);
    }
    
    string winName = "result";
    cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);
    cv::imshow(winName, srcImg);

    cv::waitKey();
}

void exer06() {
    // rotate 45 degree, then find horizontal lines
}

void exer07() {
    // subtract and mask
}

void exer08() {
    string imageFileName = "/Users/zsg/Desktop/bicycle.jpeg";
    cv::Mat srcImg = cv::imread(imageFileName, cv::IMREAD_COLOR);
    
    string winName = "original";
    cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);
    cv::imshow(winName, srcImg);

    
    cv::Mat result1;
    int64 gbTicks = cv::getTickCount();
    cv::GaussianBlur(srcImg, result1, cv::Size(51,51), 0);
    gbTicks = cv::getTickCount() - gbTicks;
    float gbSec = gbTicks/1000000000.0;
    cout << "direct gaussBlur seconds(51x51): " << gbSec << endl;
    string winName1 = "gaussBlur";
    cv::namedWindow(winName1, cv::WINDOW_AUTOSIZE);
    cv::imshow(winName1, result1);

    
    cv::Mat result2;
    cv::Mat k1 = cv::getGaussianKernel(51, 0);
    cv::Mat k2 = cv::getGaussianKernel(51, 0);
    cv::Mat kernel = k1 * k2.t();
    int64 dft_ticks = cv::getTickCount();
    dft_color(srcImg, kernel, result2);
    dft_ticks = cv::getTickCount() - dft_ticks;
    double dftSecs = dft_ticks/1000000000.0;
    cout << "dft gaussBlur seconds(51x51): " << dftSecs << endl;
    string winName2 = "dft Blur";
    cv::namedWindow(winName2, cv::WINDOW_AUTOSIZE);
    cv::imshow(winName2, result2);
    
    cv::waitKey();

}

void dft_color(cv::Mat srcImg, cv::Mat filter, cv::Mat& dstImg) {
    int dft_M = srcImg.rows + filter.rows - 1;
    int dft_N = srcImg.cols + filter.cols - 1;
        
    cv::Mat filter_dft = cv::Mat::zeros(dft_M, dft_N, CV_32F);
    cv::Mat result_dft_part = filter_dft(cv::Rect(0, 0, filter.cols, filter.rows));
    filter.convertTo(result_dft_part, filter_dft.type());
    cv::dft(filter_dft, filter_dft);
    vector<cv::Mat> imgFreqs;
    
    vector<cv::Mat> imgParts;
    cv::split(srcImg, imgParts);
    for (size_t i=0; i<imgParts.size(); ++i) {
        cv::Mat imgTemp = cv::Mat::zeros(dft_M, dft_N, CV_32F);
        cv::Mat imgTemp_part = imgTemp(cv::Rect(0, 0, srcImg.cols, srcImg.rows));
        imgParts[i].convertTo(imgTemp_part, imgTemp.type());
        cv::dft(imgTemp, imgTemp);
        imgFreqs.push_back(imgTemp);
    }
    
    vector<cv::Mat> freqMuls;
    for (size_t i=0; i<imgFreqs.size(); ++i) {
        cv::Mat mulTemp;
        cv::mulSpectrums(imgFreqs[i], filter_dft, mulTemp, 0);
        cv::idft(mulTemp, mulTemp, cv::DFT_SCALE);
        cv::Mat mulTemp_part = mulTemp(cv::Rect( (filter.cols-1)/2, (filter.rows-1)/2, srcImg.cols, srcImg.rows));
        freqMuls.push_back(mulTemp_part);
//        freqMuls.push_back(mulTemp);
    }
    
    cv::merge(freqMuls, dstImg);
    cv::normalize(dstImg, dstImg, 0, 1, cv::NORM_MINMAX, dstImg.type());

}

void exer09() {
    src_img = cv::imread("/Users/zsg/Desktop/刘涛.jpeg");
    cv::namedWindow(win1, cv::WINDOW_AUTOSIZE);
    cv::imshow(win1, src_img);
    cv::setMouseCallback(win1, onMouse);
    
    cv::Mat fgModel, bgModel;
    while(1 ){
        char c = cv::waitKey(0);
        if (c == 'o') {
            dst_img = cv::Mat::zeros(src_img.size(), CV_8UC1);
            cv::grabCut(src_img, dst_img, fgRect, bgModel, fgModel, 1, cv::GC_INIT_WITH_RECT);
            cv::compare(dst_img, cv::GC_PR_FGD, dst_img, cv::CMP_NE);
            
            cv::Mat fg = cv::Mat(src_img.size(), CV_8UC3, cv::Scalar(0,0,0));
            src_img.copyTo(fg, dst_img);
            cv::imshow(win1, fg);
            fg.copyTo(src_img);
        } else if ( c == 27) {
            break;
        }
    }
    cv::waitKey();
    cv::destroyWindow(win1);
}

void exer10() {
}

void exer11() {
}

void exer12() {
}
