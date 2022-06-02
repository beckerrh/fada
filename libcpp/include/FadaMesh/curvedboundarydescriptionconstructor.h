#ifndef  __FadaMesh_CurvedBoundaryDescriptionConstructor_h
#define  __FadaMesh_CurvedBoundaryDescriptionConstructor_h

#include  "curvedboundaryinformation.h"

/*---------------------------------------------------*/

namespace FadaMesh
{
  class CurvedBoundaryDescriptionConstructor
  {
public:
    ~CurvedBoundaryDescriptionConstructor();
    // CurvedBoundaryDescriptionConstructor(CurvedBoundaryInformation& BD, const Alat::StringVector names, const Alat::IntVector& colors);
    CurvedBoundaryDescriptionConstructor();

    FadaMesh::CurvedBoundaryDescriptionInterface* newDescription(std::string name) const;
  };
}

/*---------------------------------------------------*/

#endif
