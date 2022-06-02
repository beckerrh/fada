#include  "Fada/applicationwithexactsolution.h"
#include  "Fada/domainmodel.h"
#include  "Alat/filescanner.h"
#include  "Fada/integrator.h"
#include  "Fada/integratormanager.h"
#include  "Fada/variablemanager.h"
#include  "Alat/tokenize.h"

using namespace Fada;

/*--------------------------------------------------------------------------*/
DomainModel::~DomainModel()
{}

DomainModel::DomainModel() : Model(), DomainModelInterface(){}
DomainModel::DomainModel(const DomainModel& model) : Model(model), DomainModelInterface(model){}

DomainModel& DomainModel::operator=(const DomainModel& model)
{
  Model::operator=(model);
}

DomainModelInterface* DomainModel::clone() const
{
  assert(0);
}

/*--------------------------------------------------------------------------*/
void DomainModel::basicInit(const Alat::ParameterFile* parameterfile, const FadaMesh::MeshInterface* mesh, FadaEnums::looptype looptype)
{
  std::string blockname ="Model_"+getName(); 
  std::string matrixstoragestring;
  Alat::DataFormatHandler dataformathandler;
  dataformathandler.insert("matrixstorage", &matrixstoragestring, "none");
  Alat::FileScanner filescanner(dataformathandler,parameterfile, blockname, 0);
  if(matrixstoragestring=="none")
  {
    _error_string("basicInit", "please give \"matrixstorage\" in block \"" + blockname+"\"");
  }

  Alat::StringVector matrixstoragevector = Alat::Tokenize(matrixstoragestring, "|");
  for(int ii = 0; ii < matrixstoragevector.size(); ii++)
  {
    Alat::StringVector matrixstoragetype = Alat::Tokenize(matrixstoragevector[ii], ":");
    Alat::StringVector matrixstoragevariable = Alat::Tokenize(matrixstoragetype[0], "-");
    assert(matrixstoragevariable.size() == 2);
    if(matrixstoragetype.size() == 1)
    {
      // _matrixstorage(matrixstoragevariable[0], matrixstoragevariable[1]) = "default";
      // _matrixstorage[make_pair(matrixstoragevariable[0], matrixstoragevariable[1])] = "default";
      _matrixstorage[Alat::makePair(matrixstoragevariable[0], matrixstoragevariable[1])] = "default";
    }
    else
    {
      assert(matrixstoragetype.size() == 2);
      // _matrixstorage(matrixstoragevariable[0], matrixstoragevariable[1]) = matrixstoragetype[1];
      // _matrixstorage[make_pair(matrixstoragevariable[0], matrixstoragevariable[1])] = matrixstoragetype[1];
      _matrixstorage[Alat::makePair(matrixstoragevariable[0], matrixstoragevariable[1])] = matrixstoragetype[1];
    }
  }

  Model::basicInit(parameterfile, mesh, looptype);
}

/*--------------------------------------------------------------------------*/
std::string DomainModel::matrixcoupling(const std::string& variableout, const std::string& variablein) const
{
  // if(_matrixstorage.find(make_pair(variableout, variablein))!=_matrixstorage.end())
  if(_matrixstorage.find(Alat::makePair(variableout, variablein))!=_matrixstorage.end())
  {
    // return _matrixstorage[make_pair(variableout, variablein)];
    return _matrixstorage[Alat::makePair(variableout, variablein)];
  }
  // if(_matrixstorage.hasKey(variableout, variablein))
  // {
  //   return _matrixstorage(variableout, variablein);
  // }
  else
  {
    return "none";
  }
}
