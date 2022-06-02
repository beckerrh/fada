// #ifndef _TransportIntegratorSupg_h
// #define _TransportIntegratorSupg_h

#include  "Fada/integratorsinglevariable.h"
#include  "Alat/doublematrix.h"
#include  "Alat/doublevector.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class IntegrationFormulaInterface;
}

// namespace Integrators
// {
  class TransportIntegratorSupg : public Fada::IntegratorSingleVariable
  {
  private:
    double _stabilization, _diffusion;
    double _compute_stabilization(double bnorm, double h) const;
    
protected:
    std::string _symmetrytype;

    void rhsBoundarySide(AlatEnums::residualstatus& status, int iS, double weight, int color) const;
    void formCell(AlatEnums::residualstatus& status, int iK, double weight) const;
    void formBoundarySide(AlatEnums::residualstatus& status, int iS, int iK, double weight, int color) const;
    void formCouplingSide(AlatEnums::residualstatus& status, int iKL, int iKR, double weight, double measureSmallSide) const;
    void matrixCell(AlatEnums::residualstatus& status, int iK, double weight) const;
    void matrixBoundarySide(AlatEnums::residualstatus& status, int iS, int iK, double weight, int color) const;
    void matrixCouplingSide(AlatEnums::residualstatus& status,  int iKL, int iKR, double weight, double measureSmallSide) const;
    Fada::IntegrationFormulaInterface* _IFboundarytrapez;

public:
    ~TransportIntegratorSupg();
    TransportIntegratorSupg();
    TransportIntegratorSupg( const TransportIntegratorSupg& transportintegrator);
    TransportIntegratorSupg& operator=( const TransportIntegratorSupg& transportintegrator);
    TransportIntegratorSupg* clone() const;
    std::string getName() const;

    void initData();
    void setDomainsOfTerms();
  };
// }

/*--------------------------------------------------------------------------*/

// #endif
