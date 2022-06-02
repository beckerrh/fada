#ifndef  __Fada_enum_h
#define  __Fada_enum_h

#include  <string>

/*---------------------------------------------*/

namespace FadaEnums
{
  enum domain {BoundarySides, Cells, CouplingSides, InteriorSides};
  enum term {ComputeNormSquared, Error, Form, Initialize, Matrix, PostProcess, RHS, SortCriterion, TimeRHS, Visu};
  enum looptype {DynamicLoop, DynamicPostProcessLoop, ErrorEstimatorLoop, InterpolationLoop, LinearLoop, OwnLoop, PostProcessLoop, StaticLoop};

  std::string domainToString(domain d);
  std::string termToString(term t);
  std::string loopTypeToString(looptype l);
  looptype stringToLoopType(std::string string);

  int domain_begin();
  int domain_end();
  int term_begin();
  int term_end();
}

#endif
