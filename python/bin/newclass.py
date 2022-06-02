#!/usr/bin/env python
# encoding: utf-8
"""
newclass.py

Created by Roland Becker on 2015-10-05.
Copyright (c) 2007 Fada. All rights reserved.
"""
import sys, os, shutil
import argparse

# ------------------------------------- #
def main():
    parser = argparse.ArgumentParser(description='new cpp and hpp files')
    # parser.add_argument('scriptandargs', help='script and args to launch', nargs='*')
    parser.add_argument('-class', type=str, help='class name', required=True)
    parser.add_argument('-parent', type=str, help='parent class name')
    parser.add_argument('-namespace', type=str, help='namespace name')
    parser.add_argument('--wrap', default = False, action="store_true", help='wrap cpp')
    parser.add_argument('--clone', default = False, action="store_true", help='add clone fct')
    args = vars(parser.parse_args(sys.argv[1:]))
    if args['wrap']:
        assert not args['parent']
        cppclass = CppClass(args)
        cppclass.writePythonWrap()
        sys.exit(1)
    cppclass = CppClass(args)
    cppclass.writeHpp()
    cppclass.writeCpp()

class CppClass(object):
  def __init__(self, args):
    self.classname = args['class']
    self.parentclass = args['parent']
    self.namespace = args['namespace']
    self.clone = args['clone']
    classnamesplit = self.classname.split('/')
    if not self.parentclass:
      # assert namespace
      assert len(classnamesplit)==1
      ifndefbase = self.namespace + "Wrap_" + self.classname + "Wrapper_hpp\n"
      self.ifndef = "#ifndef __"+ifndefbase + "#define __"+ifndefbase
      # self.hppfilename = string.lower(self.classname) + 'wrapper.hpp'
      self.hppfilename = self.classname.lower() + 'wrapper.hpp'
      if os.path.isfile(self.hppfilename) :
        raise ValueError("*** ERROR: file \"",self.hppfilename, "\"exists")
      # self.cppfilename = string.lower(self.classname) + 'wrapper.cpp'
      self.cppfilename = self.classname.lower() + 'wrapper.cpp'
      if os.path.isfile(self.cppfilename) :
        raise ValueError("*** ERROR: file \"",self.cppfilename, "\"exists")
      # self.cppbasefilename = string.lower(self.classname) + '.cpp'
      self.cppbasefilename = self.classname.lower() + '.cpp'
      if os.path.isfile(self.cppbasefilename) :
        raise ValueError("*** ERROR: file \"",self.cppbasefilename, "\"exists")
      return
    if len(classnamesplit)==2:
      localdirname = classnamesplit[0]
      self.classname = classnamesplit[1]
    else:
      localdirname=None
    parentclasssplit = self.parentclass.split("::")
    if self.namespace:
        self.namespacesplit = self.namespace.split("::")
    self.hppfilepath = ""
    if self.namespace:
        for name in self.namespacesplit:
          self.hppfilepath += name.title() + os.sep
        if localdirname:
          self.hppfilepath = self.hppfilepath + localdirname + os.sep
    # self.hppfilename = self.hppfilepath + string.lower(self.classname) + '.hpp'
    self.hppfilename = self.hppfilepath + self.classname.lower() + '.hpp'
    if not self.namespace:
        ifndefbase = "_" + self.classname + "_hpp\n"
    else:
        ifndefbase = ""
        n = len(self.namespacesplit)
        for i in range(n):
          if i < n-1:
            ifndefbase += self.namespacesplit[i] + "_"
          else:
            ifndefbase += self.namespacesplit[i] + "_" + self.classname + "_hpp\n"
    self.ifndef = "#ifndef __"+ifndefbase + "#define __"+ifndefbase

    self.parenthppfile = ""
    self.parentclass = ""
    n = len(parentclasssplit)
    for i in range(n):
      if i < n-1:
        self.parentclass += parentclasssplit[i].lower() + "::"
        self.parenthppfile += parentclasssplit[i].title() + "/"
      else:
        self.parentclass += parentclasssplit[i]
        self.parenthppfile += parentclasssplit[i].lower() + ".hpp"
    # print 'self.parenthppfile', self.parenthppfile
    # print 'self.hppfilepath', self.hppfilepath
    # sys.exit(1)

    if os.path.isfile(self.hppfilename) :
      raise ValueError("*** ERROR: file \"",self.hppfilename, "\"exists")
    # self.cppfilename = string.lower(self.classname) + ".cpp"
    self.cppfilename = self.classname.lower() + ".cpp"
    if localdirname:
      self.cppfilename = localdirname + os.sep + self.cppfilename
    if os.path.isfile(self.cppfilename) :
      raise ValueError("*** ERROR: file \"",self.cppfilename, "\"exists")

