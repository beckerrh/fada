#include  "Alat/getlinesplit.h"
#include  <iterator>
#include  <sstream>

using namespace Alat;

/*---------------------------------------------------------*/

Alat::StringVector Alat::getLineSplit(std::ifstream& file)
{
  std::string line;
  while(line == "")
  {
    getline(file, line);
  }
  std::istringstream is(line);
  return Alat::StringVector( std::istream_iterator<std::string>(is), std::istream_iterator<std::string>() );
}
