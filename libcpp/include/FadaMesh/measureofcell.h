#ifndef __FadaMesh_MeasureOfCell_h
#define __FadaMesh_MeasureOfCell_h

#include  "Alat/doublevector.h"
#include  "geometryobject.h"

/*--------------------------------------------------------------------------*/

namespace FadaMesh
{
  class MeasureOfCell : public virtual FadaMesh::GeometryObject, public Alat::DoubleVector
  {
public:

private:

protected:

public:

    std::string getName() const
    {
      return "MeasureOfCell";
    }

    void read(std::string filename);
    void write(std::string filename, std::string datatype) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
