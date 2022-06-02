#include  "Alat/doublevector.h"
#include  "Fada/dirichletzero.h"

using namespace Fada;

/*--------------------------------------------------------*/
DirichletZero::~DirichletZero() {}
DirichletZero::DirichletZero() : DirichletInterface() {}
DirichletZero::DirichletZero(const DirichletZero& dirichlet) : DirichletInterface(dirichlet) {}
DirichletZero& DirichletZero::operator=(const DirichletZero& dirichlet)
{
  assert(0);
}
DataInterface* DirichletZero::clone() const
{
  assert(0);
}
std::string DirichletZero::getName() const
{
  return "DirichletZero";
}

/*--------------------------------------------------------*/
void DirichletZero::getValue(Alat::DoubleVector& g, int color, const Alat::Node& normal, double x, double y, double z, double t) const
{
  g.zeros();
}
