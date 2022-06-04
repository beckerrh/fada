# -*- coding: utf-8 -*-
import os, shutil, glob
import tools.pyfrunexecutable
from . import osadd

# ------------------------------------------------------------------------
class MeshInfo():
    """
    This class contains the following parameters:
      dimension
      nrefine
      geometry
    """

    def __repr__(self):
        string = "\tgeometrytype=" + self.geometrytype
        string += "\tgeometry=" + self.geometry
        string += "\tnrefine=" + str(self.nrefine)
        string += "\tquadtotri=" + str(self.quadtotri)
        string += "\tmarking_type=" + str(self.marking_type)
        string += "\trefinement_parameter=" + str(self.refinement_parameter)
        string += "\tcoarsen_parameter=" + str(self.coarsen_parameter)
        string += "\tnlevels=" + str(self.nlevels)
        string += "\tncellscoarse=" + str(self.ncellscoarse)
        return string

    def __init__(self, nrefine=-1, geometrytype="Quad", geometry="none", quadtotri=False, refinement_parameter=1.0, coarsen_parameter=0.0, marking_type="random", nlevels=10, ncellscoarse=10):
        self.nrefine = nrefine
        self.geometrytype = geometrytype
        self.geometry = geometry
        self.quadtotri = quadtotri
        self.meshtype = "QuadrilateralMesh"
        self.refinement_parameter = refinement_parameter
        self.coarsen_parameter = coarsen_parameter
        self.marking_type = marking_type
        self.nlevels = nlevels
        self.ncellscoarse = ncellscoarse

