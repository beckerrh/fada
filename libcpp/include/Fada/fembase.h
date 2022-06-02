#ifndef __Fada_FEMBase_h
#define __Fada_FEMBase_h

#include  "Alat/doublematrix.h"
#include  "Alat/doublevector.h"
#include  "Fada/feminterface.h"
#include  "FadaMesh/meshinterface.h"
#include  "Fada/transformationinterface.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class IntegrationFormulaInterface;
  class AssembleVector;
}
namespace Fada
{
  class FEMBase : public virtual Fada::FemInterface
  {
private:
// Pointeur sur la transformation
    Fada::TransformationInterface* _transformation;
    Fada::TransformationInterface* _transformationcurved;
    mutable bool _curved;
    Alat::Vector<Alat::Node> _dphi;
    Alat::DoubleVector _phi;
    Alat::Vector<Alat::Node> _d2phi;

protected:
    Fada::TransformationInterface*& getTransformationCurvedPointer();
    Fada::TransformationInterface*& getTransformationPointer();
    Fada::TransformationInterface* getTransformation();
    const Fada::TransformationInterface* getTransformation() const;
    const Alat::DoubleMatrix& getTransformationInverse() const;
    Alat::Vector<Alat::Node>& dphi();
    Alat::DoubleVector& phi();
    const Alat::Node& NonUnitNormal();

public:
    ~FEMBase();
    FEMBase();
    FEMBase(const FEMBase& orig);
    FEMBase& operator=(const FEMBase& F);
    std::string getName() const;

    const FadaMesh::MeshInterface* getMesh() const;
    void basicInit(const FadaMesh::MeshInterface* M);
    void reInitCell(const Alat::Vector<Alat::Node>& nodes);
    void reInitCellAndSide(const Alat::Vector<Alat::Node>& nodes, int iiS);
    const Alat::Node& x() const;
    double J() const;
    double G() const;
    double phi(int ii) const;
    const Alat::Node& dphi(int ii) const;
    const Alat::Node& getNormal() const;
    const Alat::Node& NonUnitNormal() const;
    // valeur de h pour une face
    double getHOfSide() const;
    // valeur de h pour un element
    double getHOfCell() const;
    void computeFunctionAndGradient(Alat::DoubleVector& u, Alat::Vector<Alat::Node>& du, const Alat::AssembleVector& uloc) const;
    void computeFunction(Alat::DoubleVector& u, const Alat::AssembleVector& uloc) const;
    void computeLaplacian(Alat::DoubleVector& lu, const Alat::AssembleVector& uloc) const;
    void reInitPhysicalPoint(const Alat::Node& x);
    void reInitPhysicalPoint1(const Alat::Node& x);
    void reInitPhysicalPoint2(const Alat::Node& x);
    void reInitReferenceCenterPoint();
    void reInitReferencePoint(const Alat::Node& xhat);
    void reInitReferencePointBoundary(const Alat::Node& shat);
  };
}

/*--------------------------------------------------------------------------*/

#endif
