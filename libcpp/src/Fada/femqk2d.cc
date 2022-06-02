#include  "Fada/femqk2d.h"
#include  "Fada/q12dtransformation.h"
#include  "Fada/q22dtransformation.h"
#include <fstream>

using namespace Fada;
using namespace std;

/*---------------------------------------------------------*/
//!            2
//!      3 ----------2
//!      |           |
//!      |           |
//!   3  |           |   1
//!      |           |
//!      |           |
//!      0 --------- 1
//!            0

/*---------------------------------------------------------*/
template<int K>
FemQk2d<K>::~FemQk2d<K>( ) {}

/*---------------------------------------------------------*/
template<int K>
FemQk2d<K>::FemQk2d() : FEMBase() {}

/*---------------------------------------------------------*/
template<int K>
FemQk2d<K>::FemQk2d(const FemQk2d<K>& orig) : FEMBase(orig)
{}

/*---------------------------------------------------------*/
template<int K>
FemQk2d<K>& FemQk2d<K>::operator=(const FemQk2d<K>& orig)
{
  assert(0);
  return *this;
}

/*---------------------------------------------------------*/
template<int K>
std::string FemQk2d<K>::getName() const
{
  return "FemQk2d";
}

/*---------------------------------------------------------*/

template<int K>
std::string FemQk2d<K>::getReferenceElementType() const
{
  return "Quadrilateral";
}

/*---------------------------------------------------------*/
template<int K>
int FemQk2d<K>::ndoflocal() const
{
  return ( K+1 )*( K+1 );
}

/*---------------------------------------------------------*/
template<int K>
Fada::TransformationInterface* FemQk2d<K>::newTransformation() const
{
  return new Q12DTransformation;
}

/*---------------------------------------------------------*/
template<int K>
Fada::TransformationInterface* FemQk2d<K>::newTransformationCurved()
{
  // return new Q12DTransformation;
  return new Q22DTransformation;
}

/*---------------------------------------------------------*/
template<int K>
void FemQk2d<K>::getReferenceCenterPoint(Alat::Node& xhat) const
{
  xhat.zeros();
}

/*---------------------------------------------------------*/

template class Fada::FemQk2d<0>;
template class Fada::FemQk2d<1>;
template class Fada::FemQk2d<2>;
template class Fada::FemQk2d<3>;
template class Fada::FemQk2d<4>;
