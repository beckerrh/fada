# -*- coding: utf-8 -*-
import sys, os, shutil, glob, subprocess
path =  '/'.join(os.path.dirname(sys.argv[0]).split('/')[0:-1])
resources = sys.argv.pop(1)
sys.path.append(path)

import tools.pyfpathmanager
import tools.pyfcompile
try:
  from argparse import ArgumentParser
except:
  from lib.argparse_1_2_1 import ArgumentParser

pathmanager = tools.pyfpathmanager.pyfPathManager(resource_dir=resources)
compiler = tools.pyfcompile.pyfCompile()

parser = ArgumentParser(description='pyf.')
parser.add_argument('scriptandargs', help='script and args to launch', nargs='*')
parser.add_argument('-copy', default = False, action="store_true", help='copy script')
parser.add_argument('-python', default = False, action="store_true", help='run python')
parser.add_argument('--compile', default = False, action="store_true", help='compile local')
parser.add_argument('--compileall', default = False, action="store_true", help='compile library and local (not binaries)')
parser.add_argument('--cleanlib', default = False, action="store_true", help='clean library')
parser.add_argument('-compare', default = False, action="store_true", help='compare rundirs (at least one)')
parser.add_argument('--erase', default = True, action="store_true", help='erase rundir')
parser.add_argument('--eraseall', default = False, action="store_true", help='erase all rundirs')
compiler.addArgumentParser(parser)
args = vars(parser.parse_args())
compiler.getArgumentsFormParser(args)
copy_script = args['copy']
run_python = args['python']
compile_project = args['compile']
compile_all = args['compileall']
clean_lib = args['cleanlib']
compare = args['compare']
erase = args['erase']
eraseall = args['eraseall']

#--- je pense qu'il faut faire cela pour la compile (??)
os.environ["LD_LIBRARY_PATH"] = os.environ.get('LD_LIBRARY_PATH', '') + ':' + pathmanager.install_dir+'/lib'
os.environ["DYLD_LIBRARY_PATH"] = os.environ.get('DYLD_LIBRARY_PATH', '') + ':' + pathmanager.install_dir+'/lib'


#----------------------------------------------------------------------------------------
def clean():
  compiler.cleanFada(source_dir=pathmanager.source_dir, build_dir=pathmanager.build_dir, install_dir=pathmanager.install_dir)

def compileProject(projectname):
  compiler.compileProject(install_dir=pathmanager.install_dir, source_dir=pathmanager.source_dir, projectname=projectname)

def copyScript():
  pyftestdir = pathmanager.source_dir + '/pyf/pyftest/'
  if len(args['scriptandargs'])==0:
    print('available scripts:\n', '\n'.join(os.listdir(pyftestdir)))
    sys.exit(1)
  script = args['scriptandargs'][0]
  pyftestfile = pyftestdir + script
  if os.path.isfile(script):
    print('script \'' + script + '\' existe déjà')
    sys.exit(1)
  shutil.copy(pyftestfile,'.')
  sys.exit(1)

def runCommandOnRunDir(rundir, commandin):
  # print "### runCommandOnRunDir: rundir=%s commandin=%s" %(rundir, commandin)
  sys.path.insert(0,rundir)
  module = __import__('script')
  main = module.Main(pathmanager=pathmanager, args=args['scriptandargs'][1:])
  main.initFromRundir(rundir=rundir)
  command = 'main.%s()' %(commandin)
  # print 'rundir', rundir, 'command', command
  eval(command)
  sys.exit(1)


def runPython(scriptdir):
  if len(args['scriptandargs'])==0:
    scripts = [os.path.basename(x).split('.')[0] for x in glob.glob(scriptdir+'/*.py')]
    print('available scripts in %s:\n' %(scriptdir), '\n'.join(scripts))
    sys.exit(1)
  script = args['scriptandargs'][0]
  argsscript = args['scriptandargs'][1:]
  scriptsourcefile = os.path.join(scriptdir, script) + '.py'
  if not os.path.isfile(scriptsourcefile):
    scripts = [os.path.basename(x).split('.')[0] for x in glob.glob(scriptdir+'/*.py')]
    print('script \'' + script + '\' n\'existe pas')
    print('available scripts in %s:\n' %(scriptdir), '\n'.join(scripts))
    sys.exit(1)
  source_dir=pathmanager.source_dir
  pythonsource = os.path.join(source_dir, "python")
  sys.path.insert(0,pythonsource)
  print("pythonsource", pythonsource)
  print("resources", resources)
  print("scriptdir", scriptdir)
  print("script", script)
  sys.path.insert(0,scriptdir)
  module = __import__(script)
  module.main(pathmanager=pathmanager, args=argsscript)


def runCompare():
  if len(args['scriptandargs']) ==0 :
    raise KeyError('wrong number of arguments (need >=1: rundir )')
  datas={}
  from os import sys, path
  for arg in args['scriptandargs']:
      rundir = arg.rstrip('/')
      rundirshort = rundir.strip('Run_')
      sys.path.insert(0,rundir)
      module = __import__('script')
      main = module.Main(pathmanager=pathmanager, args=args['scriptandargs'][1:])
      main.initFromRundir(rundir=rundir)
      datas[rundirshort] = main.getPostProcess()
  fempydir = path.abspath(path.join(main.pathmanager.source_dir, '../..'))
  sys.path.append(fempydir)
  from fempy.tools.latexwriter import LatexWriter
  latexwriter = LatexWriter()
  for dataname,data in datas.items():
    print('dataname=', dataname)
    latexwriter.addFadaLightData(data, method=dataname)
  latexwriter.write()
  latexwriter.compile()
  plot = False
  if not plot:
    return
  import matplotlib.pyplot as plt
  names = sorted(data.keys())
  names.remove('N')
  if 'iter' in names:
    names.remove('iter')
  nplots = len(names)
  for iplot, name in enumerate(names):
    plt.subplot('%1d1%1d' %(nplots, iplot))
    if name.split('_')[0]=='Err':
      for dataname,data in datas.items():
        plt.loglog(data['N'], data[name], label=dataname)
    else:
      for dataname,data in datas.items():
        plt.plot(data['iter'], data[name], label=dataname)
    plt.title(name)
    plt.legend(loc=4)
    plt.grid(True)
    plt.xlabel('N')
    plt.ylabel(name)
  plt.show()
  sys.exit(1)


#----------------------------------------------------------------------------------------

if compile_all:
  compiler.installFada(source_dir=pathmanager.source_dir, build_dir=pathmanager.build_dir, install_dir=pathmanager.install_dir)
  projectname= os.path.split(os.getcwd())[-1]
  compileProject(projectname)
  sys.exit(1)
if clean_lib:
  clean()
  sys.exit(1)
if compile_project:
  projectname= os.path.split(os.getcwd())[-1]
  compileProject(projectname)
  sys.exit(1)
if copy_script:
  copyScript()
  sys.exit(1)
if run_python:
  scriptdir = pathmanager.source_dir + '/projects/' + os.path.split(os.getcwd())[-1]
  runPython(scriptdir)
  sys.exit(1)
if compare:
  runCompare()
  sys.exit(1)


# no arguments
