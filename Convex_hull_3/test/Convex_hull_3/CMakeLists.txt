# Created by the script cgal_create_cmake_script
# This is the CMake script for compiling a CGAL application.


cmake_minimum_required(VERSION 3.1...3.15)
project( Convex_hull_3_Tests )


find_package(CGAL QUIET)

if ( CGAL_FOUND )

  find_package( OpenMesh QUIET )

  if ( OpenMesh_FOUND )
    include( UseOpenMesh )
  else()
    message(STATUS "Examples that use OpenMesh will not be compiled.")
  endif()

  include_directories (BEFORE "include")

  # create a target per cppfile
  file(GLOB cppfiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
  foreach(cppfile ${cppfiles})
    create_single_source_cgal_program( "${cppfile}" )
  endforeach()

else()

    message(STATUS "This program requires the CGAL library, and will not be compiled.")

endif()

