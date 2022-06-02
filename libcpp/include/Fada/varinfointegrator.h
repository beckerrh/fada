#ifndef __Fada_VarInfoIntegrator_h
#define __Fada_VarInfoIntegrator_h

// #include  "Alat/intvector.h"
#include  "Alat/map.h"
#include  "Alat/set.h"
#include  "Alat/stringvector.h"
#include  "Alat/stringset.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class FemManagerInterface;
  class FemManagerData;
  class FemInterface;
  class ProblemData;
  class VariableManager;
  template<class T>
  class Vector;

  class VarInfoIntegrator
  {
private:
    bool _basicinitcalled;
    const Fada::VariableManager* _variablemanager;
    const Fada::FemManagerInterface* _femmanager;

    Alat::StringIntMap _ncompdata, _ncompin, _ncompout, _ndoflocaldata, _ndoflocalin, _ndoflocalout;
    Alat::StringSet _var_output_names, _var_input_names, _var_data_names;
    Alat::Map<std::string, Alat::StringVector> _postproctype;
    Alat::StringSet _unknown_output_names, _unknown_input_names;
    Alat::StringSet _postprocess_name, _data_name;


    // Alat::IntVector _ncompdata, _ncompin, _ncompout, _ndoflocaldata, _ndoflocalin, _ndoflocalout;
    // Alat::StringIntMap  _index_of_varname_in, _index_of_varname_out, _index_of_varname_data;
    // Alat::StringVector _var_output_names, _var_input_names, _var_data_names;
    // Alat::Vector<Alat::StringVector> _postproctype;
    // Alat::StringVector _unknown_output_names, _unknown_input_names;
    // Alat::StringVector _postprocess_name, _data_name;
    // Alat::IntVector _indexunknownout, _indexpostprocess, _indexunknownin, _indexdata, _indexvarout;
    // Alat::IntVector _femindexin, _femindexout;

protected:
    bool varExists(const std::string& varname, const Fada::VariableManager* variablemanager) const;

public:
    ~VarInfoIntegrator();
    VarInfoIntegrator();
    VarInfoIntegrator( const VarInfoIntegrator& integrator);
    VarInfoIntegrator& operator=( const VarInfoIntegrator& integrator);
    void checkOutVariables(Alat::StringSet& nonexisting, const Fada::VariableManager* variablemanager) const;
    void basicInit(const Fada::VariableManager* variablemanager, const Fada::FemManagerInterface* femmanager);
    void defineVariables(const std::string& output, const std::string& input, const std::string& data);
    std::string getName() const;

    bool varNameIsInput(std::string varname) const;
    bool varNameIsOutput(std::string varname) const;
    const Alat::StringSet& getVarNamesOutput() const;
    const Alat::StringSet& getVarNamesInput() const;
    const Alat::StringSet& getVarNamesData() const;
    int getNDofLocalIn(std::string ivar) const;
    int getNDofLocalOut(std::string ivar) const;
    int getNCompData(std::string ivar) const;
    int getNCompIn(std::string ivar) const;
    int getNCompOut(std::string ivar) const;
    std::string getPostProcType(std::string ivar, int icomp) const;
    int getNVarData() const;
    int getNVarOutput() const;
    int getNVarInput() const;


    // const Alat::IntVector& getIndexUnknownVarIn() const;
    // const Alat::IntVector& getIndexUnknownVarOut() const;
    // const Alat::IntVector& getIndexData() const;
    // int getIndexUnknownVarIn(int ivar) const;
    // ???
    // int getIndexUnknownVarOut(int ivar) const;
    // int getIndexData(int ivar) const;
    // int getIndexVarOut(int ivar) const;
    // int getNVarData() const;
    // int getNVarOutput() const;
    // int getNVarInput() const;
    // std::string getVarNameOutput(int i) const;
    // std::string getVarNameInput(int i) const;
    // std::string getUnknownInputName(int index) const;
    // ???
    // std::string getUnknownOutputName(int index) const;
    // std::string getDataName(int index) const;
    // int getNDofLocalIn(int ivar) const;
    // int getNDofLocalOut(int ivar) const;
    // const Alat::IntVector& getNCompIn() const;
    // const Alat::IntVector& getNCompOut() const;
    // int getNCompData(int ivar) const;
    // int getNCompIn(int ivar) const;
    // int getNCompOut(int ivar) const;
    // std::string getPostProcType(int ivar, int icomp) const;

    // utilise que dans UnstructuredIntegrationLoop

    // utilise que dans lps/gls
    // const Alat::IntVector& getNDofLocalIn() const;
    // const Alat::IntVector& getNDofLocalOut() const;

    // int getIndexOfInput(std::string varname) const;
    // int getIndexOfOutput(std::string varname) const;
    // void setIThread();
    // int getFemIndexIn(int ivar) const;
    // int getFemIndexOut( int ivar) const;
  };
  std::ostream& operator<<(std::ostream& os, const VarInfoIntegrator& varinfointegrator);
}

/*--------------------------------------------------------------------------*/

#endif
