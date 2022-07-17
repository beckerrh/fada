// #ifndef __Model_h
// #define __Model_h

#include  "laplace.h"

/*--------------------------------------------------------------------------*/


// namespace FadaConvectionDiffusionReaction
// {

namespace Fada
{
  class ModelInterface;
}

  class ConvectionDiffusionReaction : public Laplace
  {
public:
    ~ConvectionDiffusionReaction();
    ConvectionDiffusionReaction();
    ConvectionDiffusionReaction( const ConvectionDiffusionReaction& model);
    ConvectionDiffusionReaction& operator=( const ConvectionDiffusionReaction& model);
    Fada::ModelInterface* clone() const;
    std::string getName() const;

    void addStringsAndParameters(const Alat::ParameterFile* parameter_file, const FadaMesh::MeshInterface* mesh);
    void defineIntegrators(Fada::IntegratorManager* integratormanager) const;
    void defineParametersForProblemData();
  };
// }

/*--------------------------------------------------------------------------*/

// #endif
