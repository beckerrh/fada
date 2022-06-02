#ifndef __Integrators_LaplaceIntegrator_h
#define __Integrators_LaplaceIntegrator_h

#include  "Fada/integratorsinglevariable.h"

/*--------------------------------------------------------------------------*/

namespace Integrators
{
  class LaplaceIntegrator : public Fada::IntegratorSingleVariable
  {
private:
    double _diffusion, _boundarypenalty, _interiorpenalty, _interiorderivativepenalty, _sigma;
    // int _indexin, _femindexin, _indexout, _femindexout, _ncomp, _nlocalin, _nlocalout;
    bool _penalty;

protected:
    void rhsBoundarySide(AlatEnums::residualstatus& status, int iS, double weight, int color) const;
    void formCell(AlatEnums::residualstatus& status, int iK, double weight) const;
    void formBoundarySide(AlatEnums::residualstatus& status, int iS, int iK, double weight, int color) const;
    void formCouplingSide(AlatEnums::residualstatus& status, int iKL, int iKR, double weight, double measureSmallSide) const;
    void matrixCell(AlatEnums::residualstatus& status, int iK, double weight) const;
    void matrixBoundarySide(AlatEnums::residualstatus& status, int iS, int iK, double weight, int color) const;
    void matrixCouplingSide(AlatEnums::residualstatus& status,  int iKL, int iKR, double weight, double measureSmallSide) const;

public:
    ~LaplaceIntegrator();
    LaplaceIntegrator();
    LaplaceIntegrator( const LaplaceIntegrator& laplaceintegrator);
    LaplaceIntegrator& operator=( const LaplaceIntegrator& laplaceintegrator);
    LaplaceIntegrator* clone() const;

    std::string getName() const;

    void initData();
    void setDomainsOfTerms();
  };
}

/*--------------------------------------------------------------------------*/

#endif
