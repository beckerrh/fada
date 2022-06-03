# ==============================================================================
#                              Find external libraries and save variables to file
# ==============================================================================

#
#---- BLAS ---------
find_package(BLAS REQUIRED)
#
#---- MPI ---------
SET(MPICOMPILE "ON")
IF(MPICOMPILE STREQUAL "ON")
  find_package(MPI)
  if(MPI_FOUND)
  else()
  add_definitions(-DNOMPI)
  endif()
else()
  add_definitions(-DNOMPI)
ENDIF()
#
#---- Boost ---------
Find_Package( Boost COMPONENTS graph)
IF(Boost_FOUND)
	add_definitions(-DBOOST)
ELSE(Boost_FOUND)
ENDIF(Boost_FOUND)
#
#---- umfpack ---------
FIND_LIBRARY(UMFPACK_LIBRARIES NAMES umfpack libumfpack UMFPACK libUMFPACK
               PATH_SUFFIXES ${CMAKE_LIBRARY_ARCHITECTURE})
#
#---- Armadillo ---------
find_package(Armadillo REQUIRED)
#
#---- Hdf5 ---------
find_package(HDF5 REQUIRED)

#
#---- Metis ---------
FIND_LIBRARY(METIS_LIBRARY metis)
IF(NOT METIS_LIBRARY_NOTFOUND)
	add_definitions(-DMETIS)
ENDIF()

# ==============================================================================
#                              Find external libraries and save variables to file
# ==============================================================================

MESSAGE(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
set(CacheForExecutable ${CMAKE_INSTALL_PREFIX}/cmake/CMakeCacheForExecutable.cmake)
file(WRITE ${CacheForExecutable} "")

set(Vars
ARMADILLO_FOUND
ARMADILLO_INCLUDE_DIRS
ARMADILLO_LIBRARIES
BLAS_LIBRARIES
Boost_FOUND
Boost_LIBRARIES
METIS_LIBRARY
METIS_INCLUDE_DIR
UMFPACK_LIBRARIES
HDF5_LIBRARIES
)

set(Fada_EXTERNAL_LIBRARIES
	${ARMADILLO_LIBRARY}
  ${BLAS_LIBRARIES}
  ${METIS_LIBRARY}
  ${MPI_LIBRARIES}
	${UMFPACK_LIBRARIES}
	${HDF5_LIBRARIES}
)
set(Fada_EXTERNAL_INCLUDE
  ${ARMADILLO_INCLUDE_DIRS}
)
file(APPEND ${CacheForExecutable} "set(Fada_EXTERNAL_LIBRARIES \"${Fada_EXTERNAL_LIBRARIES}\")\n")
file(APPEND ${CacheForExecutable} "set(Fada_EXTERNAL_INCLUDE \"${Fada_EXTERNAL_INCLUDE}\")\n")
