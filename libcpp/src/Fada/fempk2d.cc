#include  "Fada/fempk2d.h"

using namespace Fada;
using namespace std;

/*---------------------------------------------------------*/
// //!      2
// //!      |   \
// // //!      |     \
// // //!      |       \
// // //!      |         \
// // //!      0 -------- 1

/*---------------------------------------------------------*/
/*---------------------------------------------------------*/
template<int K>
FemPk2d<K>::~FemPk2d<K>( ) {}

/*---------------------------------------------------------*/
template<int K>
FemPk2d<K>::FemPk2d() : FEMBase() {}

/*---------------------------------------------------------*/
template<int K>
FemPk2d<K>::FemPk2d(const FemPk2d<K>& orig) : FEMBase(orig)
{}

/*---------------------------------------------------------*/
template<int K>
FemPk2d<K>& FemPk2d<K>::operator=(const FemPk2d<K>& orig)
{
  assert(0);
  return *this;
}

/*---------------------------------------------------------*/
template<int K>
std::string FemPk2d<K>::getName() const
{
  return "FemPk2d";
}

/*---------------------------------------------------------*/

template<int K>
std::string FemPk2d<K>::getReferenceElementType() const
{
  return "Triangle";
}

/*---------------------------------------------------------*/

template<int K>
int FemPk2d<K>::ndoflocal() const
{
  return ( K+1 )*( K+2 )/2;
}

/*---------------------------------------------------------*/

template<int K>
Fada::TransformationInterface* FemPk2d<K>::newTransformation() const
{
  return new P12DTransformation;
}

/*---------------------------------------------------------*/

template<int K>
Fada::TransformationInterface* FemPk2d<K>::newTransformationCurved()
{
   return new P12DTransformation;
}

/*---------------------------------------------------------*/
template<int K>
void FemPk2d<K>::getReferenceCenterPoint(Alat::Node& xhat) const
{
  xhat.zeros();
  xhat[0]=1.0/3.0;
  xhat[1]=1.0/3.0;
}

/*---------------------------------------------------------*/

template class Fada::FemPk2d<0>;
template class Fada::FemPk2d<1>;
template class Fada::FemPk2d<2>;
template class Fada::FemPk2d<3>;
