#ifndef  __Alat_enum_h
#define  __Alat_enum_h

#include  <string>

/*---------------------------------------------*/

namespace AlatEnums
{
  enum iterationstatus {IterationStatusNone, IterationStatusConverged, IterationStatusDiverged, IterationStatusMaxIter, IterationStatusRunning, IterationStatusProblem};
  enum matrixstatus {MatrixStatusOk, MatrixStatusNotOk, MatrixStatusNone};
  enum residualstatus {ResidualStatusOk, ResidualStatusNotOk, ResidualStatusNone};
  enum newtonstatus {NewtonStatusResidualStatusNotOk, NewtonStatusConverged, NewtonStatusDiverged, NewtonStatusLinearNotOk, NewtonStatusNone, NewtonStatusBadReduction, NewtonStatusTooManyIterations, NewtonStatusMaxLineSearchAttained};

  std::string iterationStatusToString(iterationstatus i);
  std::string matrixStatusToString(matrixstatus r);
  std::string residualStatusToString(residualstatus r);
  std::string newtonStatusToString(newtonstatus n);
}

#endif
