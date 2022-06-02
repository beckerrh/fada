#include  "Fada/fadalightenums.h"
#include  <assert.h>
#include  <iostream>

namespace FadaEnums
{
  // enum domain {BoundarySides, Cells, CouplingSides, EdgesAndCells, InteriorSides};
  int domain_begin()
  {
    return BoundarySides;
  }

  int domain_end()
  {
    return InteriorSides+1;
  }

  // enum term {ComputeNormSquared, Error, Form, Initialize, Matrix, PostProcess, RHS, SortCriterion, TimeRHS, Visu};
  int term_begin()
  {
    return ComputeNormSquared;
  }

  int term_end()
  {
    return Visu+1;
  }

  std::string domainToString(domain d)
  {
    if(d == BoundarySides)
    {
      return "BoundarySides";
    }
    else if(d == Cells)
    {
      return "Cells";
    }
    else if(d == CouplingSides)
    {
      return "CouplingSides";
    }
    else if(d == InteriorSides)
    {
      return "InteriorSides";
    }
    std::cerr<<"****domainToString: domain not defined: "<<d<<'\n';
    assert(0);
  }

  std::string termToString(term t)
  {
    if(t == Matrix)
    {
      return "Matrix";
    }
    else if(t == Form)
    {
      return "Form";
    }
    else if(t == RHS)
    {
      return "RHS";
    }
    else if(t == Initialize)
    {
      return "Initialize";
    }
    else if(t == Error)
    {
      return "PostProcess";
    }
    else if(t == Visu)
    {
      return "Visu";
    }
    else if(t == PostProcess)
    {
      return "PostProcess";
    }
    else if(t == TimeRHS)
    {
      return "TimeRHS";
    }
    else if(t == ComputeNormSquared)
    {
      return "ComputeNormSquared";
    }
    else if(t == SortCriterion)
    {
      return "SortCriterion";
    }
    else
    {
      std::cerr<<"****termToString: term not defined "<<t<<'\n';
      assert(0);
    }
  }

  std::string loopTypeToString(looptype l)
  {
    if(l == DynamicLoop)
    {
      return "dynamic";
    }
    else if(l == DynamicPostProcessLoop)
    {
      return "dynamicpostprocess";
    }
    else if(l == ErrorEstimatorLoop)
    {
      return "errorestimator";
    }
    else if(l == InterpolationLoop)
    {
      return "interpolation";
    }
    else if(l == LinearLoop)
    {
      return "linear";
    }
    else if(l == OwnLoop)
    {
      return "ownloop";
    }
    else if(l == PostProcessLoop)
    {
      return "postprocess";
    }
    else if(l == StaticLoop)
    {
      return "static";
    }
    std::cerr<<"****loopTypeToString: looptype not defined "<<l<<'\n';
    assert(0);
  }

  looptype stringToLoopType(std::string string)
  {
    if(string == "dynamic")
    {
      return DynamicLoop;
    }
    else if(string == "dynamicpostprocess")
    {
      return DynamicPostProcessLoop;
    }
    else if(string == "errorestimator")
    {
      return ErrorEstimatorLoop;
    }
    else if(string == "interpolation")
    {
      return InterpolationLoop;
    }
    else if(string == "linear")
    {
      return LinearLoop;
    }
    else if(string == "ownloop")
    {
      return OwnLoop;
    }
    else if(string == "postprocess")
    {
      return PostProcessLoop;
    }
    else if(string == "static")
    {
      return StaticLoop;
    }
    std::cerr<<"****stringToLoopType: looptype not defined "<<string<<'\n';
    assert(0);
  }
}
