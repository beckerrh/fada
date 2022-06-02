# -*- coding: utf-8 -*-


class MeshInfo():
    """
    This class contains the following parameters:
      dimension
      nrefine
      geometry
    """

    def __repr__(self):
        # string =  "\n\tdimension="+str(self.dimension)+"\n"
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
        if self.geometrytype == "Hex": self.meshtype = "HexahedralMesh"
        self.refinement_parameter = refinement_parameter
        self.coarsen_parameter = coarsen_parameter
        self.marking_type = marking_type
        self.nlevels = nlevels
        self.ncellscoarse = ncellscoarse
