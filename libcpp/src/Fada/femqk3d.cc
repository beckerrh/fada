#include  "Fada/femqk3d.h"
#include  "Fada/q13dtransformation.h"

using namespace Fada;
using namespace std;

/*---------------------------------------------------------*/
//!
//!      3 ----------2                  7 ----------6
//!      |           |                  |           |
//!      |           |                  |           |
//!      |           |                  |           |
//!      |           |                  |           |
//!      |           |                  |           |
//!      0 --------- 1                  4 --------- 5
//!

/*---------------------------------------------------------*/
template<int K>
FemQk3d<K>::~FemQk3d<K>( ) {}

/*---------------------------------------------------------*/
template<int K>
FemQk3d<K>::FemQk3d() : FEMBase() {}

/*---------------------------------------------------------*/
template<int K>
FemQk3d<K>::FemQk3d(const FemQk3d<K>& orig) : FEMBase(orig) {}

/*---------------------------------------------------------*/
template<int K>
FemQk3d<K>& FemQk3d<K>::operator=(const FemQk3d<K>& orig)
{
  assert(0);
  return *this;
}

/*---------------------------------------------------------*/

template<int K>
std::string FemQk3d<K>::getName() const
{
  return "FemQk3d";
}

/*---------------------------------------------------------*/

template<int K>
std::string FemQk3d<K>::getReferenceElementType() const
{
  return "Hexahedral";
}

/*---------------------------------------------------------*/

template<int K>
Fada::TransformationInterface* FemQk3d<K>::newTransformation() const
{
  return new Q13DTransformation;
}

/*---------------------------------------------------------*/

template<int K>
int FemQk3d<K>::ndoflocal() const
{
  return ( K+1 )*( K+1 )*( K+1 );
}
/*---------------------------------------------------------*/
template<int K>
void FemQk3d<K>::getReferenceCenterPoint(Alat::Node& xhat) const
{
  xhat.zeros();
}
/*---------------------------------------------------------*/
/*---------------------------------------------------------*/

template class Fada::FemQk3d<0>;
template class Fada::FemQk3d<1>;
template class Fada::FemQk3d<2>;
