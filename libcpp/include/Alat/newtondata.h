#ifndef __Alat_NewtonData_h
#define __Alat_NewtonData_h

#include "enums.h"

/*-------------------------------------------------------------*/

namespace Alat
{
  class NewtonData
  {
public:
    int _n_linear_iter;
    int _n_newton_iter;
    int _n_redo_matrix;
    
protected:
    double _first_residual;
    mutable AlatEnums::residualstatus _residual_status;
    mutable AlatEnums::iterationstatus _linear_solver_status;
    mutable AlatEnums::newtonstatus _newton_status;
    mutable AlatEnums::matrixstatus _matrix_status;

public:
    NewtonData();
    std::string getName() const;

    int iteration, irelax;
    double residual, residual_old, modelfactor, regularization;
    bool use_linesearch, use_regularization;

    void reset();

    int getNLinearIteration() const;
    int& getNLinearIteration();
    int getNNewtonIteration() const;
    int& getNNewtonIteration();
    int getNRedoMatrix() const;
    int& getNRedoMatrix();
    double getFirstResidual() const;
    double& getFirstResidual();
    AlatEnums::residualstatus getResidualStatus() const;
    AlatEnums::residualstatus& getResidualStatus();
    AlatEnums::newtonstatus getNewtonStatus() const;
    AlatEnums::newtonstatus& getNewtonStatus();
    void setNewtonStatus(AlatEnums::newtonstatus status) const;
    AlatEnums::iterationstatus getLinearSolverStatus() const;
    AlatEnums::iterationstatus& getLinearSolverStatus();
    AlatEnums::matrixstatus getMatrixStatus() const;
    AlatEnums::matrixstatus& getMatrixStatus();
  };
  std::ostream& operator<<(std::ostream& os, const NewtonData& n);
}


/*-------------------------------------------------------*/

#endif
