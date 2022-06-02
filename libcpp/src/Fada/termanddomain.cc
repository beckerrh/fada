#include  "Fada/termanddomain.h"
#include  <iostream>

using namespace Fada;

/*--------------------------------------------------------------------------*/

TermAndDomain::~TermAndDomain() {}
TermAndDomain::TermAndDomain() : Alat::Pair<FadaEnums::term, FadaEnums::domain>() {}
TermAndDomain::TermAndDomain(const TermAndDomain& termanddomain) : Alat::Pair<FadaEnums::term, FadaEnums::domain>(termanddomain) {}
TermAndDomain::TermAndDomain(FadaEnums::term term, FadaEnums::domain domain) : Alat::Pair<FadaEnums::term, FadaEnums::domain>(term, domain) {}
TermAndDomain& TermAndDomain::operator=(const TermAndDomain& termanddomain)
{
  Alat::Pair<FadaEnums::term, FadaEnums::domain>::operator=(termanddomain);
  return *this;
}

std::string TermAndDomain::toString() const
{
  return termToString(first)+'_'+domainToString(second);
}

/*--------------------------------------------------------------------------*/

bool TermAndDomain::operator==(const TermAndDomain& termanddomain)
{
  return first == termanddomain.first && second == termanddomain.second;
}

/*--------------------------------------------------------------------------*/
std::ostream& Fada::operator<<(std::ostream& os, const TermAndDomain& termanddomain)
{
  os << termanddomain.toString();
  return os;
}

bool Fada::operator<(const TermAndDomain& tanddx, const TermAndDomain& tanddy)
{
  if(tanddx.first == tanddy.first)
  {
    return tanddx.second < tanddy.second;
  }
  else
  {
    return tanddx.first < tanddy.first;
  }
}