# ------------------------------------- #
  def separator(self, file) :
    file.write("/*--------------------------------------------------------------------------*/\n")

# ------------------------------------- #
  def writeHpp(self):
    classname, parentclass = self.classname, self.parentclass
    hppfile=open(self.hppfilename,"w")
    hppfile.write(self.ifndef)
    if self.parenthppfile:
        hppfile.write("\n#include  \"" + self.parenthppfile + "\"\n\n")
    self.separator(hppfile)

    if self.namespace:
      n = len(self.namespacesplit)
      for i in range(n):
        # hppfile.write("namespace " + string.lower(self.namespacesplit[i]) + "\n{\n")
        hppfile.write("namespace " + self.namespacesplit[i].lower() + "\n{\n")

    if parentclass != "none":
      hppfile.write("  class "+classname+" : public "+parentclass+"\n")
    else:
      hppfile.write("  class "+classname+"\n")
    hppfile.write("  {\n")
    hppfile.write("  private:\n")
    hppfile.write("  protected:\n")
    hppfile.write("  public:\n")
    hppfile.write("    ~"+classname+"();\n")
    hppfile.write("    "+classname+"();\n")
    hppfile.write("    "+classname+"( const " +  classname +"& "+classname.lower() +");\n")
    hppfile.write("    "+classname+"& operator="+"( const " +  classname +"& "+classname.lower()+");\n")
    hppfile.write("    std::string getClassName() const;\n")
    if self.clone:
        hppfile.write("    std::unique_ptr<" +parentclass + "> clone() const;\n")
    hppfile.write("  };\n")
    if self.namespace:
      n = len(self.namespacesplit)
      for i in range(n):
        hppfile.write("}\n")
    hppfile.write("\n")
    self.separator(hppfile)
    hppfile.write("#endif\n")
    hppfile.close()

# ------------------------------------- #
  def writeCpp(self):
    classname, parentclass = self.classname, self.parentclass
    cppfile=open(self.cppfilename,"w")
    # cppfile.write("#include  \""+self.hppfilepath+string.lower(classname)+".hpp\"\n")
    cppfile.write("#include  \""+self.hppfilepath+classname.lower()+".hpp\"\n")
    cppfile.write("#include  <cassert>\n\n")
    if self.namespace:
      n = len(self.namespacesplit)
      if n >0 :
        # cppfile.write("using namespace " + string.lower(self.namespace)+";\n\n")
        cppfile.write("using namespace " + self.namespace.lower()+";\n\n")
    self.separator(cppfile)
    # Destructor
    cppfile.write(classname+"::~"+classname+"() "+"{}\n")
    if parentclass == "none":
      cppfile.write(classname+"::"+classname+"() "+"{}\n")
      core = "{\n  (*this).operator=("+classname.lower()+");\n}"
      cppfile.write(classname+"::"+classname+"( const " +  classname + "& "+classname.lower()+") "+"\n"+core+"\n")
      core = "\n{  assert(0);\n  return *this;\n}\n"
      cppfile.write(classname+"& "+classname+"::operator=( const " +  classname + "& "+classname.lower()+") "+core)
    else:
    # Constructor void
      cppfile.write(classname+"::"+classname+"(): "+parentclass + "(){}\n")
    # Constructor copy
      core = "{\nassert(0);\n}"
      cppfile.write(classname+"::"+classname+"( const " +  classname + "& "+classname.lower()+"): "+parentclass+"("+classname.lower()+")\n"+core+"\n")
    # operator =
      core = "\n{\n  assert(0);\n  " + parentclass+ "::operator=("+classname.lower()+ ");" +"\n  return *this;\n}\n"
      cppfile.write(classname+"& "+classname+"::operator=( const " +  classname + "& "+classname.lower()+")"+core)
    # getClassName()
    cppfile.write("std::string " + classname  +"::getClassName() const \n{\n  return \""+classname+"\";\n}\n")
    if self.clone:
        core = "{\n  return std::unique_ptr< "+parentclass+">(new "+classname+"(*this));\n}"
        cppfile.write("std::unique_ptr<" + parentclass + "> " + classname  +"::clone() const \n"+core+"\n")
    self.separator(cppfile)
    cppfile.close()

