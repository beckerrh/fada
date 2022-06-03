#ifndef __Fada_MainInterface_h
#define __Fada_MainInterface_h

#include  "Alat/interfacebase.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class LoopInterface;
  class ModelInterface;
  class SolverManager;

  class MainInterface : public Alat::InterfaceBase
  {
protected:
    std::string getInterfaceName() const;

    virtual Fada::SolverManager* newSolverManager(std::string meshtype) const = 0;
    virtual Fada::LoopInterface* newLoop(std::string loopname) const = 0;
    virtual Fada::ModelInterface* newModel() const = 0;

public:
    ~MainInterface();
    MainInterface();
    MainInterface( const MainInterface& maininterface);
    MainInterface& operator=( const MainInterface& maininterface);
    std::string getName() const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
