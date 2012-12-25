
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

namespace drakej
{
    
    template<typename T, typename Enable = void>
    class Matrix;
    
    template<typename T, typename Enable = void>
    class StridedIterator;
    
    template<typename T>
    class StridedIterator<T, typename std::enable_if<std::is_arithmetic<T>::value>::type>
        : public std::iterator<std::random_access_iterator_tag, T>
    {
        using size_type = typename std::vector<T>::size_type;
        using difference_type = typename std::vector<T>::difference_type;

        std::vector<T>& vec_;
        size_type offset_;
        difference_type stride_;
        
        friend Matrix<T>;
        
        StridedIterator(std::vector<T>& vec, size_type start, difference_type increment)
        : vec_(vec), offset_(start), stride_(increment)
        {
        }

        StridedIterator<T>& onepluslast()
        {
            offset_ = vec_.size();
            return *this;
        }
        
        public:
        StridedIterator(StridedIterator& it)
        : vec_(it.vec_), offset_(it.offset_), stride_(it.stride_)
        {
        }
        
        StridedIterator(StridedIterator&& it)
        : vec_(it.vec_), offset_(it.offset_), stride_(it.stride_) 
        {
        }



        StridedIterator &operator++()
        {
            offset_ += stride_;

            return *this; 
        }

        StridedIterator &operator--()
        {
            offset_ -= stride_;

            return *this;
        }
        

        T operator*()
        {
            return vec_[offset_];
        }

        // any offset past the end is equal to any other offset past the end
        bool operator==(StridedIterator const& it) const
        {
            // this == it

            if (it.vec_ != vec_)
                return false; // not the same matrix

            // check for end iterator in it
            if (it.offset_ >= vec_.size())
            {
                return offset_ >= vec_.size();
            }

            // check to see if we are an end iterator
            if (offset_ >= vec_.size())
            {
                return it.offset_ >= vec_.size();
            }

            // otherwise, just compare them
            return it.offset_ == offset_;
        }

        bool operator!=(StridedIterator const& it) const
        {
            return !(operator==(it));
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
                std::initializer_list<T> l) :  p_(l), row_(row), col_(col)
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
                
                os << std::setw(width) << std::setprecision(this->precision_) << std::fixed << it;
                
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
        
        auto column_begin(size_t column = 0) -> StridedIterator<T>
        {
            return StridedIterator<T>(p_, column, col_);
        }

        auto column_end(size_t column = 0) -> StridedIterator<T>
        {
            StridedIterator<T> it(p_, 0, 0);
            it.onepluslast();

            return it;
        }
    };
    
   
}   
template<typename T, typename U>
std::ostream &operator<<(std::ostream &os, drakej::Matrix<T, U> const &m)
{
    return m.print(os);
}
