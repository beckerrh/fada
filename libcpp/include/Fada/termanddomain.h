#ifndef __Fada_TermAndDomain_h
#define __Fada_TermAndDomain_h

#include  "Alat/pair.h"
#include  "fadaenums.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class TermAndDomain : public Alat::Pair<FadaEnums::term, FadaEnums::domain>
  {
public:
    ~TermAndDomain();
    TermAndDomain();
    TermAndDomain(const TermAndDomain& termanddomain);
    TermAndDomain(FadaEnums::term term, FadaEnums::domain domain);
    TermAndDomain& operator=(const TermAndDomain& termanddomain);
    bool operator==(const TermAndDomain& termanddomain);
    std::string toString() const;
  };
  bool operator<(const TermAndDomain& termanddomain1, const TermAndDomain& termanddomain2);
  std::ostream& operator<<(std::ostream& os, const TermAndDomain& termanddomain);
}

/*--------------------------------------------------------------------------*/

#endif
