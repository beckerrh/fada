#ifndef  __Alat_Tokenize_h
#define  __Alat_Tokenize_h

#include  "stringvector.h"

/*---------------------------------------------*/

namespace Alat
{
  Alat::StringVector Tokenize(const std::string& str, const std::string& sep = " ");
  Alat::StringVector Tokenize(const std::string& str, const std::string& sep1, const std::string& sep2);
}

#endif
