# -*- coding: utf-8 -*-

import os, sys, re
from types import *

#------------------------------------------------------------------------
class pyfParamater(object):
  """A class containing all kind of info for a single parameter """
  def __repr__(self):
    return "%s (%s) (%s %s): %s" %(self.name, self.block, self.dtype, self.ignore, self.value)
  def __str__(self):
    return "Parameter '%s' in block '%s' (%s): %s" %(self.name, self.block, self.dtype, self.value)
  def __init__(self, name, block, dtype=None, value=None):
    self.name = name
    self.block = block
    self.dtype = dtype
    self.value = value

#------------------------------------------------------------------------
class pyfParamaterCollection(object):
  """
    Container class for  pyfParamater
    name: name of parameter (to be inserted in .fadaparam)
    block: name of block in output (to be inserted in .fadaparam), ignored if block="none"
    value: if None raises an error at output
    keyword: name of block in database
    own_to_parameter: used to read from database, no output
  """
  def __str__(self):
    string = "pyfParamaterCollection %s:\n" %self.keyword
    string += "===name_to_parameters===\n"
    for name, parameter in self.name_to_parameter.items():
      string += "%-30s %-30s %-20s (%s)\n" %(str(parameter.block), name, str(parameter.value), str(parameter.dtype))
    string += "===own_to_parameters===\n"
    for name, parameter in self.own_to_parameter.items():
      string += "%-30s %-20s (%s)\n" %(name, str(parameter.value), str(parameter.dtype))
    return string

  def __init__(self, keyword):
    self.keyword=keyword
    self.name_to_parameter = {}
    self.own_to_parameter = {}
  def addOut(self, name, blockout, dtype, value=None):
    self.name_to_parameter[name] = pyfParamater(name=name, block=blockout, dtype=dtype, value=value)
  def add(self, name, dtype, value=None):
    self.own_to_parameter[name] = pyfParamater(name=name, block=None, dtype=dtype, value=value)
  def getOut(self, name, checkNone=False):
    value = self.name_to_parameter[name].value
    if checkNone and value is None:
      raise KeyError("value is None for parameter '%s'" %(name))
    return value
  def getOwn(self, name, checkNone=False):
    value = self.own_to_parameter[name].value
    if checkNone and value is None:
      raise KeyError("value is None for parameter '%s'" %(name))
    return value
  # def setValue(self, block, name, value):
  #   self.block_to_parameter[block][name].value = value
  def readDataBase(self, filename, addtoblock=None):
    if addtoblock is not None:
      assert isinstance(addtoblock, dict)
    filehandle = open(filename)
    for line in filehandle.readlines():
      text = line.strip()
      if not text: continue
      if text.lstrip()[0]=='#': continue
      textsplit = text.split('=')
      if len(textsplit) !=2: continue
      keyandname = textsplit[0].split()
      if len(keyandname) <2: continue
      if keyandname[0].strip() != self.keyword: continue
      name = keyandname[1].strip()
      addparameter = None
      if addtoblock is not None and name in list(addtoblock.keys()):
        assert len(keyandname)==3
        addparameter = addtoblock[name]
        name = keyandname[2].strip()
      else:
        assert len(keyandname)==2
      value = textsplit[1].strip().strip('"')
      if name in list(self.name_to_parameter.keys()):
        parameter = self.name_to_parameter[name]
      elif name in list(self.own_to_parameter.keys()):
        parameter = self.own_to_parameter[name]
      elif addparameter is not None:
        parameter = pyfParamater(name=name, block=addparameter, dtype=StringType)
        self.name_to_parameter[name] = parameter
      else:
        raise KeyError("no such parameter '%s' for '%s'" %(name, self.keyword))
      if parameter.dtype == IntType:
        parameter.value = int(value)
      elif parameter.dtype == FloatType:
        parameter.value = float(value)
      elif parameter.dtype == StringType:
        parameter.value = str(value)
      elif parameter.dtype == BooleanType:
        if value == "False" or value == "false" or value == "0":
          parameter.value = False
        else:
          parameter.value = True
      else:
        raise KeyError("unknown datatype '%s' for parameter '%s'" %(str(parameter)))
    filehandle.close()
  def setParameters(self, cpp_param):
    block_to_parameter={}
    for name,parameter in self.name_to_parameter.items():
      block = parameter.block
      if block not in list(block_to_parameter.keys()):
        block_to_parameter[block] = {}
      block_to_parameter[block][name]=parameter
    for block, parameters in block_to_parameter.items():
      cpp_param.addBlock(block)
      for name,parameter in parameters.items():
        assert name == parameter.name
        if parameter.value is None or parameter.value == "None":
          raise KeyError("No value set for parameter %s" %str(parameter))
        cpp_param.parameters[block][parameter.name] = parameter.value


