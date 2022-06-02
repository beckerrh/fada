#ifndef __Alat_Vector_h
#define __Alat_Vector_h

#include  <cassert>
#include  <iostream>
#include  <iterator>
#include  <string>
#include  <vector>

/*--------------------------------------------------------------------------*/

namespace Alat
{
  template<class T>
  class Vector : public std::vector<T>
  {
public:
    typedef typename std::vector<T>::iterator iterator;
    typedef typename std::vector<T>::const_iterator const_iterator;

    ~Vector<T>( );
    Vector<T>( );
    Vector<T>( const Vector<T>&vector );
    Vector<T>( size_t n );
    Vector<T>( size_t n, const T d );
    Vector<T>( std::istream_iterator<T> begin, std::istream_iterator<T> end ) : std::vector<T>(begin, end) {}
    Vector<T>( const T* begin, const T* end ) : std::vector<T>(begin, end) {}
    Vector<T>& operator=( const Vector<T>& vector);
    std::string getName() const;

    void set_size(size_t n);
    void set_size(size_t n, const T);
    void equal(const T);
    void equal(const Vector<T>&);

    std::ostream& writeBin(std::ostream& out) const;
    std::istream& readBin (std::istream& in);
    std::ostream& saveFada(std::ostream& out, const std::string datatype = "binary") const;
    std::istream& loadFada(std::istream& in);
  };
  template<class T>
  std::ostream& operator<<(std::ostream& s, const Vector<T>& A);
  template<class T>
  std::istream& operator>>(std::istream& s, Vector<T>& A);

/*--------------------------------------------------------------------------*/

  template<class T>
  Vector<T>::~Vector<T>( )
  {}

/*--------------------------------------------------------------------------*/

  template<class T>
  Vector<T>::Vector( ) : std::vector<T>()
  {}

/*--------------------------------------------------------------------------*/

  template<class T>
  Vector<T>::Vector( const Vector<T>& vector ) : std::vector<T>(vector)
  {}

/*--------------------------------------------------------------------------*/

  template<class T>
  Vector<T>::Vector(size_t n) : std::vector<T>(n)   {}

/*--------------------------------------------------------------------------*/

  template<class T>
  Vector<T>::Vector( size_t n, const T d ) : std::vector<T>(n, d) {}

/*--------------------------------------------------------------------------*/

  template<class T>
  Vector<T>& Vector<T>::operator=( const Vector& vector)
  {
    std::vector<T>::operator=(vector);
    return *this;
  }

/*--------------------------------------------------------------------------*/

  template<class T>
  void Vector<T>::set_size(size_t n)
  {
    std::vector<T>::reserve(n);
    std::vector<T>::resize(n);
  }

/*--------------------------------------------------------------------------*/

  template<class T>
  void Vector<T>::set_size(size_t n, const T d)
  {
    std::vector<T>::reserve(n);
    std::vector<T>::resize(n, d);
  }

/*----------------------------------------------------------*/

  template<class T>
  std::string Vector<T>::getName() const
  {
    return "Vector";
  }

/*----------------------------------------------------------*/

  template<class T>
  inline void Vector<T>::equal (const T d)
  {
    iterator first  = std::vector<T>::begin();
    const_iterator last   = std::vector<T>::end();

    while(first != last)
    {
      ( *first++ ) = d;
    }
  }

/*----------------------------------------------------------*/

  template<class T>
  inline void Vector<T>::equal(const Vector<T>& v)
  {
    assert( std::vector<T>::size() == v.std::vector<T>::size() );
    iterator first  = std::vector<T>::begin();
    const_iterator last   = std::vector<T>::end();
    const_iterator vfirst = v.std::vector<T>::begin();

    while( first != last)
    {
      *first++ = *vfirst++;
    }
  }

/*----------------------------------------------------------*/

  template<class T>
  std::ostream& Vector<T>::writeBin(std::ostream& out) const
  {
    out.write( reinterpret_cast<const char*>( &( *this )[0] ), this->size()*sizeof( T ) );
    return out;
  }

/*----------------------------------------------------------*/

  template<class T>
  std::istream& Vector<T>::readBin(std::istream& in)
  {
    in.read( reinterpret_cast<char*>( &( *this )[0] ), this->size()*sizeof( T ) );
    return in;
  }

/*----------------------------------------------------------*/

  template<class T>
  std::ostream& Vector<T>::saveFada(std::ostream& out, const std::string datatype) const
  {
    // std::cerr <<  Vector<T>::size() << " datatype "  << datatype << " " << std::endl;
    out<<Vector<T>::size()<<" "<<datatype<<" "<<std::endl;
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

  template<class T>
  std::istream& Vector<T>::loadFada(std::istream& in)
  {
    int n;
    std::string datatype;
    in>>n>>datatype;
    set_size(n);
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
      std::cerr<<"*** Vector<T>::read() : wrong datatype \""<<datatype<<"\"\n";
      assert(0);
    }
    return in;
  }

/*----------------------------------------------------------*/

  template<class T>
  std::ostream& operator<<(std::ostream& s, const Vector<T>& A)
  {
    // s << "size= " << A.size() << "\n";
    copy( A.begin(), A.end(), std::ostream_iterator<T>(s, " ") );
    return s;
  }

/*----------------------------------------------------------*/

  template<class T>
  std::istream& operator>>(std::istream& s, Vector<T>& A)
  {
    typename Vector<T>::iterator p = A.begin();
    while( p != A.end() )
    {
      s>>*p++;
    }
    return s;
  }
}

/*--------------------------------------------------------------------------*/

#endif
