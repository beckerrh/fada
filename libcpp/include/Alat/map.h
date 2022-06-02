#ifndef __Alat_Map_h
#define __Alat_Map_h

#include  "set.h"
#include  <cassert>
#include  <iostream>
#include  <map>
#include  <stdlib.h>
#include  <string>

/*---------------------------------------------------------*/

namespace Alat
{
  template<typename KEY, typename VAL>
  class Map : public std::map<KEY, VAL>
  {
public:
    typedef typename std::map<KEY, VAL>::iterator iterator;
    typedef typename std::map<KEY, VAL>::const_iterator const_iterator;

public:
    ~Map();
    Map();
    Map(const Map& M);
    Map& operator=(const Map& M);
    std::string getName() const;
    VAL& operator[](const KEY& key);
    const VAL& operator[](const KEY& key) const;
    bool hasKey(const KEY& key) const;
    void write(std::ostream& s, std::string datatype) const;
    void read(std::istream& s);
    Set<KEY> keys() const;
  };

  template<typename KEY, typename VAL>
  std::ostream& operator<<(std::ostream& s, const Map<KEY, VAL>& A)
  {
    for(typename Map<KEY, VAL>::const_iterator p = A.begin(); p != A.end(); p++)
    {
      s << p->first << " -> " << p->second << std::endl;
    }
    return s;
  }
  typedef Alat::Map<int,int> IntMap;
  typedef Alat::Map<std::string,std::string> StringMap;
  typedef Alat::Map<std::string,int> StringIntMap;

  /*---------------------------------------------------------*/

  template<typename KEY, typename VAL>
  Map<KEY, VAL>::~Map<KEY, VAL>( ) {}

  /*---------------------------------------------------------*/

  template<typename KEY, typename VAL>
  Map<KEY, VAL>::Map() : std::map<KEY, VAL>() {}

  /*---------------------------------------------------------*/

  template<typename KEY, typename VAL>
  Map<KEY, VAL>::Map(const Map& M) : std::map<KEY, VAL>(M)
  {
    // assert(0);
  }

  /*---------------------------------------------------------*/

  template<typename KEY, typename VAL>
  Map<KEY, VAL>& Map<KEY, VAL>::operator=(const Map<KEY, VAL>& M)
  {
    std::map<KEY, VAL>::operator=(M);
    return *this;
  }

  /*---------------------------------------------------------*/

  template<typename KEY, typename VAL>
  std::string Map<KEY, VAL>::getName() const
  {
    return "Alat::Map";
  }

  /*---------------------------------------------------------*/

  template<typename KEY, typename VAL>
  VAL& Map<KEY, VAL>::operator[](const KEY& key)
  {
    return std::map<KEY, VAL>::operator[](key);
  }

  /*---------------------------------------------------------*/

  template<typename KEY, typename VAL>
  const VAL& Map<KEY, VAL>::operator[](const KEY& key) const
  {
    const_iterator p = this->find(key);
    if( p == this->end() )
    {
      std::cerr << "Alat::Map : not found value of key \"" << key << "\"\n" << *this << "\n I have:\n";
      write(std::cerr, "ascii");
      assert(0);
      exit(6);
    }
    return p->second;
  }

  /*---------------------------------------------------------*/

  template<typename KEY, typename VAL>
  bool Map<KEY, VAL>::hasKey(const KEY& key) const
  {
    const_iterator p = this->find(key);
    bool b = ( p != this->end() );
    return b;
  }

  /*---------------------------------------------------------*/

  template<typename KEY, typename VAL>
  void Map<KEY, VAL>::write(std::ostream& s, std::string datatype) const
  {
    s << this->size() << " " << datatype << "\n";
    for(const_iterator p = this->begin(); p != this->end(); p++)
    {
      s << p->first << " " << p->second << " ";
    }
  }

  /*---------------------------------------------------------*/

  template<typename KEY, typename VAL>
  void Map<KEY, VAL>::read(std::istream& s)
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

  /*---------------------------------------------------------*/

  template<typename KEY, typename VAL>
  Set<KEY> Map<KEY, VAL>::keys() const
  {
    Set<KEY> keys;
    for(const_iterator p = this->begin(); p != this->end(); p++)
    {
      // keys.push_back(p->first);
      keys.insert(p->first);
    }
    return keys;
  }
}

/*---------------------------------------------------------*/

#endif
