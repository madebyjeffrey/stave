/* 
 * File:   main.cpp
 * Author: drakej
 *
 * Created on December 17, 2012, 3:22 PM
 */

#include <cstdlib>
#include <iostream>
#include <vector>

#include "Matrix.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    cout << "Testing" << endl;
    
    drakej::Matrix<float> a(3,3, {1,2,3,4,5,6,7,8,9});
    drakej::Matrix<float> b(3,3, {1,1,1,1,1,1,1,1,1});
    
//    std::vector<float> c { 1, 2, 3, 4 };
    
//    auto x = drakej::leading_digits(c);
    
//    cout << "Digits: " << ceil(log10(1001)) << endl;
//    cout << "Digits: " << x << endl;
    
    cout << " a = " << endl << a << endl;
    cout << " b = " << endl << b << endl;
    
    auto c = a + b;
    cout << " a + b = " << endl << a + b << endl;
    
    return 0;
}

