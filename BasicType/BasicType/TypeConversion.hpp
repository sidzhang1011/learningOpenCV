//
//  TypeConversion.hpp
//  BasicType
//
//  Created by Sid Zhang on 2022/8/19.
//

#include <string>
#ifndef TypeConversion_h
#define TypeConversion_h
class TypeConversion {
private:
    std::string type;
public:
    explicit TypeConversion(const std::string& s) {
        type = s;
    }
};
// explicit(!) type conversion from strings

#endif /* TypeConversion_h */
