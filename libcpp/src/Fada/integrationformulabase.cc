#include  "Fada/integrationformulabase.h"
#include  <sstream>

using namespace Fada;

/*--------------------------------------------------------------------------*/
template<int N>
IntegrationFormulaBase<N>::~IntegrationFormulaBase<N>(){}
template<int N>
IntegrationFormulaBase<N>::IntegrationFormulaBase() : Fada::IntegrationFormulaInterface(){}
template<int N>
IntegrationFormulaBase<N>::IntegrationFormulaBase(const IntegrationFormulaBase<N>& integrationformulabase) : Fada::IntegrationFormulaInterface(integrationformulabase), _w( integrationformulabase._w ), _c( integrationformulabase._c) {}
template<int N>
IntegrationFormulaBase<N>& IntegrationFormulaBase<N>::operator=(const IntegrationFormulaBase<N>& integrationformulabase)
{
  Fada::IntegrationFormulaInterface::operator=(integrationformulabase);
  _w = integrationformulabase._w;
  _c = integrationformulabase._c;

  assert(0);
  return *this;
}
template<int N>
std::string IntegrationFormulaBase<N>::getName() const
{
  std::stringstream ss;
  ss << "Fada::IntegrationFormulaBase("<<n()<<" points)";
  return ss.str();
}

/*--------------------------------------------------------------------------*/
template<int N>
double& IntegrationFormulaBase<N>::w(int k)
{
  return _w[k];
}
template<int N>
Alat::Node& IntegrationFormulaBase<N>::c(int k)
{
  return _c[k];
}
template<int N>
int IntegrationFormulaBase<N>::n() const
{
  return N;
}

/*--------------------------------------------------------------------------*/
template<int N>
double IntegrationFormulaBase<N>::weight(int k) const
{
  return _w[k];
}
template<int N>
void IntegrationFormulaBase<N>::point(Alat::Node& v, int k)  const
{
  v = _c[k];
}

/*--------------------------------------------------------------------------*/

template class Fada::IntegrationFormulaBase<1>;
template class Fada::IntegrationFormulaBase<2>;
template class Fada::IntegrationFormulaBase<3>;
template class Fada::IntegrationFormulaBase<4>;
template class Fada::IntegrationFormulaBase<5>;
template class Fada::IntegrationFormulaBase<6>;
template class Fada::IntegrationFormulaBase<7>;
template class Fada::IntegrationFormulaBase<8>;
template class Fada::IntegrationFormulaBase<9>;
template class Fada::IntegrationFormulaBase<10>;
template class Fada::IntegrationFormulaBase<11>;
template class Fada::IntegrationFormulaBase<12>;
template class Fada::IntegrationFormulaBase<14>;
template class Fada::IntegrationFormulaBase<15>;
template class Fada::IntegrationFormulaBase<16>;
template class Fada::IntegrationFormulaBase<19>;
template class Fada::IntegrationFormulaBase<20>;
template class Fada::IntegrationFormulaBase<25>;
template class Fada::IntegrationFormulaBase<27>;
template class Fada::IntegrationFormulaBase<36>;
template class Fada::IntegrationFormulaBase<49>;
template class Fada::IntegrationFormulaBase<64>;
template class Fada::IntegrationFormulaBase<81>;
template class Fada::IntegrationFormulaBase<100>;
template class Fada::IntegrationFormulaBase<121>;
template class Fada::IntegrationFormulaBase<125>;
