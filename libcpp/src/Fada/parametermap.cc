#include  "Alat/filescanner.h"
#include  "Fada/parametermap.h"
#include  <sstream>

using namespace Fada;

/*--------------------------------------------------------------------------*/

template<class T>
ParameterMap<T>::~ParameterMap<T>( ) {}

/*--------------------------------------------------------------------------*/

template<class T>
ParameterMap<T>::ParameterMap() : Alat::Map<std::string, T>(), _name() {}

/*--------------------------------------------------------------------------*/

template<class T>
ParameterMap<T>::ParameterMap(const ParameterMap<T>& P) : Alat::Map<std::string, T>(P), _name(P._name) {}

/*--------------------------------------------------------------------------*/

template<class T>
ParameterMap<T>::ParameterMap(const std::string& name) : Alat::Map<std::string, T>(), _name(name) {}

/*--------------------------------------------------------------------------*/

template<class T>
ParameterMap<T>& ParameterMap<T>::operator=(const ParameterMap<T>& P)
{
  Alat::Map<std::string, T>::operator=(P);
  _name = P._name;
  return *this;
}

/*--------------------------------------------------------------------------*/

template<class T>
void ParameterMap<T>::basicInit(const Alat::ParameterFile* parameterfile)
{
  Alat::DataFormatHandler dataformathandler;
  // std::cerr << "reading in Block="<<_name<<" parameterfile=" << *parameterfile<<"\n";
  for(typename Alat::Map<std::string, T>::iterator p = this->begin(); p != this->end(); p++)
  {
  // std::cerr << "ParameterMap<T>::basicInit() parameter="<<p->first<<"\n";
    dataformathandler.insert(p->first, &p->second);
  }
  Alat::FileScanner filescanner(dataformathandler, parameterfile, _name, 0);
}

/*--------------------------------------------------------------------------*/
template<class T>
void ParameterMap<T>::addModelNameAndId(const std::string& modelname, int id)
{
  _name=_name+"_"+modelname;
  if(id != -1)
  {
    std::stringstream ss;
    ss<<id;
    _name += "_"+ss.str();
  }
}

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

template class Fada::ParameterMap<std::string>;
template class Fada::ParameterMap<double>;
