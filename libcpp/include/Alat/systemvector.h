#ifndef __Alat_SystemVector_h
#define __Alat_SystemVector_h

#include  "map.h"
#include  "vectorinterface.h"
#include  "variablevectorinterface.h"

/*--------------------------------------------------------------------------*/
namespace Alat
{
  class IntVector;
  class StringDoubleVectorMap;
  class StringSet;

  class SystemVector : public Alat::Map<std::string, Alat::VariableVectorInterface*>, public Alat::VectorInterface
  {
public:
    typedef Alat::Map<std::string, Alat::VariableVectorInterface*>::const_iterator const_iterator;
    typedef Alat::Map<std::string, Alat::VariableVectorInterface*>::iterator iterator;

    ~SystemVector();
    SystemVector();
    SystemVector( const SystemVector& postprocessvector);
    SystemVector& operator=( const SystemVector& postprocessvector);
    std::string getName() const;
    SystemVector* clone() const;

    void set_size(const Alat::StringSet& variables);
    bool hasVector(std::string name) const;
    const Alat::VariableVectorInterface* getVector(std::string name) const;
    Alat::VariableVectorInterface* getVector(std::string name);
    int getNVars() const;
    Alat::StringSet getVariables() const;

    void zeros();
    double norm() const;
    void scale(const double& d);
    double scalarProduct(const Alat::VectorInterface* v) const;
    void equal(const Alat::VectorInterface* v);
    void equal(double d);
    void add(const double& d, const Alat::VectorInterface* v);
    std::ostream& writeAscii(std::ostream& os) const;
    void scalePerVariables(const Alat::StringDoubleVectorMap& scales);
    void scalePerVariablesInverse(const Alat::StringDoubleVectorMap& scales);
    void savehdf5(const std::string& filename, const Alat::Map<std::string, Alat::StringVector>& blocknames) const;
    void loadhdf5(const std::string& filename, const Alat::Map<std::string, Alat::StringVector>& blocknames);
  };
  std::ostream& operator<<(std::ostream& s, const SystemVector& v);
}

/*--------------------------------------------------------------------------*/

#endif
