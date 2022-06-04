# -*- coding: utf-8 -*-
from . import osadd

# ------------------------------------------------------------------------
class XdmfWriter:

    def __init__(self, rundir, resultsdirbase, meshinfodir, resultmanager, solverloop, visutype):
        self.rundir = rundir
        self.resultsdirbase = resultsdirbase
        self.meshinfodir = meshinfodir
        self.resultmanager = resultmanager
        self.solverloop = solverloop
        self.visutype = visutype
        self.nblocks = 1

# ------------------------------------------------------------------------
    def visu(self, iteration, resultsdir, infomesh, infovars, name="toto", level=0):
        self.infomesh = infomesh
        self.infovars = infovars
        if self.solverloop == "static" or self.solverloop == "linear":
            if self.visutype == "ml":
                self.visuStaticMl(iteration, name)
            else:
                self.visuStatic(iteration, name, level)
        elif self.solverloop == "dynamic":
            self.visuDynamic(name, level)
        else:
            raise KeyError("unknown xdmfwriter for loop '{}' visutype '{}'".format(
                self.solverloop, self.visutype))

# ------------------------------------------------------------------------
    def visuOneIteration(self, iter, level, filehandle, gridname, time, timestep=-1):
        (_meshtype, _topologydimensions, _nodesdimensions, _cellsdimensions, _nnodespercells)=self.infomesh
        (varunknown, varpostproc) = self.infovars
        resultsdir = self.resultsdirbase + "_iter_%04d/" % (iter)
        gridnameplusiter = gridname + "_iter_%04d" % (iter)
        filehandle.write('<Grid Name="%s" GridType="Collection" CollectionType="Spacial">\n' % (gridnameplusiter))
        filehandle.write('<Time Value="%g"/>\n' % (time))
        # for block in range(self.nblocks):
        block = 0
        block_name = "iter_%04d" % (iter)
        filehandle.write('<Grid Name="%s" GridType="Uniform">\n' % (block_name))
        h5meshfile = resultsdir + self.meshinfodir + '/Mesh' +"_level_%02d.h5" % (level)
        topologydimensions = _topologydimensions[level]
        meshtype = _meshtype[level]
        ncells = _cellsdimensions[level]
        nnodes = _nodesdimensions[level]
        nnodespercell = _nnodespercells[level]
        meshdim = "%s %s" % (nnodes, self.resultmanager.dimension)
        filehandle.write('<Topology TopologyType="%s" NumberOfElements="%s">\n' % (meshtype, topologydimensions))
        filehandle.write('<DataItem Format="HDF" Dimensions="%s %s" >\n' % (ncells, nnodespercell))
        filehandle.write(h5meshfile + ":/connectivities\n")
        filehandle.write("</DataItem>\n")
        filehandle.write("</Topology>\n")
        nodedesc = "XY"
        if (self.resultmanager.dimension == 3): nodedesc += "Z"
        filehandle.write('<Geometry GeometryType="%s">\n' % (nodedesc))
        filehandle.write('<DataItem Format="HDF" Dimensions="%s">\n' % (meshdim))
        filehandle.write(h5meshfile + ":/%s\n" % (nodedesc))
        filehandle.write("</DataItem>\n")
        filehandle.write("</Geometry>\n")
        for varname, visutype in varunknown.items():
            filehandle.write('<Attribute Name="%s" Center="%s">\n' % (varname, visutype))
            if (visutype == "cell"): dimension = ncells
            elif (visutype == "node"): dimension = nnodes
            elif (visutype == "grid"): dimension = "1"
            else:
                raise ValueError("unknown visutype \'" + visutype + "\'")
            filehandle.write('<DataItem Format="HDF" Dimensions="%s">\n' % (dimension))
            if timestep == -1:
                h5variablefile = resultsdir + "/Unknowns/U_level_%02d.h5" % (level)
            else:
                h5variablefile = resultsdir + "/Unknowns/U_timestep_%07d_level_%02d.h5" % (timestep, level)
            filehandle.write(h5variablefile + ":/%s\n" % (varname))
            filehandle.write("</DataItem>\n")
            filehandle.write("</Attribute>\n")
        for varname, visutype in varpostproc.items():
            filehandle.write('<Attribute Name="%s" Center="%s">\n' % (varname, visutype))
            if (visutype == "cell"): dimension = ncells
            elif (visutype == "node"): dimension = nnodes
            elif (visutype == "grid"): dimension = "1"
            else:
                raise ValueError("unknown visutype \'" + visutype + "\'")
            filehandle.write('<DataItem Format="HDF" Dimensions="%s">\n' % (dimension))
            h5variablefile = resultsdir + "/PostProcess/P_level_%02d.h5" % (level)
            filehandle.write(h5variablefile + ":/%s\n" % (varname))
            filehandle.write("</DataItem>\n")
            filehandle.write("</Attribute>\n")
        filehandle.write("</Grid>\n")
        filehandle.write("</Grid>\n")

