#ifndef __Alat_MatrixAllVariables_h
#define __Alat_MatrixAllVariables_h

#include  "map.h"
#include  "pair.h"
#include  "systemmatrixinterface.h"
#include  "variablematrixinterface.h"

/*--------------------------------------------------------------------------*/
namespace Alat
{
  class SystemAssembleMatrix;

  class MatrixAllVariables : public Alat::Map<Alat::StringPair, Alat::VariableMatrixInterface*>, public Alat::SystemMatrixInterface
  {
public:
    ~MatrixAllVariables();
    MatrixAllVariables();
    MatrixAllVariables( const MatrixAllVariables& matrixallvariables);
    MatrixAllVariables& operator=( const MatrixAllVariables& matrixallvariables);
    std::string getName() const;
    MatrixAllVariables* clone() const;

    Alat::StringSet getVariables() const;
    Alat::StringSet getVariablesOut() const;
    Alat::StringSet getVariablesIn() const;

    void set_size(const Alat::StringSet& variables);
    void set_size(const Alat::StringSet& variablesOut, const Alat::StringSet& variablesIn);
    const Alat::VariableMatrixInterface* getMatrix(std::string varOut, std::string varIn) const;
    Alat::VariableMatrixInterface* getMatrix(std::string varOut, std::string varIn);
    Alat::VariableMatrixInterface*& getMatrixPointer(std::string varOut, std::string varIn);

    void zeros();
    std::ostream& write(std::ostream& os) const;
    void matrixVectorProduct(Alat::SystemVector* out, const Alat::SystemVector* in, double d = 1.0) const;
    void addMatrix(const SystemMatrixInterface* sparsematrix, double d = 1.0);
  };
  std::ostream& operator<<(std::ostream& s, const MatrixAllVariables& v);
}

/*--------------------------------------------------------------------------*/

#endif
