#ifndef __FadaMesh_HangingInfo_h
#define __FadaMesh_HangingInfo_h

#include  <list>
#include  <vector>
#include  <map>  
#include  <iostream>   

/*---------------------------------------------------------*/

namespace FadaMesh
{
  class HangingInfo  
  { 
  public:
    typedef std::map<std::pair<int,int>, std::vector<int> > HangingOfCoarseCell;
  protected:
    int cell_number;
    HangingOfCoarseCell _hangingnodes;
    HangingOfCoarseCell _hangingsides;
  public:
    ~HangingInfo(){}
    HangingInfo(){}
    HangingInfo(const HangingInfo& hanginginfo){}
    HangingInfo(int nc): cell_number(nc) {}
    
    int CellNumber(){return cell_number;}
    HangingOfCoarseCell& getHangingNodes() {return _hangingnodes;}
    HangingOfCoarseCell& getHangingSides() {return _hangingsides;}
   
    std::ostream& write(std::ostream & os) 
    {
      os<<"CELL "<<'\n';
      os<<cell_number<<" "<<_hangingnodes.size()<<'\n';
      for(HangingOfCoarseCell::iterator it=_hangingnodes.begin();it!=_hangingnodes.end();it++)
      {
        os<<"cell_local_side_and_nbr_hanging_nodes" <<'\n';
        os<<(it->first).first<<"  "<<(it->first).second<<" "<<(it->second).size()<<'\n';
        os<<"liste_hn" <<'\n';
        for(std::vector<int>::iterator it2=(it->second).begin(); it2!=(it->second).end();it2++)
        {
          os<<*it2<<" ";
        } 
        os<< '\n';
      }
      return os;  
    }
  };
}
#endif
