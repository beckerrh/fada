#ifndef __Alat_IterationInfo_h
#define __Alat_IterationInfo_h

#include  <iostream>
#include  "enums.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class ParameterFile;
}
namespace Alat
{
  class IterationInfo
  {
private:
    std::string _id;
    std::string _blockname;

    // user-defined parameters (via ParameterFile)
    int _maxiter, _printstep;
    double _rtol, _gtol, _divfactor;

    // information computed during execution
    mutable int _iteration;
    mutable double _lastresidual, _firstresidual, _reductionrate, _previuousreductionrate;

    void printFirstIteration() const;
    void printIteration() const;

public:
    ~IterationInfo();
    IterationInfo();
    IterationInfo( const IterationInfo& iterationinfo);
    IterationInfo& operator=( const IterationInfo& iterationinfo);
    std::string getName() const;
    IterationInfo* clone() const;

    void basicInit(const Alat::ParameterFile* parameterfile, std::string blockname);
    std::ostream& printLoopInformation(std::ostream& os) const;
    void setId(std::string id);
    const std::string& getId() const;
    const double& getGlobalTol() const;
    double& getGlobalTol();
    const double& getRTol() const;
    double& getRTol();
    int& getMaxiter();
    const int& getMaxiter() const;
    int& getPrintStep();
    void reset() const;
    int& getNumberOfIterations() const;
    const double& getFirstResidual() const;
    const double& getLastResidual() const;
    const int& getPrintstep() const;
    void checkIteration(AlatEnums::iterationstatus& status, double res) const;
    void checkIteration(AlatEnums::iterationstatus& status, double res, int iter, bool print = 1) const;
    double getLastReductionRate() const;
    double getPreviousReductionRate() const;
    double getMissingRate() const;
    
    mutable std::string _printbuff, _printbufff;
  };
  std::ostream& operator<<(std::ostream& stream, const IterationInfo& A);
}

/*--------------------------------------------------------------------------*/

#endif
