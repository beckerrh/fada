#ifndef __Alat_GhostVector_h
#define __Alat_GhostVector_h

#include  "ghost.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class GhostVector : public Ghost
  {
  private:
    std::string _type;
    
public:
    ~GhostVector();
    GhostVector();
    GhostVector( const GhostVector& ghostvector);
    GhostVector(const std::string name, const std::string& type, int level=0);
    GhostVector(const std::string name, const std::string& type, const Alat::StringSet& variables, int level=0);
    GhostVector& operator=( const GhostVector& ghostvector);
    std::string getClassName() const;
    void setType(const std::string& type);
    const std::string& getType() const;
    // bool operator==(const GhostVector& v) const;
    // bool operator<(const GhostVector& v) const;
  };
  std::ostream& operator<<(std::ostream& os, const GhostVector& g);
}

/*--------------------------------------------------------------------------*/

#endif
