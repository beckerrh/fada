#ifndef __Fada_VisitorIntegrationLoop_h
#define __Fada_VisitorIntegrationLoop_h

#include  "Fada/fadalightenums.h"
#include  "Alat/systemassemblematrix.h"
#include  "Alat/systemassemblevector.h"
#include  "integrationloopinterface.h"
#include  "Fada/femfunctionsmap.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class AssembleVector;
  class IntVector;
  class StringDoubleMap;
}
namespace Alat
{
  class SystemMatrixInterface;
  class SystemVector;
}
namespace Alat
{
  class MatrixAllVariables;
  class SystemVector;
}

namespace Fada
{
  class CouplingMatrixInterface;
  class FemManagerInterface;
  class IntegratorInterface;
  class IntegratorInterface;
  class LocalGlobal;
  class LocalGlobalCoupling;
  class LocalVectors;
  class VariableManager;

  class VisitorIntegrationLoop
  {
  protected:
    const Fada::LocalGlobal& _localglobal;
    const Fada::VariableManager* _variablemanager;
    Alat::SystemVector* _vector;
    mutable Alat::SystemAssembleVector _flocL, _flocR;

  public:
    VisitorIntegrationLoop(const Fada::LocalGlobal& localglobal, const Fada::VariableManager* variablemanager, Alat::SystemVector* vector = NULL);
    void assembleOutput(int iK, const Alat::StringSet& variables);
    void assembleOutput(int iKL, int iKR, const Alat::StringSet& variablesL, const Alat::StringSet& variablesR);
    void setOuputZero() const;
    void setOuputZeroAll() const;
    Alat::SystemVector* getOutputVector();
    void setAssembleData(const Fada::IntegratorInterface* integrator) const;
  };

  class VisitorIntegrationLoopComputeNormSquared : public VisitorIntegrationLoop
  {
  protected:
    Alat::StringDoubleMap& _norms;

  public:
    VisitorIntegrationLoopComputeNormSquared(const Fada::LocalGlobal& localglobal, const Fada::VariableManager* variablemanager, Alat::StringDoubleMap& norms);
    FadaEnums::term getTerm() const;
    void perCell(AlatEnums::residualstatus& status, int iK, const Fada::IntegratorInterface* integrator, double weight, const Fada::FemFunctionsMap& femfcts) const;
    void perCellBeforeIntegration(AlatEnums::residualstatus& status, int iK, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectors) const;
    void assembleOutput(int iK, const Alat::StringSet& variables);
    void setOuputZero() const;
  };

  class VisitorIntegrationLoopPostProcess : public VisitorIntegrationLoop
  {
  public:
    VisitorIntegrationLoopPostProcess(const Fada::LocalGlobal& localglobal, const Fada::VariableManager* variablemanager, Alat::SystemVector* vector);
    FadaEnums::term getTerm() const;
    void perCell(AlatEnums::residualstatus& status, int iK, const Fada::IntegratorInterface* integrator, double weight, const Fada::FemFunctionsMap& femfcts) const;
    void perBoundarySide(AlatEnums::residualstatus& status, int iS, int iK, const Fada::IntegratorInterface* integrator, double weight, const Fada::FemFunctionsMap& femfcts, int color) const;
    void perCouplingSide(AlatEnums::residualstatus& status, int iKL, int iKR,const Fada::IntegratorInterface* integrator, double weight, double measureSmallSide, const Fada::FemFunctionsMap& femfctsL, const Fada::FemFunctionsMap& femfctsR) const;
    void perCouplingSideBeforeIntegration(AlatEnums::residualstatus& status, int iS, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectorsL, const Fada::LocalVectors& localvectorsR) const;
    void assembleOutput(int iK, const Alat::StringSet& variables);
    void assembleOutput(int iKL, int iKR, const Alat::StringSet& variablesL, const Alat::StringSet& variablesR);
    void perCellBeforeIntegration(AlatEnums::residualstatus& status, int iK, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectors) const;
    void perBoundarySideBeforeIntegration(AlatEnums::residualstatus& status, int iS, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectors, int color) const;
  };

