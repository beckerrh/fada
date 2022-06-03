#ifndef ___Model_h
#define ___Model_h

#include  "Fada/model.h"

/*--------------------------------------------------------------------------*/
class Model : public Fada::Model
{
protected:
  mutable bool _dg;

public:
  ~Model();
  Model();
  Model( const Model& model);
  Model& operator=( const Model& model);
  std::string getName() const;

  void addStringsAndParameters(const Alat::ParameterFile* parameter_file, const FadaMesh::MeshInterface* mesh);
  int numberOfBoundaryConditionsPerColor() const;
  void definePossibleBoundaryConditions(Fada::ModelInterface::BoundaryConditionToVariables& bdryconditions) const;
  void defineVariables(Fada::VariableManager* variable_manager) const;
  void defineIntegrators(Fada::IntegratorManager* integratormanager) const;
  Fada::ApplicationInterface* newApplication(std::string applicationname) const;
};

/*--------------------------------------------------------------------------*/
#endif
