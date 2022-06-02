#ifndef __FadaMesh_MeshAdaptorInterface_h
#define __FadaMesh_MeshAdaptorInterface_h

/*--------------------------------------------------------------------------*/

namespace FadaMesh
{
/// classe de base d'un adaptateur de maillages
  class MeshAdaptorInterface: public Alat::InterfaceBase
  {
public:

    std::string getInterfaceName() const
    {
      return "MeshAdaptorInterface";
    }
/// fonction virtuelle d'adaptation de maillage
    virtual void adaptMesh(std::string filename)=0;
/// fonction virtuelle de raffinement global
    virtual void globalRefine(int nref)
    {
       _notWritten("globalRefine");
    }
  };
}

/*--------------------------------------------------------------------------*/

#endif