# ------------------------------------- #
  def writePythonWrap(self):
    classname, namespace = self.classname, self.namespace
    hppfile=open(self.hppfilename,"w")
    hppfile.write(self.ifndef)
    text = "\n#include  \"%s/%s.hpp\"\n\n" %(namespace,classname.lower())
    hppfile.write(text)
    self.separator(hppfile)
    text = "class %sWrapper : public %s::%s\n" %(classname, namespace,classname)
    hppfile.write(text)
    hppfile.write("{\n")
    hppfile.write("  public:\n")
    text = "  %sWrapper();\n" %(classname)
    hppfile.write(text)
    hppfile.write("};\n")
    text = "std::ostream &operator<<(std::ostream &os, const %sWrapper& %s);\n" %(classname,classname.lower())
    hppfile.write(text)
    self.separator(hppfile)
    text = "\nvoid wrap%s();\n" %(classname)
    hppfile.write(text)
    hppfile.write("\n")
    self.separator(hppfile)
    hppfile.write("#endif\n")
    hppfile.close()


    cppfile=open(self.cppfilename,"w")
    text = "#include  <string>\n#include  <cassert>\n#include  <iostream>\n#include  \"%swrapper.hpp\"\n\n" %(classname.lower())
    cppfile.write(text)
    self.separator(cppfile)
    text = "%sWrapper::%sWrapper() : %s() {}\n\n" %(classname,classname,classname)
    cppfile.write(text)
    self.separator(cppfile)
    text = "std::ostream& operator<<(std::ostream &os, const %sWrapper& %s)\n" %(classname,classname.lower())
    cppfile.write(text)
    cppfile.write("{\n")
    text = "  os << %s.getInfo() << \"\\n\";\n  return os;\n" %(classname.lower())
    cppfile.write(text)
    cppfile.write("}\n")
    cppfile.close()

    cppfile=open(self.cppbasefilename,"w")
    text = "#include  <string>\n"\
      "#include  <cassert>\n"\
      "#include  <iostream>\n"\
      "#include  <boost/python.hpp>\n"\
      "#include  <boost/python/numpy.hpp>\n"\
      "#include  \"%swrapper.hpp\"\n\n"\
      "namespace bp = boost::python;\n"\
      "namespace np = boost::python::numpy;\n\n"\
      %(classname.lower())
    cppfile.write(text)
    self.separator(cppfile)
    text = "void wrap%s()\n"\
    "{\n"\
    "  bp::class_< %sWrapper >(\"%s\")\n"\
    "  .def(\"load\", &%sWrapper::load)\n"\
    "  .def(\"save\", &%sWrapper::save)\n"\
    "  .def(bp::self_ns::str(bp::self_ns::self));\n"\
    "}\n"\
    %(classname,classname,classname,classname,classname)
    cppfile.write(text)
    cppfile.close()


# ------------------------------------- #
if __name__ == '__main__':
    main()
  # cppclass.writeCpp()
