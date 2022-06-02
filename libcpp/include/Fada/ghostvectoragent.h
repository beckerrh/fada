#ifndef __Fada_GhostVectorAgent_h
#define __Fada_GhostVectorAgent_h

#include  "Alat/ghostvector.h"
#include  "Alat/map.h"
#include  "Alat/vectorinterface.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class GhostVectorAgent : public Alat::Map<Alat::GhostVector, Alat::VectorInterface*>
  {
public:
    typedef Alat::Map<Alat::GhostVector, Alat::VectorInterface*>::const_iterator const_iterator;
    typedef Alat::Map<Alat::GhostVector, Alat::VectorInterface*>::iterator iterator;

public:
    ~GhostVectorAgent();
    GhostVectorAgent();
    GhostVectorAgent( const GhostVectorAgent& ghostvectoragent);
    GhostVectorAgent& operator=( const GhostVectorAgent& ghostvectoragent);
    std::string getName() const;
    std::ostream& printLoopInformation(std::ostream& os) const;
    bool exists(const Alat::GhostVector& mg) const;
    void enrol(const Alat::GhostVector& mg );
    Alat::VectorInterface* operator()(const Alat::GhostVector& g) const;
    Alat::StringIntMap statistics() const;
  };
  std::ostream& operator<<(std::ostream& os, const GhostVectorAgent& gva);
}

/*--------------------------------------------------------------------------*/

#endif
