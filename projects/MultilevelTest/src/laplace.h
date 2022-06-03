// #ifndef __FadaLaplace_Model_h
// #define __FadaLaplace_Model_h

#include  "model.h"

/*--------------------------------------------------------------------------*/


// namespace FadaLaplace
// {
  class Laplace : public Model
  {
public:
    ~Laplace();
    Laplace();
    Laplace( const Laplace& model);
    Laplace& operator=( const Laplace& model);
    Fada::ModelInterface* clone() const;
    std::string getName() const;

    Fada::DomainIntegrationLoopInterface* newDiscretization() const;
    void addStringsAndParameters(const Alat::ParameterFile* parameter_file, const FadaMesh::MeshInterface* mesh);
    void defineIntegrators(Fada::IntegratorManager* integratormanager) const;
    void defineParametersForProblemData();
  };
// }

/*--------------------------------------------------------------------------*/

// #endif
