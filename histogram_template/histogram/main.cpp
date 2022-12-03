//
//  main.cpp
//  histogram
//
//  Created by Sid Zhang on 2022/11/23.
//

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/photo.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace std;

void testcalc();
void example_1301();
void example_1302();
void example_1303();
void template_matching();

cv::Mat hue;
int bins = 25;
void hist_and_backproj(int, void *);

int main(int argc, const char * argv[]) {
//    testcalc();
//    example_1301();
//    example_1302();
//    example_1303();
    template_matching();
    return 0;
}

void testcalc() {
//    cv::Mat src = cv::Mat::ones(4, 4, CV_8UC3);
//    cv::imshow("test1", src);
//    cout << "src:" << src << endl << endl;
//
//    cv::Mat hist;
//    int histSize[] = {4, 4};
//    float ranges1[] = {0,2};
//    float ranges2[] = {0,2};
    cv::Mat src = cv::imread("/Users/zsg/Desktop/刘涛.jpeg");
    cv::imshow("test1", src);
    
    cv::Mat hist;
    int histSize[] = {8, 8};
    float ranges1[] = {0,256};
    float ranges2[] = {0,256};
    const float *ranges[] = {ranges1, ranges2};
    int ch[] = {0, 1};
    cv::calcHist(&src, 1, ch, cv::noArray(), hist, 2, histSize, ranges);
    cout << "hist:" << hist.size() << endl <<hist << endl;
    
    cv::waitKey();

}

void example_1301() {
    cv::Mat src = cv::imread("/Users/zsg/Desktop/刘涛.jpeg");
    // Compute the HSV image, and decompose it into separate planes.
    //
    cv::Mat hsv;
    cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);
    float h_ranges[]      = {0, 180}; // hue is [0, 180]
    float s_ranges[]      = {0, 256};
    const float* ranges[] = {h_ranges, s_ranges};
    int histSize[]        = {60, 32}, ch[] = {0, 1};
    cv::Mat hist;
    // Compute the histogram
    //
    cv::calcHist(&hsv, 1, ch, cv::noArray(), hist, 2, histSize, ranges, true);
    cv::normalize(hist, hist, 0, 255, cv::NORM_MINMAX);
    int scale = 10;
    cv::Mat hist_img(histSize[0]*scale, histSize[1]*scale, CV_8UC3);
    // Draw our histogram.
    //
    cv::rectangle(
                  hist_img,
                  cv::Rect(0, 0, histSize[1]*scale, histSize[0]*scale),
                  cv::Scalar(0, 0, 255),
                  -1
                  );
    for( int h = 0; h < histSize[0]; h++ ) {
        for( int s = 0; s < histSize[1]; s++ ) {
            float hval = hist.at<float>(h, s);
            cv::rectangle(
                          hist_img,
                          cv::Rect(s*scale,h*scale,scale,scale),
                          cv::Scalar::all(hval),
                          -1
                          );
        }
    }
    cv::imshow("image", src);
    cv::imshow("histogram", hist_img);
    cv::waitKey();
}

