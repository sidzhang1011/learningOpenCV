//
//  main.cpp
//  LargeArray
//
//  Created by Sid Zhang on 2022/8/23.
//

#include <iostream>
#include <opencv2/core/core.hpp>
#include "NumericTypeWriter.hpp"

void handlematdata();
void handleSparseMat();
void showWriter();

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    handlematdata();
    
    handleSparseMat();
    
    showWriter();
    
    
    return 0;
}

void handlematdata() {
    // step
    cv::Mat m1(100, 20, CV_8UC3, cv::Scalar(1, 0, 1));
    std::cout << m1.step[0] << " " << m1.step[1] << " " << m1.step[2] << std::endl;
    
    // handle data
    // at
    std::cout << m1.at<cv::Vec3b>(40,20) << std::endl;
    
    cv::Mat m2 = cv::Mat::eye(10, 10, CV_32FC3);
    std::cout << m2.at<cv::Vec3f>(3,3) << std::endl;

    cv::Mat m3 = cv::Mat::eye(10, 10, CV_32FC2);
    printf(
          "Element (3,3) is %f + i%f\n",
          m3.at<cv::Complexf>(3,3).re,
          m3.at<cv::Complexf>(3,3).im);
    
    // ptr
    cv::Mat m4 = cv::Mat::eye(4, 4, CV_32FC3);
    cv::Vec3f *p3 = m4.ptr<cv::Vec3f>(3);
    std::cout << p3[3] << std::endl;
    
    // data
    float *f4 = (float *)m4.data;
    std::cout << std::endl << "matrix f4: " << (m4.isContinuous() ? "continuous data" : "discontinuous data")<< std::endl;
    for (int j=0; j<4; j++) {
        for (int i=0; i < 12; i++) {
            std::cout << f4[j*12 + i] << " ";
        }
        std::cout<<std::endl;
    }
    
    // iterate
    // normal iterator
    int sz[3] = { 4, 4, 4 };
    cv::Mat m5( 3, sz, CV_32FC3 );      // A three-dimensional array of size 4-by-4-by-4
    cv::randu( m5, -1.0f, 1.0f );       // fill with random numbers from -1.0 to 1.0
    float max = 0.0f;                   // minimum possible value of L2 norm
    float len2 = 0.0f;
    cv::MatConstIterator_<cv::Vec3f> it = m5.begin<cv::Vec3f>();
    int count = 0;
    while( it != m5.end<cv::Vec3f>() ) {
        len2 = (*it)[0]*(*it)[0]+(*it)[1]*(*it)[1]+(*it)[2]*(*it)[2];
        if( len2 > max ) max = len2;
        it++;
        ++count;
    }
    std::cout << "count: " << count << ", max: " << max << std::endl;
    
    // n-ary iterate
    const int n_mat_size = 5;
    const int n_mat_sz[] = { n_mat_size, n_mat_size, n_mat_size };
    cv::Mat n_mat0( 3, n_mat_sz, CV_32FC1 );
    cv::Mat n_mat1( 3, n_mat_sz, CV_32FC1 );
    cv::RNG rng;
    rng.fill( n_mat0, cv::RNG::UNIFORM, 0.f, 1.f );
    rng.fill( n_mat1, cv::RNG::UNIFORM, 0.f, 1.f );
    const cv::Mat* arrays[] = { &n_mat0, &n_mat1, 0 };
    cv::Mat my_planes[2];
    cv::NAryMatIterator it2( arrays, my_planes );
    float s = 0.f;                    // Total sum over all planes in both arrays
    int   n = 0;                      // Total number of planes
    for(int p = 0; p < it2.nplanes; p++, ++it) {
        auto t = cv::sum(it2.planes[0]);
        s += cv::sum(it2.planes[0])[0];
        s += cv::sum(it2.planes[1])[0];
        n++;
    }
    
    
    // block
    cv::Mat m6 = cv::Mat::ones(3, 5, CV_8UC1);
    for (int i=0; i<3; i++) {
        for (int j = 0; j<5; j++) {
            m6.at<uint8_t>(i, j) = (i+1)*(j+1);
//            m6.at<cv::Vec3b>(i,j)[0] = (i+1)*(j+1);
//            m6.at<cv::Vec3b>(i,j)[1] = (i+1)*(j+1)*2;
//            m6.at<cv::Vec3b>(i,j)[2] = (i+1)*(j+1)*3;
        }
    }
    std::cout << m6.row(0) << std::endl << m6.col(2) << std::endl<< std::endl;
    std::cout << m6.rowRange(0, 2) << std::endl << m6.colRange(2, 4) << std::endl<< std::endl;
    std::cout << m6(cv::Range(0,2), cv::Range(1,3)) << std::endl << m6(cv::Rect(0,0,2,2)) << std::endl;

    
    std::cout<<std::endl;

}

void handleSparseMat() {
    // Create a 10x10 sparse matrix with a few nonzero elements
    //
    int size[] = {10,10};
    cv::SparseMat sm( 2, size, CV_32F );
    for( int i=0; i<10; i++ ) {
      int idx[2];
      idx[0] = size[0] * (arc4random() % 100) /100;
      idx[1] = size[1] * (arc4random() % 100) /100;
      sm.ref<float>( idx ) += 1.0f;
    }
    // Print out the nonzero elements
    //
    cv::SparseMatConstIterator_<float> it = sm.begin<float>();
    cv::SparseMatConstIterator_<float> it_end = sm.end<float>();
    for(; it != it_end; ++it) {
      const cv::SparseMat::Node* node = it.node();
      printf(" (%3d,%3d) %f\n", node->idx[0], node->idx[1], *it );
    }
}

void showWriter() {
    std::shared_ptr<NumericTypeWriter> writer(new NumericTypeWriter());
    writer->showTypeWriter();
}
