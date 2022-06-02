#ifndef __Fada_TransformationBase_h
#define __Fada_TransformationBase_h

#include  "Alat/doublematrix.h"
#include  "Alat/doublevector.h"
#include  "Alat/vector.h"
#include  "Fada/integrationformulainterface.h"
#include  "Alat/node.h"
#include  "Fada/transformationinterface.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class TransformationBase : public virtual Fada::TransformationInterface
  {
private:
    bool _basicinit;
    bool _sfcomputed;
    Alat::Node _normalhat;
    // dimension de l'espace
    int _dim;
    // Nombre de noeuds physique de l'élément
    // _trans: Fonctions de base de la transformation de l'element de reference sur l'element K
    // _dtrans: Dérivees des fonctions de base de la transformation
    Alat::Vector<Alat::Node> _dtrans;
    Alat::DoubleVector _trans;
    // _Jacobian: Jacobienne de la transformation
    // _InvJacobian Inverse transposée de la Jacobienne
    Alat::DoubleMatrix _Jacobian, _InvJacobian, _CoFacJacobian;
    //  Tableau des noeuds de l'element
    Alat::Vector<Alat::Node> _nodes;
    // _DetJ déterminant de la Jacobienne
    double _DetJ;
    // coordonnée d'un point de l'élément K
    Alat::Node _x;
    // _iis numero local de la face
    int _iiS;
    // _Normal: normale unitaire
    // _NonUnitNormal: normale non unitaire
    Alat::Node _Normal, _NonUnitNormal;
    // _SF Jacobien surfacique
    double _SF;
    // Normale normalisée
    void _computeNormal( int iiS);
    // Normale non normailsée
    // void NonUnitNormal( int iiS);
    double _computeNonUnitNormal(const Alat::Node& norhat);

protected:
    // defaut : do nothing
    // ok for tr, tet and hexa, but not for quad because of incompatibility of reference sides
    //
    void correctIntegrationPoint(Alat::Node& shat) const {}

    bool donebasicInit() const
    {
      return _basicinit;
    }

    void _memory(int dim);
    Alat::Vector<Alat::Node>& getNodes()
    {
      return _nodes;
    }

    const Alat::Vector<Alat::Node>& getDTransformation() const
    {
      return _dtrans;
    }

    const Alat::DoubleVector& getTransformation() const
    {
      return _trans;
    }

    Alat::Vector<Alat::Node>& getDTransformation()
    {
      return _dtrans;
    }

    Alat::DoubleVector& getTransformation()
    {
      return _trans;
    }

    // Defintion des fonctions de base de la transformee
    virtual void trans(const Alat::Node& xhat)
    {
      _notWritten("trans");
    }

    // Derivee des fonctions de base de la transformee
    virtual void dtrans(const Alat::Node& xhat)
    {
      _notWritten("dtrans");
    }

    // Passage de l'abscisse curviligne a xhat
    virtual void ShatToXhat( Alat::Node& xhat, const Alat::Node& s, int iiS)
    {
      _notWritten("ShatToXhat");
    }

    int getLocalIndexOfSide() const
    {
      return _iiS;
    }

    virtual void setNormalHat( int iiS, Alat::Node& normalhat) const
    {
      _notWritten("setNormalHat");
    }

    virtual double getSurfaceJacobianHat(int iiS) const
    {
      _notWritten("getSurfaceJacobianHat");
      return 0.0;
    }

public:
    ~TransformationBase();
    TransformationBase();

    TransformationBase(const TransformationBase& orig);
    TransformationBase& operator=( const TransformationBase& transformationinterface);


    std::string getName() const
    {
      return "TransformationBase";
    }

    const Alat::Vector<Alat::Node>& getNodes() const
    {
      return _nodes;
    }

    void basicInit(const FadaMesh::MeshInterface* M);

    void reInitCell(const Alat::Vector<Alat::Node>& nodes);
    void reInitPointCell(const Alat::Node& xhat);
    void reInitPointCell();
    void reInitSide(int iiS);
    void reInitSide(int iiS, const Alat::Node& xhat);
    void reInitSide(const Alat::Node& xhat, const Alat::Node& normalhat);
    void reInitPointSide(const Alat::Node& xhat);

    void computeReferencePoint(Alat::Node& xhat, const Alat::Node& shat);

    // Passage de xhat a x
    void xhattox(const Alat::Node& xhat);

    const Alat::Node& x() const
    {
      return _x;
    }

    const Alat::Node& getNormal() const
    {
      return _Normal;
    }

    const Alat::Node& getNonUnitNormal() const
    {
      return _NonUnitNormal;
    }

    const Alat::DoubleMatrix& getJacobian() const
    {
      return _Jacobian;
    }

    const Alat::DoubleMatrix& getInvJacobian() const
    {
      return _InvJacobian;
    }

    const double getDetJacobian() const
    {
      return _DetJ;
    }

    const double getSurfaceJacobian() const
    {
      assert(_SF > 0);
      return _SF;
    }

    const double getHOfSide() const;
    const double getHOfCell() const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