void example_1302() {
    string fileNames[] = {"/Users/zsg/Desktop/HandIndoorColor.jpg", "/Users/zsg/Desktop/HandOutdoorColor.jpg", "/Users/zsg/Desktop/HandOutdoorSunColor.jpg", "/Users/zsg/Desktop/figure.jpg"};
    vector<cv::Mat> src(5);
    cv::Mat tmp;
    int i;
    
    tmp = cv::imread(fileNames[0]);
    
    // Parse the first image into two image halves divided halfway on y
    //
    cv::Size size = tmp.size();
    int width = size.width;
    int height = size.height;
    int halfheight = height >> 1;
    
    cout << "Getting size [[" <<tmp.cols << "] [" << tmp.rows <<"]]\n" << endl;
    cout << "Got size (w, h): (" << size.width << "," << size.height << ")" << endl;
    
    src[0] = cv::Mat(cv::Size(width, halfheight), CV_8UC3);
    src[1] = cv::Mat(cv::Size(width, halfheight), CV_8UC3);
    
    // Divide the first image into top and bottom halves into src[0] and src[1]
    //
    cv::Mat_<cv::Vec3b>::iterator tmpit = tmp.begin<cv::Vec3b>();
    
    // top half
    //
    cv::Mat_<cv::Vec3b>::iterator s0it = src[0].begin<cv::Vec3b>();
    for(i = 0; i < width*halfheight; ++i, ++tmpit, ++s0it)
        *s0it = *tmpit;
    
    // bottom half
    //
    cv::Mat_<cv::Vec3b>::iterator s1it = src[1].begin<cv::Vec3b>();
    for(i=0; i < width*halfheight; ++i, ++tmpit, ++s1it)
        *s1it = *tmpit;
    
    // Load the other three images
    //
    for (i = 2; i < 5; ++i) {
        src[i] = cv::imread(fileNames[i-1]);
        if (src[i].empty()) {
            cerr << "Error on reading image " << fileNames[i-1] << endl;
            return;
        }
    }
    
    // Compute the HSV image, and decompose it into separate planes.
    //
    vector<cv::Mat> hsv(5), hist(5), hist_img(5);
    int             h_bins = 8;
    int             s_bins = 8;
    int             hist_size[] = { h_bins, s_bins}, ch[] = {0, 1};
    float           h_ranges[] = { 0, 180 };
    float           s_ranges[] = { 0, 256 };
    const float*  ranges[] = { h_ranges, s_ranges };
    int             scale = 10;
    
    for(i = 0; i<5; ++i) {
        cv::cvtColor(src[i], hsv[i], cv::COLOR_BGR2HSV);
        cv::calcHist(&hsv[i], 1, ch, cv::noArray(), hist[i], 2, hist_size, ranges, true);
        cv::normalize( hist[i], hist[i], 0, 255, cv::NORM_MINMAX);
        hist_img[i] = cv::Mat::zeros( hist_size[0]*scale, hist_size[1]*scale, CV_8UC3 );
        
        // Draw our histogram for the 5 images
        //
        for( int h = 0; h < hist_size[0]; h++ ) {
            for( int s = 0; s< hist_size[1]; s++) {
                float hval = hist[i].at<float>(h, s);
                cv::rectangle(hist_img[i], cv::Rect(h*scale, s*scale, scale, scale), cv::Scalar::all(hval), -1);
            }
        }
    }
    
    // Display
    //
    cv::imshow( "Source0", src[0]);
    cv::imshow("HS Histogram0", hist_img[0]);
    cv::moveWindow("Source0", 0, 0);
    cv::moveWindow("HS Histogram0", 100, 0);

    cv::imshow( "Source1", src[1]);
    cv::imshow("HS Histogram1", hist_img[1]);
    cv::moveWindow("Source1", 400, 0);
    cv::moveWindow("HS Histogram1", 500, 0);

    cv::imshow( "Source2", src[2]);
    cv::imshow("HS Histogram2", hist_img[2]);
    cv::moveWindow("Source2", 800, 0);
    cv::moveWindow("HS Histogram2", 900, 0);

    cv::imshow( "Source3", src[3]);
    cv::imshow("HS Histogram3", hist_img[3]);
    cv::moveWindow("Source3", 0, 400);
    cv::moveWindow("HS Histogram3", 300, 400);

    cv::imshow( "Source4", src[4]);
    cv::imshow("HS Histogram4", hist_img[4]);
    cv::moveWindow("Source4", 600, 400);
    cv::moveWindow("HS Histogram4", 1000, 400);

    // Compare the histogram src0 vs 1, vs 2, vs 3, vs 4
    cout << "Comparison:\n"
        << "Corr                 Chi                 Intersect          Bhat\n"
        << endl;
    for( i = 1; i<5; ++i) {  // For each histogram
        cout << "Hist[0] vs Hist[" << i << "]: " << endl;;
        for(int j=0; j<4; ++j) { // For each comparison type
            cout << "method[" << j << "]: " << cv::compareHist(hist[0],hist[i],j) << "  ";
        }
        cout << endl;
    }
    //Do EMD and report
    //
    vector<cv::Mat> sig(5);
    cout << "\nEMD: " << endl;

    // Oi Vey, parse histograms to earth movers signatures
    //
    for( i=0; i<5; ++i) {
        vector<cv::Vec3f> sigv;
        // (re)normalize histogram to make the bin weights sum to 1.
        //
        cv::normalize(hist[i], hist[i], 1, 0, cv::NORM_L1);
        for( int h = 0; h < h_bins; h++ )
            for( int s = 0; s < s_bins; s++ ) {
                float bin_val = hist[i].at<float>(h, s);
                if( bin_val != 0 )
                    sigv.push_back( cv::Vec3f(bin_val, (float)h, (float)s));
            }
        // make Nx3 32fC1 matrix, where N is the number of nonzero histogram bins
        //
        sig[i] = cv::Mat(sigv).clone().reshape(1);
        if( i > 0 )
            cout << "Hist[0] vs Hist[" << i << "]: "
            << EMD(sig[0], sig[i], cv::DIST_L2) << endl;
    }
    cv::waitKey(0);
}

