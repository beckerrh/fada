#include  "FadaMesh/couplingsideinformation.h"
#include <fstream>
#include  <cassert>

using namespace FadaMesh;

/*--------------------------------------------------------------------------*/
CouplingSideInformation::~CouplingSideInformation(){}
CouplingSideInformation::CouplingSideInformation() : Alat::InterfaceBase(){}
CouplingSideInformation::CouplingSideInformation( const CouplingSideInformation& couplingsideinformation) : Alat::InterfaceBase(couplingsideinformation)
{
//  assert(0);
}
CouplingSideInformation& CouplingSideInformation::operator=( const CouplingSideInformation& couplingsideinformation)
{
  Alat::InterfaceBase::operator=(couplingsideinformation);
  assert(0);
  return *this;
}
std::string CouplingSideInformation::getName() const
{
  return "CouplingSideInformation";
}
std::string CouplingSideInformation::getInterfaceName() const
{
  return "CouplingMeshInterface";
}
CouplingSideInformation* CouplingSideInformation::clone() const
{
  assert(0);
  return new CouplingSideInformation(*this);
}

/*--------------------------------------------------------------------------*/

int CouplingSideInformation::getNCouplingDatas() const
{
    return _ndata;
}

/*--------------------------------------------------------------------------*/

int CouplingSideInformation::getiKL(int icoupling) const
{
    return _ikl[icoupling];
}

/*--------------------------------------------------------------------------*/

int CouplingSideInformation::getiKR(int icoupling) const
{
   return  _ikr[icoupling];
}

/*--------------------------------------------------------------------------*/

Alat::Node CouplingSideInformation::getOriginL(int icoupling) const
{
  return _originl[icoupling];
}

/*--------------------------------------------------------------------------*/

Alat::Node &CouplingSideInformation::getOriginL(int icoupling)
{
  return _originl[icoupling];
}
/*--------------------------------------------------------------------------*/

Alat::Node CouplingSideInformation::getOriginR(int icoupling) const
{
  return _originr[icoupling];
}

/*--------------------------------------------------------------------------*/

Alat::Node &CouplingSideInformation::getOriginR(int icoupling)
{
  return _originr[icoupling];
}

/*--------------------------------------------------------------------------*/

double CouplingSideInformation::getHsub(int icoupling, int idir) const
{
  return _hsub(icoupling,idir);
}

/*--------------------------------------------------------------------------*/

double& CouplingSideInformation::getHsub(int icoupling, int idir)
{
  return _hsub(icoupling,idir);
}

/*--------------------------------------------------------------------------*/

double CouplingSideInformation::getHL(int icoupling, int idir) const
{
  return _hl(icoupling,idir);
}

/*--------------------------------------------------------------------------*/

double& CouplingSideInformation::getHL(int icoupling, int idir)
{
  return _hl(icoupling,idir);
}

/*--------------------------------------------------------------------------*/

double CouplingSideInformation::getHR(int icoupling, int idir) const
{
  return _hr(icoupling,idir);
}

/*--------------------------------------------------------------------------*/

double& CouplingSideInformation::getHR(int icoupling, int idir)
{
  return _hr(icoupling,idir);
}

/*--------------------------------------------------------------------------*/

const Alat::IntVector& CouplingSideInformation::getiKL() const
{
    return _ikl;
}

/*--------------------------------------------------------------------------*/

Alat::IntVector& CouplingSideInformation::getiKL()
{
    return _ikl;
}

/*--------------------------------------------------------------------------*/

const Alat::IntVector& CouplingSideInformation::getiKR() const
{
    return _ikr;
}

/*--------------------------------------------------------------------------*/

Alat::IntVector& CouplingSideInformation::getiKR()
{
    return _ikr;
}

/*--------------------------------------------------------------------------*/

Alat::IntVector& CouplingSideInformation::getNodeIdsL()
{
    return _nodeidsl;
}

/*--------------------------------------------------------------------------*/

const Alat::IntVector& CouplingSideInformation::getNodeIdsL() const
{
    return _nodeidsl;
}

/*--------------------------------------------------------------------------*/

Alat::IntVector& CouplingSideInformation::getNodeIdsR()
{
    return _nodeidsr;
}

/*--------------------------------------------------------------------------*/

const Alat::IntVector& CouplingSideInformation::getNodeIdsR() const
{
    return _nodeidsr;
}

/*--------------------------------------------------------------------------*/

int CouplingSideInformation::getNodeIdsL(int icoupling) const
{
    return _nodeidsl[icoupling];
}

/*--------------------------------------------------------------------------*/

int CouplingSideInformation::getNodeIdsR(int icoupling) const
{
    return _nodeidsr[icoupling];
}

/*--------------------------------------------------------------------------*/
void CouplingSideInformation::write(std::string filename, std::string datatype) const
{
    std::ofstream file( filename.c_str() );
    assert( file.is_open() );
    _originl.saveFada(file,datatype);
    _originr.saveFada(file,datatype);
    _hsub.saveFada(file,datatype);
    _hl.saveFada(file,datatype);
    _hr.saveFada(file,datatype);
    _ikl.saveFada(file,datatype);
    _ikr.saveFada(file,datatype);
    file.close();
}

/*--------------------------------------------------------------------------*/

void CouplingSideInformation::read(std::string filename)
{
    std::ifstream file( filename.c_str() );
    assert( file.is_open() );
    _originl.loadFada(file);
    _originr.loadFada(file);
    _hsub.loadFada(file);
    _hl.loadFada(file);
    _hr.loadFada(file);
    _ikl.loadFada(file);
    _ikr.loadFada(file);
    _ndata=_ikr.size();
}
/*--------------------------------------------------------------------------*/
void CouplingSideInformation::set_size(int n, int dimension)
{
    _ndata=n;
    _originl.set_size(n);
    _originr.set_size(n);
    _hsub.set_size(n,dimension-1);
    _hl.set_size(n,dimension-1);
    _hr.set_size(n,dimension-1);
    _ikl.set_size(n);
    _ikr.set_size(n);
    _nodeidsl.set_size(n+1);
    _nodeidsr.set_size(n+1);
}
