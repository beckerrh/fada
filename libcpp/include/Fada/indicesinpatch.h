#ifndef __Fada_IndicesInPatch_h
#define __Fada_IndicesInPatch_h

// #include  "Alat/fixarray.h"
#include  "Alat/vector.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class IntVector;
}
namespace Fada
{
  class LocalDofInformation;

  class IndicesInPatch
  {
private:
    Alat::Vector<Alat::IntVector> _nodesinpatch, _edgesinpatch, _sidesinpatch, _cellsinpatch;
    void _basicInitTri(const LocalDofInformation& dofinfo);
    void _basicInitQuad(const LocalDofInformation& dofinfo);
    void _basicInitHex(const LocalDofInformation& dofinfo);
    void _setDimensions(std::string referenceelementname);

public:
    int ncells, nsides, nedges, nnodes, ncellspatch, nsidespatch, nedgespatch, nnodespatch;

    ~IndicesInPatch();
    IndicesInPatch();
    IndicesInPatch( const IndicesInPatch& indicesinpatch);
    IndicesInPatch& operator=( const IndicesInPatch& indicesinpatch);
    std::string getName() const;
    IndicesInPatch* clone() const;

    void basicInit(std::string referenceelementname, const LocalDofInformation& dofinfo);
    int getNcellspatch() const;

    const Alat::IntVector& nodeindices(int iK) const;
    const Alat::IntVector& edgeindices(int iK) const;
    const Alat::IntVector& sideindices(int iK) const;
    const Alat::IntVector& cellindices(int iK) const;
  };
  std::ostream& operator<<(std::ostream& os, const IndicesInPatch& A);
}

/*--------------------------------------------------------------------------*/

#endif
