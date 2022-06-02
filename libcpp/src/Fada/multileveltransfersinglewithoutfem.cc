#include  "Fada/multileveltransfersinglewithoutfem.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
MultiLevelTransferSingleWithoutFem::~MultiLevelTransferSingleWithoutFem() 
{
}

MultiLevelTransferSingleWithoutFem::MultiLevelTransferSingleWithoutFem(): MultiLevelTransferSingleFemInterface()
{
}

MultiLevelTransferSingleWithoutFem::MultiLevelTransferSingleWithoutFem( const MultiLevelTransferSingleWithoutFem& multileveltransfersinglewithoutfem): MultiLevelTransferSingleFemInterface(multileveltransfersinglewithoutfem)
{
(*this).operator=(multileveltransfersinglewithoutfem);
}

MultiLevelTransferSingleWithoutFem& MultiLevelTransferSingleWithoutFem::operator=( const MultiLevelTransferSingleWithoutFem& multileveltransfersinglewithoutfem) 
{
MultiLevelTransferSingleFemInterface::operator=(multileveltransfersinglewithoutfem);
assert(0);
return *this;
}

std::string MultiLevelTransferSingleWithoutFem::getName() const 
{
return "MultiLevelTransferSingleWithoutFem";
}

MultiLevelTransferSingleWithoutFem* MultiLevelTransferSingleWithoutFem::clone() const 
{
return new MultiLevelTransferSingleWithoutFem(*this);
}

/*--------------------------------------------------------------------------*/
void MultiLevelTransferSingleWithoutFem::basicInit(const FadaMesh::MeshInterface* mesh, const Fada::FemInterface* fem)
{}
void MultiLevelTransferSingleWithoutFem::prolongate(int level, Alat::VariableVectorInterface* ufine, const Alat::VariableVectorInterface* ucoarse, double d) const
{
  ufine->add(d, ucoarse);
}
void MultiLevelTransferSingleWithoutFem::restrict(int level, Alat::VariableVectorInterface* ucoarse, const Alat::VariableVectorInterface* ufine ) const
{
  ucoarse->equal(ufine);
}
void MultiLevelTransferSingleWithoutFem::project(int level, Alat::VariableVectorInterface* ucoarse, const Alat::VariableVectorInterface* ufine) const
{
  ucoarse->equal(ufine);  
}
