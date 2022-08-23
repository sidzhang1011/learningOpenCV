//
//  TypeTraits.hpp
//  BasicType
//
//  Created by Sid Zhang on 2022/8/22.
//

#ifndef TypeTraits_h
#define TypeTraits_h
class TypeTraits {
public:
    TypeTraits() {
    }
    
public:
    template <typename T>
    void printType (const T& val){
        if (std::is_pointer<T>::value) {
            std::cout << "foo() called for a pointer" << std::endl;
        }
        else {
            std::cout << "foo() called for a value" << std::endl; }
    }
    
    template <typename T1, typename T2>
    typename std::common_type<T1, T2>::type min(const T1& x, const T2& y) {
        if(x <= y)
            return x;
        else
            return y;
    }
};

#endif /* TypeTraits_h */
