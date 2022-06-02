#include  "Fada/dirichletzero.h"
#include  "Fada/neumannzero.h"
#include  "Fada/problemdata.h"
#include  "Fada/righthandsidezero.h"

using namespace Fada;

/*--------------------------------------------------------*/
ProblemData::~ProblemData()
{
  for(MapDataMap::iterator it = _data.begin(); it != _data.end(); it++)
  {
    for(DataMap::iterator p = ( it->second ).begin(); p != ( it->second ).end(); p++)
    {
      if(p->second != NULL)
      {
        delete p->second;
        p->second = NULL;
      }
    }
  }
}

ProblemData::ProblemData(){}
ProblemData::ProblemData(const ProblemData& problemdata)
{
  assert(0);
}

ProblemData& ProblemData::operator=(const ProblemData& problemdata)
{
  // std::cerr << " problemdata.data " << problemdata._data << "\n";
  _data = problemdata._data;
  // std::cerr << " _data " << _data << "\n";
  return *this;
}

std::string ProblemData::getName() const
{
  return "ProblemData";
}

/*--------------------------------------------------------*/
std::ostream& ProblemData::print(std::ostream& os) const
{
  for(MapDataMap::const_iterator it = _data.begin(); it != _data.end(); it++)
  {
    os << it->first << " : ";
    for(DataMap::const_iterator p = ( it->second ).begin(); p != ( it->second ).end(); p++)
    {   
      os << p->first << " --> ";
      if(p->second != NULL)
      {
        os <<  p->second->getName() <<"\n";
      }
    }
  }
  return os;
}

/*--------------------------------------------------------*/
  ProblemData::MapDataMap::const_iterator ProblemData::begin() const
  {
    return _data.begin();
  }

  ProblemData::MapDataMap::const_iterator ProblemData::end() const
  {
    return _data.end();
  }

/*--------------------------------------------------------*/
  void ProblemData::clear()
  {
    _data.clear();
  }

/*--------------------------------------------------------*/
  void ProblemData::basicInit()
  {
    for(MapDataMap::iterator it = _data.begin(); it != _data.end(); it++)
    {
      for(DataMap::iterator p =  it->second.begin(); p != it->second.end(); p++)
      {
        p->second->basicInit();
      }
    }
  }

/*--------------------------------------------------------*/
  std::ostream& Fada::operator<<( std::ostream& os, const ProblemData& problemdata )
  {
    for(ProblemData::MapDataMap::const_iterator it = problemdata.begin(); it != problemdata.end(); it++)
    {
      for(ProblemData::DataMap::const_iterator p = ( it->second ).begin(); p != ( it->second ).end(); p++)
      {
        os<<"ProblemData: "<<p->first<<" -->  "<<it->first<<" --> "<<p->second->getName()<<'\n';
      }
    }
    return os;
  }

/*--------------------------------------------------------*/
  void ProblemData::setData(const std::string& varname, const std::string& dataname, const Fada::DataInterface* data)
  {
    if( _data[dataname].hasKey(varname) )
    {
      std::cerr<<"*** ERROR in ProblemData::setData(): "<<dataname<<" for variable "<<varname<<" already defined\n";
      std::cerr<<"data = "<<_data<<"\n";
      assert(0);
      exit(1);
    }
    assert(0);
    // _data[dataname][varname] = data->clone();
  }

  Fada::DataInterface*& ProblemData::setData(const std::string& varname, const std::string& dataname)
  {
    // assert(_data.hasKey(dataname));
    if( _data[dataname].hasKey(varname) )
    {
      std::cerr<<"*** ERROR in ProblemData::setData(): "<<dataname<<" for variable "<<varname<<" already defined\n";
      std::cerr<<"data = "<<_data<<"\n";
      assert(0);
      exit(1);
    }
    return _data[dataname][varname];
  }

/*--------------------------------------------------------*/
  bool ProblemData::hasKey(const std::string& varname, const std::string& dataname ) const
  {
    return ( _data.hasKey(dataname) )&&( _data[dataname].hasKey(varname) );
  }

/*--------------------------------------------------------*/
  Fada::DataInterface* ProblemData::getData(const std::string& varname, const std::string& dataname )
  {
    if( _data.hasKey(dataname) )
    {
      if( _data[dataname].hasKey(varname) )
      {
        return _data[dataname][varname];
      }
    }
    return NULL;
  }

  const Fada::DataInterface* ProblemData::getData(const std::string& varname, const std::string& dataname ) const
  {
    if( _data.hasKey(dataname) )
    {
      if( _data[dataname].hasKey(varname) )
      {
        return _data[dataname][varname];
      }
    }
    return NULL;
  }

/*--------------------------------------------------------*/
  void ProblemData::replace(ProblemData& problem_data) const
  {
    //Replace a given ProblemData (problem_data) by (*this) ProblemData
    //problem_data.clear();
    for(MapDataMap::const_iterator it = begin(); it != end(); it++)
    {
      for(DataMap::const_iterator p = ( it->second ).begin(); p != ( it->second ).end(); p++)
      {
//          std::cerr<<"ProblemData::replace "<<p->first<<"  "<<it->first<<"  "<<p->second->getName()<<'\n';
        problem_data.setData(p->first, it->first) = p->second;
      }
    }
  }

/*--------------------------------------------------------*/
  void ProblemData::replaceByZeroData()
  {
    //Replace  the data of (*this) ProblemData by zero data
    ProblemData temp;
    for(MapDataMap::const_iterator it = begin(); it != end(); it++)
    {
      for(DataMap::const_iterator p = ( it->second ).begin(); p != ( it->second ).end(); p++)
      {
        std::cerr<<"ProblemData::replaceByZeroData "<<p->first<<"  "<<it->first<<"  "<<p->second->getName()<<'\n';
        temp.setData(p->first, it->first) = p->second;
      }
    }
    clear();
    for(MapDataMap::const_iterator it = temp.begin(); it != temp.end(); it++)
    {
      for(DataMap::const_iterator p = ( it->second ).begin(); p != ( it->second ).end(); p++)
      {
        std::cerr<<"ProblemData::replaceByZeroData "<<p->first<<"  "<<it->first<<"  "<<p->second->getName()<<'\n';
        Fada::DataInterface* _data(NULL);
        Fada::RightHandSideInterface* rhs_data = dynamic_cast<Fada::RightHandSideInterface*>( p->second );
        if(rhs_data)
        {
          _data = new RightHandSideZero();
        }
        Fada::DirichletInterface* dirichlet_data = dynamic_cast<Fada::DirichletInterface*>( p->second );
        if(dirichlet_data)
        {
          _data = new DirichletZero();
        }
        Fada::NeumannInterface* neumann_data = dynamic_cast<Fada::NeumannInterface*>( p->second );
        if(neumann_data)
        {
          _data = new NeumannZero();
        }
        if(!_data)
        {
          std::cerr<<"**** Error:ProblemData::replaceByZeroData: "<<"unknown data interface type :"<<( p->second )->getName()<<'\n';
          assert(0);
          exit(1);
        }
        setData(p->first, it->first) = _data;
      }
    }
  }
