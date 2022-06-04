# -*- coding: utf-8 -*-

import os, shutil, glob
import numpy as np

# ------------------------------------------------------------------------
class ResultManager:

    def __init__(self, rundir, resultsdirbase):
        self.rundir = rundir
        self.resultsdirbase = resultsdirbase
        self.outputtimes = {}
        self.ntimestep = {}
        self.nblocks = 1

    def getSolverInfo(self, resultsdir):
        # resultsdir = self.data.rundir +'/'+self.data.resultsdir
        filename = resultsdir + '/RunInfo/IterationInfo'
        filehandle = open(filename, 'r')
        info = filehandle.readline().split()
        filehandle.close()
        self.nnonlineariteration = int(info[0])
        self.nlineariteration = int(info[1])
        filename = resultsdir + '/RunInfo/TimerStatic'
        filehandle = open(filename, 'r')
        self.cpudetails = {}
        self.cpudetailstotal = {}
        for line in filehandle.readlines():
            linesplit = line.split()
            if not linesplit:
                continue
            if linesplit[0] == 'Total':
                chrononame = linesplit[2]
                self.cpudetails[chrononame] = {}
                self.cpudetailstotal[chrononame] = linesplit[3]
            else:
                self.cpudetails[chrononame][linesplit[0]] = linesplit[2].split('%')[
                    0]
        self.cpu = float(self.cpudetailstotal['SolverManager'])
        filehandle.close()

    def getResultDir(self, iter):
        return os.path.join(self.rundir, self.resultsdirbase + "_iter_%04d" % (iter))

    def getAllScalarPosrprocess(self, niter):
        allvalues = []
        for iter in range(niter):
            resultsdir = self.getResultDir(iter)
            file = open(resultsdir + '/PostProcess/ScalarsStatic.data', 'r')
            if iter==0:
                names = file.readline().split('#')[1].strip().split()
                n = len(names)
                allvalues = np.zeros(shape = (niter, n))
            allvalues[iter] = np.loadtxt(file)
            file.close()
        result = {}
        for i,name in enumerate(names):
            result[name] = allvalues[:,i]
        return result

    def getAllCpus(self, niter):
        self.cpudetails = {}
        self.cpudetailstotal = {}
        for iter in range(niter):
            resultsdir = self.getResultDir(iter)
            filename = resultsdir + '/RunInfo/Timer'
            filehandle = open(filename, 'r')
            for line in filehandle.readlines():
                linesplit = line.split()
                if not linesplit:
                    continue
                if linesplit[0] == 'Total':
                    chrononame = linesplit[2]
                    if iter == 0:
                        self.cpudetails[chrononame] = {}
                        self.cpudetailstotal[chrononame] = {}
                    self.cpudetailstotal[chrononame][iter] = linesplit[3]
                else:
                    if iter == 0:
                        self.cpudetails[chrononame][linesplit[0]] = {}
                    self.cpudetails[chrononame][linesplit[0]][iter] = linesplit[2].split('%')[
                        0]
            filehandle.close()

    def getMeshInfo(self, iteration, resultsdir):
        # print(f"######### {resultsdir=}")
        # resultsdir = self.data.rundir +'/'+self.data.resultsdir
        meshinfofile = resultsdir + '/MeshVisu/MeshInfo'
        filehandle = open(meshinfofile, 'r')
        self.dimension = int(filehandle.readline().strip())
        self.nlevels = int(filehandle.readline().strip())
        info = filehandle.readline().split()
        # self.nblocks = int(info[0])
        shape = (self.nlevels, self.nblocks)
        meshtype = np.zeros(shape=shape, dtype=object)
        topologydimensions = np.zeros(shape=shape, dtype=int)
        nodesdimensions = np.zeros(shape=shape, dtype=int)
        cellsdimensions = np.zeros(shape=shape, dtype=int)
        nnodespercells = np.zeros(shape=shape, dtype=int)

        filehandle.close()
        for level in range(self.nlevels):
            for iblock in range(self.nblocks):
                # blockmeshinfofile = meshinfofile + "_block_%04d_level_%02d" % (iblock, level)
                blockmeshinfofile = meshinfofile + "_level_%02d" % (level)
                filehandle = open(blockmeshinfofile, 'r')
                meshtype[level, iblock] = str(filehandle.readline().strip())
                topologydimensions[level, iblock] = int(filehandle.readline().strip())
                nodesdimensions[level, iblock] = int(filehandle.readline().strip())
                cellsdimensions[level, iblock] = int(filehandle.readline().strip())
                nnodespercells[level, iblock] = int(filehandle.readline().strip())
                filehandle.close()
        return (meshtype, topologydimensions, nodesdimensions, cellsdimensions, nnodespercells)

    def getDynamicLoopInfo(self, iteration, resultsdir):
        # resultsdir = self.data.rundir +'/'+self.data.resultsdir
        filename = resultsdir + "/RunInfo/DynamicLoopInfo"
        filehandle = open(filename, 'r')
        line = filehandle.readline().split()
        self.ntimestep[iteration] = int(line[0]) - 1
        outputtimesofiteration = filehandle.readline().split()
        self.outputtimes[iteration] = outputtimesofiteration

    def getVariablesInfo(self, resultsdir):
        # resultsdir = self.data.rundir +'/'+self.data.resultsdir
        self.variablesinfo = [{}] * self.nblocks
        self.nvarsinblock = [None] * self.nblocks
        filenamebase = resultsdir + '/Unknowns/UInfo'
        level = 0
        for iblock in range(self.nblocks):
            # filename = filenamebase + "_block_%04d_level_%02d" % (iblock, level)
            filename = filenamebase
            filehandle = open(filename, 'r')
            info = filehandle.readlines()
            self.nvarsinblock[iblock] = len(info)
            varinblock = {}
            for line in info:
                lineinfo = line.strip().split()
                varname = lineinfo[0]
                varinblock[varname] = lineinfo[1]
            self.variablesinfo[iblock] = varinblock
        self.postprocessinfo = [{}] * self.nblocks
        self.npostprocessinblock = [None] * self.nblocks
        filenamebase = resultsdir + '/PostProcess/PInfo'
        # if not os.path.isfile(filenamebase + '_block_%04d' % (0)):
        #     return
        for iblock in range(self.nblocks):
            # filename = filenamebase + '_block_%04d' % (iblock)
            filename = filenamebase
            filehandle = open(filename, 'r')
            info = filehandle.readlines()
            self.npostprocessinblock[iblock] = len(info)
            varinblock = {}
            for line in info:
                lineinfo = line.strip().split()
                varname = lineinfo[0]
                varinblock[varname] = lineinfo[1]
            self.postprocessinfo[iblock] = varinblock

    # def write(self, iteration):
    #   if iteration == 0:
    #     filehandle = open(self.getResultsFileName(), 'w')
    #     filehandle.write("#%7s %8s %8s %8s %8s %12s" %("iter", "nliter", "liniter", "nblocks", "N", "cpu" ))
    #     for name, value in sorted(self.postprocess.iteritems()):
    #       count=0
    #       for v in value.split():
    #         namev = name + '_'+ str(count)
    #         filehandle.write("  %14s" %( namev  ))
    #         count = count+1
    #     filehandle.write("\n")
    #     filehandle.close()
    #   filehandle = open(self.getResultsFileName(), 'a')
    #   filehandle.write("%8d %8d %8d %8d %8d %9.3e" %(iteration, self.nnonlineariteration, self.nlineariteration, self.nblocks, self.ncells, self.cpu ))
    #   for name, value in sorted(self.postprocess.iteritems()):
    #     for v in value.split():
    #       filehandle.write("%16.6e" %(float(v) ))
    #   filehandle.write("\n")
    #   filehandle.close()
