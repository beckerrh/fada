#ifndef __Fada_MultiLevelTransferCgP12d_h
#define __Fada_MultiLevelTransferCgP12d_h

#include  "Fada/multileveltransfersinglefeminterface.h"
#include  "Alat/intvector.h"
#include  "Alat/variablematrix.h"

/*--------------------------------------------------------------------------*/
namespace Fada
{
  class MeshInterface;
}

namespace Fada
{
  class MultiLevelTransferCgP12d : public Fada::MultiLevelTransferSingleFemInterface
  {
  protected:
    Alat::Vector<Alat::IntVector> _nodeids;
    Alat::Vector<Alat::VariableMatrix> _matrixtransfer;
    Alat::Vector<Alat::VariableMatrix> _matrixproject;

    void _constructP12d(Alat::VariableMatrix& matrixtransfer, Alat::VariableMatrix& matrixproject, Alat::IntVector& nodeids, const FadaMesh::MeshInterface* coarsemesh, const FadaMesh::MeshInterface* finemesh);

  public:
    ~MultiLevelTransferCgP12d();
    MultiLevelTransferCgP12d();
    MultiLevelTransferCgP12d( const MultiLevelTransferCgP12d& multileveltransfercgp12d);
    MultiLevelTransferCgP12d& operator=( const MultiLevelTransferCgP12d& multileveltransfercgp12d);
    std::string getName() const;
    MultiLevelTransferCgP12d* clone() const;

    void basicInit(const FadaMesh::MeshInterface* mesh, const Fada::FemInterface* fem);
    void prolongate(int level, Alat::VariableVectorInterface* ufine, const Alat::VariableVectorInterface* ucoarse, double d = 1.0) const;
    void restrict(int level, Alat::VariableVectorInterface* ucoarse, const Alat::VariableVectorInterface* ufine ) const;
    void project(int level, Alat::VariableVectorInterface* ucoarse, const Alat::VariableVectorInterface* ufine) const;
  };
}

/*--------------------------------------------------------------------------*/
#endif
