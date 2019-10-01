# Copyright (c) 2018-2019, CNRS
# Authors: Sanghyun Kim <ggory15@snu.ac.kr>

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

INCLUDE(cmake/base.cmake)
INCLUDE(cmake/boost.cmake)
INCLUDE(cmake/eigen.cmake)
INCLUDE(cmake/python.cmake)
INCLUDE(cmake/ide.cmake)
INCLUDE(cmake/apple.cmake)

SET(PROJECT_NAME timeopt)
SET(PROJECT_DESCRIPTION "COM Generator")
SET(PROJECT_URL "http://github.com/ggory15")
OPTION(INSTALL_DOCUMENTATION "Generate and install the documentation" OFF)

SET(DOXYGEN_USE_MATHJAX YES)

# Handle APPLE Cmake policy
IF(APPLE)
  APPLY_DEFAULT_APPLE_CONFIGURATION()
ENDIF(APPLE)

# Disable -Werror on Unix for now.
SET(CXX_DISABLE_WERROR True)
SET(CMAKE_VERBOSE_MAKEFILE True)

SETUP_PROJECT()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -g -Wall -Wwrite-strings -pedantic -O3 -funroll-loops -fPIC")

IF(WIN32)
  SET(LINK copy_if_different)
ELSE(WIN32)
  SET(LINK create_symlink)
ENDIF(WIN32)

# --- OPTIONS ----------------------------------------
OPTION (BUILD_PYTHON_INTERFACE "Build the python binding" ON)
OPTION (BUILD_UNIT_TESTS "Build the unitary tests" ON)
OPTION (INITIALIZE_WITH_NAN "Initialize Eigen entries with NaN" OFF)
OPTION (EIGEN_RUNTIME_NO_MALLOC "If ON, it can assert in case of runtime allocation" ON)
OPTION (EIGEN_NO_AUTOMATIC_RESIZING "If ON, it forbids automatic resizing of dynamics arrays and matrices" OFF)

IF(INITIALIZE_WITH_NAN)
  MESSAGE(STATUS "Initialize with NaN all the Eigen entries.")
  ADD_DEFINITIONS(-DEIGEN_INITIALIZE_MATRICES_BY_NAN)
ENDIF(INITIALIZE_WITH_NAN)

IF(EIGEN_RUNTIME_NO_MALLOC)
  MESSAGE(STATUS "Option EIGEN_RUNTIME_NO_MALLOC on.")
  ADD_DEFINITIONS(-DEIGEN_RUNTIME_NO_MALLOC)
ENDIF(EIGEN_RUNTIME_NO_MALLOC)

IF(EIGEN_NO_AUTOMATIC_RESIZING)
  MESSAGE(STATUS "Option EIGEN_NO_AUTOMATIC_RESIZING on.")
  ADD_DEFINITIONS(-DEIGEN_NO_AUTOMATIC_RESIZING)
ENDIF(EIGEN_NO_AUTOMATIC_RESIZING)

# ----------------------------------------------------
# --- DEPENDANCIES -----------------------------------
# ----------------------------------------------------
ADD_REQUIRED_DEPENDENCY("eigen3 >= 3.2.0") # Eigen::Ref appeared from 3.2.0
ADD_REQUIRED_DEPENDENCY("momentumopt")
ADD_REQUIRED_DEPENDENCY("yaml-cpp")


SET(BOOST_REQUIERED_COMPONENTS filesystem system)
SET(BOOST_BUILD_COMPONENTS unit_test_framework)
SET(BOOST_OPTIONAL_COMPONENTS "")

IF(BUILD_PYTHON_INTERFACE)
  SET(BOOST_OPTIONAL_COMPONENTS ${BOOST_OPTIONAL_COMPONENTS} python)
  FINDPYTHON()
  INCLUDE_DIRECTORIES(SYSTEM ${PYTHON_INCLUDE_DIRS})
ENDIF(BUILD_PYTHON_INTERFACE)

SET(BOOST_COMPONENTS ${BOOST_REQUIERED_COMPONENTS} ${BOOST_OPTIONAL_COMPONENTS} ${BOOST_BUILD_COMPONENTS})
SEARCH_FOR_BOOST()

# Path to boost headers
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
# ----------------------------------------------------
# --- INCLUDE ----------------------------------------
# ----------------------------------------------------

SET(${PROJECT_NAME}_TIMEOPT_HEADERS
    include/timeopt/fwd.hpp
    include/timeopt/phase.hpp
    include/timeopt/problem.hpp

    include/timeopt/interface/contactplanner.hpp
  )
SET(${PROJECT_NAME}_SERIALIZATION_HEADERS
  include/serialization/eigen-matrix.hpp
  include/serialization/spatial.hpp
  include/serialization/fwd.hpp
  include/serialization/xml.hpp
  include/serialization/archive.hpp
)


SET(HEADERS
    ${${PROJECT_NAME}_SERIALIZATION_HEADERS}
    ${${PROJECT_NAME}_TIMEOPT_HEADERS}
 )

LIST(REMOVE_DUPLICATES HEADERS)

SET(HEADERS_FULL_PATH "")
FOREACH(header ${HEADERS})
  LIST(APPEND HEADERS_FULL_PATH "${CMAKE_SOURCE_DIR}/${header}")
  GET_FILENAME_COMPONENT(headerName ${header} NAME)
  GET_FILENAME_COMPONENT(headerPath ${header} PATH)
  INSTALL(FILES ${${PROJECT_NAME}_SOURCE_DIR}/${header}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/${headerPath}
          PERMISSIONS OWNER_READ GROUP_READ WORLD_READ OWNER_WRITE)
ENDFOREACH(header ${HEADERS})

# --- MAIN LIBRARY -------------------------------------------------------------
ADD_SUBDIRECTORY(src)

#--- BINDINGS ----------------------------------------------------------------
IF(BUILD_PYTHON_INTERFACE)
  SET(PYWRAP ${PROJECT_NAME}_pywrap)
  ADD_SUBDIRECTORY(bindings)
ENDIF(BUILD_PYTHON_INTERFACE)
# --- UNIT TESTS ---------------------------------------------------------------
ADD_SUBDIRECTORY(unittest)

# --- PACKAGING ----------------------------------------------------------------
PKG_CONFIG_APPEND_LIBS(${PROJECT_NAME})

SETUP_PROJECT_FINALIZE()
