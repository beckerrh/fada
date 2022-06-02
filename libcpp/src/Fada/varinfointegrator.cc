#include  "Fada/dofinformationinterface.h"
#include  "Fada/dofmanagerallvariables.h"
#include  "Fada/femmanager.h"
#include  "Fada/feminterface.h"
#include  "Fada/variablemanager.h"
#include  "Alat/variablevector.h"
#include  "Fada/varinfointegrator.h"
// #include  "Alat/stlio.h"
#include  "Alat/stringset.h"
#include  "Alat/tokenize.h"
#include  <cassert>

using namespace Fada;


/*--------------------------------------------------------------------------*/
VarInfoIntegrator::~VarInfoIntegrator(){}
VarInfoIntegrator::VarInfoIntegrator() : _basicinitcalled(false){}
VarInfoIntegrator::VarInfoIntegrator( const VarInfoIntegrator& varinfo)
{
  _variablemanager = varinfo._variablemanager;
  _femmanager = varinfo._femmanager;
  _ncompdata = varinfo._ncompdata;
  _ncompin = varinfo._ncompin;
  _ncompout = varinfo._ncompout;
  _ndoflocaldata = varinfo._ndoflocaldata;
  _ndoflocalin = varinfo._ndoflocalin;
  _ndoflocalout = varinfo._ndoflocalout;
  _var_output_names = varinfo._var_output_names;
  _var_input_names = varinfo._var_input_names;
  _var_data_names = varinfo._var_data_names;
  _unknown_output_names = varinfo._unknown_output_names;
  _unknown_input_names = varinfo._unknown_input_names;
  _postprocess_name = varinfo._postprocess_name;
  _data_name = varinfo._data_name;
  
  // _var_output_names.set_size( varinfo._var_output_names.size() );
  // _var_output_names = varinfo._var_output_names;
  // _var_input_names.set_size( varinfo._var_input_names.size() );
  // _var_input_names = varinfo._var_input_names;
  // _var_data_names.set_size( varinfo._var_data_names.size() );
  // _var_data_names = varinfo._var_data_names;
  // _postproctype.set_size( varinfo._postproctype.size() );
  // for(int i = 0; i < varinfo._postproctype.size(); i++)
  // {
  //   _postproctype[i].set_size( varinfo._postproctype[i].size() );
  //   _postproctype[i] = varinfo._postproctype[i];
  // }
  // _unknown_output_names.set_size( varinfo._unknown_output_names.size() );
  // _unknown_output_names = varinfo._unknown_output_names;
  // _unknown_input_names.set_size( varinfo._unknown_input_names.size() );
  // _unknown_input_names = varinfo._unknown_input_names;
  // _postprocess_name.set_size( varinfo._postprocess_name.size() );
  // _postprocess_name = varinfo._postprocess_name;
  // _index_of_varname_in = varinfo._index_of_varname_in;
  // _index_of_varname_out = varinfo._index_of_varname_out;
  // _index_of_varname_data = varinfo._index_of_varname_data;
  // _ncompdata.set_size( varinfo._ncompdata.size() );
  // _ncompdata = varinfo._ncompdata;
  // _ncompin.set_size( varinfo._ncompin.size() );
  // _ncompin = varinfo._ncompin;
  // _ncompout.set_size( varinfo._ncompout.size() );
  // _ncompout = varinfo._ncompout;
  // _ndoflocaldata.set_size( varinfo._ndoflocaldata.size() );
  // _ndoflocaldata = varinfo._ndoflocaldata;
  // _ndoflocalin.set_size( varinfo._ndoflocalin.size() );
  // _ndoflocalin = varinfo._ndoflocalin;
  // _ndoflocalout.set_size( varinfo._ndoflocalout.size() );
  // _ndoflocalout = varinfo._ndoflocalout;
  // _indexvarout.set_size( varinfo._indexvarout.size() );
  // _indexvarout = varinfo._indexvarout;
  // _indexunknownout.set_size( varinfo._indexunknownout.size() );
  // _indexunknownout = varinfo._indexunknownout;
  // _indexpostprocess.set_size( varinfo._indexpostprocess.size() );
  // _indexpostprocess = varinfo._indexpostprocess;
  // _indexunknownin.set_size( varinfo._indexunknownin.size() );
  // _indexunknownin = varinfo._indexunknownin;
  // _indexdata.set_size( varinfo._indexdata.size() );
  // _indexdata = varinfo._indexdata;
}

