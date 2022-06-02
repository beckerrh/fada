#ifndef __Alat_Matrix_h
#define __Alat_Matrix_h

#include  "vector.h"

/*-----------------------------------------------------------------*/

namespace Alat
{
  /*!
     This class is a
     simple dense matrix class for use with Alat::Vector<T>.
     see also the template
     Alat::Vector
     size_t is unsigned int
   */
  template<class T>
  class Matrix : public Vector<T>
  {
protected:

    //! _n is the number of lines and _m the number of columns
    int _n, _m;

public:
    typedef typename  Vector<T>::const_iterator const_iterator;
    typedef typename  Vector<T>::iterator iterator;

    ~Matrix<T>();
    Matrix<T>();
    Matrix<T>(const Matrix<T>&A);
    Matrix<T>(size_t n);
    // Matrix<T>(size_t n, const T &d);
    Matrix<T>(size_t n, size_t m);
    // Matrix<T>(size_t n, size_t m, const T &d);
    Matrix<T>& operator=(const Matrix<T>& A);

    void set_size(size_t n, size_t m);
    void set_size(size_t n);
    size_t n() const
    {
      return _n;
    }

    size_t m() const
    {
      return _m;
    }

    const T& operator()(int i, int j) const
    {
      return ( *this )[j+_m*i];
    }

    T& operator()(int i, int j)
    {
      return ( *this )[j+_m*i];
    }

    const T& value(int i, int j) const
    {
      return ( *this )[j+_m*i];
    }

    T& value(int i, int j)
    {
      return ( *this )[j+_m*i];
    }

    // const_iterator rowstart(int i) const
    // {
    //   return std::vector<T>::begin()+_m*i;
    // }

    // iterator rowstart(int i)
    // {
    //   return std::vector<T>::begin()+_m*i;
    // }
    //
    // const_iterator rowend(int i) const
    // {
    //   return std::vector<T>::begin()+_m*i+_m;
    // }

    void transpose();
    // void fillWithTransposed(Matrix<T>& AT) const;
  };
  
  
  
  /*-----------------------------------------------------------------*/

  template<class T>
  Matrix<T>::~Matrix<T>( ) {}

  /*-----------------------------------------------------------------*/

  template<class T>
  Matrix<T>::Matrix() : Vector<T>(), _n(0), _m(0) {}

  /*-----------------------------------------------------------------*/

  template<class T>
  Matrix<T>::Matrix(const Matrix<T>& A) : Vector<T>(A), _n( A.n() ), _m( A.m() )
  {
    *this = A;
  }

  /*-----------------------------------------------------------------*/

  template<class T>
  Matrix<T>::Matrix(size_t n)        : Vector<T>(n*n), _n(n), _m(n) {}

  /*-----------------------------------------------------------------*/

  // template<class T>
  // Matrix<T>::Matrix(size_t n, const T& d)        : Vector<T>(n*n, d), _n(n), _m(n) {}

  /*-----------------------------------------------------------------*/

  template<class T>
  Matrix<T>::Matrix(size_t n, size_t m) : Vector<T>(n*m), _n(n), _m(m) {}

  /*-----------------------------------------------------------------*/

  // template<class T>
  // Matrix<T>::Matrix(size_t n, size_t m, const T& d) : Vector<T>(n*m, d), _n(n), _m(m) {}

  /*-----------------------------------------------------------------*/

  template<class T>
  Matrix<T>& Matrix<T>::operator=(const Matrix<T>& A)
  {
    _n = A.n();
    _m = A.m();
    Vector<T>::operator=(A);
    return *this;
  }

  /*-----------------------------------------------------------------*/

  template<class T>
  void Matrix<T>::set_size(size_t n, size_t m)
  {
    _n = n;
    _m = m;
    Vector<T>::set_size(n*m);
  }

  /*-----------------------------------------------------------------*/

  template<class T>
  void Matrix<T>::set_size(size_t n)
  {
    Matrix<T>::set_size(n, n);
  }

  /*-----------------------------------------------------------------*/

  template<class T>
  void Matrix<T>::transpose()
  {
    assert( n() == m() );
    Matrix<T> B( n() );
    B = *this;
    for(int i = 0; i < m(); i++)
    {
      for(int j = 0; j < n(); j++)
      {
        ( *this )( i, j ) = B(j, i);
      }
    }
  }

  /*-----------------------------------------------------------------*/

  // template<class T>
  // void Matrix<T>::fillWithTransposed(Matrix<T>& AT) const
  // {
  //   assert( n() == AT.m() );
  //   assert( m() == AT.n() );
  //   for(int i = 0; i < n(); i++)
  //   {
  //     for(int j = 0; j < m(); j++)
  //     {
  //       AT( j, i ) = ( *this )( i, j );
  //     }
  //   }
  // }

}

#endif