void example_1303() {
    cv::Mat src = cv::imread("/Users/zsg/Desktop/Back_Projection_Theory0.jpg");
    cv::Mat hsv;
    cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);
    cout << hsv.depth() << endl;
    hue.create(hsv.size(), hsv.depth());
    int ch[] = {0, 0};
    cv::mixChannels(&hsv, 1, &hue, 1, ch, 1);
    cout << hue.channels()  << endl;
    
    const char* window_image = "Source image";
    cv::namedWindow(window_image);
    cv::createTrackbar("* Hue bins: ", window_image, &bins, 180, hist_and_backproj);
    hist_and_backproj(0, 0);
    
    cv::imshow(window_image, src);
    cv::imshow("hue ", hue);

    
    // Wait until user exits the program
    cv::waitKey();
}

void hist_and_backproj(int, void *) {
    int histSize = MAX(bins, 2);
    float hue_range[] = { 0, 180};
    const float* ranges[] = { hue_range };
    
    cv::Mat hist;
    cv::calcHist(&hue, 1, 0, cv::Mat(), hist, 1, &histSize, ranges, true, false);
    cv::normalize( hist, hist, 0, 255, cv::NORM_MINMAX, -1, cv::Mat());
    
    cv::Mat backproj;
    cv::calcBackProject(&hue, 1, 0, hist, backproj, ranges, 1, true);
    cout << backproj.size() << endl;
    cout << backproj << endl;
    
    
    cv::imshow( "BackProj", backproj);
    
    int w = 400, h = 400;
    int bin_w = cvRound((double)w/ histSize);
    cv::Mat histImg = cv::Mat::zeros( h, w, CV_8UC3);
    
    for (int i=0; i < bins; i++) {
        cv::rectangle(histImg, cv::Point( i*bin_w, h - cvRound( hist.at<float>(i)*h/255.0) ), cv::Point( (i+1)*bin_w, h ), cv::Scalar( 0, 0, 255 ), cv::FILLED);
    }
    
    cv::imshow( "Histogram", histImg);
}

void template_matching() {
    cv::Mat src, templ, ftmp[6];            // ftmp is what to display on
    
    src = cv::imread("/Users/zsg/Desktop/Template_Matching_Original_Image.jpg");
    templ = cv::imread("/Users/zsg/Desktop/Template_Matching_Template_Image.jpg");
    
    // Do the matching of the template with the image
    for(int i=0; i<6; ++i){
      cv::matchTemplate( src, templ, ftmp[i], i);
      cv::normalize( ftmp[i], ftmp[i], 1, 0, cv::NORM_MINMAX);
    }
    
    // Display
    cv::imshow("Template", templ);
    cv::imshow("Image", src);
    
    cv::imshow( "SQDIFF", ftmp[0] );
    cv::imshow( "SQDIFF_NORMED", ftmp[1] );
    cv::imshow( "CCORR", ftmp[2] );
    cv::imshow( "CCORR_NORMED", ftmp[3] );
    cv::imshow( "CCOEFF", ftmp[4] );
    cv::imshow( "CCOEFF_NORMED", ftmp[5] );
    // Let user view results:
    //
    cv::waitKey(0);
    
}
