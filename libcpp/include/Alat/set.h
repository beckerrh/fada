#ifndef __Alat_Set_h
#define __Alat_Set_h

#include  <iostream>
#include  <iterator>
#include  <set>

/*----------------------------------------------------------*/

namespace Alat
{
  template<class T>
  class Set : public std::set<T>
  {
public:
    typedef typename std::set<T>::iterator iterator;
    typedef typename std::set<T>::const_iterator const_iterator;

    ~Set();
    Set();
    Set(const T& v);
    Set(const Set& v);
    Set(const std::set<T>& v);
    template <class InputIterator>
    Set(InputIterator first, InputIterator last);
    Set& operator=(const Set& v);
    friend std::ostream& operator<<(std::ostream& s, const Set<T>& A)
    {
      if( !A.size() )
      {
        return s;
      }
      std::ostream_iterator<T>  os(s, " ");
      copy(A.begin(), A.end(), os);
      return s;
    }
  };
  typedef Alat::Set<int> IntSet;

  /*----------------------------------------------------------*/
  template<class T>
  Set<T>::~Set<T>( )  {}

  /*----------------------------------------------------------*/
  template<class T>
  Set<T>::Set() : std::set<T>()    {}

  /*----------------------------------------------------------*/
  template<class T>
  Set<T>::Set(const T& v) : std::set<T>()    
  {
    std::set<T>::insert(v);
  }

  /*----------------------------------------------------------*/
  template<class T>
  Set<T>::Set(const Set& v) : std::set<T>(v)
  {
    *this = v;
  }

  /*----------------------------------------------------------*/
  template<class T>
  Set<T>::Set(const std::set<T>& v) : std::set<T>(v)   {}

  /*----------------------------------------------------------*/
  template<class T>
  template <class InputIterator>
  Set<T>::Set(InputIterator first, InputIterator last) : std::set<T>(first, last)   {}

  /*----------------------------------------------------------*/
  template<class T>
  Set<T>& Set<T>::operator=(const Set<T>& v)
  {
    std::set<T>::operator=(v);
    return *this;
  }
}

#endif
