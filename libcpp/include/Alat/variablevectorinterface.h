#ifndef __Alat_VariableVectorInterface_h
#define __Alat_VariableVectorInterface_h

#include  "interfacebase.h"

/*--------------------------------------------------------------------------*/
namespace Alat
{
  class IntVector;
  class DoubleVector;
  class StringVector;
  class AssembleVector;
}
namespace Alat
{
  class VariableVectorInterface : public virtual Alat::InterfaceBase
  {
protected:
    std::string getInterfaceName() const;

public:
    ~VariableVectorInterface();
    VariableVectorInterface();
    VariableVectorInterface( const VariableVectorInterface& vectorinterface);
    VariableVectorInterface& operator=( const VariableVectorInterface& vectorinterface);
    virtual VariableVectorInterface* clone() const;

    virtual void zeros() = 0;
    virtual double scalarProduct(const VariableVectorInterface* v) const = 0;
    virtual void equal(const Alat::VariableVectorInterface* v) = 0;
    virtual void equal(double d) = 0;
    virtual void add(const double& d, const VariableVectorInterface* v) = 0;
    virtual void scale(double s) = 0;
    virtual void scale(const Alat::DoubleVector& scale) = 0;
    virtual void scaleinv(const Alat::DoubleVector& scale) = 0;
    virtual void scaleIntVector(const Alat::IntVector& d);
    virtual void set(const Alat::AssembleVector& vloc, const Alat::IntVector& indices);
    virtual void assemble(const Alat::AssembleVector& vloc, const Alat::IntVector& indices, const Alat::DoubleVector& scale, double d=1.0);
    virtual void extract(Alat::AssembleVector& uloc, const Alat::IntVector& indices, const Alat::DoubleVector& scale) const;
    virtual void setSingleVectorFromDirectSolver(int offset, const Alat::DoubleVector& u);
    virtual void addVectorRhsForDirectSolver(int offset, Alat::DoubleVector& f) const;
    virtual void set_size(int ncomp, int n)=0;
    virtual int getNComponents() const;
    virtual std::ostream& writeAscii(std::ostream& os) const;
    virtual int n() const=0;
    virtual double norm () const=0;
    virtual double maxnorm(int icomp) const=0;
    virtual void savehdf5(const std::string& filename, const Alat::StringVector& blocknames) const =0;
    virtual void loadhdf5(const std::string& filename, const Alat::StringVector& blocknames)=0;
  };
}

/*--------------------------------------------------------------------------*/

#endif
