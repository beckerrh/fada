#!/usr/bin/env python3

import sys, os, shutil, subprocess
sys.path.append(os.getcwd() + os.sep + "python")

import tools.pyfpathmanager
import tools.pyfcompile
from argparse import ArgumentParser

root_dir = os.getcwd()
install_dir_default = os.path.join(root_dir,'../installdir')
build_dir_default = os.path.join(root_dir,'../build')

projects = os.listdir('projects')
pathhere = os.path.dirname(os.getcwd())
projectsrun = pathhere+'/projectsRun'
if not os.path.isdir(projectsrun) :
  os.mkdir(projectsrun)
for project in projects:
  newdir = projectsrun + '/' + project
  if not os.path.isdir(newdir) :
    os.mkdir(newdir)

compiler = tools.pyfcompile.pyfCompile()
parser = ArgumentParser(description='Installation of Fada.')
parser.add_argument('scriptandargs', help='script and args to launch', nargs='*')
#parser.add_argument('-i', default = install_dir_default, help='installation directory', metavar="/path/to/install/dir")
#parser.add_argument('-b', default = build_dir_default, help='compilation directory', metavar="/path/to/build/dir")
parser.add_argument('--doc', default = False, action="store_true", help='generate doc')
#parser.add_argument('--all', default = False, action="store_true", help='install all: external tools and Fada')
#parser.add_argument('--py', default = False, action="store_true", help='install python')
#parser.add_argument('--cmake', default = True, action="store_true", help='install CMakeModules')
parser.add_argument('--erase', default = False, action="store_true", help='erase build and install dirs')
compiler.addArgumentParser(parser)
args = vars(parser.parse_args(sys.argv[1:]))
generate_doc = args['doc']
# install_all = args['all']
# install_py = args['py']
# install_cmake = args['cmake']
erase = args['erase']
# install_dir = os.path.abspath(args['i'])
# build_dir = os.path.abspath(args['b'])
install_dir = install_dir_default
build_dir = build_dir_default

compiler.getArgumentsFormParser(args)
if erase:
  if os.path.isdir(install_dir): shutil.rmtree(install_dir)
  if os.path.isdir(build_dir): shutil.rmtree(build_dir)
if not os.path.isdir(install_dir): os.mkdir(install_dir)
if not os.path.isdir(build_dir): os.mkdir(build_dir)

if generate_doc:
  compiler.generateDoc(source_dir=root_dir, build_dir=build_dir, install_dir=install_dir)
  print('ready doc!\n')
  sys.exit(1)

# if install_py:
#   compiler.installFada(source_dir=root_dir, build_dir=build_dir, install_dir=install_dir, name="python")
#
# if install_cmake:
#   compiler.installFada(source_dir=root_dir, build_dir=build_dir, install_dir=install_dir, name="CMakeModules")


pathmanager = tools.pyfpathmanager.pyfPathManager(install_dir=install_dir)
pathmanager.storePaths(source_dir=root_dir, build_dir=build_dir)

returncode = compiler.installFada(source_dir=root_dir, build_dir=build_dir, install_dir=install_dir)
print('returncode', returncode)
