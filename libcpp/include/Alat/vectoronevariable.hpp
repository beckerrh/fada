#ifndef __Alat_VectorOneVariable_h
#define __Alat_VectorOneVariable_h

#include  "armadillo.hpp"
#include  "vectoronevariableinterface.hpp"

/*----------------------------------------------------------*/
namespace Alat
{
  class StringVector;

  class VectorOneVariable : public  alat::armavec, public virtual alat::VectorOneVariableInterface
  {
public:
    typedef alat::armavec::const_iterator const_iterator;
    typedef alat::armavec::iterator iterator;

protected:
    int _ncomp, _n;
    iterator begin(int icomp);
    iterator end(int icomp);

public:
    ~VectorOneVariable();
    VectorOneVariable();
    VectorOneVariable(int ncomp);
    VectorOneVariable(const VectorOneVariable& v);
    VectorOneVariable& operator=(const VectorOneVariable& v);
    std::unique_ptr<VectorOneVariableInterface> clone() const;
    std::string getClassName() const;

    const_iterator begin(int icomp) const;
    const_iterator end(int icomp) const;
    void copyFrom(const VectorOneVariable& u);
    void scaleIntVector(const alat::armaivec& count);
    void loadhdf5(const arma::hdf5_name& spec);
    void savehdf5(const arma::hdf5_name& spec) const;
    void save(std::ostream& os, arma::file_type datatype = arma::arma_binary) const;
    int n() const;
    int ncomp() const;
    void setNcompAndN(int ncomp, int n);

    int size() const;
    void set_size(int n);
    void scale(double s);
    void fillzeros();
    double norm() const;
    double dot(const alat::VectorOneVariableInterface* v) const;
    void equal(const alat::VectorOneVariableInterface* v);
    void equal(double d);
    void add(const double& d, const alat::VectorOneVariableInterface* v);
    void setVectorFromDirectSolver(int offset, const alat::armavec& u);
    void addVectorRhsForDirectSolver(int offset, alat::armavec& f) const;

    void assemble(const alat::armaimat& indices, const arma::mat& local, double d=1.0);
    void extract(const alat::armaimat& indices, arma::mat& local) const;
  };
  std::ostream& operator<<(std::ostream& os, const VectorOneVariable& g);
}

#endif