  class VisitorIntegrationLoopRightHandSide : public VisitorIntegrationLoop
  {
  public:
    VisitorIntegrationLoopRightHandSide(const Fada::LocalGlobal& localglobal, const Fada::VariableManager* variablemanager, Alat::SystemVector* vector);
    FadaEnums::term getTerm() const;
    void perCell(AlatEnums::residualstatus& status, int iK, const Fada::IntegratorInterface* integrator, double weight, const Fada::FemFunctionsMap& femfcts) const;
    void perBoundarySide(AlatEnums::residualstatus& status, int iS, int iK, const Fada::IntegratorInterface* integrator, double weight, const Fada::FemFunctionsMap& femfcts, int color) const;
    void perCouplingSide(AlatEnums::residualstatus& status, int iKL, int iKR,const Fada::IntegratorInterface* integrator, double weight, double measureSmallSide, const Fada::FemFunctionsMap& femfctsL, const Fada::FemFunctionsMap& femfctsR) const;
    void perCouplingSideBeforeIntegration(AlatEnums::residualstatus& status, int iS, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectorsL, const Fada::LocalVectors& localvectorsR) const;
    void perCellBeforeIntegration(AlatEnums::residualstatus& status, int iK, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectors) const;
    void perBoundarySideBeforeIntegration(AlatEnums::residualstatus& status, int iS, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectors, int color) const;
  };

  class VisitorIntegrationLoopTimeRhs : public VisitorIntegrationLoop
  {
  private:
    double _d;
  public:
    VisitorIntegrationLoopTimeRhs(const Fada::LocalGlobal& localglobal, const Fada::VariableManager* variablemanager, Alat::SystemVector* vector, double d);
    FadaEnums::term getTerm() const;
    void perCell(AlatEnums::residualstatus& status, int iK, const Fada::IntegratorInterface* integrator, double weight, const Fada::FemFunctionsMap& femfcts) const;
    void perBoundarySide(AlatEnums::residualstatus& status, int iS, int iK, const Fada::IntegratorInterface* integrator, double weight, const Fada::FemFunctionsMap& femfcts, int color) const;
    void perCouplingSide(AlatEnums::residualstatus& status, int iKL, int iKR,const Fada::IntegratorInterface* integrator, double weight, double measureSmallSide, const Fada::FemFunctionsMap& femfctsL, const Fada::FemFunctionsMap& femfctsR) const;
    void perCouplingSideBeforeIntegration(AlatEnums::residualstatus& status, int iS, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectorsL, const Fada::LocalVectors& localvectorsR) const;
    void perCellBeforeIntegration(AlatEnums::residualstatus& status, int iK, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectors) const;
    void perBoundarySideBeforeIntegration(AlatEnums::residualstatus& status, int iS, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectors, int color) const;
  };

  class VisitorIntegrationLoopForm : public VisitorIntegrationLoop
  {
  public:
    VisitorIntegrationLoopForm(const Fada::LocalGlobal& localglobal, const Fada::VariableManager* variablemanager, Alat::SystemVector* vector);
    FadaEnums::term getTerm() const;
    void assembleOutput(int iK, const Alat::StringSet& variables);
    void assembleOutput(int iKL, int iKR, const Alat::StringSet& variablesL, const Alat::StringSet& variablesR);
    void perCell(AlatEnums::residualstatus& status, int iK, const Fada::IntegratorInterface* integrator, double weight, const Fada::FemFunctionsMap& femfcts) const;
    void perBoundarySide(AlatEnums::residualstatus& status, int iS, int iK, const Fada::IntegratorInterface* integrator, double weight, const Fada::FemFunctionsMap& femfcts, int color) const;
    void perCouplingSide(AlatEnums::residualstatus& status, int iKL, int iKR,const Fada::IntegratorInterface* integrator, double weight, double measureSmallSide, const Fada::FemFunctionsMap& femfctsL, const Fada::FemFunctionsMap& femfctsR) const;
    void perCouplingSideBeforeIntegration(AlatEnums::residualstatus& status, int iS, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectorsL, const Fada::LocalVectors& localvectorsR) const;
    void perCellBeforeIntegration(AlatEnums::residualstatus& status, int iK, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectors) const;
    void perBoundarySideBeforeIntegration(AlatEnums::residualstatus& status, int iS, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectors, int color) const;
  };