VarInfoIntegrator& VarInfoIntegrator::operator=( const VarInfoIntegrator& varinfo)
{
  VarInfoIntegrator::operator=(varinfo);
  assert(0);
  return *this;
}
std::string VarInfoIntegrator::getName() const
{
  return "VarInfoIntegrator";
}

/*--------------------------------------------------------------------------*/
std::ostream& Fada::operator<<(std::ostream& os, const VarInfoIntegrator& varinfointegrator)
{
  os << "getVarNamesOutput()="<< varinfointegrator.getVarNamesOutput()<<"\n";
  os << "getVarNamesInput()="<< varinfointegrator.getVarNamesInput()<<"\n";
  os << "getVarNamesData()="<< varinfointegrator.getVarNamesData()<<"\n";
  return os;
}
/*--------------------------------------------------------------------------*/
int VarInfoIntegrator::getNDofLocalIn(std::string ivar) const
{
  return _ndoflocalin[ivar];
}
int VarInfoIntegrator::getNDofLocalOut(std::string ivar) const
{
  return _ndoflocalout[ivar];
}
int VarInfoIntegrator::getNCompData(std::string ivar) const
{
  return _ncompdata[ivar];
}
int VarInfoIntegrator::getNCompIn(std::string ivar) const
{
  return _ncompin[ivar];
}
int VarInfoIntegrator::getNCompOut(std::string ivar) const
{
  return _ncompout[ivar];
}
std::string VarInfoIntegrator::getPostProcType(std::string ivar, int icomp) const
{
  return _postproctype[ivar][icomp];
}

/*--------------------------------------------------------------------------*/
void VarInfoIntegrator::defineVariables(const std::string& output, const std::string& input, const std::string& data)
{
  {
    std::string s = output;
    s.erase( s.begin() );
    s.erase(s.size()-1);
    Alat::StringVector v = Alat::Tokenize(s, ",");
    // _var_output_names.set_size( v.size() );
    _var_output_names.clear();
    for(int i = 0; i < v.size(); i++)
    {
      _var_output_names.insert(v[i]);
      // std::string name = v[i];
      // _var_output_names[i] = name;
      // _index_of_varname_out[name] = i;
    }
  }
  {
    std::string s = input;
    s.erase( s.begin() );
    s.erase(s.size()-1);
    Alat::StringVector v = Alat::Tokenize(s, ",");
    // _var_input_names.set_size( v.size() );
    _var_input_names.clear();
    for(int i = 0; i < v.size(); i++)
    {
      _var_input_names.insert(v[i]);
      // std::string name = v[i];
      // _var_input_names[i] = name;
      // _index_of_varname_in[name] = i;
    }
  }
  {
    std::string s = data;
    s.erase( s.begin() );
    s.erase(s.size()-1);
    Alat::StringVector v = Alat::Tokenize(s, ",");
    // _var_data_names.set_size( v.size() );
    _var_data_names.clear();
    for(int i = 0; i < v.size(); i++)
    {
      _var_data_names.insert(v[i]);
      // std::string name = v[i];
      // _var_data_names[i] = name;
      // _index_of_varname_data[name] = i;
    }
  }
}

/*--------------------------------------------------------------------------*/

bool VarInfoIntegrator::varExists(const std::string& varname, const Fada::VariableManager* variablemanager) const
{
  return variablemanager->variableIsUnknown(varname) | variablemanager->variableIsData(varname) | variablemanager->variableIsPostProcess(varname);
}

/*--------------------------------------------------------------------------*/
void VarInfoIntegrator::checkOutVariables(Alat::StringSet& nonexisting, const Fada::VariableManager* variablemanager) const
{
  for(Alat::StringSet::const_iterator p = _var_output_names.begin(); p!=_var_output_names.end();p++)
  {
    if( not varExists(*p, variablemanager) )
    {
      nonexisting.insert(*p);
    }    
  }
  // for(int ivar = 0; ivar < _var_output_names.size(); ivar++)
  // {
  //   if( not varExists(_var_output_names[ivar], variablemanager) )
  //   {
  //     nonexisting.insert(_var_output_names[ivar]);
  //   }
  // }
}

