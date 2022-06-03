#ifndef ___DiscretizationTransport_h
#define ___DiscretizationTransport_h

#include  "Fada/unstructuredintegrationloop.h"
#include  "FadaMesh/patchinfo.h"
#include  "Alat/stringvector.h"
#include  "Alat/armadillo.h"

/*--------------------------------------------------------------------------*/
namespace Alat
{
  class VectorInterface;
}
namespace Fada
{
  class DirichletInterface;
  class ModelInterface;
  class FemInterface;
  class IntegrationFormulaInterface;
  class MeshInterface;
  class Node;
  class RightHandSideInterface;
}
namespace FadaMesh
{
  class MultiLevelMesh;
}

class DiscretizationData
{
public:
  int npatchmax;
  Alat::DoubleVector area;
  Fada::FemInterface* fem;
  FadaMesh::PatchInfo patchinfo;
  arma::vec betaside;
  Alat::Vector<Alat::Node> betacell;
  arma::cube downwindcoef;
  arma::field<arma::vec> patchdiff;

  // void basicInit(const FadaMesh::MeshInterface* mesh);
};

class DiscretizationTransport : public Fada::UnStructuredIntegrationLoop
{
protected:
  const Fada::ModelInterface* _model;
  const FadaMesh::MultiLevelMesh* _mmesh;
  Alat::Vector<DiscretizationData> _discdata;

  Alat::StringVector _variablenames;
  const Fada::DataInterface* _betafct;
  Alat::Vector<const Fada::RightHandSideInterface*> _righthandside;
  Alat::Vector<Alat::Vector<const Fada::DirichletInterface*> > _dirichlet;
  mutable Alat::Vector<Alat::DoubleVector> _help1, _help2;
  
  Fada::IntegrationFormulaInterface* _IFside;
  Fada::IntegrationFormulaInterface* _IFcell;
  
  void integrateBetan(const FadaMesh::MeshInterface* mesh, DiscretizationData& discdata) const;
  void computeDownwind(const FadaMesh::MeshInterface* mesh, DiscretizationData& discdata) const;
  void computeArea(const FadaMesh::MeshInterface* mesh, DiscretizationData& discdata) const;
  virtual void computePatchDiff(const FadaMesh::MeshInterface* mesh, DiscretizationData& discdata) const=0;

public:
  ~DiscretizationTransport();
  DiscretizationTransport();
  DiscretizationTransport( const DiscretizationTransport& discretizationtransport);
  DiscretizationTransport& operator=( const DiscretizationTransport& discretizationtransport);
  std::string getName() const;
  DiscretizationTransport* clone() const;

  void basicInit(const Fada::DomainSolverInterface* solver);

  void matrixConnectivityPerLevel(const std::string& varnamei, const std::string& varnamej, const Fada::IntegratorManager* integratormanager, int level, int n, Alat::VariableMatrixInterface* sparsematrix) const;
  void constructRightHandSide(AlatEnums::residualstatus& status, Alat::VectorInterface* f) const;
  void constructJacobianMatrix(AlatEnums::residualstatus& status, Alat::MatrixInterface* A, const Alat::VectorInterface* u, const Alat::VectorInterface* uold=NULL, const Alat::VectorInterface* uveryold=NULL) const;
  void constructForm(AlatEnums::residualstatus& status, Alat::VectorInterface* f, const Alat::VectorInterface* u, const Alat::VectorInterface* uold, const Alat::VectorInterface* uveryold) const;  
  void constructJacobianMatrix(int level, AlatEnums::residualstatus& status, Alat::SystemMatrixInterface* A, const Alat::SystemVector* u, const Alat::SystemVector* uold, const Alat::SystemVector* uveryold) const;
  
  virtual void matrixSparsityPattern(const std::string& varnamei, const std::string& varnamej, Alat::SparsityPatternSoft& sparsitypatternsoft, const FadaMesh::MeshInterface* mesh, const DiscretizationData& discdata) const=0;

  virtual void rightHandSideCells(AlatEnums::residualstatus& status, Alat::SystemVector* fs, const FadaMesh::MeshInterface* mesh, const DiscretizationData& discdata) const=0; 
  virtual void rightHandSideBoundarySides(AlatEnums::residualstatus& status, Alat::SystemVector* fs, const FadaMesh::MeshInterface* mesh, const DiscretizationData& discdata) const=0; 
  virtual void formCells(AlatEnums::residualstatus& status, Alat::SystemVector* fs, const Alat::SystemVector* us, const FadaMesh::MeshInterface* mesh, const DiscretizationData& discdata) const=0; 
  virtual void formBoundarySides(AlatEnums::residualstatus& status, Alat::SystemVector* fs, const Alat::SystemVector* us, const FadaMesh::MeshInterface* mesh, const DiscretizationData& discdata) const=0; 
  virtual void matrixCells(AlatEnums::residualstatus& status, Alat::SystemMatrixInterface* As, const Alat::SystemVector* us, const FadaMesh::MeshInterface* mesh, const DiscretizationData& discdata) const=0; 
  virtual void matrixBoundarySides(AlatEnums::residualstatus& status, Alat::SystemMatrixInterface* As, const Alat::SystemVector* us, const FadaMesh::MeshInterface* mesh, const DiscretizationData& discdata) const=0; 
  
};

/*--------------------------------------------------------------------------*/
#endif