  class VisitorIntegrationLoopLinearization : public VisitorIntegrationLoopForm
  {
  public:
    VisitorIntegrationLoopLinearization(const Fada::LocalGlobal& localglobal, const Fada::VariableManager* variablemanager, Alat::SystemVector* vector);
    FadaEnums::term getTerm() const;
    void assembleOutput(int iK, const Alat::StringSet& variables);
    void assembleOutput(int iKL, int iKR, const Alat::StringSet& variablesL, const Alat::StringSet& variablesR);
    void assembleOutputEdge(int iE, const Alat::IntVector& iKCoE, const Alat::IntVector& indices, const Alat::StringSet& variables);
    void perCell(AlatEnums::residualstatus& status, int iK, const Fada::IntegratorInterface* integrator, double weight, const Fada::FemFunctionsMap& femfcts) const;
    void perBoundarySide(AlatEnums::residualstatus& status, int iS, int iK, const Fada::IntegratorInterface* integrator, double weight, const Fada::FemFunctionsMap& femfcts, int color) const;
    void perCouplingSide(AlatEnums::residualstatus& status, int iKL, int iKR,const Fada::IntegratorInterface* integrator, double weight, double measureSmallSide, const Fada::FemFunctionsMap& femfctsL, const Fada::FemFunctionsMap& femfctsR) const;
    void perCouplingSideBeforeIntegration(AlatEnums::residualstatus& status, int iS, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectorsL, const Fada::LocalVectors& localvectorsR) const;
    void perCellBeforeIntegration(AlatEnums::residualstatus& status, int iK, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectors) const;
    void perBoundarySideBeforeIntegration(AlatEnums::residualstatus& status, int iS, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectors, int color) const;
  };


  class VisitorIntegrationLoopMatrix
  {
  protected:
    const Fada::LocalGlobal& _localglobal;
    const Fada::VariableManager* _variablemanager;
    Alat::MatrixAllVariables* _matrix;
    mutable Alat::SystemAssembleMatrix _AlocLL, _AlocLR, _AlocRL, _AlocRR;
    std::string _couplingtype;

  public:
    VisitorIntegrationLoopMatrix( const LocalGlobal& localglobal, const VariableManager* variablemanager, Alat::SystemMatrixInterface* matrix, std::string couplingtype);
    Alat::SystemVector* getOutputVector();
    void setOuputZero() const;
    void setOuputZeroAll() const;
    FadaEnums::term getTerm() const;
    void assembleOutput(int iK, const Alat::StringSet& variables);
    void assembleOutput(int iKL, int iKR, const Alat::StringSet& variablesL, const Alat::StringSet& variablesR);
    void perCell(AlatEnums::residualstatus& status, int iK, const Fada::IntegratorInterface* integrator, double weight, const Fada::FemFunctionsMap& femfcts) const;
    void perBoundarySide(AlatEnums::residualstatus& status, int iS, int iK, const Fada::IntegratorInterface* integrator, double weight, const Fada::FemFunctionsMap& femfcts, int color) const;
    void perCouplingSide(AlatEnums::residualstatus& status, int iKL, int iKR,const Fada::IntegratorInterface* integrator, double weight, double measureSmallSide, const Fada::FemFunctionsMap& femfctsL, const Fada::FemFunctionsMap& femfctsR) const;
    void perCouplingSideBeforeIntegration(AlatEnums::residualstatus& status, int iS, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectorsL, const Fada::LocalVectors& localvectorsR) const;
    void perCellBeforeIntegration(AlatEnums::residualstatus& status, int iK, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectors) const;
    void perBoundarySideBeforeIntegration(AlatEnums::residualstatus& status, int iS, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectors, int color) const;
    void setAssembleData(const Fada::IntegratorInterface* integrator) const;
  };

  /*--------------------------------------------------------------------------*/
  /*--------------------------------------------------------------------------*/

  class VisitorIntegrationLoopPostProcessCoupling
  {
  protected:
    const Fada::LocalGlobalCoupling& _localglobal;
    Alat::SystemVector* _vectorL, * _vectorR;
    mutable Alat::SystemAssembleVector _flocL, _flocR;
    const Fada::VariableManager* _variablemanagerL, * _variablemanagerR;
  public:
    VisitorIntegrationLoopPostProcessCoupling(const Fada::LocalGlobalCoupling& localglobal, const Fada::VariableManager* variablemanagerL, const Fada::VariableManager* variablemanagerR, Alat::SystemVector* vectorL, Alat::SystemVector* vectorR);
    FadaEnums::term getTerm() const;
    void perCouplingSide(AlatEnums::residualstatus& status, int iKL, int iKR,const Fada::IntegratorInterface* integrator, double weight, double measureSmallSide, const Fada::FemFunctionsMap& femfctsL, const Fada::FemFunctionsMap& femfctsR) const;
    void perCouplingSideBeforeIntegration(AlatEnums::residualstatus& status, int iS, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectorsL, const Fada::LocalVectors& localvectorsR) const;
    void assembleOutput(int iKl, int iKr, const Alat::IntVector& indicesnodesleft, const Alat::IntVector& indicesnodesright, const Alat::StringSet& variablesL, const Alat::StringSet& variablesR);
    void setOuputZeroAll() const;
  };

