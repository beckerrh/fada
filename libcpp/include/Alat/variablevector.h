#ifndef __Alat_VariableVector_h
#define __Alat_VariableVector_h

#include  "armadillo.h"
#include  "variablevectorinterface.h"

/*----------------------------------------------------------*/
namespace Alat
{
  class VariableVector : public virtual Alat::armamat, public virtual Alat::VariableVectorInterface
  {
public:
  typedef Alat::armamat::const_iterator const_iterator;
  typedef Alat::armamat::iterator iterator;

    ~VariableVector();
    VariableVector();
    VariableVector(int ncomp, int n);
    VariableVector(int ncomp);
    VariableVector(const VariableVector& v);
    VariableVector& operator=(const VariableVector& v);
    Alat::VariableVectorInterface* clone() const;
    std::string getName() const;

    int n() const;
    int getNComponents() const;
    const_iterator begin(int icomp) const;
    iterator begin(int icomp);
    iterator end(int icomp);
    const_iterator end(int icomp) const;

    void set_size(int ncomp, int n);
    void scale(double s);
    void scale(const Alat::DoubleVector& scale);
    void scaleinv(const Alat::DoubleVector& scale);
    void scaleIntVector(const Alat::IntVector& count);
    void zeros();
    void zero(int icomp);
    double norm() const;
    double norm(int icomp) const;
    double scalarProduct(const Alat::VariableVectorInterface* v) const;
    void equal(const Alat::VariableVectorInterface* v);
    void equal(double d);
    void add(const double& d, const Alat::VariableVectorInterface* v);
    void assemble(const Alat::AssembleVector& vloc, const Alat::IntVector& indices, const Alat::DoubleVector& scale, double d=1.0);
    void set(const Alat::AssembleVector& vloc, const Alat::IntVector& indices);
    void extract(Alat::AssembleVector& uloc, const Alat::IntVector& indices, const Alat::DoubleVector& scale) const;
    void setSingleVectorFromDirectSolver(int offset, const Alat::DoubleVector& u);
    void addVectorRhsForDirectSolver(int offset, Alat::DoubleVector& f) const;
    std::ostream& writeAscii(std::ostream& os) const;

    double min(int icomp) const;
    double max(int icomp) const;
    double maxnorm(int icomp) const;
    double sum(int icomp) const;
    void savehdf5(const std::string& filename, const Alat::StringVector& blocknames) const;
    void loadhdf5(const std::string& filename, const Alat::StringVector& blocknames);

    std::ostream& saveFada(std::ostream& out, const std::string datatype = "binary") const;
    std::istream& loadFada(std::istream& in);
  };
}

#endif
