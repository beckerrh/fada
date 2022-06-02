#ifndef __Alat_SpaiVariableMatrix_h
#define __Alat_SpaiVariableMatrix_h

#include  "variablematrix.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class SpaiVariableMatrix : public VariableMatrix
  {
protected:
    void _computeSpaiSimple(double relax, const Alat::VariableMatrix* variablematrix);
    void _computeSpaiSimple2(double relax, const Alat::VariableMatrix* variablematrix);
    void _computeSpaiDiagonal(double relax, const Alat::VariableMatrix* variablematrix);
    void _computeSpaiFull(double relax, const Alat::VariableMatrix* variablematrix);

public:
    ~SpaiVariableMatrix();
    SpaiVariableMatrix(int ncomp, std::string type = "full");
    SpaiVariableMatrix( const SpaiVariableMatrix& variablematrix);
    SpaiVariableMatrix& operator=( const SpaiVariableMatrix& variablematrix);
    std::string getName() const;
    SpaiVariableMatrix* clone() const;

		void set_size(const VariableMatrix* variablematrix);
    void computeSpai(double relax, const Alat::VariableMatrixInterface* variablematrix);
  };
}

/*--------------------------------------------------------------------------*/

#endif
