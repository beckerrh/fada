#ifndef __FadaMesh_getMeshType_h
#define __FadaMesh_getMeshType_h

#include  <string>
#include  "Alat/pair.h"

/*--------------------------------------------------------------------------*/

namespace FadaMesh
{
  Alat::StringPair getMeshType(const std::string& meshname);
}

/*--------------------------------------------------------------------------*/

#endif
