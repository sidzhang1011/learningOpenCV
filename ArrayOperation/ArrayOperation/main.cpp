//
//  main.cpp
//  ArrayOperation
//
//  Created by Sid Zhang on 2022/8/29.
//

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;

void arrayop();
void drawing();
void functors();
void dataPersistence();
void mouseEvent();

int main(int argc, const char * argv[]) {
//    arrayop();
//    drawing();
//    functors();
//    dataPersistence();
    mouseEvent();
    return 0;
}

void arrayop() {
    cv::Mat m1 = cv::Mat::zeros(4, 4, CV_32FC1);
    m1.at<float>(1, 1) = 10;
    std::cout << m1 << std::endl;
    
    cv::Mat m2 = cv::Mat::eye(4, 4, CV_32FC1);
    cv::Mat mdiff = m1 - m2;
    std::cout << m2 <<std::endl;
//    std::cout << mdiff <<std::endl;
//    std::cout << cv::abs(mdiff) << std::endl;
    
    cv::Mat mabsdiff;
    cv::absdiff(m1, m2, mabsdiff);
    std::cout << mabsdiff << std::endl;
    
    // calcCovar
//    cv::RNG rng;
//    rng.fill( n_mat0, cv::RNG::UNIFORM, 0.f, 1.f );
    std::cout << std::endl << "calc covariance and mean" << std::endl;
    cv::Mat m3 = cv::Mat::zeros(3, 4, CV_32FC1);
    float* data = (float *)m3.data;
    data[0] = 1.0;
    data[3] = 1.0;
    data[5] = 1.0;
    data[10] = 1.0;
    std::cout << m3 << std::endl;
    cv::Mat covar1;
    cv::Mat mean1;
    cv::calcCovarMatrix(m3, covar1, mean1, cv::COVAR_NORMAL|cv::COVAR_ROWS);
    std::cout << "mean:" << std::endl << mean1 << std::endl;
    std::cout << "covariance:" << std::endl << covar1 << std::endl;
    
    std::cout << m3 << std::endl;
    cv::Mat m4 = cv::Mat::zeros(3, 4, CV_32FC1);
    cv::copyTo(m3, m4, cv::noArray());
    for (int i =0; i<3; i++) {
        for (int j=0; j<4; j++) {
            m4.at<float>(i, j) -= mean1.at<double>(0, j);
        }
    }
    cv::Mat m41;
    cv::transpose(m4, m41);
    cv::Mat covar2 = m41 * m4;
    cv::Mat diffcovar;
    diffcovar.create(4,4,CV_32FC1);
    cv::subtract(covar2, covar1, diffcovar, cv::noArray(), CV_64FC1);
    std::cout << "m4:" << m4 << std::endl << "m41:" << m41 << std::endl << "covar2" << covar2 << std::endl;
    std::cout << diffcovar <<std::endl;
    
    // eigen
    cv::Mat m5 = cv::Mat::eye(3, 3, CV_32FC1);
    m5.at<float>(1,2) = 2.0;
    m5.at<float>(0, 1) = 2.5;
    cv::Mat  eigenvalues;
    cv::Mat eigenvectors;
    cv::eigen(m5, eigenvalues, eigenvectors);
    std::cout << eigenvalues << std::endl << eigenvectors << std::endl;
    
    // checkrange
    try {
        bool inRange = cv::checkRange(m5, false, 0, 0, 1);
    } catch(const cv::Exception& e) {
        std::cout << e.msg <<std::endl;
    }
    
    // determinant
    double t = cv::determinant(m5);
    std::cout << t << std::endl;
    
    // meanstddev
    cv::Mat mean2;
    cv::Mat stddev;
    cv::meanStdDev(m5, mean2, stddev);
    std::cout << mean2 << std::endl << stddev << std::endl;
    
    // merge
    cv::Mat m6[2];
    m6[0] = cv::Mat::ones(3, 3, CV_32FC1);
    m6[1] = cv::Mat::eye(3, 3, CV_32FC1);
    cv::Mat m63;
    cv::merge(m6, 2, m63);
    std::cout << m63 << std::endl;
    
    // norm
    cv::Mat m7 = cv::Mat::eye(3, 3, CV_32FC1);
    double n1 = cv::norm(m7, cv::NORM_INF);
    double n2 = cv::norm(m7, cv::NORM_L1);
    double n3 = cv::norm(m7, cv::NORM_L2);
    std::cout<<"norm: INF:"<<n1<<" L1:" << n2 << " L2:" << n3 <<std::endl;

    // randu
    cv::Mat randm;
    randm.create(4, 4, CV_8UC2);
    cv::randu(randm, cv::Scalar(0, 0), cv::Scalar(255, 255));
    std::cout<< randm << std::endl;
    
    // reduce & repeat, important but easy
    
    // solve
    cv::Mat lhs, rhs;
    lhs.create(4, 4, CV_32FC1);
    cv::randu(lhs, 0, 1);
    rhs.create(4, 1, CV_32FC1);
    cv::randu(rhs, 0, 1);
    cv::Mat m8;
    cv::solve(lhs,rhs, m8);
    std::cout<<m8<< std::endl;
    std::cout << lhs * m8 - rhs << std::endl;

}

