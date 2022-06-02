#ifndef __Fada_TransferMatrixConstructor_h
#define __Fada_TransferMatrixConstructor_h

#include  "Alat/doublematrix.h"
#include  "Alat/doublevector.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class DofInformation;
  class TransferMatrixConstructor
  {
private:
    typedef Alat::Vector<Alat::Node> NodeVector;

    // int _dim;
    Alat::DoubleMatrix _phicoarse, _phifine;
    Alat::DoubleVector _weights;

    void _setNodes(NodeVector& nodes, double xmin, double xmax, double ymin, double ymax, double zmin, double zmax) const;
    void _computePhi(DofInformation* dofinformation);
    void _computePhiTri(DofInformation* dofinformation, int ncellsrefinedrefelement);
    void _computePhiQuadHex(DofInformation* dofinformation, int ncellsrefinedrefelement);
    void _computeMatrices(Alat::DoubleMatrix& interpolmat, Alat::DoubleMatrix& projectmat);
    void _computePhi2(Fada::IntegrationFormulaInterface* IF, DofInformation* dofinformation, const Alat::Vector<NodeVector>& nodesfine, int nfine);

public:
    ~TransferMatrixConstructor();
    TransferMatrixConstructor();
    TransferMatrixConstructor( const TransferMatrixConstructor& transfermatrixconstructor);
    TransferMatrixConstructor(Alat::DoubleMatrix& interpolmat, Alat::DoubleMatrix& projectmat, DofInformation* dofinformation);
    TransferMatrixConstructor& operator=( const TransferMatrixConstructor& transfermatrixconstructor);
    std::string getName() const;
    TransferMatrixConstructor* clone() const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
