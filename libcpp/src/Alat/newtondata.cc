#include "Alat/newtondata.h"
#include  <iostream>

using namespace Alat;

/*-------------------------------------------------------------*/
NewtonData::NewtonData() : _residual_status(AlatEnums::ResidualStatusNone), _linear_solver_status(AlatEnums::IterationStatusNone), _newton_status(AlatEnums::NewtonStatusNone), _matrix_status(AlatEnums::MatrixStatusNone), _n_linear_iter(0), _n_newton_iter(0), _n_redo_matrix(0), use_linesearch(false), use_regularization(false)  {}

std::string NewtonData::getName() const
{
  return "NewtonData";
}

/*-------------------------------------------------------------*/
void NewtonData::reset()
{
  _residual_status = AlatEnums::ResidualStatusNone;
  _linear_solver_status = AlatEnums::IterationStatusNone;
  _newton_status = AlatEnums::NewtonStatusNone;
  _matrix_status = AlatEnums::MatrixStatusNone;
  _n_linear_iter = 0;
  _n_newton_iter = 0;
  _n_redo_matrix = 0;
  iteration = 0;
}
std::ostream& Alat::operator<<(std::ostream& os, const NewtonData& n)
{
  os << "(liniter/newtoniter/redomatrix/firstresidual:) " << n.getNLinearIteration() <<"/"<< n.getNNewtonIteration() <<"/"<< n.getNRedoMatrix()<<"/"<<n.getFirstResidual();
  return os;
}

/*-------------------------------------------------------------*/
int NewtonData::getNLinearIteration() const
{
  return _n_linear_iter;
}

int& NewtonData::getNLinearIteration()
{
  return _n_linear_iter;
}

int NewtonData::getNNewtonIteration() const
{
  return _n_newton_iter;
}

int& NewtonData::getNNewtonIteration()
{
  return _n_newton_iter;
}

int NewtonData::getNRedoMatrix() const
{
  return _n_redo_matrix;
}

int& NewtonData::getNRedoMatrix()
{
  return _n_redo_matrix;
}

double NewtonData::getFirstResidual() const
{
  return _first_residual;
}

double& NewtonData::getFirstResidual()
{
  return _first_residual;
}

AlatEnums::residualstatus NewtonData::getResidualStatus() const
{
  return _residual_status;
}

AlatEnums::residualstatus& NewtonData::getResidualStatus()
{
  return _residual_status;
}

AlatEnums::newtonstatus NewtonData::getNewtonStatus() const
{
  return _newton_status;
}

AlatEnums::newtonstatus& NewtonData::getNewtonStatus()
{
  return _newton_status;
}

void NewtonData::setNewtonStatus(AlatEnums::newtonstatus status) const
{
  _newton_status = status;
}

AlatEnums::iterationstatus NewtonData::getLinearSolverStatus() const
{
  return _linear_solver_status;
}

AlatEnums::iterationstatus& NewtonData::getLinearSolverStatus()
{
  return _linear_solver_status;
}

AlatEnums::matrixstatus NewtonData::getMatrixStatus() const
{
  return _matrix_status;
}

AlatEnums::matrixstatus& NewtonData::getMatrixStatus()
{
  return _matrix_status;
}
