#ifndef __Alat_FixArray_h
#define __Alat_FixArray_h

#include  <iostream>
#include  <iterator>
#include  <string>
#include  <cassert>

/*--------------------------------------------------------------------------*/

namespace Alat
{
  template<int N, class T>
  class FixArray
  {
public:
    typedef  T*        iterator;
    typedef  const T*  const_iterator;

private:
    T val[N];

public:
    ~FixArray<N, T>();
    FixArray<N, T>();
    FixArray<N, T>(const T &d);
    FixArray<N, T>( const FixArray<N, T>&fixarray);
    FixArray<N, T>& operator=( const FixArray<N, T>& fixarray);
    FixArray<N, T>& operator=( const T& t);
    std::string getName() const;
    const T* begin() const;
    const T* end() const;
    T* begin();
    T* end();
    size_t   size()            const;
    const T& operator[](int i) const;
    T&  operator[](int i);
    FixArray<N, T>& operator+=(const FixArray<N, T>& v);
    bool operator<(const FixArray<N, T>& v) const;
    bool operator!=(const FixArray<N, T>& v) const;
    bool operator==(const FixArray<N, T>& v) const;

    friend std::ostream& operator<<(std::ostream& s, const FixArray<N, T>& A)
    {
      copy( A.begin(), A.end(), std::ostream_iterator<T>(s, " ") );
      return s;
    }

    friend std::istream& operator>>(std::istream& s, FixArray<N, T>& A)
    {
      typename FixArray<N, T>::iterator p = A.begin();
      while( p != A.end() )
      {
        s >> *p++;
      }
      return s;
    }
    std::ostream& writeBin(std::ostream& out) const;
    std::istream& readBin (std::istream& in);
    std::ostream& write(std::ostream& out, const std::string datatype = "binary") const;
    std::istream& read(std::istream& in);
  };
  

/*----------------------------------------------------------*/

  template<int N, class T>
  std::ostream& FixArray<N, T>::writeBin(std::ostream& out) const
  {
    out.write( reinterpret_cast<const char*>( &( *this )[0] ), N*sizeof( T ) );
    return out;
  }

/*----------------------------------------------------------*/

  template<int N, class T>
  std::istream& FixArray<N, T>::readBin(std::istream& in)
  {
    in.read( reinterpret_cast<char*>( &( *this )[0] ), N*sizeof( T ) );
    return in;
  }

/*----------------------------------------------------------*/

  template<int N, class T>
  std::ostream& FixArray<N, T>::write(std::ostream& out, const std::string datatype) const
  {
    out<<datatype<<" "<<std::endl;
    if(datatype == "ascii")
    {
      out<<*this;
    }
    else
    {
      writeBin(out);
    }
    return out;
  }

/*----------------------------------------------------------*/

  template<int N, class T>
  std::istream& FixArray<N, T>::read(std::istream& in)
  {
    std::string datatype;
    in>>datatype;
    if(datatype == "ascii")
    {
      in>>*this;
    }
    else if(datatype == "binary")
    {
      std::string str;
      while(str == "")
      {
        getline(in, str);
      }
      readBin(in);
    }
    else
    {
      std::cerr<<"*** FixArray<N, T>::read() : wrong datatype \""<<datatype<<"\"\n";
      assert(0);
    }
    return in;
  }
  
  
  
  /*--------------------------------------------------------------------------*/

  template<int N, class T>
  FixArray<N, T>::~FixArray<N, T>( )
  {}

  /*--------------------------------------------------------------------------*/

  template<int N, class T>
  FixArray<N, T>::FixArray( )
  {}

  /*--------------------------------------------------------------------------*/

  template<int N, class T>
  FixArray<N, T>::FixArray( const FixArray<N, T>& fixarray )
  {
    std::copy( fixarray.begin(), fixarray.end(), begin() );
  }

  /*--------------------------------------------------------------------------*/

  template<int N, class T>
  FixArray<N, T>::FixArray( const T& d )
  {
    *this = d;
  }

  /*--------------------------------------------------------------------------*/

  template<int N, class T>
  FixArray<N, T>& FixArray<N, T>::operator=( const FixArray<N, T>& fixarray)
  {
    std::copy( fixarray.begin(), fixarray.end(), begin() );
    return *this;
  }

  /*--------------------------------------------------------------------------*/

  template<int N, class T>
  FixArray<N, T>& FixArray<N, T>::operator=( const T& t)
  {
    iterator p( begin() );
    while( p < end() )
    {
      *p++ = t;
    }
    return *this;
  }

  /*--------------------------------------------------------------------------*/

  template<int N, class T>
  std::string FixArray<N, T>::getName() const
  {
    return "FixArray";
  }

  /*--------------------------------------------------------------------------*/

  template<int N, class T>
  const T* FixArray<N, T>::begin() const
  {
    return &( val[0] );
  }

  /*--------------------------------------------------------------------------*/

  template<int N, class T>
  const T* FixArray<N, T>::end() const
  {
    return &( val[0] )+N;
  }

  /*--------------------------------------------------------------------------*/

  template<int N, class T>
  T* FixArray<N, T>::begin()
  {
    return &( val[0] );
  }

  /*--------------------------------------------------------------------------*/

  template<int N, class T>
  T* FixArray<N, T>::end()
  {
    return &( val[0] )+N;
  }

  /*--------------------------------------------------------------------------*/

  template<int N, class T>
  size_t FixArray<N, T>::size() const
  {
    return N;
  }

  /*--------------------------------------------------------------------------*/

  template<int N, class T>
  const T& FixArray<N, T>::operator[](int i) const
  {
    return val[i];
  }

  /*--------------------------------------------------------------------------*/

  template<int N, class T>
  T& FixArray<N, T>::operator[](int i)
  {
    return val[i];
  }

  /*--------------------------------------------------------------------------*/

  template<int N, class T>
  FixArray<N, T>& FixArray<N, T>::operator+=(const FixArray<N, T>& v)
  {
    iterator p( begin() );
    const_iterator q( v.begin() );
    while( p < end() )
    {
      *p++ += *q++;
    }
    return *this;
  }

  /*--------------------------------------------------------------------------*/

  template<int N, class T>
  bool FixArray<N, T>::operator<(const FixArray<N, T>& v) const
  {
    const_iterator p( begin() );
    const_iterator q( v.begin() );
    while( p < end() )
    {
      if(*p < *q)
      {
        return true;
      }
      if(*q < *p)
      {
        return false;
      }
      p++;
      q++;
    }
    return false;
  }

  /*--------------------------------------------------------------------------*/

  template<int N, class T>
  bool FixArray<N, T>::operator==(const FixArray<N, T>& v) const
  {
    return not operator!=(v);
  }

  template<int N, class T>
  bool FixArray<N, T>::operator!=(const FixArray<N, T>& v) const
  {
    const_iterator p( begin() );
    const_iterator q( v.begin() );
    while( p < end() )
    {
      if(*p != *q)
      {
        return true;
      }
      p++;
      q++;
    }
    return false;
  }

}

/*--------------------------------------------------------------------------*/

#endif
