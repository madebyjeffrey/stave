
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

namespace drakej
{
    
    template<typename T, typename Enable = void>
    class Matrix;
    
    template<typename T, typename Enable = void>
    class StridedIterator;
    
    template<typename T>
    class StridedIterator<T, typename std::enable_if<std::is_arithmetic<T>::value::type>>
        : public std::iterator<std::random_access_iterator_tag, T>
    {
        std::vector<T> &vec_;
        typename std::vector<T>::iterator it_;
        std::ptrdiff_t stride_;
        
        friend Matrix<T>;
        
        StridedIterator(std::vector<T> &vec, 
                std::ptrdiff_t start,
                std::ptrdiff_t increment,
                std::ptrdiff_t count) 
        : vec_(vec), it_(std::begin(vec)), stride_(increment)
        {
            std::advance(it_, start);
        }
        
        public:
        StridedIterator(StridedIterator &it)
        : vec_(it.vec_), it_(it.it_), stride_(it.stride_)
        {
        }
        
        StridedIterator(StridedIterator &&it)
        : vec_(it.vec_), it_(it.it_), stride_(it.stride_)
        {
        }
    };
 
    
    template<typename T>
    class Matrix<T, typename std::enable_if<std::is_arithmetic<T>::value>::type>
    {
        std::vector<T> p_;
        std::size_t row_, col_;
        
        std::size_t precision_ = 3; // precision for printing
        
        // Max of digits before the decimal in the matrix
        std::size_t digits_ante_dot() const 
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
            
        Matrix(std::size_t row, std::size_t col) : p_(row * col), row_(row), col_(col)
        {
        }
        
        Matrix(std::size_t row, std::size_t col, 
                std::initializer_list<T> l) : row_(row), col_(col), p_(l)
        {
//            static_assert(row * col == l.size(), "Initializer must be equal to size of matrix");
            if (row * col != l.size())
                std::cerr << "Warning: Incorrect size.";
        }
        
        Matrix(Matrix const &m) : p_(m.p_), row_(m.row_), col_(m.col_)
        {
        }
        
        Matrix(Matrix &&m) : p_(m.p_), row_(m.row_), col_(m.col_)
        {
        }
        
        Matrix& operator=(Matrix const &m)
        {
            p_ = m.p_;
            row_ = m.row_;
            col_ = m.col_;
        }
        
        Matrix& operator=(Matrix &&m)
        {
            p_ = m.p_;
            row_ = m.row_;
            col_ = m.col_;
        }
        
        std::size_t rows() const
        {
            return row_;
        }
        
        std::size_t cols() const
        {
            return col_;
        }
        
        std::ostream &print(std::ostream &os) const
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
                
                os << std::setw(width) << std::setprecision(precision_) << std::fixed << it;
                
                index++;
            });
            
            os << " ] " << std::endl;
            
            return os;
        }
        
        Matrix operator+(Matrix const &m) const
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
        
        Matrix operator-(Matrix const &m) const
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
        
        auto column_begin() -> StridedIterator<T>
        {
            return StridedIterator<T>(p_, 0, col_, 0);
        }
    };
    
   
}   
template<typename T, typename U>
std::ostream &operator<<(std::ostream &os, drakej::Matrix<T, U> const &m)
{
    return m.print(os);
}
