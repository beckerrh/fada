import sys, os, shutil, subprocess

class pyfCompile:
  def __init__(self):
    self.CMAKE_BUILD_TYPES = ['Release','RelWithDebInfo','Debug','DebugFull','Profile']
    self.build_type_default = self.CMAKE_BUILD_TYPES[1]
  def runsubprocess(self, cmd):
    return subprocess.check_call(cmd, shell=True)
    return subprocess.call(cmd, shell=True)
    proc = subprocess.Popen(cmd, shell=True)
    return proc.returncode
  def addArgumentParser(self, parser):
    parser.add_argument('-t', default = self.build_type_default, help='build type', choices=self.CMAKE_BUILD_TYPES)
  def getArgumentsFormParser(self, args):
    self.build_type = args['t']

  def cmakeFada(self, source_dir, build_dir, install_dir, options=""):
    print('cmakeFada: source_dir', source_dir)
    print('cmakeFada: build_dir', build_dir)
    print('cmakeFada: install_dir', install_dir)
    try:
      os.makedirs(build_dir)
    except:
      pass
    startdir = os.getcwd()
    os.chdir(build_dir)
    command = "cmake " + source_dir + " -DCMAKE_BUILD_TYPE="+self.build_type + " -DCMAKE_INSTALL_PREFIX="+install_dir + options
    returncode = self.runsubprocess(command)
    os.chdir(startdir)
    if returncode:
      print('@@@ error in cmakeFada : command=',command)
      raise ValueError('returncode='+str(returncode))
    #raise ValueError(f"{startdir=} {command=}")
    return returncode

  def makeFada(self, source_dir, build_dir, install_dir, options=""):
    startdir = os.getcwd()
    command = "make install -j4"
    os.chdir(build_dir)
    returncode = self.runsubprocess(command)
    os.chdir(startdir)
    if returncode:
      raise ValueError('returncode='+str(returncode))
    return returncode

  def installFada(self, source_dir, build_dir, install_dir, name=None):
    print(f" {source_dir=}")
    print(f" {build_dir=}")
    print(f" {install_dir=}")
    if name is not None:
        source_dir_full = os.path.join(source_dir, name)
        build_dir_full = os.path.join(build_dir, name)
    else:
        source_dir_full = source_dir
        build_dir_full = build_dir
    build_dir_full = os.path.join(build_dir_full, self.build_type)
    print(f" {build_dir_full=}")
    # return
    self.cmakeFada(source_dir=source_dir_full, build_dir=build_dir_full, install_dir=install_dir)
    returncode = self.makeFada(source_dir=source_dir_full, build_dir=build_dir_full, install_dir=install_dir)
    return returncode
#

  def cleanFada(self, source_dir, build_dir, install_dir):
    shutil.rmtree("build", ignore_errors=True)
    build_dir_full = os.path.join(build_dir, "libcpp")
    shutil.rmtree(build_dir_full, ignore_errors=True)
    build_dir_full = os.path.join(build_dir, "bin")
    shutil.rmtree(build_dir_full, ignore_errors=True)
    shutil.rmtree(os.path.join(install_dir, "libDebug"), ignore_errors=True)
    shutil.rmtree(os.path.join(install_dir, "libRelWithDebInfo"), ignore_errors=True)
    shutil.rmtree(os.path.join(install_dir, "libRelease"), ignore_errors=True)
    shutil.rmtree(os.path.join(install_dir, "libDebugFull"), ignore_errors=True)
    shutil.rmtree(os.path.join(install_dir, "libProfile"), ignore_errors=True)
    includedir = os.path.join(install_dir, "include")
    libs = ['Fada']
    for lib in libs:
      shutil.rmtree(os.path.join(includedir, lib), ignore_errors=True)


  def compileProject(self, install_dir, source_dir, projectname, projects='projects', execname='none'):
    if (execname=='none'): execname=projectname
    startdir = os.getcwd()
    project =  projectname
    # project_source_dir = source_dir + '/projects/'+project+'/src'
    project_source_dir = os.path.join(os.path.join(os.path.join(source_dir,projects),projectname),'src')
    if projects =="none":
      project_source_dir = source_dir
    print('project_source_dir', project_source_dir)
    if(not os.path.isdir(project_source_dir)) :
      raise IOError('@@@ ERROR no such directory: '+ project_source_dir)

    build_dir = os.path.join(startdir, "build")
    build_dir_full = os.path.join(build_dir,self.build_type)
    print('pyfCompile.compileProject build_dir_full', build_dir_full)
    try:
      os.makedirs(build_dir_full)
    except:
      pass
    os.chdir(build_dir_full)
    modulepath = install_dir + '/cmake/Modules'
    print('pyfCompile.compileProject modulepath', modulepath)
    command = "cmake " + project_source_dir + " -DCMAKE_BUILD_TYPE="+self.build_type + " -DCMAKE_MODULE_PATH=" + modulepath + "  -DCMAKE_INSTALL_PREFIX="+startdir
    print('command', command)
    self.runsubprocess(command)
    command = "make install -j4"
    returncode = self.runsubprocess(command)
    os.chdir(startdir)
    if (execname!=projectname):
        os.rename(projectname,execname)
    return returncode


  def generateDoc(self, source_dir, build_dir, install_dir):
    from .replaceinfile import replaceinfile
    sourcedir = os.path.join(source_dir, 'libcpp')
    installdir = os.path.join(install_dir, 'doc')
    sourcedirdoxygen = os.path.join(source_dir, 'doc')
    builddirdoxygen = os.path.join(build_dir, 'doc')
    try:
        os.makedirs(builddirdoxygen)
    except:
        pass
    wordstochange={}
    wordstochange["INPUT                  ="] = "INPUT                  =%s" %(sourcedir)
    wordstochange["OUTPUT_DIRECTORY       ="] = "OUTPUT_DIRECTORY       =%s" %(installdir)
    infilename = os.path.join(sourcedirdoxygen, 'Doxyfile.in')
    outfilename = os.path.join(builddirdoxygen, 'Doxyfile')
    replaceinfile(infilename, outfilename, wordstochange)
    startdir = os.getcwd()
    os.chdir(builddirdoxygen)
    command = "doxygen Doxyfile"
    returncode = self.runsubprocess(command)
    os.chdir(startdir)
