#ifndef __Alat_LineScanner_h
#define __Alat_LineScanner_h

#include  <fstream>
#include  "stringvector.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class LineScanner
  {
private:
    std::ifstream _filestream;

public:
    ~LineScanner();
    LineScanner();
    LineScanner( const LineScanner& linescanner);
    LineScanner(std::string filename);
    LineScanner& operator=( const LineScanner& linescanner);
    std::string getName() const
    {
      return "LineScanner";
    }

    int nextLine(Alat::StringVector& words);
  };
}

/*--------------------------------------------------------------------------*/

#endif
