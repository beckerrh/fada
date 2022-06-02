#ifndef __Fada_UnStructuredIntegrationLoop_h
#define __Fada_UnStructuredIntegrationLoop_h

#include  "domainintegrationloop.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class SparsityPatternSoft;
}

namespace Fada
{
  class UnStructuredIntegrationLoop : public Fada::DomainIntegrationLoop
  {
private:
protected:
    void matrixConnectivityCells(const std::string& varnamei, const std::string& varnamej, Alat::SparsityPatternSoft& sparsitypatternsoft) const;
    void matrixConnectivityBoundarySides(const std::string& varnamei, const std::string& varnamej, Alat::SparsityPatternSoft& sparsitypatternsoft) const;
    void matrixConnectivityInteriorSides(const std::string& varnamei, const std::string& varnamej, Alat::SparsityPatternSoft& sparsitypatternsoft) const;

public:
    ~UnStructuredIntegrationLoop();
    UnStructuredIntegrationLoop();
    UnStructuredIntegrationLoop( const UnStructuredIntegrationLoop& unstructuredintegrationloop);
    UnStructuredIntegrationLoop& operator=( const UnStructuredIntegrationLoop& unstructuredintegrationloop);
    std::string getName() const;
    UnStructuredIntegrationLoop* clone() const;

    void matrixConnectivityPerLevel(const std::string& varnamei, const std::string& varnamej, const Fada::IntegratorManager* integratormanager, int level, int n, Alat::VariableMatrixInterface* matrix) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
