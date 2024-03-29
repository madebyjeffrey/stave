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

/*
 * 
 */
int main(int argc, char** argv) {
    std::cout << "Testing" << std::endl;

    drakej::Matrix<float> a(3, 3,{1, 2, 3, 4, 5, 6, 7, 8, 9});
    drakej::Matrix<float> b(3, 3,{1, 1, 1, 1, 1, 1, 1, 1, 1});

    //    std::vector<float> c { 1, 2, 3, 4 };

    //    auto x = drakej::leading_digits(c);

    //    cout << "Digits: " << ceil(log10(1001)) << endl;
    //    cout << "Digits: " << x << endl;

    std::cout << " a = " << std::endl << a << std::endl;
    std::cout << " b = " << std::endl << b << std::endl;
    
    /*for (auto it = a.column_begin(2); it != a.column_end(); ++it)
    {
        std::cout << *it << std::endl;
    }*/

    
    for (int i = 0; i < 3; ++i)
    {
        std::cout << a(0, i) << std::endl;
    }

    auto c = a * b;
    std::cout << " a + b = " << std::endl << c << std::endl;


    return 0;
}

