#ifndef __FadaMesh_PatchInfo_h
#define __FadaMesh_PatchInfo_h

#include  "Alat/intvector.h"
#include  "Alat/vector.h"
#include  "Alat/armadillo.h"

/*--------------------------------------------------------------------------*/
namespace FadaMesh
{
class PatchInfo
{
public:
  ~PatchInfo();
  PatchInfo();
  PatchInfo( const PatchInfo& patchinfo);
  PatchInfo& operator=( const PatchInfo& patchinfo);
  std::string getName() const;
  PatchInfo* clone() const;
  
  Alat::Vector<Alat::IntVector> cells, edgesinner, edgesouter, nodes;

  arma::field<arma::mat> sidecoeffs;
  arma::field<arma::imat> sideindices;
};
}

/*--------------------------------------------------------------------------*/
#endif
