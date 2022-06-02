#ifndef  __Fada_IntegrationFormulaBase_h
#define  __Fada_IntegrationFormulaBase_h

#include  "Alat/doublevector.h"
#include  "Fada/integrationformulainterface.h"
#include  "Alat/node.h"
#include  "Alat/vector.h"
#include  "Alat/fixarray.h"

/*-----------------------------------------*/

namespace Fada
{
  template<int N>
  class IntegrationFormulaBase : public Fada::IntegrationFormulaInterface
  {
private:
    Alat::FixArray<N, Alat::Node> _c;
    arma::vec::fixed<N> _w;

public:
    double& w(int k);
    Alat::Node& c(int k);

public:
    ~IntegrationFormulaBase<N>();
    IntegrationFormulaBase<N>();
    IntegrationFormulaBase<N>(const IntegrationFormulaBase<N>& integrationformulabase);
    IntegrationFormulaBase<N>& operator=(const IntegrationFormulaBase<N>& integrationformulabase);

    std::string getName() const;
    int n() const;
    double weight(int k) const;
    void point(Alat::Node& v, int k)  const;
  };
}

#endif