# ------------------------------------------------------------------------
class MeshManager():

    def __init__(self, installdir, sourcedir, rundir, meshinfo, datatype="binary"):
        self.meshinfo = meshinfo
        self.datatype = datatype
        self.runexecutable = tools.pyfrunexecutable.pyfRunExecutable()
        self.meshtoolspath = os.path.join(installdir, 'bin')

        self.meshdir = os.path.join(rundir, "Meshes")
        if not os.path.isdir(rundir): os.mkdir(rundir)
        if not os.path.isdir(self.meshdir): os.mkdir(self.meshdir)

        if self.meshinfo.meshtype == "QuadrilateralMesh":
            self.mesh_type_in = "quad"
        else:
            raise ValueError("wrong meshtype %s" % self.meshinfo.meshtype)
        self.geometryin = os.path.join(sourcedir, "share", "Mesh", self.meshinfo.meshtype+"es", self.meshinfo.geometry)

        self.quadtotri = meshinfo.quadtotri
        self.initial_local_name = 'start-quad'
        self.refined_local_name = 'start-quadRefined'
        self.coarsen_local_name = 'start-quadCoarsen'
        self.preceding_local_name = "start-quad"
        self.current_local_name = "start-quad"
        if self.quadtotri:
            self.initial_local_name_tri = 'start-tri'
            self.refined_local_name_tri = 'start-triRefined'
            self.coarsen_local_name_tri = 'start-triCoarsen'
            self.current_local_name_tri = "start-tri"
        self.mesh_relative_dir = "."
        self.initial_mesh_name = osadd.localPath(
            self.meshdir, self.mesh_relative_dir, self.initial_local_name)
        self.current_mesh_name = osadd.localPath(
            self.meshdir, self.mesh_relative_dir, self.current_local_name)
        self.refined_mesh_name = osadd.localPath(
            self.meshdir, self.mesh_relative_dir, self.refined_local_name)
        if self.quadtotri:
            self.initial_mesh_name_tri = osadd.localPath(
                self.meshdir, self.mesh_relative_dir, self.initial_local_name_tri)
            self.refined_mesh_name_tri = osadd.localPath(
                self.meshdir, self.mesh_relative_dir, self.refined_local_name_tri)
            self.coarsen_mesh_name_tri = osadd.localPath(
                self.meshdir, self.mesh_relative_dir, self.coarsen_local_name_tri)
            self.current_mesh_name_tri = osadd.localPath(
                self.meshdir, self.mesh_relative_dir, self.current_local_name_tri)

    def getRefinedMeshForInterpolation(self):
        if self.quadtotri:
            return self.refined_mesh_name_tri
        else:
            return self.refined_mesh_name

    def getCurrentMeshForSolver(self):
        if self.quadtotri:
            return self.current_mesh_name_tri
        return self.current_mesh_name

    def copyMesh(self, destination):
        osadd.copytree(self.current_mesh_name + '.fadalightmesh', destination + '.fadalightmesh')

    def updateMesh(self):
        osadd.copytree(
            self.current_mesh_name + '.fadalightmesh', self.preceding_mesh_name + '.fadalightmesh')
        cshort = os.path.split(self.current_mesh_name)[-1]
        pshort = os.path.split(self.preceding_mesh_name)[-1]
        adaptmeshp = self.preceding_mesh_name + \
            '.fadalightmesh/' + cshort + '.fadalightmeshadaptive'
        adaptmeshc = self.preceding_mesh_name + \
            '.fadalightmesh/' + pshort + '.fadalightmeshadaptive'
        shutil.move(adaptmeshp, adaptmeshc)
        self.current_mesh_name = self.preceding_mesh_name
        if self.quadtotri:
            file = open(self.current_mesh_name_tri +
                        '.fadalightmesh/quadfilename')
            quadname = file.readline()
            file.close()
            osadd.copytree(
                self.refined_mesh_name_tri + '.fadalightmesh', self.current_mesh_name_tri + '.fadalightmesh')
            file = open(self.current_mesh_name_tri +
                        '.fadalightmesh/quadfilename', 'w')
            file.write(quadname)

    def getNcells(self):
        if self.quadtotri:
            meshfile = self.current_mesh_name_tri + '.fadalightmesh'
        else:
            meshfile = self.current_mesh_name + '.fadalightmesh'
        file = open(meshfile + os.sep + 'name')
        file.readline()
        info = file.readline()
        file.close()
        return int(info.split('_')[-2])

    def updateAfterRefine(self):
        self.preceding_mesh_name = self.current_mesh_name
        self.current_mesh_name = self.refined_mesh_name

    def updateAfterCoarse(self):
        self.preceding_mesh_name = self.current_mesh_name
        self.current_mesh_name = self.coarsen_mesh_name

    def readGeometry(self):
        executable = os.path.join(self.meshtoolspath, 'ReaderMesh')
        args = [self.geometryin, self.initial_mesh_name, self.mesh_type_in, self.datatype]
        self.runexecutable.run(executable, args)

    def preRefine(self):
        executable = os.path.join(self.meshtoolspath, 'MeshRefiner')
        args = [self.initial_mesh_name, self.initial_mesh_name,
                self.datatype, self.meshinfo.nrefine]
        self.runexecutable.run(executable, args)
        executable = os.path.join(self.meshtoolspath, 'MultiMeshConstructor')
        nlevels = self.meshinfo.nlevels
        ncellscoarse = self.meshinfo.ncellscoarse
        args = [self.initial_mesh_name, self.datatype, nlevels, ncellscoarse]
        self.runexecutable.run(executable, args)
        if self.quadtotri:
            executable = os.path.join(self.meshtoolspath, 'ReaderMesh')
            args = [self.initial_mesh_name,
                    self.initial_mesh_name_tri, "quadtotri", self.datatype]
            self.runexecutable.run(executable, args)
            executable = os.path.join(self.meshtoolspath, 'MultiMeshConstructorQuadToTri')
            args = [self.initial_mesh_name_tri, self.datatype]
            self.runexecutable.run(executable, args)

    def refine(self, iteration, estimatorfile):
        inmeshname = self.current_mesh_name
        marking_type = self.meshinfo.marking_type
        refinement_parameter = self.meshinfo.refinement_parameter
        executable = os.path.join(self.meshtoolspath, 'EstimatorToMarkedCells')

        if self.quadtotri:
            args = [self.current_mesh_name_tri, estimatorfile, self.datatype,
                    marking_type, refinement_parameter]
            print("=== pyMeshManager 'refine' ", executable.split('/')[-1], args)
            self.runexecutable.run(executable, args)

            executable = os.path.join(self.meshtoolspath, 'MarkedCellsTriangleMeshToQuadMesh')
            args = [self.current_mesh_name_tri, self.current_mesh_name]
            print("=== pyMeshManager 'refine' ", executable.split('/')[-1], args)
            self.runexecutable.run(executable, args)
        else:
            args = [self.current_mesh_name, estimatorfile, self.datatype,
                    marking_type, refinement_parameter]
            print("=== pyMeshManager 'refine' ", executable.split('/')[-1], args)
            self.runexecutable.run(executable, args)

        executable = os.path.join(self.meshtoolspath, 'MeshRefiner')
        args = [self.current_mesh_name, self.refined_mesh_name, self.datatype]
        print("=== pyMeshManager 'refine' ", executable.split('/')[-1], args)

        self.runexecutable.run(executable, args)
        executable = os.path.join(self.meshtoolspath, 'MultiMeshConstructor')
        nlevels = self.meshinfo.nlevels
        ncellscoarse = self.meshinfo.ncellscoarse
        args = [self.refined_mesh_name, self.datatype, nlevels, ncellscoarse]
        # print "=== pyfUnStructuredMeshManager 'refine' ", executable, args
        self.runexecutable.run(executable, args)
        if self.quadtotri:
            executable = os.path.join(self.meshtoolspath, 'ReaderMesh')
            args = [self.refined_mesh_name,
                    self.refined_mesh_name_tri, "quadtotri", self.datatype]
            # print "=== pyfUnStructuredMeshManager 'refine' ", executable, args
            self.runexecutable.run(executable, args)
            executable = os.path.join(self.meshtoolspath, 'MultiMeshConstructorQuadToTri')
            args = [self.refined_mesh_name_tri, self.datatype]
            # print "=== pyfUnStructuredMeshManager 'refine' ", executable, args
            self.runexecutable.run(executable, args)

    def coarsen(self, iteration, estimatorfile):
        if iteration == self.data.niter - 1:
            return
        inmeshname = self.current_mesh_name
        marking_type = self.meshinfo.marking_type
        coarsen_parameter = self.meshinfo.coarsen_parameter
        executable = os.path.join(self.meshtoolspath, 'EstimatorToMarkedCells')
        args = [inmeshname, estimatorfile, self.datatype,
                marking_type, coarsen_parameter]
        self.runexecutable.run(executable, args)
        executable = os.path.join(self.meshtoolspath, 'MeshCoarsener')
        args = [self.current_mesh_name, self.refined_mesh_name, self.datatype]
        self.runexecutable.run(executable, args)
        executable = os.path.join(self.meshtoolspath, 'MultiMeshConstructor')
        nlevels = self.meshinfo.nlevels
        ncellscoarse = self.meshinfo.ncellscoarse
        args = [self.refined_mesh_name, self.datatype, nlevels, ncellscoarse]
        self.runexecutable.run(executable, args)
        if self.quadtotri:
            executable = os.path.join(self.meshtoolspath, 'ReaderMesh')
            args = [self.refined_mesh_name,
                    self.refined_mesh_name_tri, "quadtotri", self.datatype]
            self.runexecutable.run(executable, args)
            executable = os.path.join(self.meshtoolspath, 'MultiMeshConstructorQuadToTri')
            args = [self.refined_mesh_name_tri, self.datatype]
            self.runexecutable.run(executable, args)
