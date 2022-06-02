#ifndef __Alat_VariableMatrixInterface_h
#define __Alat_VariableMatrixInterface_h

#include  "interfacebase.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{        
  class AssembleMatrix;                     
  class DoubleVector;
  class IntVector;                  
  class SparsityPattern;
  class SparsityPatternSoft;
  class SparsityPatternWithRowIndexSoft;
  // class DofInformationInterface;
}
namespace Alat
{  
  class PreconditionerInterface; 
  class VariableVectorInterface;

  class VariableMatrixInterface : public Alat::InterfaceBase
  {
public:
    ~VariableMatrixInterface();
    VariableMatrixInterface();
    VariableMatrixInterface( const VariableMatrixInterface& variablematrixinterface);
    VariableMatrixInterface& operator=( const VariableMatrixInterface& variablematrixinterface);
    std::string getName() const;
    std::string getInterfaceName() const;
    VariableMatrixInterface* clone() const;

    virtual int getNComponents() const;
    virtual int getMComponents() const;
    virtual void set_size(int n, int ntotal);
    virtual void set_size(const Alat::SparsityPatternSoft& sparsitypatternsoft);
    virtual void set_size(const Alat::SparsityPatternWithRowIndexSoft& sparsitypatternsoft); 
    // virtual void set_size(const FadaMesh::MeshInterface* mesh, const Alat::DofInformationInterface* dofinformationi, const Alat::DofInformationInterface* dofinformationj);
    virtual std::ostream& write(std::ostream& os) const = 0;

    virtual int getN() const;
    virtual void zeros() = 0;
    virtual void assemble(const Alat::AssembleMatrix& Aloc, const Alat::IntVector& indices, const Alat::IntVector& jndices, const Alat::DoubleVector& scalei, const Alat::DoubleVector& scalej);

    virtual void addMatrix(const VariableMatrixInterface* sparsematrix, double d = 1.0);
    virtual void addMatrixForDirectSolver(int offsetivar, int offsetjvar, Alat::DoubleVector& matrixvalues, const Alat::SparsityPattern* sparsitypattern) const;
    virtual void addEntriesForDirectSolver(int offsetivar, int offsetjvar, Alat::SparsityPatternSoft& sparsitypatternsoft) const;

    virtual const Alat::SparsityPattern* getSparsityPattern() const;
    virtual Alat::SparsityPattern* getSparsityPattern();
    virtual bool greaterThan(int pos1, int pos2) const;
    virtual const double& getValue(int icomp, int jcomp, int pos) const;
    virtual double& getValue(int icomp, int jcomp, int pos);
    virtual void zeroLine(int index, int comp);
    virtual void setElement(int index, int compi, int compj, double d);
    virtual void matrixVectorProduct(VariableVectorInterface* f, const Alat::VariableVectorInterface* u, double d = 1.0) const;
    virtual void matrixVectorProductTransposed(VariableVectorInterface* f, const Alat::VariableVectorInterface* u, double d = 1.0) const;

    virtual VariableMatrixInterface* newSchurComplementMatrix(const VariableMatrixInterface* B, const VariableMatrixInterface* C, std::string type, std::string storage) const;
    virtual void computeSchurSparsityPatternSoft(Alat::SparsityPatternSoft& sparsitypatternsoft, const VariableMatrixInterface* A, const VariableMatrixInterface* B, const VariableMatrixInterface* C, const VariableMatrixInterface* D, bool fullStencil = true) const;
    virtual void computeSchurComplement(const VariableMatrixInterface* A, const VariableMatrixInterface* B, const VariableMatrixInterface* C, const VariableMatrixInterface* D);
    virtual void reInitSchurMatrix(const VariableMatrixInterface* A, const VariableMatrixInterface* B, const VariableMatrixInterface* C, const VariableMatrixInterface* D, bool fullStencil = false);

    virtual VariableMatrixInterface* newSpaiMatrixAndReinit(int ncomp, std::string type, std::string storage, const VariableMatrixInterface* matrix) const;
    virtual void computeSpai(double relax, const VariableMatrixInterface* variablematrix);

    virtual VariableMatrixInterface* newGaussSeidelMatrixAndReinit(int ncomp, std::string type, std::string storage) const;
    virtual void computeGaussSeidel(double relax, const VariableMatrixInterface* variablematrix);
    virtual void gaussSeidel(Alat::VariableVectorInterface* u, const Alat::VariableVectorInterface* f, double relax, const Alat::IntVector& p, const Alat::IntVector& pinv) const;

    virtual VariableMatrixInterface* newIluMatrixAndReinit(int ncomp, const PreconditionerInterface* preconditioner, std::string type, std::string storage) const;
    virtual void computeIlu(double relax, const VariableMatrixInterface* variablematrix, const Alat::IntVector& p, const Alat::IntVector& pinv);
    virtual void ilu(Alat::VariableVectorInterface* u, const Alat::VariableVectorInterface* f, double relax, const Alat::IntVector& p, const Alat::IntVector& pinv) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