  class VisitorIntegrationLoopTimeRhsCoupling
  {
  protected:
    double _d;
    const Fada::LocalGlobalCoupling& _localglobal;
    Alat::SystemVector* _vectorL, * _vectorR;
    mutable Alat::SystemAssembleVector _flocL, _flocR;
  public:
    VisitorIntegrationLoopTimeRhsCoupling(const Fada::LocalGlobalCoupling& localglobal, Alat::SystemVector* vectorL, Alat::SystemVector* vectorR, double d);
    FadaEnums::term getTerm() const;
    void perCouplingSide(AlatEnums::residualstatus& status, int iKL, int iKR,const Fada::IntegratorInterface* integrator, double weight, double measureSmallSide, const Fada::FemFunctionsMap& femfctsL, const Fada::FemFunctionsMap& femfctsR) const;
    void perCouplingSideBeforeIntegration(AlatEnums::residualstatus& status, int iS, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectorsL, const Fada::LocalVectors& localvectorsR) const;
    void assembleOutput(int iKl, int iKr, const Alat::IntVector& indicesnodesleft, const Alat::IntVector& indicesnodesright, const Alat::StringSet& variablesL, const Alat::StringSet& variablesR);
    void setOuputZeroAll() const;
  };

  class VisitorIntegrationLoopFormCoupling
  {
  protected:
    const Fada::LocalGlobalCoupling& _localglobal;
    Alat::SystemVector* _vectorL, * _vectorR;
    mutable Alat::SystemAssembleVector _flocL, _flocR;

  public:
    VisitorIntegrationLoopFormCoupling(const Fada::LocalGlobalCoupling& localglobal, Alat::SystemVector* vectorL, Alat::SystemVector* vectorR);
    FadaEnums::term getTerm() const;
    void perCouplingSide(AlatEnums::residualstatus& status, int iKL, int iKR,const Fada::IntegratorInterface* integrator, double weight, double measureSmallSide, const Fada::FemFunctionsMap& femfctsL, const Fada::FemFunctionsMap& femfctsR) const;
    void perCouplingSideBeforeIntegration(AlatEnums::residualstatus& status, int iS, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectorsL, const Fada::LocalVectors& localvectorsR) const;
    void assembleOutput(int iKl, int iKr, const Alat::IntVector& indicesnodesleft, const Alat::IntVector& indicesnodesright, const Alat::StringSet& variablesL, const Alat::StringSet& variablesR);
    void setOuputZeroAll() const;
  };

  class VisitorIntegrationLoopLinearizationCoupling : public VisitorIntegrationLoopFormCoupling
  {
  public:
    VisitorIntegrationLoopLinearizationCoupling(const Fada::LocalGlobalCoupling& localglobal, Alat::SystemVector* vectorL, Alat::SystemVector* vectorR);
    void perCouplingSide(AlatEnums::residualstatus& status, int iKL, int iKR,const Fada::IntegratorInterface* integrator, double weight, double measureSmallSide, const Fada::FemFunctionsMap& femfctsL, const Fada::FemFunctionsMap& femfctsR) const;
    void perCouplingSideBeforeIntegration(AlatEnums::residualstatus& status, int iS, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectorsL, const Fada::LocalVectors& localvectorsR) const;
    void assembleOutput(int iKl, int iKr, const Alat::IntVector& indicesnodesleft, const Alat::IntVector& indicesnodesright, const Alat::StringSet& variablesL, const Alat::StringSet& variablesR);
  };

  class VisitorIntegrationLoopSortCriterion
  {
  protected:
    const Fada::LocalGlobalCoupling& _localglobal;
    mutable double _value;

  public:
    VisitorIntegrationLoopSortCriterion(const Fada::LocalGlobalCoupling& localglobal);
    FadaEnums::term getTerm() const;
    bool getCriterion() const;
    void setOuputZeroAll() const;
    void perCouplingSide(AlatEnums::residualstatus& status, int iKL, int iKR,const Fada::IntegratorInterface* integrator, double weight, double measureSmallSide, const Fada::FemFunctionsMap& femfctsL, const Fada::FemFunctionsMap& femfctsR) const;
    void perCouplingSideBeforeIntegration(AlatEnums::residualstatus& status, int iS, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectorsL, const Fada::LocalVectors& localvectorsR) const;
    void assembleOutput(int iKl, int iKr, const Alat::IntVector& indicesnodesleft, const Alat::IntVector& indicesnodesright, const Alat::StringSet& variablesL, const Alat::StringSet& variablesR);
  };
}

#endif