void drawing() {
    std::string windowName = "origin";
    cv::Mat image = cv::Mat::zeros(400, 400, CV_32FC3);
    cv::namedWindow(windowName, cv::WINDOW_GUI_EXPANDED);
    
    // circle
    cv::circle(image, cv::Point(50,50), 20, cv::Scalar(0,0,255),2, cv::FILLED);
    
    // ellipse
    cv::ellipse(image, cv::Point(120, 50), cv::Size(30,20), 0, 0, 360, cv::Scalar(0,255,0), 1, cv::LINE_AA);
    
    std::vector<cv::Point> pts;
    cv::ellipse2Poly(cv::Point(120, 50), cv::Size(30,20), 0, 0, 360, 60, pts);
    std::cout << pts << std::endl;
    
    cv::fillConvexPoly(image, pts.data(), pts.size(), cv::Scalar(128, 128, 0), cv::LINE_8);
    
    // line
    cv::line(image, cv::Point(160, 20), cv::Point(180, 70), cv::Scalar(255, 128, 128));
    
    // rectangle
    cv::rectangle(image, cv::Rect(190, 30, 50, 40), cv::Scalar(255, 0, 255));
    
    // polyLines
    vector<vector<cv::Point>> lines(1);
    lines[0] = pts;
    cv::polylines(image, lines, true, cv::Scalar(255, 0, 255), 1, cv::LINE_8, 1 );
    
    // text
    for (int i = 0; i<5; i++ ) {
        cv::putText(image, "Hello World", cv::Point(100, 100 + 50*i), cv::FONT_HERSHEY_PLAIN, 0.5*(i+2), cv::Scalar(0, 255,128));
    }
    
    cv::imshow(windowName, image);
    for(; ;) {
        char c = (char)cv::waitKey(30);
        if(c == 27) {
            break;
        }
    }
    cv::destroyWindow(windowName);
}

