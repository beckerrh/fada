#include  "Alat/parameterfile.h"

using namespace Alat;

/*--------------------------------------------------------------------------*/

ParameterFile::~ParameterFile()
{}

/*--------------------------------------------------------------------------*/

ParameterFile::ParameterFile() : std::string()
{}

/*--------------------------------------------------------------------------*/

ParameterFile::ParameterFile( const ParameterFile& parameterfile) : std::string(parameterfile)
{}

/*--------------------------------------------------------------------------*/

ParameterFile::ParameterFile( const std::string& filename) : std::string(filename)
{}

/*--------------------------------------------------------------------------*/

ParameterFile& ParameterFile::operator=( const ParameterFile& parameterfile)
{
  std::string::operator=(parameterfile);
  return *this;
}
