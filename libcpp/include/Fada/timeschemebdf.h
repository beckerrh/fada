#ifndef __Fada_TimeSchemeBdf_h
#define __Fada_TimeSchemeBdf_h

#include  "timescheme.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class TimeSchemeBdf : public TimeScheme
  {
protected:
    int _order;
    double massCoeffImplicit(const TimeData& timedata, int iteration) const;
    void computeRightHandSideTimeSchemeExplicit(AlatEnums::residualstatus& status, Alat::GhostVector& f, const Alat::GhostVector& u, const Alat::Vector<Alat::GhostVector>& prev, const TimeData& timedata, int iteration) const;

public:
    ~TimeSchemeBdf();
    TimeSchemeBdf(int order, Alat::Vector<Alat::GhostVector>& memory, const Alat::NewtonData& newtondata);
    TimeSchemeBdf( const TimeSchemeBdf& timeschemebdf);
    TimeSchemeBdf& operator=( const TimeSchemeBdf& timeschemebdf);
    std::string getName() const;
    TimeSchemeBdf* clone() const;
    int getNPreviousTimeSteps() const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
