import os
from python.meshloop import MeshLoop
from python.solver import Solver
# from python.meshinfo import MeshInfo
from python.meshmanager import MeshManager
from python.meshmanager import MeshInfo
import python.osadd

# ----------------------------------------------------------------------------------

def main(pathmanager, args):
    # Step 1: create MeshInfo
    meshinfo = MeshInfo()
    meshinfo.geometrytype = "Quad"
    meshinfo.nrefine = 0
    meshinfo.geometry = "FourQuads"
    meshinfo.geometry = "Quad"
    meshinfo.geometry = "Square"
    # meshinfo.geometry = "BackwardFacingStep1"
    meshinfo.geometry = "LDomain"
    meshinfo.quadtotri = True
    meshinfo.nlevels = 100
    meshinfo.ncellscoarse = 10
    meshinfo.refinement_parameter = 0.5
    # meshinfo.refinement_parameter=1.1
    # meshinfo.coarsen_parameter = 0.25
    meshinfo.marking_type = "percentage"
    # print('meshinfo', meshinfo)

    # Step 2: create Solver
    rundir = "RunTest"
    modelname = "Laplace"
    solver = Solver(args=args, rundir=rundir)
    solver.cpp_param.addBlock("Mesh")
    solver.cpp_param.parameters["Mesh"]["meshtype"] = "MultilevelTriangleMesh"
    solver.cpp_param.addBlock("ModelManager")
    solver.cpp_param.parameters["ModelManager"]["numberofmodels"] = 1
    solver.cpp_param.parameters["ModelManager"]["model"] = modelname
    solver.cpp_param.addBlock(f"Model_{modelname}")
    solver.cpp_param.parameters[f"Model_{modelname}"]["matrixstorage"] = "U-U"
    solver.cpp_param.parameters[f"Model_{modelname}"]["applicationname"] = "ExactSolution"
    solver.cpp_param.parameters[f"Model_{modelname}"]["fem"] = "cg1"
    solver.cpp_param.addBlock(f"Application_{modelname}")
    solver.cpp_param.parameters[f"Application_{modelname}"]["variablescales"] = "U:1.0"
    solver.cpp_param.parameters[f"Application_{modelname}"]["exactsolution"] = "linear"
    solver.cpp_param.parameters[f"Application_{modelname}"]["exactsolution"]="quadratic"
    solver.cpp_param.parameters[f"Application_{modelname}"]["exactsolution"]="ldomain"
    # solver.cpp_param.parameters[f"Application_{modelname}"]["Dirichlet"] = "100|200|300|400|500|600"
    solver.cpp_param.parameters[f"Application_{modelname}"]["Dirichlet"] = "100|200|300"

    solver.cpp_param.addBlock(f"NumericalParameters_{modelname}")
    solver.cpp_param.parameters[f"NumericalParameters_{modelname}"]["boundarypenalty"] = "10"


    solver.cpp_param.addBlock("Loop")
    solver.cpp_param.parameters["Loop"]["newton"] = "newtonlinesearch_monotonicty"
    # solver.cpp_param.parameters["Loop"]["vectortype"] = "ml"
    # solver.cpp_param.writeParameters(rundir + os.sep + "C++.fadalightparam")
    # solver.cpp_param.readParameters(rundir + os.sep + "C++.fadalightparam")

    solver.cpp_param.addBlock("SolverManager")
    solver.cpp_param.parameters["SolverManager"]["ncellsdirect"]="100"
    solver.cpp_param.parameters["SolverManager"]["linearsolver"]="richardson|U:mg"
    solver.cpp_param.parameters["SolverManager"]["gtol"]="1e-14"
    solver.cpp_param.parameters["SolverManager"]["rtol"]="1e-10"
    solver.cpp_param.parameters["SolverManager"]["printstep"]="1"
    # solver.cpp_param.parameters["SolverManager"]["linearsolver"]="direct"
    solver.cpp_param.addBlock("MultiLevelLinearSolver_U")
    solver.cpp_param.parameters["MultiLevelLinearSolver_U"]["printstep"]="0"
    solver.cpp_param.parameters["MultiLevelLinearSolver_U"]["printstepsmoother"]="0"
    solver.cpp_param.parameters["MultiLevelLinearSolver_U"]["gtol"]="1e-14"
    solver.cpp_param.parameters["MultiLevelLinearSolver_U"]["rtol"]="0"
    solver.cpp_param.parameters["MultiLevelLinearSolver_U"]["maxiter"]="1"
    solver.cpp_param.parameters["MultiLevelLinearSolver_U"]["niterpost"]="1"
    solver.cpp_param.parameters["MultiLevelLinearSolver_U"]["niterpre"]="1"
    solver.cpp_param.addBlock("DomainSolver_U")
    solver.cpp_param.parameters["DomainSolver_U"]["nmaxforcoarsesolver"]="10"
    solver.cpp_param.parameters["DomainSolver_U"]["linearsolver"]="simple"
    solver.cpp_param.parameters["DomainSolver_U"]["variablesorder"]="U"
    solver.cpp_param.addBlock("PreconditionerDomainSolver_U")
    solver.cpp_param.parameters["PreconditionerDomainSolver_U"]["U_prec"]="ilu_simple_0."
    solver.cpp_param.addBlock("NonlinearSolver")
    solver.cpp_param.parameters["NonlinearSolver"]["rtol"]="1e-10"
    solver.cpp_param.parameters["NonlinearSolver"]["gtol"]="1e-14"


    # Step 3: create MeshManager
    meshmanager = MeshManager(installdir=pathmanager.install_dir, sourcedir=pathmanager.source_dir, rundir=rundir, meshinfo=meshinfo, datatype="binary")

    # Step 4: create MeshLoop
    loop = MeshLoop(meshmanager=meshmanager, solver=solver, niter=15)


    if len(args)==0 or args[0] not in ['plt', 'plot']:
        loop.run()

    result = solver.resultmanager.getAllScalarPosrprocess(loop.niter)
    print(f"{result.keys()=}")
    # print(f"{result=}")
    import matplotlib.pyplot as plt
    import numpy as np
    plt.loglog(result["N"], np.sqrt(result["estimator0"]), '-x', label="est0")
    plt.loglog(result["N"], np.sqrt(result["estimator1"]), '-x', label="est1")
    plt.loglog(result["N"], result["Err_U_H1"], '-x', label="err")
    # plt.loglog(result["N"], result["Est2_U"], '-x', label="Est2_U")
    plt.plot(result["N"], np.power(result["N"],-0.5), '-', label="-1/2")
    plt.grid()
    plt.legend()
    plt.show()
