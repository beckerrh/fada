#ifndef __Fada_MultiLevelTransferSingleWithoutFem_h
#define __Fada_MultiLevelTransferSingleWithoutFem_h

#include  "multileveltransfersinglefeminterface.h"

/*--------------------------------------------------------------------------*/
namespace Fada
{
  class MultiLevelTransferSingleWithoutFem : public MultiLevelTransferSingleFemInterface
  {
  private:
  protected:
  public:
    ~MultiLevelTransferSingleWithoutFem();
    MultiLevelTransferSingleWithoutFem();
    MultiLevelTransferSingleWithoutFem( const MultiLevelTransferSingleWithoutFem& multileveltransfersinglewithoutfem);
    MultiLevelTransferSingleWithoutFem& operator=( const MultiLevelTransferSingleWithoutFem& multileveltransfersinglewithoutfem);
    std::string getName() const;
    MultiLevelTransferSingleWithoutFem* clone() const;

    void basicInit(const FadaMesh::MeshInterface* mesh, const Fada::FemInterface* fem);
    void prolongate(int level, Alat::VariableVectorInterface* ufine, const Alat::VariableVectorInterface* ucoarse, double d = 1.0) const;
    void restrict(int level, Alat::VariableVectorInterface* ucoarse, const Alat::VariableVectorInterface* ufine ) const;
    void project(int level, Alat::VariableVectorInterface* ucoarse, const Alat::VariableVectorInterface* ufine) const;
  };
}

/*--------------------------------------------------------------------------*/
#endif
