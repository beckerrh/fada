// #ifndef __FadaTransport_Model_h
// #define __FadaTransport_Model_h

#include  "model.h"

/*--------------------------------------------------------------------------*/


// namespace FadaTransport
// {
  class Transport : public Model
  {
public:
    ~Transport();
    Transport();
    Transport( const Transport& model);
    Transport& operator=( const Transport& model);
    Fada::ModelInterface* clone() const;
    std::string getName() const;

    void addStringsAndParameters(const Alat::ParameterFile* parameter_file, const FadaMesh::MeshInterface* mesh);
    void defineIntegrators(Fada::IntegratorManager* integratormanager) const;
    Fada::ApplicationInterface* newApplication(std::string applicationname) const;
    void defineParametersForProblemData();
    Fada::DomainIntegrationLoopInterface* newDiscretization() const;
  };
// }

/*--------------------------------------------------------------------------*/

// #endif
