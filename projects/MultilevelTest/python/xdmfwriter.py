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
    def visu(self, iteration, resultsdir, info, name="toto", level=0):
        self.info = info
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
        (_meshtype, _topologydimensions, _nodesdimensions, _cellsdimensions, _nnodespercells)=self.info
        resultsdir = self.resultsdirbase + "_iter_%04d/" % (iter)
        gridnameplusiter = gridname + "_iter_%04d" % (iter)
        filehandle.write(
            '<Grid Name="%s" GridType="Collection" CollectionType="Spacial">\n' % (gridnameplusiter))
        filehandle.write('<Time Value="%g"/>\n' % (time))
        for block in range(self.nblocks):
            # block_name = "iter_%04d_block_%07d" % (iter, block)
            block_name = "iter_%04d" % (iter)
            filehandle.write(
                '<Grid Name="%s" GridType="Uniform">\n' % (block_name))
            # h5meshfile = resultsdir + self.meshinfodir + '/Mesh' +"_block_%04d_level_%02d.h5" % (block, level)
            h5meshfile = resultsdir + self.meshinfodir + '/Mesh' +"_level_%02d.h5" % (level)
            topologydimensions = _topologydimensions[level, block]
            meshtype = _meshtype[level, block]
            ncells = _cellsdimensions[level, block]
            nnodes = _nodesdimensions[level, block]
            nnodespercell = _nnodespercells[level, block]
            meshdim = "%s %s" % (nnodes, self.resultmanager.dimension)

            filehandle.write('<Topology TopologyType="%s" NumberOfElements="%s">\n' % (
                meshtype, topologydimensions))
            filehandle.write(
                '<DataItem Format="HDF" Dimensions="%s %s" >\n' % (ncells, nnodespercell))
            filehandle.write(h5meshfile + ":/connectivities\n")
            filehandle.write("</DataItem>\n")
            filehandle.write("</Topology>\n")
            nodedesc = "XY"
            if (self.resultmanager.dimension == 3):
                nodedesc += "Z"
            filehandle.write('<Geometry GeometryType="%s">\n' % (nodedesc))
            filehandle.write(
                '<DataItem Format="HDF" Dimensions="%s">\n' % (meshdim))
            filehandle.write(h5meshfile + ":/%s\n" % (nodedesc))
            filehandle.write("</DataItem>\n")
            filehandle.write("</Geometry>\n")
            #
            varinblock = self.resultmanager.variablesinfo[block]
            for varname, visutype in varinblock.items():
                filehandle.write(
                    '<Attribute Name="%s" Center="%s">\n' % (varname, visutype))
                if (visutype == "cell"):
                    dimension = ncells
                elif (visutype == "node"):
                    dimension = nnodes
                elif (visutype == "grid"):
                    dimension = "1"
                else:
                    raise ValueError("unknown visutype \'" + visutype + "\'")
                filehandle.write(
                    '<DataItem Format="HDF" Dimensions="%s">\n' % (dimension))
                if timestep == -1:
                    # h5variablefile = resultsdir + "/Unknowns/U_block_%04d_level_%02d.h5" % (block, level)
                    h5variablefile = resultsdir + "/Unknowns/U_level_%02d.h5" % (level)
                else:
                    h5variablefile = resultsdir + "/Unknowns/U_timestep_%07d_level_%02d.h5" % (
                            timestep, level)
                filehandle.write(h5variablefile + ":/%s\n" % (varname))
                filehandle.write("</DataItem>\n")
                filehandle.write("</Attribute>\n")
            #
            #
            varinblock = self.resultmanager.postprocessinfo[block]
            for varname, visutype in varinblock.items():
                filehandle.write(
                    '<Attribute Name="%s" Center="%s">\n' % (varname, visutype))
                if (visutype == "cell"):
                    dimension = ncells
                elif (visutype == "node"):
                    dimension = nnodes
                elif (visutype == "grid"):
                    dimension = "1"
                else:
                    raise ValueError("unknown visutype \'" + visutype + "\'")
                filehandle.write(
                    '<DataItem Format="HDF" Dimensions="%s">\n' % (dimension))
                # h5variablefile = resultsdir + "/PostProcess/P_block_%04d_level_%02d.h5" % (block, level)
                h5variablefile = resultsdir + "/PostProcess/P_level_%02d.h5" % (level)
                filehandle.write(h5variablefile + ":/%s\n" % (varname))
                filehandle.write("</DataItem>\n")
                filehandle.write("</Attribute>\n")
            #
            filehandle.write("</Grid>\n")
        filehandle.write("</Grid>\n")

