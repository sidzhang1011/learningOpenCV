//
//  SmartPointers.hpp
//  BasicType
//
//  Created by Sid Zhang on 2022/8/19.
//

#ifndef SmartPointers_h
#define SmartPointers_h

#include <fstream>

using namespace std;

class SmartPointers {
private:
    std::shared_ptr< std::vector<int> > li;
public:
    SmartPointers() {
    }
    void testShared() {
        shared_ptr<string> pNico4(new string("nicola"));
//        pNico4 = new string("nico"); // ERROR: no assignment for ordinary pointers
        pNico4.reset(new string("nico")); // OK
        // two shared pointers representing two persons by their name
//        shared_ptr<string> pNico(new string("nico"));
//        shared_ptr<string> pNico = new string("nico"); // ERROR
        shared_ptr<string> pNico(new string("nico"),
                                 [](string* p) {
           cout << "delete " << *p << endl;
           delete p;
       }); // OK
//        pNico = make_shared<string>("nico");
        shared_ptr<string> pJutta(new string("jutta"));
        // capitalize person names
        (*pNico)[0] = 'N';
        pJutta->replace(0,1,"J");
        // put them multiple times in a container
        vector<shared_ptr<string>> whoMadeCoffee;
        whoMadeCoffee.push_back(pJutta);
        whoMadeCoffee.push_back(pJutta);
        whoMadeCoffee.push_back(pNico);
        whoMadeCoffee.push_back(pJutta);
        whoMadeCoffee.push_back(pNico);
        
        // print all elements
        for (auto ptr : whoMadeCoffee) {
            cout << *ptr << "  ";
        }
        cout << endl;
        // overwrite a name again
        *pNico = "Nicolai";

        // print all elements again
        for (auto ptr : whoMadeCoffee) {
            cout << *ptr << "  ";
        }
        cout << endl;
        // print some internal data
        cout << "use_count: " << whoMadeCoffee[0].use_count() << " " <<  whoMadeCoffee[2].use_count()<< endl;
        cout << "use_count: " << pNico.use_count()<< endl;
        
        pNico = nullptr;
        whoMadeCoffee.resize(2);
        cout << "use_count: " << pNico.use_count()<< endl;
        
        int *p = new int(10);
        shared_ptr<int> sp1(p);
//        shared_ptr<int> sp2(p); // error, p is not smart pointer, so sp2 will release it.
        
        
        
        std::cout << endl;
    }
    
    void testWeak() {
        try {
              shared_ptr<string> sp(new string("hi"));
              weak_ptr<string> wp = sp;
              sp.reset();
              cout << wp.use_count() << endl;
              cout << boolalpha << wp.expired() << endl;
              shared_ptr<string> p(wp);
           }
           catch (const std::exception& e) {
              cerr << "exception: " << e.what() << endl;
           }
    }
    
    void  testUnique() {
        std::unique_ptr<int> up(new int);
        *up = 10;
        std::cout << *up << endl;
    }
    
};

class FileDeleter
{
// for ofstream
// for shared_ptr // for remove()
  private:
    std::string filename;
  public:
    FileDeleter (const std::string& fn)
     : filename(fn) {
    }
    void operator () (std::ofstream* fp) {
        fp->close();                    // close.file
        std::remove(filename.c_str());  // delete file
    }
};

#endif /* SmartPointers_h */
