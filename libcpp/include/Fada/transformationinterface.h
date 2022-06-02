#ifndef __Fada_TransformationInterface_h
#define __Fada_TransformationInterface_h

#include  "Alat/interfacebase.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class DoubleMatrix;
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
  class TransformationInterface : public Alat::InterfaceBase
  {
private:
protected:
    std::string getInterfaceName() const;
    virtual int getNNodes() const = 0;
    virtual void correctIntegrationPoint(Alat::Node& shat) const = 0;

public:
    ~TransformationInterface();
    TransformationInterface();
    TransformationInterface( const TransformationInterface& transformationinterface);
    TransformationInterface& operator=( const TransformationInterface& transformationinterface);
    virtual TransformationInterface* clone() const = 0;

    virtual void basicInit(const FadaMesh::MeshInterface* M) = 0;
    virtual void reInitCell(const Alat::Vector<Alat::Node>& nodes) = 0;
    virtual void reInitSide(int iiS) = 0;
    virtual void reInitPointCell() = 0;
    virtual void reInitPointCell(const Alat::Node& xhat) = 0;
    virtual void reInitPointSide(const Alat::Node& xhat) = 0;
    virtual void computeReferencePoint(Alat::Node& xhat, const Alat::Node& shat) = 0;
    virtual const Alat::Node& x() const = 0;

    // Matrice Jacobienne
    virtual const Alat::DoubleMatrix& getJacobian() const = 0;
    // Inverse
    virtual const Alat::DoubleMatrix& getInvJacobian() const = 0;
    // Determinant (Jacobien)
    virtual const double getDetJacobian() const = 0;
    // Jacobien surfacique
    virtual const double getSurfaceJacobian() const = 0;
    virtual const Alat::Vector<Alat::Node>& getNodes() const = 0;

    // Normale normalisée
    virtual const Alat::Node& getNormal() const = 0;

    // Normale non normalisée
    virtual const Alat::Node& getNonUnitNormal() const = 0;
    // h pour une face
    virtual const double getHOfSide() const = 0;
    // h pour une cellule
    virtual const double getHOfCell() const = 0;
  };
}

/*--------------------------------------------------------------------------*/

#endif