# ------------------------------------------------------------------------
    def visuStatic(self, iteration, name="toto", level=0):
        level = int(level)
        if len(self.resultmanager.variablesinfo) != self.nblocks:
            raise KeyError("wrong size of self.resultmanager.variablesinfo " + str(len(
                self.resultmanager.variablesinfo)) + " != self.nblocks", str(self.nblocks))
        if len(self.resultmanager.postprocessinfo) != self.nblocks:
            raise KeyError("wrong size of self.resultmanager.postprocessinfo " + str(len(
                self.resultmanager.postprocessinfo)) + " != self.nblocks", str(self.nblocks))
        gridname = name + "_" + self.solverloop
        xdmffile = osadd.localPath(
            self.rundir, name + '.xmf')
        filehandle = open(xdmffile, 'w')
        filehandle.write('<?xml version="1.0" ?>\n')
        filehandle.write(
            '<Xdmf  Version="2.0" xmlns:xi="http://www.w3.org/2003/XInclude">\n')
        filehandle.write("<Domain>\n")
        filehandle.write(
            '<Grid Name="%s" GridType="Collection" CollectionType="Temporal">\n' % (gridname))
        for iter in range(iteration + 1):
            self.visuOneIteration(
                iter=iter, level=level, filehandle=filehandle, gridname=gridname, time=iter)
        filehandle.write("</Grid>\n")
        filehandle.write("</Domain>\n")
        filehandle.write("</Xdmf>\n")
        filehandle.close()

# ------------------------------------------------------------------------
    def visuStaticMl(self, iteration, name="toto"):
        name = "%s_%02d" % (name, iteration)
        if len(self.resultmanager.variablesinfo) != self.nblocks:
            raise KeyError("wrong size of self.resultmanager.variablesinfo " + str(len(
                self.resultmanager.variablesinfo)) + " != self.nblocks", str(self.nblocks))
        if len(self.resultmanager.postprocessinfo) != self.nblocks:
            raise KeyError("wrong size of self.resultmanager.postprocessinfo " + str(len(
                self.resultmanager.postprocessinfo)) + " != self.nblocks", str(self.nblocks))
        gridname = name + "_" + self.solverloop
        xdmffile = osadd.localPath(self.rundir, name + '.xmf')
        filehandle = open(xdmffile, 'w')
        filehandle.write('<?xml version="1.0" ?>\n')
        filehandle.write(
            '<Xdmf  Version="2.0" xmlns:xi="http://www.w3.org/2003/XInclude">\n')
        filehandle.write("<Domain>\n")
        filehandle.write(
            '<Grid Name="%s" GridType="Collection" CollectionType="Temporal">\n' % (gridname))
        nlevels = self.resultmanager.nlevels
        for level in range(nlevels):
            self.visuOneIteration(
                iter=iteration, level=level, filehandle=filehandle, gridname=gridname, time=level)
        filehandle.write("</Grid>\n")
        filehandle.write("</Domain>\n")
        filehandle.write("</Xdmf>\n")
        filehandle.close()

