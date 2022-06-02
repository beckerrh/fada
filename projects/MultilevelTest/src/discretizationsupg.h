#ifndef ___DiscretizationSupg_h
#define ___DiscretizationSupg_h

#include  "discretizationtransport.h"
#include  "slopelimiterinterface.h"
#include  "shockcapturinginterface.h"
#include  "patchdata.h"

/*--------------------------------------------------------------------------*/
namespace Alat
{
  class SystemMatrixInterface;
  class SystemVectorInterface;
}
namespace Alat
{
  class VariableVector;
}

class DiscretizationSupg : public DiscretizationTransport
{
protected:
   void computePatchDiff(const FadaMesh::MeshInterface* mesh, DiscretizationData& discdata) const;
   SlopeLimiterInterface* _sl;
   ShockCapturingInterface* _sc;
   bool _symmetric;
   std::string _discretization;
   void _computePatchGrads(int iN, PatchData& pdata, const Alat::VariableVector* uv , const FadaMesh::PatchInfo& patchinfo, const arma::field<arma::vec>& patchdiff) const;

public:
  ~DiscretizationSupg();
  DiscretizationSupg(std::string discretization);
  DiscretizationSupg( const DiscretizationSupg& discretizationsupg);
  DiscretizationSupg& operator=( const DiscretizationSupg& discretizationsupg);
  std::string getName() const;
  DiscretizationSupg* clone() const;

  void basicInit(const Fada::DomainSolverInterface* solver);
  void matrixSparsityPattern(const std::string& varnamei, const std::string& varnamej, Alat::SparsityPatternSoft& sparsitypatternsoft, const FadaMesh::MeshInterface* mesh, const DiscretizationData& discdata) const;
  void rightHandSideCells(AlatEnums::residualstatus& status, Alat::SystemVectorInterface* fs, const FadaMesh::MeshInterface* mesh, const DiscretizationData& discdata) const; 
  void rightHandSideBoundarySides(AlatEnums::residualstatus& status, Alat::SystemVectorInterface* fs, const FadaMesh::MeshInterface* mesh, const DiscretizationData& discdata) const; 
  void formCells(AlatEnums::residualstatus& status, Alat::SystemVectorInterface* fs, const Alat::SystemVectorInterface* us, const FadaMesh::MeshInterface* mesh, const DiscretizationData& discdata) const; 
  void formBoundarySides(AlatEnums::residualstatus& status, Alat::SystemVectorInterface* fs, const Alat::SystemVectorInterface* us, const FadaMesh::MeshInterface* mesh, const DiscretizationData& discdata) const; 
  void matrixCells(AlatEnums::residualstatus& status, Alat::SystemMatrixInterface* As, const Alat::SystemVectorInterface* us, const FadaMesh::MeshInterface* mesh, const DiscretizationData& discdata) const; 
  void matrixBoundarySides(AlatEnums::residualstatus& status, Alat::SystemMatrixInterface* As, const Alat::SystemVectorInterface* us, const FadaMesh::MeshInterface* mesh, const DiscretizationData& discdata) const; 
};

/*--------------------------------------------------------------------------*/
#endif
