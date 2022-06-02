#ifndef __Alat_UniqueMap_h
#define __Alat_UniqueMap_h

#include  <cassert>
#include  <iostream>
#include  <map>
#include  <string>

/*---------------------------------------------------------*/

namespace Alat
{
  template<typename KEY, typename VAL>
  class UniqueMap : public std::map<KEY, VAL>
  {
public:
    typedef typename std::map<KEY, VAL>::iterator iterator;
    typedef typename std::map<KEY, VAL>::const_iterator const_iterator;

public:
    ~UniqueMap();
    UniqueMap();
    UniqueMap(const UniqueMap& M);
    UniqueMap& operator=(const UniqueMap& M);
    std::string getName() const;
    VAL& operator[](const KEY& key);
    const VAL& operator[](const KEY& key) const;
    bool hasKey(const KEY& key) const;
    void write(std::ostream& s, std::string datatype) const;
    void read(std::istream& s);

    friend std::ostream& operator<<(std::ostream& s, const UniqueMap<KEY, VAL>& A)
    {
      for(const_iterator p = A.begin(); p != A.end(); p++)
      {
        s << p->first << " -> " << p->second << std::endl;
      }
      return s;
    }
  };

  /*---------------------------------------------------------*/

  template<typename KEY, typename VAL>
  UniqueMap<KEY, VAL>::~UniqueMap<KEY, VAL>( ) {}

  /*---------------------------------------------------------*/

  template<typename KEY, typename VAL>
  UniqueMap<KEY, VAL>::UniqueMap() : std::map<KEY, VAL>() {}

  /*---------------------------------------------------------*/

  template<typename KEY, typename VAL>
  UniqueMap<KEY, VAL>::UniqueMap(const UniqueMap& M) : std::map<KEY, VAL>(M)
  {
    // assert(0);
  }

  /*---------------------------------------------------------*/

  template<typename KEY, typename VAL>
  UniqueMap<KEY, VAL>& UniqueMap<KEY, VAL>::operator=(const UniqueMap<KEY, VAL>& M)
  {
    std::map<KEY, VAL>::operator=(M);
    return *this;
  }

  /*---------------------------------------------------------*/

  template<typename KEY, typename VAL>
  std::string UniqueMap<KEY, VAL>::getName() const
  {
    return "Alat::UniqueMap";
  }

  /*---------------------------------------------------------*/

  template<typename KEY, typename VAL>
  VAL& UniqueMap<KEY, VAL>::operator[](const KEY& key)
  {
    const_iterator p = this->find(key);
    if( p != this->end() )
    {
      std::cerr << "Alat::UniqueMap : key already used \"" << key << "\"\n" << *this << "\n I have:\n";
      write(std::cerr, "ascii");
      assert(0);
      exit(6);
    }
    return std::map<KEY, VAL>::operator[](key);
  }

  /*---------------------------------------------------------*/

  template<typename KEY, typename VAL>
  const VAL& UniqueMap<KEY, VAL>::operator[](const KEY& key) const
  {
    const_iterator p = this->find(key);
    if( p == this->end() )
    {
      std::cerr << "Alat::UniqueMap : not found value of key \"" << key << "\"\n" << *this << "\n I have:\n";
      write(std::cerr, "ascii");
      assert(0);
      exit(6);
    }
    return p->second;
  }

  /*---------------------------------------------------------*/

  template<typename KEY, typename VAL>
  bool UniqueMap<KEY, VAL>::hasKey(const KEY& key) const
  {
    const_iterator p = this->find(key);
    bool b = ( p != this->end() );
    return b;
  }

  /*---------------------------------------------------------*/

  template<typename KEY, typename VAL>
  void UniqueMap<KEY, VAL>::write(std::ostream& s, std::string datatype) const
  {
    s << this->size() << " " << datatype << "\n";
    for(const_iterator p = this->begin(); p != this->end(); p++)
    {
      s << p->first << " " << p->second << " ";
    }
  }

  /*---------------------------------------------------------*/

  template<typename KEY, typename VAL>
  void UniqueMap<KEY, VAL>::read(std::istream& s)
  {
    int n;
    std::string datatype;
    s >> n >> datatype;
    KEY k;
    VAL v;
    for(int i = 0; i < n; i++)
    {
      s >> k >> v;
      ( *this )[k] = v;
    }
  }
}

/*---------------------------------------------------------*/

#endif
