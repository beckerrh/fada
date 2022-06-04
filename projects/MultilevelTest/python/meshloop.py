import os, shutil, glob

# ----------------------------------------------------------------------------------

class MeshLoop():

# ----------------------------------------------------------------
    def __init__(self, meshmanager, solver, niter=3):
        self.solver = solver
        self.niter = niter
        self.meshmanager = meshmanager

# ----------------------------------------------------------------
    def run(self):
        self.meshmanager.readGeometry()
        self.meshmanager.preRefine()
        for iteration in range(self.niter):
            ncells = self.meshmanager.getNcells()
            print(72 * '=')
            print("========== MeshLoop: iter {:2d} ({:7d}) ==".format(
                iteration, ncells))
            print(72 * '=')
            currentmesh = self.meshmanager.getCurrentMeshForSolver()

            self.solver.solve(iteration, currentmesh)

            if (iteration == self.niter - 1): return

            estimatorfile = os.path.join(self.solver.getResultDir(iteration), "estimator")
            self.meshmanager.refine(iteration, estimatorfile)

            refined_mesh = self.meshmanager.getRefinedMeshForInterpolation()
            print('========== refined_mesh', refined_mesh, 'currentmesh', currentmesh)
            self.solver.interpolate(iteration, currentmesh, refined_mesh)

            self.meshmanager.updateAfterRefine()
            self.meshmanager.updateMesh()
