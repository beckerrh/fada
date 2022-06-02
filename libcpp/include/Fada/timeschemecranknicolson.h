#ifndef __Fada_TimeSchemeCrankNicolson_h
#define __Fada_TimeSchemeCrankNicolson_h

#include  "timescheme.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class TimeSchemeCrankNicolson : public TimeScheme
  {
protected:
    int _order;
    double massCoeffImplicit(const TimeData& timedata, int iteration) const;
    void computeRightHandSideTimeSchemeExplicit(AlatEnums::residualstatus& status, Alat::GhostVector& f, const Alat::GhostVector& u, const Alat::Vector<Alat::GhostVector>& prev, const TimeData& timedata, int iteration) const;

public:
    ~TimeSchemeCrankNicolson();
    TimeSchemeCrankNicolson(int order, Alat::Vector<Alat::GhostVector>& memory, const Alat::NewtonData& newtondata);
    TimeSchemeCrankNicolson( const TimeSchemeCrankNicolson& timeschemebdf);
    TimeSchemeCrankNicolson& operator=( const TimeSchemeCrankNicolson& timeschemebdf);
    std::string getName() const;
    TimeSchemeCrankNicolson* clone() const;
    int getNPreviousTimeSteps() const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
