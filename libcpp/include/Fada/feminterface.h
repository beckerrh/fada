#ifndef __Fada_FemInterface_h
#define __Fada_FemInterface_h

#include  "Alat/interfacebase.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class AssembleVector;
  template<int N, class T>
  class FixArray;
  class DoubleVector;
  class IntVector;
  template<class T>
  class Vector;
  class Node;
}
namespace FadaMesh
{
  class MeshInterface;
}

namespace Fada
{
  class IntegrationFormulaInterface;
  class LocalDofInformation;
  class TransformationInterface;

  class FemInterface : public Alat::InterfaceBase
  {
protected:
    std::string getInterfaceName() const;

public:
    ~FemInterface();
    FemInterface();
    FemInterface( const FemInterface& finiteelementinterface);
    FemInterface& operator=( const FemInterface& finiteelementinterface);
    virtual FemInterface* clone() const;

    virtual void basicInit(const FadaMesh::MeshInterface* M) = 0;

    virtual TransformationInterface* newTransformation() const = 0;
    virtual TransformationInterface* newTransformationCurved() const;
    virtual const TransformationInterface* getTransformation() const = 0;
    virtual TransformationInterface* getTransformation()  = 0;

    virtual const FadaMesh::MeshInterface* getMesh() const = 0;

    virtual bool interpolationMatrixWrong() const;
    virtual void interpolate(Alat::AssembleVector& uh, const Alat::AssembleVector& uH) const;

    /// utilise
    virtual void setDofDescription(Fada::LocalDofInformation& dof_description) const = 0;
    virtual int ndoflocal() const = 0;
    virtual int getOrder() const = 0;
    virtual const Alat::Node& x() const = 0;
    virtual double J() const = 0;
    virtual double G() const = 0;
    virtual const Alat::Node& getNormal() const = 0;

    virtual void reInitReferencePoint(const Alat::Node& xhat) = 0;
    virtual void reInitReferenceCenterPoint() = 0;
    virtual void reInitPhysicalPoint(const Alat::Node& x) = 0;
    virtual void reInitReferencePointBoundary(const Alat::Node& xhat) = 0;
    virtual void reInitCell(const Alat::Vector<Alat::Node>& nodes) = 0;
    virtual void reInitCellAndSide(const Alat::Vector<Alat::Node>& nodes, int ii) = 0;

    virtual std::string getReferenceElementType() const = 0;
    virtual void computeFunction(Alat::DoubleVector& u, const Alat::AssembleVector& uloc) const = 0;
    virtual void computeFunctionAndGradient(Alat::DoubleVector& u, Alat::Vector<Alat::Node>& du, const Alat::AssembleVector& uloc) const = 0;
    virtual void getReferenceCenterPoint(Alat::Node& xhat) const;
    virtual double getHOfSide() const = 0;
    virtual double getHOfCell() const = 0;
    virtual double phi(int ii) const = 0;
    virtual const Alat::Node& dphi(int ii) const = 0;
    virtual Fada::IntegrationFormulaInterface* newIntegrationFormulaTransfer() const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
