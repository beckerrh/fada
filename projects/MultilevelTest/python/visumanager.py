# -*- coding: utf-8 -*-
from . import osadd

# ------------------------------------------------------------------------
class VisuManager:

    def __init__(self, rundir, resultsdirbase, meshinfodir, resultmanager, solverloop, visutype):
        self.rundir = rundir
        self.resultsdirbase = resultsdirbase
        self.meshinfodir = meshinfodir
        self.resultmanager = resultmanager
        self.solverloop = solverloop
        self.visutype = visutype

# ------------------------------------------------------------------------
    def visu(self, iteration, name="toto", level=0):
        if self.solverloop == "static" or self.solverloop == "linear":
            if self.visutype == "ml":
                self.visuStaticMl(iteration, name)
            else:
                self.visuStatic(iteration, name, level)
        elif self.solverloop == "dynamic":
            self.visuDynamic(name, level)
        else:
            raise KeyError("unknown visumanager for loop '{}' visutype '{}'".format(
                self.solverloop, self.visutype))

# ------------------------------------------------------------------------
    def visuOneIteration(self, iter, level, filehandle, gridname, time, timestep=-1):
        resultdir = self.resultsdirbase + "_iter_%04d/" % (iter)
        gridnameplusiter = gridname + "_iter_%04d" % (iter)
        filehandle.write(
            '<Grid Name="%s" GridType="Collection" CollectionType="Spacial">\n' % (gridnameplusiter))
        filehandle.write('<Time Value="%g"/>\n' % (time))
        for block in range(self.resultmanager.nblocks):
            block_name = "iter_%04d_block_%07d" % (iter, block)
            filehandle.write(
                '<Grid Name="%s" GridType="Uniform">\n' % (block_name))
            h5meshfile = resultdir + self.meshinfodir + '/Mesh' + \
                "_block_%04d_level_%02d.h5" % (block, level)
            # print 'iter', iter, 'block', block, 'level', level
            # print 'self.resultmanager.blocktopologydimensions', self.resultmanager.blocktopologydimensions
            topologydimensions = self.resultmanager.blocktopologydimensions[iter][level, block]
            # print 'topologydimensions', topologydimensions
            meshtype = self.resultmanager.blockmeshtype[iter][level, block]
            ncells = self.resultmanager.blockcellsdimensions[iter][level, block]
            nnodes = self.resultmanager.blocknodesdimensions[iter][level, block]
            nnodespercell = self.resultmanager.blocknnodespercells[iter][level, block]
            # meshdim=self.resultmanager.blockmeshdatadimensions[iter][level,block]
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
                    h5variablefile = resultdir + \
                        "/Unknowns/U_block_%04d_level_%02d.h5" % (block, level)
                else:
                    h5variablefile = resultdir + \
                        "/Unknowns/U_timestep_%07d_block_%04d_level_%02d.h5" % (
                            timestep, block, level)
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
                h5variablefile = resultdir + \
                    "/PostProcess/P_block_%04d_level_%02d.h5" % (block, level)
                filehandle.write(h5variablefile + ":/%s\n" % (varname))
                filehandle.write("</DataItem>\n")
                filehandle.write("</Attribute>\n")
            #
            filehandle.write("</Grid>\n")
        filehandle.write("</Grid>\n")

# ------------------------------------------------------------------------
    def visuStatic(self, iteration, name="toto", level=0):
        level = int(level)
        if len(self.resultmanager.variablesinfo) != self.resultmanager.nblocks:
            raise KeyError("wrong size of self.resultmanager.variablesinfo " + str(len(
                self.resultmanager.variablesinfo)) + " != self.resultmanager.nblocks", str(self.resultmanager.nblocks))
        if len(self.resultmanager.postprocessinfo) != self.resultmanager.nblocks:
            raise KeyError("wrong size of self.resultmanager.postprocessinfo " + str(len(
                self.resultmanager.postprocessinfo)) + " != self.resultmanager.nblocks", str(self.resultmanager.nblocks))
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
        if len(self.resultmanager.variablesinfo) != self.resultmanager.nblocks:
            raise KeyError("wrong size of self.resultmanager.variablesinfo " + str(len(
                self.resultmanager.variablesinfo)) + " != self.resultmanager.nblocks", str(self.resultmanager.nblocks))
        if len(self.resultmanager.postprocessinfo) != self.resultmanager.nblocks:
            raise KeyError("wrong size of self.resultmanager.postprocessinfo " + str(len(
                self.resultmanager.postprocessinfo)) + " != self.resultmanager.nblocks", str(self.resultmanager.nblocks))
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
        if len(self.resultmanager.variablesinfo) != self.resultmanager.nblocks:
            raise KeyError("wrong size of self.resultmanager.variablesinfo " + str(len(
                self.resultmanager.variablesinfo)) + " != self.resultmanager.nblocks", str(self.resultmanager.nblocks))
        if len(self.resultmanager.postprocessinfo) != self.resultmanager.nblocks:
            raise KeyError("wrong size of self.resultmanager.postprocessinfo " + str(len(
                self.resultmanager.postprocessinfo)) + " != self.resultmanager.nblocks", str(self.resultmanager.nblocks))
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
            resultdir = self.resultsdirbase + "_iter_%04d/" % (iter)
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
            resultdir = self.data.resultsdirbase + "_iter_%04d/" % (iter)
            for timestep, time in enumerate(self.resultmanager.outputtimes[iter]):
                gridnameplusiter = gridname + \
                    "_iter_%04d_timestep_%07d" % (iter, timestep)
                filehandle.write(
                    '<Grid Name="%s" GridType="Collection" CollectionType="Spacial">\n' % (gridnameplusiter))
                filehandle.write('<Time Value="%s"/>\n' % (time))
                for block in range(self.resultmanager.nblocks):
                    block_name = "iter_%04d_block_%04d" % (iter, block)
                    filehandle.write(
                        '<Grid Name="%s" GridType="Uniform">\n' % (block_name))
                    h5meshfile = resultdir + \
                        '/MeshVisu/Mesh_block_%04d.h5' % (block)
                    topologydimensions = self.resultmanager.blocktopologydimensions[iter][block]
                    meshtype = self.resultmanager.blockmeshtype[iter][block]
                    filehandle.write('<Topology TopologyType="%s" NumberOfElements="%s">\n' % (
                        meshtype, topologydimensions))
                    if (meshtype == "Hexahedron" or meshtype == "Quadrilateral"):
                        ncells = self.resultmanager.blockcellsdimensions[iter][block]
                        nnodespercell = self.resultmanager.blocknnodespercells[iter][block]
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
                    # meshdim=self.resultmanager.blockmeshdatadimensions[iter][block]
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
                            dimension = self.resultmanager.blockcellsdimensions[iter][block]
                        else:
                            dimension = self.resultmanager.blocknodesdimensions[iter][block]
                        filehandle.write(
                            '<DataItem Format="HDF" Dimensions="%s">\n' % (dimension))
                        h5variablefile = resultdir + \
                            "/Unknowns/U_timestep_%07d_block_%04d.h5" % (
                                timestep, block)
                        filehandle.write(h5variablefile + ":/%s\n" % (varname))
                        filehandle.write("</DataItem>\n")
                        filehandle.write("</Attribute>\n")
                    filehandle.write("</Grid>\n")
                filehandle.write("</Grid>\n")
        filehandle.write("</Grid>\n")
        filehandle.write("</Domain>\n")
        filehandle.write("</Xdmf>\n")
        filehandle.close()
