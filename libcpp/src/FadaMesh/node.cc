#include  "FadaMesh/node.h"

using namespace std;
using namespace FadaMesh;

/*--------------------------------------------------------------------------*/

Node::Node() : _oldid(-1), _id(-1), _v() 
{
}

/*--------------------------------------------------------------------------*/

Node::Node(std::istream& in)
{
  in >> _id >> _oldid >> _v;
}

/*--------------------------------------------------------------------------*/

void Node::write(std::ostream& out, std::string datatype) const
{
  out << id() << " " << oldid() << " " << getNode() << "  ";
}

/*--------------------------------------------------------------------------*/

void Node::read(std::istream& in, std::string datatype)
{
  in >> _id >> _oldid >> _v; 
}
/*--------------------------------------------------------------------------*/

std::istream& operator >>(std::istream& in,Node & n)
{
  in>> n.id() >> n.oldid()>>n.getNode();
  return in;
}
