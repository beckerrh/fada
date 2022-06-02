#ifndef __FadaMesh_CouplingSideInformation_h
#define __FadaMesh_CouplingSideInformation_h

#include  "Alat/interfacebase.h"
#include  "Alat/node.h"
#include  "Alat/doublematrix.h"
#include  "Alat/intvector.h"
#include  "Alat/vector.h"

/*--------------------------------------------------------------------------*/
namespace FadaMesh
{
  class CouplingSideInformation : public Alat::InterfaceBase
  {
protected:
    int _ndata;
    Alat::Vector<Alat::Node> _originl;
    Alat::Vector<Alat::Node> _originr;
    Alat::DoubleMatrix _hsub;
    Alat::DoubleMatrix _hl;
    Alat::DoubleMatrix _hr;
    Alat::IntVector _ikl;
    Alat::IntVector _ikr;
    std::string getInterfaceName() const;
    Alat::IntVector _nodeidsl, _nodeidsr;
    
public:
    ~CouplingSideInformation();
    CouplingSideInformation();
    CouplingSideInformation( const CouplingSideInformation& couplingsideinformation);
    CouplingSideInformation& operator=( const CouplingSideInformation& couplingsideinformation);
    std::string getName() const;
    void set_size(int n, int dimension = 2);
    CouplingSideInformation* clone() const;
    int getNCouplingDatas() const;
    int getiKL(int icoupling) const;
    // int& getiKL(int icoupling);
    int getiKR(int icoupling) const;
    // int& getiKR(int icoupling);
    Alat::Node getOriginL(int icoupling) const;
    Alat::Node& getOriginL(int icoupling);
    Alat::Node getOriginR(int icoupling) const;
    Alat::Node& getOriginR(int icoupling);
    double getHsub(int icoupling, int idir = 0) const;
    double& getHsub(int icoupling, int idir = 0);
    double getHL(int icoupling, int idir = 0) const;
    double& getHL(int icoupling, int idir = 0);
    double getHR(int icoupling, int idir = 0) const;
    double& getHR(int icoupling, int idir = 0);
    const Alat::IntVector& getiKL() const;
    Alat::IntVector& getiKL();
    const Alat::IntVector& getiKR() const;
    Alat::IntVector& getiKR();
    const Alat::IntVector& getNodeIdsL() const;
    Alat::IntVector &getNodeIdsL();
    const Alat::IntVector& getNodeIdsR() const;
    Alat::IntVector& getNodeIdsR();
    int getNodeIdsL(int icoupling) const;
    // int& getNodeIdsL(int icoupling);
    int getNodeIdsR(int icoupling) const;
    // int& getNodeIdsR(int icoupling);
    void write(std::string filename, std::string datatype) const;
    void read(std::string filename);
  };
}

/*--------------------------------------------------------------------------*/

#endif
