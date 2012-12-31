
#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <type_traits>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <utility>
#include <cmath>
#include <functional>
#include <stdexcept>
#include <limits>
#include <memory>

// #include "Tensor.h"

namespace drakej
{
    
    template<typename T, typename Enable = void>
    class Matrix;
        
    template<typename T>
    class Matrix<T, typename std::enable_if<std::is_arithmetic<T>::value>::type>
    {
        std::vector<T> p_;
        std::size_t row_, col_;
        
        std::size_t precision_ = 3; // precision for printing
        
        // Max of digits before the decimal in the matrix
        std::size_t 
        digits_ante_dot() const 
        {
            std::size_t digits = 0;
        
            std::for_each(std::begin(p_), std::end(p_), 
                    [&digits](T v) 
            {
                std::size_t i = ceil(log10(v+1));
                
                digits = digits > i ? digits : i;
            });

            return digits;
        }
        
        public: 

        using size_type = typename std::vector<T>::size_type;
        using value_type = T;
            
        Matrix(std::size_t row, std::size_t col) : p_(row * col), row_(row), col_(col)
        {
        }
        
        Matrix(std::size_t row, std::size_t col, std::initializer_list<T> l) 
        :  p_(l), row_(row), col_(col)
        {
//            static_assert(row * col == l.size(), "Initializer must be equal to size of matrix");
            if (row * col != l.size())
                std::cerr << "Warning: Incorrect size.";
        }
        
        Matrix(Matrix const &m) 
        : p_(m.p_), row_(m.row_), col_(m.col_)
        {
        }
        
        Matrix(Matrix &&m) 
        : p_(m.p_), row_(m.row_), col_(m.col_)
        {
        }
        
        Matrix& 
        operator=(Matrix const &m)
        {
            p_ = m.p_;
            row_ = m.row_;
            col_ = m.col_;
        }
        
        Matrix& 
        operator=(Matrix &&m)
        {
            p_ = m.p_;
            row_ = m.row_;
            col_ = m.col_;
        }

        std::size_t 
        rows() const
        {
            return row_;
        }
        
        std::size_t 
        cols() const
        {
            return col_;
        }
        
        std::ostream&
        print(std::ostream &os) const
        {
            std::size_t index = 0;
            bool first = true;
            
            std::size_t width = digits_ante_dot() + precision_ + 1 + 2; // 1 for decimal, 1 for space on either side
            
            for_each(std::begin(p_), std::end(p_), 
                    [&index, this, &first, &os, &width](T const&it)
            {
                if (!(index % this->col_))
                {
                    os << (first ? "  [" : "\n   ");
                    
                }
                
                first = false;
                
                os << std::setw(width) << std::setprecision(this->precision_) << std::fixed << it;
                
                index++;
            });
            
            os << " ] " << std::endl;
            
            return os;
        }

        Matrix 
        operator+(Matrix const &m) const
        {
            if (this->col_ == m.col_ && this->row_ == m.row_)
            {
                Matrix dst(this->row_, this->row_);
                
                std::transform(std::begin(m.p_), std::end(m.p_), 
                        std::begin(this->p_),
                        std::begin(dst.p_),
                        std::plus<T>());
                
                return dst;
            } else throw std::domain_error("Matrix sizes mismatch");
        }
        
        Matrix 
        operator-(Matrix const &m) const
        {
            if (this->col_ == m.col_ && this->row_ == m.row_)
            {
                Matrix dst(this->row_, this->row_);
                
                std::transform(std::begin(m.p_), std::end(m.p_), 
                        std::begin(this->p_),
                        std::begin(dst.p_),
                        std::minus<T>());
                
                return dst;
            } else throw std::domain_error("Matrix sizes mismatch");
        }
        
        template<typename Y>
//        std::enable_if<std::is_arithmetic<Y>::value, Matrix>
        Matrix
        operator+(Y y) const
        {
            Matrix dst(*this);
           
            std::for_each(std::begin(dst.p_), std::end(dst.p_), 
                    [&y](T &c) { c += y; });
                    
            return dst;
        }
        
        template<typename Y>
        Matrix                          // enable if is not working here for some reason
        operator-(Y y) const
        {
            Matrix dst(*this);
            
            std::for_each(std::begin(dst.p_), std::end(dst.p_), 
                    [&y](T&c) { c = c - y; });
            
            return dst;
        }
        /*
        auto column_begin(size_t column) -> StridedIterator<T>
        {
            return StridedIterator<T>(p_, column, col_);
        }

        auto column_end() -> StridedIterator<T>
        {
            StridedIterator<T> it(p_, 0, 0);
            it.onepluslast();

            return it;
        }
*/
        value_type&
        operator() (size_type row, size_type column)
        {
            return p_[row * col_ + column];
        }

        value_type const&
        operator() (size_type row, size_type column) const
        {
            return p_[row * col_ + column];
        }


        // scalar multiplication
        template<typename Y>
        Matrix
        operator*(Y y) const
        {
            Matrix dst(*this);
            
            std::for_each(std::begin(dst.p_), std::end(dst.p_), 
                    [&y](T&c) { c = c * y; });
            
            return dst;   
        }

        // matrix multiplication
        Matrix
        operator*(Matrix const& B) const
        {
            Matrix const& A = *this;

            if (A.col_ == B.row_)
            {
                Matrix C(A.row_, B.col_);
                
                for (size_type a = 0; a < A.row_; ++a)
                    for (size_type b = 0; b < B.col_; ++b)
                        for (size_type y = 0; y < A.col_; ++y)
                            // for (size_type d = 0; d < B.row_; ++d)
                            {
                                C(a, b) += A(a, y) * B(y, b);
                                // std::cout << "Destination C("<<a << "," << b << ") = "
                                          // << "A("<<a << "," << y << ") * B("<<y << "," << b << ")" << std::endl;
                            }

                return C;   
            }
            else throw std::domain_error("Matrix sizes mismatch");
        }

    };
    
   
}   
template<typename T, typename U>
std::ostream&
operator<<(std::ostream &os, drakej::Matrix<T, U> const &m)
{
    return m.print(os);
}

#endif