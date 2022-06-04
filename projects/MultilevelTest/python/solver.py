import os
import tools.pyfrunexecutable
import tools.pyfparam
from .resultmanager import ResultManager
from .xdmfwriter import XdmfWriter

# ----------------------------------------------------------------------------------
class Solver():
    def __init__(self, args, rundir):
        self.args = args
        self.rundir = rundir
        self.runexecutable = tools.pyfrunexecutable.pyfRunExecutable()

        # self.visutype = "ml"
        self.visutype = "normal"

        self.readunknownvariables = "no"
        self.meshinfodir = "MeshVisu"
        self.resultsdirbase = "results"
        self.solverloop = "static"

        self.cpp_param = tools.pyfparam.pyfParamatersForFada()

        self.resultmanager = ResultManager(
            rundir=self.rundir, resultsdirbase=self.resultsdirbase)
        self.visu_manager = XdmfWriter(
            rundir=self.rundir, resultsdirbase=self.resultsdirbase, meshinfodir=self.meshinfodir, resultmanager=self.resultmanager, solverloop=self.solverloop, visutype=self.visutype)

# ----------------------------------------------------------------------------------
    def runloop(self, loopname):
        project_exec = "./" + os.path.basename(os.getcwd())
        paramfile = project_exec + ".fadalightparam"
        self.cpp_param.writeParameters(os.path.join(self.rundir, paramfile))
        args_list = [project_exec, loopname, self.rundir]
        print("============= runloop command = '{}''".format(args_list))
        self.runexecutable.run_subprocess(args_list)

    def getResultDir(self, iteration):
        return os.path.join(self.rundir, "{}_iter_{:04d}".format(self.resultsdirbase, iteration))

# ----------------------------------------------------------------------------------
    def solve(self, iteration, currentmesh):
        resultsdir = self.getResultDir(iteration)
        resultsdirshort = os.path.split(resultsdir)[-1]
        self.cpp_param.setParameter("IoManager", "resultsdir", resultsdirshort)
        readu = 'yes'
        if iteration == 0: readu = self.readunknownvariables
        self.cpp_param.setParameter("IoManager", "readunknownvariables", readu)
        self.cpp_param.setParameter("Mesh", "meshname", currentmesh)
        # self.runloop("prolongate")
        self.runloop("static")
        self.runloop("postprocess")
        infomesh = self.resultmanager.getMeshInfo(iteration, resultsdir)
        infovars = self.resultmanager.getVariablesInfo(resultsdir)
        self.visu_manager.visu(iteration, resultsdir, infomesh, infovars)

# ----------------------------------------------------------------------------------
    def interpolate(self, iteration, currentmesh, refined_mesh):
        # resultsdirnext = self.resultsdirbase + "_iter_%04d" % (iteration + 1)
        resultsdirnext = self.getResultDir(iteration+1)
        resultsdirnextshort = os.path.split(resultsdirnext)[-1]
        resultsdir = self.getResultDir(iteration)
        inputdir = os.path.join(resultsdir, "Unknowns")
        self.cpp_param.setParameter("Mesh", "refinedmeshname", refined_mesh)
        self.cpp_param.setParameter("IoManager", "inputdir", inputdir)
        # print("######### resultsdirnext",resultsdirnext)
        self.cpp_param.setParameter("IoManager", "resultsdir", resultsdirnextshort)
        self.runloop("interpolation")
        self.cpp_param.erase("IoManager", "inputdir")
        self.cpp_param.erase("IoManager", "inputfile")
        # self.resultmanager.getMeshInfo(iteration, resultsdirnext)
        # self.resultmanager.getVariablesInfo(resultsdirnext)
        # self.visu_manager.visu(iteration, name="interpolated")
