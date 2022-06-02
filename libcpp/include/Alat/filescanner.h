#ifndef __Alat_FileScanner_h
#define __Alat_FileScanner_h

#include  "dataformathandler.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class ParameterFile;

  class FileScanner
  {
private:
		static int _count;
    DataFormatHandler& _dataformathandler;
    std::string _blocksymbol;
    std::string _blockname;
    bool _complain;

    void FormatToValue(const Alat::StringVector& words);
    void print(std::ostream& os) const;
    void _assert(bool b, const Alat::StringVector& words) const;

public:
    ~FileScanner();
    FileScanner(DataFormatHandler& dataformathandler, const Alat::ParameterFile* parameterfile, const std::string& blockname, bool complain = 1);
    FileScanner( const FileScanner& filescanner);    
    FileScanner& operator=( const FileScanner& filescanner);
    std::string getName() const;

    void readfile(const Alat::ParameterFile* parameterfile);
  };
}

/*--------------------------------------------------------------------------*/

#endif
