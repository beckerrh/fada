#ifndef __Fada_MultiLevelTransferSingleFemInterface_h
#define __Fada_MultiLevelTransferSingleFemInterface_h

#include  "Alat/interfacebase.h"
#include  "Alat/variablevectorinterface.h"

/*--------------------------------------------------------------------------*/
namespace FadaMesh
{
  class MeshInterface;
}
namespace Fada
{
  class FemInterface;
  
  class MultiLevelTransferSingleFemInterface : public Alat::InterfaceBase
  {
  protected:
    std::string getInterfaceName() const;

  public:
    ~MultiLevelTransferSingleFemInterface();
    MultiLevelTransferSingleFemInterface();
    MultiLevelTransferSingleFemInterface( const MultiLevelTransferSingleFemInterface& multileveltransfersinglefeminterface);
    MultiLevelTransferSingleFemInterface& operator=( const MultiLevelTransferSingleFemInterface& multileveltransfersinglefeminterface);
    std::string getName() const;
    MultiLevelTransferSingleFemInterface* clone() const;
  
    virtual void basicInit(const FadaMesh::MeshInterface* mesh, const Fada::FemInterface* fem)=0;
    virtual void prolongate(int level, Alat::VariableVectorInterface* ufine, const Alat::VariableVectorInterface* ucoarse, double d = 1.0) const=0;
    virtual void restrict(int level, Alat::VariableVectorInterface* ucoarse, const Alat::VariableVectorInterface* ufine ) const=0;
    virtual void project(int level, Alat::VariableVectorInterface* ucoarse, const Alat::VariableVectorInterface* ufine) const=0;
    virtual void interpolate(int level, Alat::VariableVectorInterface* ufine, const Alat::VariableVectorInterface* ucoarse) const;
  
  };
}

/*--------------------------------------------------------------------------*/
#endif
