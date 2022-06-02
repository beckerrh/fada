#ifndef __Fada_Chronometer_h
#define __Fada_Chronometer_h

#include  "Alat/map.h"
#include  "Alat/set.h"
#include  "stopwatch.h"
#include  <string>

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class Chronometer
  {
public:
    typedef Alat::Map<std::string, Fada::StopWatch>::const_iterator const_iterator;
    typedef Alat::Map<std::string, Fada::StopWatch>::iterator iterator;

private:
    // std::string _classname, _dirname;
    std::string _classname;
    Alat::Map<std::string, Fada::StopWatch> _M;
    Alat::Map<std::string, bool> _sum;

public:
    ~Chronometer();
    Chronometer();
    Chronometer(const Chronometer& C);
    Chronometer(std::string classname);
    Chronometer& operator= (const Chronometer& C);

    std::string getClassName() const;
    void setClassName(const std::string& classname);
    // void setDirectoryName(const std::string& dirname);
    Fada::StopWatch& get(std::string name);

    const std::map<std::string, Fada::StopWatch>& get() const;
    void enrol(std::string name, bool sum = 0);
    void reset(std::string name);
    void start(std::string name);
    double stop(std::string name);
    double total() const;
    // void print() const;
    void print(std::ostream& os) const;
  };
}

#endif
