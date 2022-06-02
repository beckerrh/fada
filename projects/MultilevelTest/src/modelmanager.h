#include  "Fada/modelmanager.h"

/*--------------------------------------------------------------------------*/

// namespace FadaMain
// {
  class ModelManager : public Fada::ModelManager
  {
public:
    ~ModelManager();
    ModelManager();
    ModelManager( const ModelManager& modelmanager);
    ModelManager& operator=( const ModelManager& modelmanager);
    std::string getName() const;
    ModelManager* clone() const;

    Fada::DomainModelInterface* constructModelByName(const std::string& modelname) const;
  };
// }

/*--------------------------------------------------------------------------*/
