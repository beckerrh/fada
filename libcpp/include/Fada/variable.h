#ifndef __Fada_Variable_h
#define __Fada_Variable_h

#include  "Alat/stringvector.h"
#include  "variableinterface.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class Variable : public VariableInterface
  {
private:
    std::string _varname, _visutype, _femname;
    int _ncomp;
    bool _timeintegral;
    mutable Alat::StringVector _visunames;
    Alat::StringVector _postproctype;

protected:
public:
    ~Variable();
    Variable();
    Variable( const Variable& variable);
    Variable( std::string varname, const Variable& variable);
    Variable( std::string varname, std::string femname, int ncomp, std::string visutype = "node");
    Variable( std::string varname, int ncomp, std::string postproctype = "none", bool timeintegral = false);
    Variable& operator=( const Variable& variable);
    std::ostream& write(std::ostream& os) const;

    std::string getName() const;
    std::string getVarName() const;
    std::string getFemName() const;
    int getNComponents() const;
    int getN() const;

    std::string getVisualizationType() const;
    std::string getPostProcType(int icomp) const;
    void setPostProcType(int icomp, std::string type);
    void getVisualizationNames(Alat::StringVector& visunames) const;
    void setVisualizationName(int icomp, const std::string& name);
    const std::string&  getVisualizationName(int icomp) const;
    void setVisualizationNames(Alat::StringVector& visunames) const;
    bool timeIntegral() const;
  };
  std::ostream& operator<<(std::ostream& os, const Variable& variable);
}

/*--------------------------------------------------------------------------*/

#endif
