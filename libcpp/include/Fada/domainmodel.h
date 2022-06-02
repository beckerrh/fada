#ifndef __Fada_DomainModel_h
#define __Fada_DomainModel_h

#include  "Alat/map.h"
// #include  "Alat/stringmatrix.h"
#include  "applicationinterface.h"
#include  "boundarymanager.h"
#include  "domainmodelinterface.h"
#include  "functionalmap.h"
#include  "FadaMesh/meshinterface.h"
#include  "model.h"
#include  "Alat/parameterfile.h"
#include  "Alat/map.h"
#include  "Alat/pair.h"
#include  "parametermap.h"
#include  "parametertranslator.h"
#include  "problemdata.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class DomainModel : public virtual Model, public virtual DomainModelInterface
  {
protected:
  // Alat::StringMatrix<std::string> _matrixstorage;
  // Alat::Map<std::pair<std::string, std::string>, std::string> _matrixstorage;
  Alat::Map<Alat::StringPair, std::string> _matrixstorage;

public:
    ~DomainModel();
    DomainModel();
    DomainModel(const DomainModel& model);
    DomainModel& operator=(const DomainModel& model);
    DomainModelInterface* clone() const;

    void basicInit(const Alat::ParameterFile* parameterfile, const FadaMesh::MeshInterface* mesh, FadaEnums::looptype looptype);
    std::string matrixcoupling(const std::string& variableout, const std::string& variablsin) const;
    // void defineApplication(const Alat::StringSet& bdryconditions);
  };
}

/*--------------------------------------------------------------------------*/

#endif
