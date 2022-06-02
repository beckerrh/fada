#ifndef __Alat_Ghost_h
#define __Alat_Ghost_h

#include  "stringset.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class Ghost
  {
private:
    int _level;
    std::string _name, _description;
    Alat::StringSet _variables;

public:
    virtual ~Ghost();
    Ghost();
    Ghost( const Ghost& ghost);
    Ghost(const std::string name, int level = 0);
    Ghost(const std::string name, const std::string description,int level = 0);
    Ghost(const std::string name, const Alat::StringSet& variables, int level = 0);
    Ghost& operator=( const Ghost& ghost);

    int getLevel() const;
    void setLevel(int level);
    void setVariables(const Alat::StringSet& variables);
    const Alat::StringSet& getVariables() const;
    void setName(const std::string& name);
    const std::string& getDescription() const;
    void setDescription(const std::string& description);
    const std::string& getName() const;
    virtual std::string getClassName() const;
    bool operator==(const Ghost& v) const;
    bool operator<(const Ghost& v) const;
  };
  std::ostream& operator<<(std::ostream& os, const Ghost& g);
}

/*--------------------------------------------------------------------------*/

#endif
