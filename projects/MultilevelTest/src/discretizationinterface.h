#ifndef ___DiscretizationInterface_h
#define ___DiscretizationInterface_h

#include  "Fada/domainintegrationloopinterface.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class ModelInterface;
}
namespace Alat
{
  class VectorInterface;
}
namespace FadaMesh
{
  class MultiLevelMesh;
}

class DiscretizationInterface : public Fada::DomainIntegrationLoopInterface
{
private:
protected:
public:
  ~DiscretizationInterface();
  DiscretizationInterface();
  DiscretizationInterface( const DiscretizationInterface& discretizationinterface);
  DiscretizationInterface& operator=( const DiscretizationInterface& discretizationinterface);
  std::string getName() const;
  DiscretizationInterface* clone() const;
  
  virtual void setMesh(const FadaMesh::MultiLevelMesh* mmesh, const Fada::ModelInterface* model)=0;

  void basicInit(const Fada::DomainSolverInterface* solver);
  void computeLinearization(AlatEnums::residualstatus& status, Alat::VectorInterface* f, const Alat::VectorInterface* u, const Alat::VectorInterface* du, const Alat::VectorInterface* uold, const Alat::VectorInterface* uveryold) const;
  void postProcess(AlatEnums::residualstatus& status, Alat::VectorInterface* pp, const Alat::VectorInterface* u, const Alat::VectorInterface* uold) const;
  void initSolution(Alat::VectorInterface* u) const;
  void setDataVector(Alat::VectorInterface* data);
  void integrateTimeRhs(AlatEnums::residualstatus& status, Alat::VectorInterface* f, const Alat::VectorInterface* u, const Alat::VectorInterface* bdf, double d) const;
  void computeNormSquared(AlatEnums::residualstatus& status, Alat::StringDoubleMap& norms, const Alat::VectorInterface* u, const Alat::VectorInterface* du) const;
};

/*--------------------------------------------------------------------------*/
#endif
