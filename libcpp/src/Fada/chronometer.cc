#include  "Fada/chronometer.h"
#include  <cassert>
#include  <fstream>
#include  <iomanip>
#include  <iostream>

using namespace Fada;

/*--------------------------------------------------------------------------*/

Chronometer::~Chronometer() {}

/*--------------------------------------------------------------------------*/

Chronometer::Chronometer() : _classname("") {}

/*--------------------------------------------------------------------------*/

Chronometer::Chronometer(const Chronometer& C) : _classname("") {}

/*--------------------------------------------------------------------------*/

// Chronometer::Chronometer(std::string classname) : _classname(classname), _dirname("") {}
Chronometer::Chronometer(std::string classname) : _classname(classname) {}

/*--------------------------------------------------------------------------*/

Chronometer& Chronometer::operator= (const Chronometer& C)
{
  assert(0);
  return *this;
}

// /*--------------------------------------------------------------------------*/
// void Chronometer::print() const
// {
//   assert(_dirname!="");
//   std::string filename = _dirname  +"/" + "Timer_"+ _classname;
//   std::ofstream file( filename.c_str());
//   assert(file.is_open());
//   double totaltime = total();
//   // os << "\n   ---- Chronometer: " << getClassName() << " ----\n";
//   file << "   Total        :  " << totaltime << " s\n";
//   for(Chronometer::const_iterator p = _M.begin(); p != _M.end(); p++)
//   {
//     double singletime = p->second.read();
//     if(singletime == 0.0)
//     {
//       continue;
//     }
//     if(_sum[p->first])
//     {
//       file << std::setiosflags(std::ios::left);
//       file << std::setw(30) << p->first << "\t" << std::setiosflags(std::ios::fixed) << std::setprecision(5);
//       file << std::resetiosflags(std::ios::left);
//       file << singletime  << std::setw(12) << std::setiosflags(std::ios::fixed) << std::setprecision(2)<<  100.0*singletime/totaltime << "\% \n";
//     }
//     else
//     {
//       file << std::setiosflags(std::ios::left);
//       file << std::setw(30) << p->first << "\t" << std::setiosflags(std::ios::fixed) << std::setprecision(5);
//       file << std::resetiosflags(std::ios::left);
//       file << singletime  << std::setw(12) << "---\n";
//     }
//   }
//   file << "\n";
//   file.close();
// }

/*--------------------------------------------------------------------------*/
void Chronometer::print(std::ostream& os) const
{
  double totaltime = total();
  os << "   Total        :  "  << getClassName() << "   " << std::setiosflags(std::ios::fixed)<< std::setprecision(4) << totaltime << " s\n";
  for(Chronometer::const_iterator p = _M.begin(); p != _M.end(); p++)
  {
    double singletime = p->second.read();
    // if(singletime == 0.0)
    // {
    //   continue;
    // }
    if(_sum[p->first])
    {
      os << std::setiosflags(std::ios::left);
      os << std::setw(30) << p->first << "\t" << std::setiosflags(std::ios::fixed) << std::setprecision(5);
      os << std::resetiosflags(std::ios::left);
      os << singletime  << std::setw(12) << std::setiosflags(std::ios::fixed) << std::setprecision(2)<<  100.0*singletime/totaltime << "\% \n";
    }
    else
    {
      os << std::setiosflags(std::ios::left);
      os << std::setw(30) << p->first << "\t" << std::setiosflags(std::ios::fixed) << std::setprecision(5);
      os << std::resetiosflags(std::ios::left);
      os << singletime  << std::setw(12) << "---\n";
    }
  }
  os << "\n";
}

/*--------------------------------------------------------------------------*/

std::string Chronometer::getClassName() const
{
  return _classname;
}

void Chronometer::setClassName(const std::string& classname)
{
  _classname = classname;
}

// void Chronometer::setDirectoryName(const std::string& dirname)
// {
//   _dirname = dirname;
// }

/*--------------------------------------------------------------------------*/

const std::map<std::string, Fada::StopWatch>& Chronometer::get() const
{
  return _M;
}

/*--------------------------------------------------------------------------*/

void Chronometer::enrol(std::string name, bool sum)
{
  _M[name];
  _sum[name] = sum;
}

/*--------------------------------------------------------------------------*/

void Chronometer::reset(std::string name)
{
  get(name).reset();
}

/*--------------------------------------------------------------------------*/

void Chronometer::start(std::string name)
{
  if( get(name).running() )
  {
    std::cerr << "*** ERROR in Chronometer::start() already running function '" << name << "' in class "<< _classname <<"\n";
    assert(0);
    exit(1);
  }
  else
  {
    get(name).start();
  }
}

/*--------------------------------------------------------------------------*/

double Chronometer::stop(std::string name)
{
  return get(name).stop();
}

/*--------------------------------------------------------------------------*/

Fada::StopWatch& Chronometer::get(std::string name)
{
  return _M[name];
}

/*--------------------------------------------------------------------------*/

double Chronometer::total() const
{
  double d = 0.;
  for(const_iterator p = _M.begin(); p != _M.end(); p++)
  {
    if( _sum[p->first] )
    {
      d += p->second.read();
    }
  }
  return d;
}
