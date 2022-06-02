#ifndef __Alat_NonlinearSolverVisitorInterface_h
#define __Alat_NonlinearSolverVisitorInterface_h

#include  "interfacebase.h"
#include  "enums.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{  
  class GhostLinearSolver; 
  class GhostMatrix;  
  class GhostVector; 
  class ParameterFile;
}  
namespace Alat
{  
  class NonlinearSolverVisitorInterface : public Alat::InterfaceBase
  {
protected:
    std::string getInterfaceName() const;

public:
    ~NonlinearSolverVisitorInterface();
    NonlinearSolverVisitorInterface();
    NonlinearSolverVisitorInterface( const NonlinearSolverVisitorInterface& nonlinearsolvervisitorinterface);
    NonlinearSolverVisitorInterface& operator=( const NonlinearSolverVisitorInterface& nonlinearsolvervisitorinterface);
    std::string getName() const;
    NonlinearSolverVisitorInterface* clone() const;

    virtual void basicInit(const Alat::ParameterFile* parameterfile, std::string blockname);
    virtual std::ostream& printLoopInformation(std::ostream& os) const;
    virtual std::string getVectorType() const;
    virtual int getVectorLevel() const=0;
    virtual void newVector(Alat::GhostVector* u) = 0;
    virtual double residual(AlatEnums::residualstatus& status, Alat::GhostVector& r, const Alat::GhostVector& u, const Alat::GhostVector& f) const=0;
    virtual int solve(AlatEnums::iterationstatus& status, const Alat::GhostLinearSolver& B, const Alat::GhostMatrix& A, Alat::GhostVector& du, const Alat::GhostVector& r) const=0;
    virtual void setLinearTolerance(double rtol, double gtol, Alat::GhostLinearSolver& B)=0;
    
    virtual void vectorZero(Alat::GhostVector& u) const=0;
    virtual void vectorAdd(Alat::GhostVector& u, double s, const Alat::GhostVector& du) const=0;
    virtual double vectorNorm(const Alat::GhostVector& r) const=0; 
    virtual double scalarProduct(const Alat::GhostVector& r, const Alat::GhostVector& p) const=0;
    virtual void vectorEqual(Alat::GhostVector& u, const Alat::GhostVector& du) const=0;
    virtual std::ostream& vectorWrite(std::ostream& os, const Alat::GhostVector& r) const = 0;
     
    virtual void computeLinearization(AlatEnums::residualstatus& status, Alat::GhostVector& h, const Alat::GhostVector& u, const Alat::GhostVector& du) const=0;
    virtual void constructMatrixAndLinearSolvers(AlatEnums::residualstatus& status, Alat::GhostLinearSolver& B, Alat::GhostMatrix& A, Alat::GhostVector& u)=0;
    virtual void setLavrentievParameter(double parameter) const=0;
    virtual double computeNormSquared(AlatEnums::residualstatus& status, const Alat::GhostVector& u, const Alat::GhostVector& du) const=0;
  };
}

/*--------------------------------------------------------------------------*/

#endif