void functors() {
    // PCA
    cv::Mat m1;
    m1.create(2, 2, CV_32FC1);
    cv::RNG rng;
    rng.fill( m1, cv::RNG::UNIFORM, 0.f, 1.f );
//    cv::PCA p1(m1, 0, 0);
    
    // SVD
    cv::Mat m2;
    m2.create(3, 5, CV_32FC1);
    rng.fill(m2, cv::RNG::UNIFORM, -10, 10);
    cv::SVD svd(m2);
    using namespace cv;
    Mat W,U,Vt;
    svd.compute(m2, W, U, Vt);
    Mat W2 = Mat::zeros(3,5, CV_32FC1);
    W2.at<float>(0,0) = W.at<float>(0,0);
    W2.at<float>(1,1) = W.at<float>(1,0);
    W2.at<float>(2,2) = W.at<float>(2,0);
    Mat Vt2 = Mat::zeros(5,5,CV_32FC1);

    Mat subVt2 = Vt2.rowRange(0, 3);
    Vt.copyTo(subVt2);

    cout<< "SVD: m2:" << m2 << endl << U <<endl << W <<endl << Vt2 <<endl;
    Mat diff = U*W2*Vt2 - m2;
    cout << diff << endl;
    
    Mat z;
    cv::SVD::solveZ(m2, z);
    cout << m2 * z << endl;
    
    // Random Number Generator (cv::RNG)
    cv::RNG rng2(getTickCount());
    cout << endl << endl;
    for(int i=0; i<10; i++ ) {
        double gs = rng2.gaussian(0.5);
        cout << gs << endl;
    }
    cout << endl << "3 float(0.0-1.0)" << endl;
    for (int i=0; i< 3; ++i) {
        float f1 = rng2.uniform(0.0, 1.0);
        cout << f1 <<" ";
    }
    cout << endl;
    cout << endl << "3 double (gaussian dis) sigma = 1.0" << endl;
    for (int i=0; i< 3; ++i) {
        double f1 = rng2.gaussian(1.0);
        cout << f1 <<" ";
    }
    cout << endl;
    
    // exercise 3
    cv::Mat m3;
    m3.create(3, 100, CV_8UC1);
    rng2.fill(m3.row(0), RNG::NORMAL, 64, 10);
    rng2.fill(m3.row(1), RNG::NORMAL, 192, 10);
    rng2.fill(m3.row(2), RNG::NORMAL, 128, 2);

    cout << m3 << endl;
    cv::PCA pca(m3, noArray(), PCA::DATA_AS_ROW, 2);
    cv::Mat m3p;
    pca.project(m3, m3p);
    auto meanp = cv::mean(m3p);
    cout << meanp << endl;

    // exercise 4
    Mat m4 = Mat::ones(3, 2, CV_32FC1);
    m4.at<float>(1,0) = 0;
    m4.at<float>(2,0) = -1;
    Mat m4t;
    transpose(m4, m4t);
    Mat eigenValues, eigenVectors;
    eigen(m4t*m4, eigenValues, eigenVectors);
    Mat m4tao;
    sqrt(eigenValues, m4tao);
    cout << eigenValues << endl << eigenVectors<<endl << m4tao << endl;
    Mat U4 = Mat::zeros(3,3, CV_32FC1);
    Mat u21 = 1/m4tao.at<float>(0, 0) *m4*eigenVectors.col(0);
    u21.copyTo(U4.col(0));
    Mat u22 = 1/m4tao.at<float>(1, 0)*m4*eigenVectors.col(1);
    u22.copyTo(U4.col(1));
    cout << U4 << endl;
    Mat U4t;
    transpose(U4, U4t);
    Mat z2;
    SVD::solveZ(U4t, z2);
    cout << z2 << endl;
    z2.copyTo(U4.col(2));
    float det2 = determinant(U4);
    cout << "previous determinant: " << det2 << endl;
    U4.col(2) = U4.col(2)/det2;
    cout << "U4: " << U4 << endl << "determinant: " << determinant(U4) << endl;
    
    Mat sigma4 = Mat::zeros(3, 2, CV_32FC1);
    sigma4.at<float>(0, 0) = m4tao.at<float>(0,0);
    sigma4.at<float>(1, 1) = m4tao.at<float>(1,0);
    Mat v4t;
    transpose(eigenVectors, v4t);
    Mat diff4 = m4 - U4 * sigma4 * v4t;
    cout << diff4 << endl;
    
    Mat U42, S42, V42t;
    SVD svd2;
    svd2.compute(m4, S42, U42, V42t);
    cout << U42 << endl << S42 << endl << V42t << endl;

}

