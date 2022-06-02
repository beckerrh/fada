#ifndef  __Alat_AlatIo_h
#define  __Alat_AlatIo_h

#include  <string>
#include  <iostream>
#include  <vector>

/*---------------------------------------------*/
namespace Alat
{
  class DoubleVector;
  class IntVector;
  
  std::ostream& writeBin(std::ostream& out, const std::vector<double>& vec);
  std::istream& readBin(std::istream& in, std::vector<double>& vec);
  std::ostream& saveAlat(std::ostream& out, const std::vector<double>& vec, const std::string datatype = "binary");
  std::istream& loadAlat(std::istream& in, std::vector<double>& vec);

  void reInitFromString(std::string valuechain, Alat::IntVector& vec, const std::string& sep = "|");
  void reInitFromString(std::string valuechain, Alat::DoubleVector& vec, const std::string& sep = "|");
}

#endif