#------------------------------------------------------------------------
class pyfParamatersForFada(object):
  """
  A class containing the content of a .fadalightparam file
  """

  def __init__(self):
    self.parameters = {}

  def __str__(self) :
    string = "Blocks: "
    for block in list(self.parameters.keys()) :
      string += block + " "
    string += "\n"
    for block, blockdict in self.parameters.items() :
      for param, value in blockdict.items() :
        string += "\t%20s: %20s = %20s\n" % (block, param, value)
    return string

  def readParameters(self, paramfilename) :
    try:
      file = open(paramfilename, 'r')
    except IOError :
      raise ValueError("@@@pythonERROR in pyfParamatersForFada() : cannot read paramfile \""+ paramfilename+ "\"")
    block_name = ""
    for line in file:
       if (re.search('//Block', line)):
           words = line.split()
           block_name = words[-1]
           if not block_name in self.parameters:
             self.parameters[block_name] = {}
       else:
         try:
           words = line.split()
           if (len(words) == 0): continue
           param_name = words[0]
           if param_name[0] == "#": continue
           param_value = ' '.join(words[1:])
           self.parameters[block_name][param_name] = param_value
         except:
           continue
    file.close()

  def writeParameters(self, paramfilename) :
    try:
      file = open(paramfilename, 'w')
    except IOError :
      raise ValueError("@@@pythonERROR in pyfParamatersForFada() ::writeParameters: cannot open paramfile \""+ paramfilename+ "\"")
    for block in sorted(self.parameters.keys()) :
      file.write("//Block %s\n"%(block))
      for param in sorted(self.parameters[block].keys()) :
        file.write("%s  %s\n" % (param, self.parameters[block][param]))
      file.write('\n')
    file.close()

  def addBlock(self, block):
    if block in list(self.parameters.keys()) : return
    self.parameters[block]={}

  def blockisDefined(self, block):
    if  block in list(self.parameters.keys()): return 1
    return 0

  def parameterisDefined(self, block, param):
    if not block in list(self.parameters.keys()): return 0
    return param in self.parameters[block]

  def setParameter(self, block, param, value):
    if not block in list(self.parameters.keys()) : self.parameters[block]={}
    self.parameters[block][param] = value

  def getParameter(self, block, param):
    try:
      return self.parameters[block][param]
    except:
      print("given: block='", block, " param='", param, "'")
      print('known block', list(self.parameters.keys()))
      if block in list(self.parameters.keys()):
        print('known parameters in block', block, list(self.parameters[block].keys()))
      raise KeyError('no such parameter \''+param+'\'')

  def getIterKeys(self):
     return  iter(self.parameters.keys())


  def erase(self,block,param):
    try:
        del self.parameters[block][param]
    except:
        return

  def write(self):
      for block in sorted(self.parameters.keys()) :
        print("//Block %s"%(block))
        for param in sorted(self.parameters[block].keys()) :
          print("%s  %s" % (param, self.parameters[block][param]))


  def getParameterIfDefined(self, block, param, defaultvalue):
    if not self.parameterisDefined(block,param):
      return defaultvalue
    return self.parameters[block][param]

  def compare(self, fadalightparam, filenameout, projectnameself="self", projectnameother="other",used=True):
     try:
       file = open(filenameout, 'w')
     except IOError :
       raise ValueError("@@@pythonERROR in pyfParamatersForFada() ::compare: cannot open file \""+ filenameout+ "\"")
     if (not used):
         file.write("WARNING: The parameters compared to the local parameters are not be used\n")
     # block compare
     file.write("Data Comparison:\n")
     for block in sorted(self.parameters.keys()) :
         if (not fadalightparam.blockisDefined(block)):
             file.write("***********Warning: Block doesnt exist: Block %s\n" %(block))
             file.write("block exists in %s but not in %s\n" %(projectnameself,projectnameother))
             continue
         #parameter compare
         for param in sorted(self.parameters[block].keys()) :
             if (not fadalightparam.parameterisDefined(block,param)):
                 file.write("***********Warning Parameter doesnt exist: in Block %s for parameter %s\n" %(block,param))
                 file.write("-------->parameter exists in %s but not in %s\n" %(projectnameself,projectnameother))
             elif (self.parameters[block][param]!= fadalightparam.getParameter(block, param)):
                 file.write("***********Warning : Difference on parameter %s in block %s\n" %(param,block))
                 file.write("value in %s: %s; in %s: %s\n" %(projectnameself,self.parameters[block][param],projectnameother,fadalightparam.parameters[block][param]))

     # block compare
     for block in sorted(fadalightparam.getIterKeys()) :
          if (not self.blockisDefined(block)):
              file.write("***********Warning: Block doesnt exist: Block %s\n" %(block))
              file.write("block exists in %s but not in %s\n" %(projectnameother,projectnameself))
              continue
          #parameter compare
          for param in sorted(fadalightparam.parameters[block].keys()) :
              if (not self.parameterisDefined(block,param)):
                  file.write("***********Warning Parameter doesnt exist: in Block %s for parameter %s\n"  %(block,param))
                  file.write("-------->parameter exists in %s but not in %s\n" %(projectnameother,projectnameself))
              #elif (self.parameters[block][param]!= fadalightparam.parameters[block][param]):
              #    file.write("***********Warning Parameter value in block %s for parameter %s\n"  %(block,param))
              #    file.write("value in %s: %s; in %s: %s\n" %(projectnameother,fadalightparam.parameters[block][param],projectnameself,self.parameters[block][param]))
     file.close()

  def add(self, pyfadalightparam):
    for block, blockdict in pyfadalightparam.parameters.items() :
      # print 'adding block', block
      self.addBlock(block)
      for param, value in blockdict.items() :
        if self.parameterisDefined(block, param) : raise ValueError('parameter '+param+' is already defined in block '+block)
        self.setParameter(block, param, value)