# ------------------------------------------------------------------------
    def visuStatic(self, iteration, name="toto", level=0):
        level = int(level)
        gridname = name + "_" + self.solverloop
        xdmffile = osadd.localPath(
            self.rundir, name + '.xmf')
        filehandle = open(xdmffile, 'w')
        filehandle.write('<?xml version="1.0" ?>\n')
        filehandle.write('<Xdmf  Version="2.0" xmlns:xi="http://www.w3.org/2003/XInclude">\n')
        filehandle.write("<Domain>\n")
        filehandle.write('<Grid Name="%s" GridType="Collection" CollectionType="Temporal">\n' % (gridname))
        for iter in range(iteration + 1):
            self.visuOneIteration(iter=iter, level=level, filehandle=filehandle, gridname=gridname, time=iter)
        filehandle.write("</Grid>\n")
        filehandle.write("</Domain>\n")
        filehandle.write("</Xdmf>\n")
        filehandle.close()

# ------------------------------------------------------------------------
    def visuStaticMl(self, iteration, name="toto"):
        name = "%s_%02d" % (name, iteration)
        gridname = name + "_" + self.solverloop
        xdmffile = osadd.localPath(self.rundir, name + '.xmf')
        filehandle = open(xdmffile, 'w')
        filehandle.write('<?xml version="1.0" ?>\n')
        filehandle.write('<Xdmf  Version="2.0" xmlns:xi="http://www.w3.org/2003/XInclude">\n')
        filehandle.write("<Domain>\n")
        filehandle.write('<Grid Name="%s" GridType="Collection" CollectionType="Temporal">\n' % (gridname))
        nlevels = self.resultmanager.nlevels
        for level in range(nlevels):
            self.visuOneIteration(iter=iteration, level=level, filehandle=filehandle, gridname=gridname, time=level)
        filehandle.write("</Grid>\n")
        filehandle.write("</Domain>\n")
        filehandle.write("</Xdmf>\n")
        filehandle.close()

# ------------------------------------------------------------------------
    def visuDynamic(self, name="toto", level=0):
        level = int(level)
        gridname = name + "_" + self.solverloop
        xdmffile = osadd.localPath(
            self.rundir, name + 'Dynamic.xmf')
        filehandle = open(xdmffile, 'w')
        filehandle.write('<?xml version="1.0" ?>\n')
        filehandle.write(
            '<Xdmf  Version="2.0" xmlns:xi="http://www.w3.org/2003/XInclude">\n')
        filehandle.write("<Domain>\n")
        filehandle.write(
            '<Grid Name="%s" GridType="Collection" CollectionType="Temporal">\n' % (gridname))
        for iter in range(self.data.iteration + 1):
            resultsdir = self.resultsdirbase + "_iter_%04d/" % (iter)
            for timestep, time in enumerate(self.resultmanager.outputtimes[iter]):
                self.visuOneIteration(iter=iter, level=level, filehandle=filehandle,gridname=gridname, time=float(time), timestep=timestep)
        filehandle.write("</Grid>\n")
        filehandle.write("</Domain>\n")
        filehandle.write("</Xdmf>\n")
        filehandle.close()
