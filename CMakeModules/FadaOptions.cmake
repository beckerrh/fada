# ==============================================================================
#                              Build options
# ==============================================================================
OPTION(BUILD_SHARED_LIBS "Build dynamic libraries" ON)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
  # using clang
	add_definitions(-DCLANG -std=gnu++11 -DARMA_USE_HDF5)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # using GCC
	add_definitions(-DGNU)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  # using Intel C++
endif()

# ==============================================================================
# COMMON flags (all CMAKE_BUILD_TYPE: Standard, Debug, Release, ...)
# ==============================================================================

# -std=c++11
SET(CMAKE_CXX_FLAGS_PROFILE "-O2 -pg")
SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
   SET(CMAKE_CXX_FLAGS_RELEASE "-O5")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
   SET(CMAKE_CXX_FLAGS_RELEASE "-O5 -ffast-math")
endif()

# SET(CMAKE_C_FLAGS_PROFILE ${CMAKE_CXX_FLAGS_PROFILE})
# SET(CMAKE_C_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
# SET(CMAKE_C_FLAGS_RELWITHDEBINFO ${CMAKE_CXX_FLAGS_RELWITHDEBINFO})
# SET(CMAKE_C_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})

IF(CMAKE_BUILD_TYPE STREQUAL "Profile")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_PROFILE}")
	# SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS_PROFILE}")
ELSEIF(CMAKE_BUILD_TYPE STREQUAL "Debug")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")
	# SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS_DEBUG}")
ELSEIF(CMAKE_BUILD_TYPE STREQUAL "Release")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_RELEASE} -DARMA_NO_DEBUG")
	# SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS_RELEASE}")
#	add_definitions(-DARMA_NO_DEBUG)
ELSEIF(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
    # SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS_RELWITHDEBINFO}")
ENDIF(CMAKE_BUILD_TYPE STREQUAL "Profile")


# ==============================================================================
# Variables for binaries and projects
# ==============================================================================
