//
//  main.cpp
//  BasicType
//
//  Created by Sid Zhang on 2022/8/18.
//

#include <iostream>
#include <opencv2/core/core.hpp>
#include "TypeConversion.hpp"
#include "SmartPointers.hpp"

using namespace std;

void testcpp();
template <typename T>
void printElements (const T& coll);
void utility();

int main(int argc, const char * argv[]) {
    testcpp();
    // Point
    cv::Point3f pt1(1,2,3);
    cv::Point3f pt2(4,5,6);
    cv::Point3f pt = pt1.cross(pt2);
    std::cout << pt << std::endl;
    
    // Rect
    cv::Rect r1(50, 50, 100, 100);
    cv::Rect r2(60, 40, 80, 120);
    std::cout << (r1 & r2) <<std::endl;
    std::cout << (r1 | r2) <<std::endl;
    cv::RotatedRect rr(cv::Point(100,100), cv::Size(50, 50), 45);
    cv::Point2f pts[4];
    rr.points(pts);
    std::cout << pts[0] << pts[1] << pts[2] << pts[3] << std::endl;
    
    // fixed matrix classes
    cv::Matx33f m33(1,2,3,2,0.33,6,4,6,7);
    std::cout << m33 << std::endl;
    std::cout << m33(1,2) << std::endl;
    std::cout << m33.t() << std::endl;
    std::cout << m33.inv() << std::endl;
    
    // Ptr
    cv::Ptr<cv::Point> md = new cv::Point(10, 20);
    md.empty();
//    md.release();
    std::cout << *md << std::endl;
    
    // DataType
    cv::DataType<float> f;
//#define CV_MAT_DEPTH_MASK       (CV_DEPTH_MAX - 1)
//#define CV_MAT_DEPTH(flags)     ((flags) & CV_MAT_DEPTH_MASK)
//
//#define CV_MAKETYPE(depth,cn) (CV_MAT_DEPTH(5) + (((cn)-1) << CV_CN_SHIFT))
//#define CV_MAKE_TYPE CV_MAKETYPE

    std::cout<<f.type << std::endl;
    std::cout<<f.fmt << std::endl;
    
    cv::DataType<cv::Rect> dr;
    std::cout << dr.fmt << ' ' << dr.channels << std::endl;
    utility();
    
    return 0;
}

void testcpp() {
    int t = 3.30;
    std::cout << t << std::endl;
    std::initializer_list<int> l{1,2,3,4,10,9};
    std::cout << "list:" << std::endl;
    printElements(l);
    std::cout << std::endl;// << std::endl << std::endl;

    std::vector<double> vec{2,4,6,8};
    for (auto &elem : vec) {
        elem *= 2;
    }
    printElements(vec);

    std::string s = R"nc(a\
                         b\nc(
                         )nc";
    std::cout << s << std::endl;
    std::string s1 = u8"我门";
    std::cout<< s1 << std::endl << "length:" << s1.length() << ", size:" <<s1.size() << std::endl;
    
    SmartPointers sp;
    sp.testShared();
    
    std::shared_ptr<std::ofstream> fp(new std::ofstream("tmpfile.txt"), FileDeleter("tmpfile.txt"));
    
    sp.testWeak();
    sp.testUnique();
    
    
    
    std::cout << std::endl;
//    int t{3.3};
}

template <typename T>
void printElements (const T& coll) {
    for (const auto& elem : coll) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    for (auto _pos=coll.begin(); _pos != coll.end(); ++_pos ) {
        const auto& elem = *_pos;
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

void utility() {
    std::pair<int, int> ages(50, 44);
    std::cout<<ages.first<<" " <<ages.second<<std::endl;
    
    tuple<int,float,string> t1(41,6.3,"nico");
    cout<< get<0>(t1) << " " << get<1>(t1) << " " << get<2>(t1) << endl;
}
