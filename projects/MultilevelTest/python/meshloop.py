import os, shutil, glob, time

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
        t0 = time.time()
        timer = {'solve':0, 'refine':0, 'interpolate':0, 'updatemesh':0}
        for iteration in range(self.niter):
            ncells = self.meshmanager.getNcells()
            print(72 * '=')
            print("========== MeshLoop: iter {:2d} ({:7d}) ==".format(
                iteration, ncells))
            print(72 * '=')
            currentmesh = self.meshmanager.getCurrentMeshForSolver()

            self.solver.solve(iteration, currentmesh)
            t1 = time.time()
            timer['solve'] += t1-t0
            t0 = t1

            if (iteration == self.niter - 1): return timer

            estimatorfile = os.path.join(self.solver.getResultDir(iteration), "estimator")
            self.meshmanager.refine(iteration, estimatorfile)
            t1 = time.time()
            timer['refine'] += t1-t0
            t0 = t1

            refined_mesh = self.meshmanager.getRefinedMeshForInterpolation()
            print('========== refined_mesh', refined_mesh, 'currentmesh', currentmesh)
            self.solver.interpolate(iteration, currentmesh, refined_mesh)
            t1 = time.time()
            timer['interpolate'] += t1-t0
            t0 = t1

            self.meshmanager.updateAfterRefine()
            self.meshmanager.updateMesh()
            t1 = time.time()
            timer['updatemesh'] += t1-t0
            t0 = t1
