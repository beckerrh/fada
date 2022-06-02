#ifndef __Alat_Pair_h
#define __Alat_Pair_h

#include  <iostream>
#include  <iterator>
#include  <utility>

/*--------------------------------------------------------------------------*/

namespace Alat
{
  template<class S, class T>
  class Pair : public std::pair<S, T>
  {
public:
    ~Pair();
    Pair();
    Pair(S s, T t);
    Pair( const Pair& pair);
    Pair& operator=( const Pair& pair);
    std::string getName() const;
    Pair* clone() const;
    friend std::ostream& operator<<(std::ostream& os, const Pair<S, T>& A)
    {
      os << " (" << A.first << " , " << A.second <<") ";
      return os;
    }
  };
  typedef Alat::Pair<int, int> IntPair;
  typedef Alat::Pair<std::string, std::string> StringPair;


  /*--------------------------------------------------------------------------*/

  template<class S, class T>
  Pair<S, T>::~Pair<S, T>()
  {}

  /*--------------------------------------------------------------------------*/

  template<class S, class T>
  Pair<S, T>::Pair() : std::pair<S, T>()
  {}

  /*--------------------------------------------------------------------------*/

  template<class S, class T>
  Pair<S, T>::Pair(S s, T t) : std::pair<S, T>(s,t)
  {}

  /*--------------------------------------------------------------------------*/

  template<class S, class T>
  Pair<S, T>::Pair( const Pair<S, T>& pair) : std::pair<S, T>(pair)
  {}

  /*--------------------------------------------------------------------------*/

  template<class S, class T>
  Pair<S, T>& Pair<S, T>::operator=( const Pair<S, T>& pair)
  {
    std::pair<S, T>::operator=(pair);
    return *this;
  }

  /*--------------------------------------------------------------------------*/

  template<class S, class T>
  std::string Pair<S, T>::getName() const
  {
    return "Pair";
  }

  /*--------------------------------------------------------------------------*/

  template<class S, class T>
  Pair<S, T>* Pair<S, T>::clone() const
  {
    return new Pair(*this);
  }
  
  template<class S, class T>
  Pair<S,T> makePair (S x, T y)
  {
    return ( Pair<S,T>(x,y) );
  }
}

/*--------------------------------------------------------------------------*/

#endif
