#ifndef __Alat_IoManager_h
#define __Alat_IoManager_h

#include  "parameterfile.h"

/*---------------------------------------------------------*/

namespace Alat
{
  class IoManager
  {
public:
    enum resultsubdir {
      Unknowns, PostProcess, RunInfo, MeshVisu
    };
private:
    const Alat::ParameterFile* _parameterfile;
    std::string _output_precision;

    std::string _rundir,  _resultsdir, _inputdir, _inputfile;
    std::string _datatype, _readunknownvariables;
private:
    std::string  resultSubDirToString(resultsubdir subdir) const;
public:
    ~IoManager();
    IoManager();
    IoManager(const Alat::IoManager& I);
    Alat::IoManager& operator=(const Alat::IoManager& I);

    std::string getName();
    void basicInit(const std::string& rundirectory, const Alat::ParameterFile* parameterfile = NULL);
    static std::string getFileNameOnBlock(const std::string& basefilename, int iblock, int level=0);
    std::ostream& printLoopInformation(std::ostream& os) const;
    const std::string& getRunDir() const;
    std::string getResultsDir() const;
    const std::string& getOutputPrecision() const;
    std::string& getOutputPrecision();
    const Alat::ParameterFile* getParameterFile();
    const std::string& getDataType() const;
    std::string getReadUnknownVariables() const;
    void setReadUnknownVariables(bool read);
    std::string getFileNameIn(int number = -1) const;
    std::string getFileNameOut(resultsubdir subdir, const std::string& name, int number = -1) const;
    std::string getDirectoryName( resultsubdir subdir) const;
  };
}

/*---------------------------------------------------------*/

#endif