# ------------------------------------------------------------------------
    def visuDynamic(self, name="toto", level=0):
        level = int(level)
        if len(self.resultmanager.variablesinfo) != self.nblocks:
            raise KeyError("wrong size of self.resultmanager.variablesinfo " + str(len(
                self.resultmanager.variablesinfo)) + " != self.nblocks", str(self.nblocks))
        if len(self.resultmanager.postprocessinfo) != self.nblocks:
            raise KeyError("wrong size of self.resultmanager.postprocessinfo " + str(len(
                self.resultmanager.postprocessinfo)) + " != self.nblocks", str(self.nblocks))
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
                self.visuOneIteration(iter=iter, level=level, filehandle=filehandle,
                                      gridname=gridname, time=float(time), timestep=timestep)
        filehandle.write("</Grid>\n")
        filehandle.write("</Domain>\n")
        filehandle.write("</Xdmf>\n")
        filehandle.close()

 # ------------------------------------------------------------------------
    def visuDynamic_old(self, name="toto"):
        gridname = name + "_" + \
            self.data.python_param.getParameter('Loop', 'loopname')
        xdmffile = osadd.localPath(
            self.data.rundir, name + 'Dynamic.xmf')
        filehandle = open(xdmffile, 'w')
        filehandle.write('<?xml version="1.0" ?>\n')
        filehandle.write(
            '<Xdmf xmlns:xi="http://www.w3.org/2003/XInclude" Version="2.1">\n')
        filehandle.write("<Domain>\n")
        filehandle.write(
            '<Grid Name="%s" GridType="Collection" CollectionType="Temporal">\n' % (gridname))
        for iter in range(self.data.iteration + 1):
            resultsdir = self.data.resultsdirbase + "_iter_%04d/" % (iter)
            for timestep, time in enumerate(self.resultmanager.outputtimes[iter]):
                gridnameplusiter = gridname + \
                    "_iter_%04d_timestep_%07d" % (iter, timestep)
                filehandle.write(
                    '<Grid Name="%s" GridType="Collection" CollectionType="Spacial">\n' % (gridnameplusiter))
                filehandle.write('<Time Value="%s"/>\n' % (time))
                for block in range(self.nblocks):
                    # block_name = "iter_%04d_block_%04d" % (iter, block)
                    block_name = "iter_%04d" % (iter)
                    filehandle.write(
                        '<Grid Name="%s" GridType="Uniform">\n' % (block_name))
                    # h5meshfile = resultsdir + '/MeshVisu/Mesh_block_%04d.h5' % (block)
                    h5meshfile = resultsdir + '/MeshVisu/Mesh.h5'
                    topologydimensions = topologydimensions[iter][block]
                    meshtype = meshtype[iter][block]
                    filehandle.write('<Topology TopologyType="%s" NumberOfElements="%s">\n' % (
                        meshtype, topologydimensions))
                    if (meshtype == "Hexahedron" or meshtype == "Quadrilateral"):
                        ncells = cellsdimensions[iter][block]
                        nnodespercell = nnodespercells[iter][block]
                        filehandle.write(
                            '<DataItem Format="HDF" Dimensions="%s %s" >\n' % (ncells, nnodespercell))
                        filehandle.write(h5meshfile + ":/connectivities\n")
                        filehandle.write("</DataItem>\n")
                    filehandle.write("</Topology>\n")
                    nodedesc = "XY"
                    if (self.resultmanager.dimension == 3):
                        nodedesc += "Z"
                    filehandle.write(
                        '<Geometry GeometryType="%s">\n' % (nodedesc))
                    # meshdim=meshdatadimensions[iter][block]
                    filehandle.write(
                        '<DataItem Format="HDF" Dimensions="%s">\n' % (meshdim))
                    filehandle.write(h5meshfile + ":/%s\n" % (nodedesc))
                    filehandle.write("</DataItem>\n")
                    filehandle.write("</Geometry>\n")
                    #
                    varinblock = self.resultmanager.variablesinfo[block]
                    for varname, visutype in varinblock.iteritems():
                        filehandle.write(
                            '<Attribute Name="%s" Center="%s">\n' % (varname, visutype))
                        if (visutype == "cell"):
                            dimension = cellsdimensions[iter][block]
                        else:
                            dimension = nodesdimensions[iter][block]
                        filehandle.write(
                            '<DataItem Format="HDF" Dimensions="%s">\n' % (dimension))
                        # h5variablefile = resultsdir + "/Unknowns/U_timestep_%07d_block_%04d.h5" % (timestep, block)
                        h5variablefile = resultsdir + "/Unknowns/U_timestep_%07d.h5" % (timestep)
                        filehandle.write(h5variablefile + ":/%s\n" % (varname))
                        filehandle.write("</DataItem>\n")
                        filehandle.write("</Attribute>\n")
                    filehandle.write("</Grid>\n")
                filehandle.write("</Grid>\n")
        filehandle.write("</Grid>\n")
        filehandle.write("</Domain>\n")
        filehandle.write("</Xdmf>\n")
        filehandle.close()