/*--------------------------------------------------------------------------*/
void VarInfoIntegrator::basicInit(const Fada::VariableManager* variablemanager, const Fada::FemManagerInterface* femmanager)
{
  assert(not _basicinitcalled);
  _basicinitcalled = true;
  _variablemanager = variablemanager;
  _femmanager = femmanager;
  // std::cerr<<"VarInfoIntegrator::basicInit "<<_femmanager<<"  "<<_femmanager<<'\n';
  
  for(Alat::StringSet::const_iterator p = _var_input_names.begin(); p!=_var_input_names.end();p++)
  {
    if( not varExists(*p, variablemanager) )
    {
      std::cerr << "VarInfoIntegrator::basicInit() input variable does not exist: "<< *p <<"\n";
      exit(1);      
    }
    // const VariableInterface* var = _variablemanager->getInputVariable(_var_input_names[ivar]);
    const VariableInterface* var = _variablemanager->getInputVariable(*p);
    _ncompin[*p] = var->getNComponents();
    const FemInterface* fem = _femmanager->getFem(*p);
    if( fem )
    {
      _ndoflocalin[*p] = fem->ndoflocal();
    }
    else
    {
      _ndoflocalin[*p] = 1;
    }
  }
  for(Alat::StringSet::const_iterator p = _var_output_names.begin(); p!=_var_output_names.end();p++)
  {
    if( not varExists(*p, variablemanager) )
    {
      std::cerr << "VarInfoIntegrator::basicInit() output variable does not exist: "<< *p <<"\n";
      exit(1);      
    }
    // const VariableInterface* var = _variablemanager->getOutputVariable(_var_output_names[ivar]);
    const VariableInterface* var = _variablemanager->getOutputVariable(*p);
    _ncompout[*p] = var->getNComponents();
    const FemInterface* fem = _femmanager->getFem(*p);
    if(fem)
    {
      _ndoflocalout[*p] = fem->ndoflocal();
    }
    else
    {
      _ndoflocalout[*p] = -1;
      _postproctype[*p].set_size(_ncompout[*p]);
      for(int icomp = 0; icomp < _ncompout[*p]; icomp++)
      {
        _postproctype[*p][icomp] = var->getPostProcType(icomp);
      }
    }
  }
  for(Alat::StringSet::const_iterator p = _var_data_names.begin(); p!=_var_data_names.end();p++)
  {
    if( not varExists(*p, variablemanager) )
    {
      std::cerr << "VarInfoIntegrator::basicInit() data variable does not exist: "<< *p <<"\n";
      exit(1);      
    }
    const VariableInterface* var = _variablemanager->getData(*p);
    _ncompdata[*p] = var->getNComponents();
    const FemInterface* fem = _femmanager->getFem(*p);
    if(fem)
    {
      _ndoflocaldata[*p] = fem->ndoflocal();
    }
    else
    {
      // ne sait pas encore gerer cela
      // assert(0);
      _ndoflocaldata[*p] = -1;
    }
  }
  // _ncompin.set_size(nvarin);
  // _ncompout.set_size(nvarout);
  // _ncompdata.set_size(nvardata);
  // _ndoflocalin.set_size(nvarin);
  // _ndoflocalout.set_size(nvarout);
  // _ndoflocaldata.set_size(nvardata);
  // _postproctype.set_size(nvarout);
  // for(int ivar = 0; ivar < nvarout; ivar++)
  // {
  //   if( !varExists(_var_output_names[ivar], variablemanager) )
  //   {
  //     continue;
  //   }
  //   const VariableInterface* var = _variablemanager->getOutputVariable(_var_output_names[ivar]);
  //   _ncompout[ivar] = var->getNComponents();
  //   // FemInterface* fem = getFemOut(ivar);
  //   FemInterface* fem = _femmanager->getFem(_var_output_names[ivar]);
  //   if(fem)
  //   {
  //     _ndoflocalout[ivar] = fem->ndoflocal();
  //   }
  //   else
  //   {
  //     _ndoflocalout[ivar] = -1;
  //     _postproctype[ivar].set_size(_ncompout[ivar]);
  //     for(int icomp = 0; icomp < _ncompout[ivar]; icomp++)
  //     {
  //       _postproctype[ivar][icomp] = var->getPostProcType(icomp);
  //     }
  //   }
  // }
  // // std::cerr << "_ncompout="<<_ncompout<<"\n";
  // for(int ivar = 0; ivar < nvarin; ivar++)
  // {
  //   if( !varExists(_var_input_names[ivar], variablemanager) )
  //   {
  //     continue;
  //   }
  //   const VariableInterface* var = _variablemanager->getInputVariable(_var_input_names[ivar]);
  //   _ncompin[ivar] = var->getNComponents();
  //   // FemInterface* fem = getFemIn(ivar);
  //   FemInterface* fem = _femmanager->getFem(_var_input_names[ivar]);
  //   if( fem )
  //   {
  //     _ndoflocalin[ivar] = fem->ndoflocal();
  //   }
  //   else
  //   {
  //     _ndoflocalin[ivar] = 1;
  //   }
  // }
  // for(int ivar = 0; ivar < nvardata; ivar++)
  // {
  //   if( !varExists(_var_data_names[ivar], variablemanager) )
  //   {
  //     continue;
  //   }
  //   const VariableInterface* var = _variablemanager->getData(_var_data_names[ivar]);
  //   _ncompdata[ivar] = var->getNComponents();
  //   FemInterface* fem = _femmanager->getFem(_var_data_names[ivar]);
  //   if(fem)
  //   {
  //     _ndoflocaldata[ivar] = fem->ndoflocal();
  //   }
  //   else
  //   {
  //     // ne sait pas encore gerer cela
  //     // assert(0);
  //     _ndoflocaldata[ivar] = -1;
  //   }
  // }
  // int nvarunknownout(0), nvarpostprocess(0);
  // for(int ivar = 0; ivar < nvarout; ivar++)
  // {
  //   if( !varExists(_var_output_names[ivar], variablemanager) )
  //   {
  //     continue;
  //   }
  //   if( _variablemanager->variableIsUnknown(_var_output_names[ivar]) )
  //   {
  //     nvarunknownout++;
  //   }
  //   else
  //   {
  //     nvarpostprocess++;
  //   }
  // }
  // std::cerr << "########## nvarin nvarunknownout nvarpostprocess nvardata " << nvarin << " " << nvarunknownout << " " << nvarpostprocess << " " << nvardata << "\n";
  // std::cerr << "########## _ncompin " << _ncompin << "\n";
  // std::cerr << "########## _ncompout " << _ncompout << "\n";
  // std::cerr << "########## _ndoflocalin " << _ndoflocalin << "\n";
  // std::cerr << "########## _ndoflocalout " << _ndoflocalout << "\n";
  // if(nvarunknownout > 0)
  // {
  //   assert(nvarpostprocess == 0);
  // }
  // if(nvarpostprocess > 0)
  // {
  //   assert(nvarunknownout == 0);
  // }
  //
  // _indexunknownin.set_size(nvarin);
  // _indexvarout.set_size(nvarout);
  // _indexunknownout.set_size(nvarunknownout);
  // // _unknown_input_names.set_size(nvarunknownin);
  // _unknown_output_names.set_size(nvarunknownout);
  //
  // _indexdata.set_size(nvardata);
  // // _data_name.set_size(nvardata);
  // _indexpostprocess.set_size(nvarpostprocess);
  // _postprocess_name.set_size(nvarpostprocess);
  //
  // int count(0), count2(0);
  // for(int ivar = 0; ivar < nvarin; ivar++)
  // {
  //   if( !varExists(_var_input_names[ivar], variablemanager) )
  //   {
  //     continue;
  //   }
  //   std::string name = _var_input_names[ivar];
  //   _indexunknownin[count++] = _variablemanager->getIndexOfUnknownVariable(name);
  // }
  // count = 0;
  // for(int ivar = 0; ivar < nvardata; ivar++)
  // {
  //   if( !varExists(_var_data_names[ivar], variablemanager) )
  //   {
  //     continue;
  //   }
  //   std::string name = _var_data_names[ivar];
  //   _indexdata[count++] = _variablemanager->getIndexOfDataVariable(name);
  // }
  // count = 0;
  // for(int ivar = 0; ivar < nvarout; ivar++)
  // {
  //   if( !varExists(_var_output_names[ivar], variablemanager) )
  //   {
  //     continue;
  //   }
  //   std::string name = _var_output_names[ivar];
  //   int index;
  //   if( _variablemanager->variableIsUnknown(name) )
  //   {
  //     index = _variablemanager->getIndexOfUnknownVariable(name);
  //     _indexunknownout[count] = index;
  //     _unknown_output_names[count++] = name;
  //   }
  //   else
  //   {
  //     index = _variablemanager->getIndexOfPostProcessVariable(name);
  //     _indexpostprocess[count2] = index;
  //     _postprocess_name[count2++] = name;
  //   }
  //   _indexvarout[ivar] = index;
  // }
  //
  // // std::cerr << "VarInfoIntegrator::basicInit() _indexunknownout="<<_indexunknownout << "\n";
  // // std::cerr << "VarInfoIntegrator::basicInit() _indexunknownin="<<_indexunknownin << "\n";
  // // std::cerr << "VarInfoIntegrator::basicInit() _indexvarout="<<_indexvarout << "\n";
  //
  // _femindexout.set_size( _indexunknownout.size() );
  // for(int ivar = 0; ivar < _femindexout.size(); ivar++)
  // {
  //   _femindexout[ivar] = femmanager->getUnknownIndex2Femindex( _indexunknownout[ivar] );
  // }
  // _femindexin.set_size(nvarin);
  // for(int ivar = 0; ivar < nvarin; ivar++)
  // {
  //   _femindexin[ivar] = femmanager->getUnknownIndex2Femindex( _indexunknownin[ivar] );
  // }
  // std::cerr << "VarInfoIntegrator::basicInit() _femindexout="<<_femindexout << "\n";
  // std::cerr << "VarInfoIntegrator::basicInit() _femindexin="<<_femindexin << "\n";
}
//
// /*--------------------------------------------------------------------------*/
//
// const Alat::IntVector& VarInfoIntegrator::getNCompIn() const
// {
//   return _ncompin;
// }
//
// /*--------------------------------------------------------------------------*/
//
// const Alat::IntVector& VarInfoIntegrator::getNCompOut() const
// {
//   return _ncompout;
// }
//
// /*--------------------------------------------------------------------------*/
//
// const Alat::IntVector& VarInfoIntegrator::getNDofLocalIn() const
// {
//   return _ndoflocalin;
// }
//
// const Alat::IntVector& VarInfoIntegrator::getNDofLocalOut() const
// {
//   return _ndoflocalout;
// }
//
// int VarInfoIntegrator::getNDofLocalIn(int ivar) const
// {
//   return _ndoflocalin[ivar];
// }
//
// int VarInfoIntegrator::getNDofLocalOut(int ivar) const
// {
//   return _ndoflocalout[ivar];
// }
//
// /*--------------------------------------------------------------------------*/
//
// int VarInfoIntegrator::getNCompData(int ivar) const
// {
//   return _ncompdata[ivar];
// }
//
// int VarInfoIntegrator::getNCompIn(int ivar) const
// {
//   return _ncompin[ivar];
// }
//
// int VarInfoIntegrator::getNCompOut(int ivar) const
// {
//   assert(_basicinitcalled);
//   return _ncompout[ivar];
// }
//
// /*--------------------------------------------------------------------------*/
//
// bool VarInfoIntegrator::varNameIsInput(std::string varname) const
// {
//   return _index_of_varname_in.find(varname) != _index_of_varname_in.end();
// }
//
// /*--------------------------------------------------------------------------*/
//
// bool VarInfoIntegrator::varNameIsOutput(std::string varname) const
// {
//   return _index_of_varname_out.find(varname) != _index_of_varname_out.end();
// }
//
// /*--------------------------------------------------------------------------*/
//
// int VarInfoIntegrator::getIndexOfInput(std::string varname) const
// {
//   return _index_of_varname_in[varname];
// }
//
// /*--------------------------------------------------------------------------*/
//
// int VarInfoIntegrator::getIndexOfOutput(std::string varname) const
// {
//   return _index_of_varname_out[varname];
// }
//
// /*--------------------------------------------------------------------------*/
//
// std::string VarInfoIntegrator::getPostProcType(int ivar, int icomp) const
// {
//   return _postproctype[ivar][icomp];
// }
//
// /*--------------------------------------------------------------------------*/
//
// const Alat::IntVector& VarInfoIntegrator::getIndexUnknownVarIn() const
// {
//   return _indexunknownin;
// }
//
// /*--------------------------------------------------------------------------*/
//
// const Alat::IntVector& VarInfoIntegrator::getIndexUnknownVarOut() const
// {
//   return _indexunknownout;
// }
//
// /*--------------------------------------------------------------------------*/
//
// const Alat::IntVector& VarInfoIntegrator::getIndexData() const
// {
//   return _indexdata;
// }
//
// /*--------------------------------------------------------------------------*/
//
// int VarInfoIntegrator::getIndexUnknownVarIn(int ivar) const
// {
//   assert(_basicinitcalled);
//   assert( ivar < _indexunknownin.size() );
//   return _indexunknownin[ivar];
// }
//
// /*--------------------------------------------------------------------------*/
//
// int VarInfoIntegrator::getIndexUnknownVarOut(int ivar) const
// {
//   assert(_basicinitcalled);
//   assert( ivar < _indexunknownout.size() );
//   return _indexunknownout[ivar];
// }
//
// /*--------------------------------------------------------------------------*/
//
// int VarInfoIntegrator::getIndexData(int ivar) const
// {
//   assert(_basicinitcalled);
//   assert( ivar < _indexdata.size() );
//   return _indexdata[ivar];
// }
//
// int VarInfoIntegrator::getIndexVarOut(int ivar) const
// {
//   assert(_basicinitcalled);
//   assert( ivar < _indexvarout.size() );
//   return _indexvarout[ivar];
// }
//
// /*--------------------------------------------------------------------------*/
//
// int VarInfoIntegrator::getNVarData() const
// {
//   return _var_data_names.size();
// }
//
// int VarInfoIntegrator::getNVarOutput() const
// {
//   return _var_output_names.size();
// }
//
// int VarInfoIntegrator::getNVarInput() const
// {
//   return _var_input_names.size();
// }
//
// /*--------------------------------------------------------------------------*/
//
// std::string VarInfoIntegrator::getVarNameOutput(int i) const
// {
//   assert( i < _var_output_names.size() );
//   return _var_output_names[i];
// }
//
// /*--------------------------------------------------------------------------*/
//
// std::string VarInfoIntegrator::getVarNameInput(int i) const
// {
//   assert( i < _var_input_names.size() );
//   return _var_input_names[i];
// }
//
// /*--------------------------------------------------------------------------*/
//
// std::string VarInfoIntegrator::getUnknownInputName(int index) const
// {
//   assert( index < _var_input_names.size() );
//   return _var_input_names[index];
// }
//
// /*--------------------------------------------------------------------------*/
//
// std::string VarInfoIntegrator::getUnknownOutputName(int index) const
// {
//   assert( index < _unknown_output_names.size() );
//   return _unknown_output_names[index];
// }
//
// /*--------------------------------------------------------------------------*/
//
// std::string VarInfoIntegrator::getDataName(int index) const
// {
//   return _var_data_names[index];
// }
//
/*--------------------------------------------------------------------------*/

const Alat::StringSet& VarInfoIntegrator::getVarNamesOutput() const
{
  return _var_output_names;
}

const Alat::StringSet& VarInfoIntegrator::getVarNamesInput() const
{
  return _var_input_names;
}

const Alat::StringSet& VarInfoIntegrator::getVarNamesData() const
{
  return _var_data_names;
}
int VarInfoIntegrator::getNVarData() const
{
  return _var_data_names.size();
}
int VarInfoIntegrator::getNVarOutput() const
{
  return _var_output_names.size();
}
int VarInfoIntegrator::getNVarInput() const
{
  return _var_input_names.size();
}

//
// /*--------------------------------------------------------------------------*/
// int VarInfoIntegrator::getFemIndexIn(int ivar) const
// {
//   return _femindexin[ivar];
// }
//
// int VarInfoIntegrator::getFemIndexOut( int ivar) const
// {
//   // std::cerr << "VarInfoIntegrator::getFemIndexOut() _femindexout="<<_femindexout<<"\n";
//   assert( ivar < _femindexout.size() );
//   return _femindexout[ivar];
// }
