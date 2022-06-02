#ifndef __Alat_ParameterFile_h
#define __Alat_ParameterFile_h

#include  <string>

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class ParameterFile : public std::string
  {
public:
    ~ParameterFile();
    ParameterFile();
    ParameterFile( const ParameterFile& parameterfile);
    ParameterFile( const std::string& filename);
    ParameterFile& operator=( const ParameterFile& parameterfile);
    std::string getName() const
    {
      return *this;
    }
  };
}

/*--------------------------------------------------------------------------*/

#endif
