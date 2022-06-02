#ifndef __Integrators_TrustRegion_h
#define __Integrators_TrustRegion_h

#include  "Fada/integratorsinglevariable.h"

/*--------------------------------------------------------------------------*/

namespace Integrators
{
class TrustRegion : public Fada::IntegratorSingleVariable
{
public:
  ~TrustRegion();
  TrustRegion();
  TrustRegion( const TrustRegion& trustregion);
  TrustRegion& operator=( const TrustRegion& trustregion);
  std::string getName() const;
  TrustRegion* clone() const;

  void setDomainsOfTerms();
  void matrixCell(AlatEnums::residualstatus& status, int iK, double weight) const;
  void computeNormSquaredCell(AlatEnums::residualstatus& status, Alat::StringDoubleMap& norms, double weight, const Fada::FemFunctionsMap& femfcts) const;
};
}

/*--------------------------------------------------------------------------*/

#endif