void dataPersistence() {
    cv::FileStorage fs("/Users/zsg/special/help.yml", cv::FileStorage::WRITE);

    fs << "frameCount" << 5;
        
    time_t rawtime; time(&rawtime);
    fs << "calibrationDate" << asctime(localtime(&rawtime));
    
    cv::Mat cameraMatrix = (
          cv::Mat_<double>(3,3)
          << 1000, 0, 320, 0, 1000, 240, 0, 0, 1);
    cv::Mat distCoeffs = (
        cv::Mat_<double>(5,1)
        << 0.1, 0.01, -0.001, 0, 0);
    fs << "cameraMatrix" << cameraMatrix << "distCoeffs" << distCoeffs;
    
    fs << "features" << "[";
    for( int i = 0; i < 3; i++ )
    {
        int x = rand() % 640;
        int y = rand() % 480;
        uchar lbp = rand() % 256;
        fs << "{:" << "x" << x << "y" << y << "lbp" << "[:";
        for( int j = 0; j < 8; j++ )
            fs << ((lbp >> j) & 1);
        fs << "]" << "}";
    }
    fs << "]";
    fs.release();
    
    
    // read
    cv::FileStorage fs2("/Users/zsg/special/help.yml", cv::FileStorage::READ);
    // first method: use (type) operator on FileNode.
    int frameCount = (int)fs2["frameCount"];
    // second method: use cv::FileNode::operator >>
    //
    std::string date;
    fs2["calibrationDate"] >> date;
    cv::Mat cameraMatrix2, distCoeffs2;
    fs2["cameraMatrix"] >> cameraMatrix2;
    fs2["distCoeffs"] >> distCoeffs2;
    cout << "frameCount: "        << frameCount
         << "calibration date: "  << date
         << "camera matrix: "     << cameraMatrix2 << endl
         << "distortion coeffs: " << distCoeffs2   << endl;
    cv::FileNode features   = fs2["features"];
    cv::FileNodeIterator it = features.begin(), it_end = features.end();
    int idx                 = 0;
    std::vector<uchar> lbpval;
    
    // iterate through a sequence using FileNodeIterator
    for( ; it != it_end; ++it, idx++ )
    {
        cout << "feature #" << idx << ": ";
        cout << "x=" << (int)(*it)["x"] << ", y=" << (int)(*it)["y"] << ", lbp: (";
        // ( Note: easily read numerical arrays using FileNode >> std::vector. )
        //
        (*it)["lbp"] >> lbpval;
        for( int i = 0; i < (int)lbpval.size(); i++ )
            cout << " " << (int)lbpval[i];
        cout << ")" << endl;
    }
    fs2.release();
        
}

//
// Define our callback which we will install for mouse events
//
void my_mouse_callback(int event, int x, int y, int flags, void *param);

cv::Rect box;
bool drawing_box = false;


// A little subroutine to draw a box onto an image
//
void draw_box(cv::Mat& img, cv::Rect box) {
    cv::rectangle(img, box.tl(), box.br(), cv::Scalar(0x00, 0x00, 0xff));
}

void help() {
    std::cout << "Show how to use a mouse to draw regions in an image." << std::endl;
}

void mouseEvent() {
    help();
    
    box = cv::Rect(-1, -1, 0, 0);
    cv::Mat image(200, 200, CV_8UC3), temp;
    image.copyTo(temp);
    
    box = cv::Rect(-1, -1, 0, 0);
    image = cv::Scalar::all(0);
    
    cv::namedWindow( "Box Example" );
    
    // Here is the crucial moment where we actually install
    // the callback. Note that we set the value of 'params' to
    // be the image we are working with so that the callback
    // will have the image to edit.
    //
      
    cv::setMouseCallback(
        "Box Example",
        my_mouse_callback,
        (void*)&image
    );
      
    // The main program loop. Here we copy the working image
    // to the temp image, and if the user is drawing, then
    // put the currently contemplated box onto that temp image.
    // Display the temp image, and wait 15ms for a keystroke,
    // then repeat.
    //
    
    for(;;) {
        image.copyTo(temp);
        if( drawing_box ) draw_box( temp, box );
        cv::imshow( "Box Example", temp );
        if( cv::waitKey( 15 ) == 27 ) break;
    }
    
}

// This is our mouse callback. If the user
// presses the left button, we start a box.
// When the user releases that button, then we
// add the box to the current image. When the
// mouse is dragged (with the button down) we
// resize the box.
//

void my_mouse_callback( int event, int x, int y, int flags, void* param ) {
    cv::Mat& image = *(cv::Mat*) param;
    switch( event ) {
        case cv::EVENT_MOUSEMOVE: {
            if( drawing_box ) {
                box.width  = x-box.x;
                box.height = y-box.y;
            }
        }
            break;
        case cv::EVENT_LBUTTONDOWN: {
            drawing_box = true;
            box = cv::Rect( x, y, 0, 0 );
        }
            break;
        case cv::EVENT_LBUTTONUP: {
            drawing_box = false;
            if( box.width < 0  ) {
                box.x += box.width;
                box.width *= -1;
            }
            if( box.height < 0 ) {
                box.y += box.height;
                box.height *= -1;
            }
            draw_box( image, box );
        }
            break;
    }
}
