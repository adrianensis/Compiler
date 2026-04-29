# ----------------------------------------------------------------------------
# COMPILER
# ----------------------------------------------------------------------------
cmake_minimum_required(VERSION 4.2 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ----------------------------------------------------------------------------
# FUNCTIONS
# ----------------------------------------------------------------------------

function(build_programs path prefix)
  
  message("BUILD PROGRAMS (${prefix}): " ${path})

  file(GLOB PROGRAM_PATHS ${path})

  foreach(PROGRAM_PATH ${PROGRAM_PATHS})

    file(GLOB_RECURSE PROGRAM_SOURCES "${PROGRAM_PATH}/*.cpp" "${PROGRAM_PATH}/*.hpp")

    include_directories("${PROGRAM_PATH}")

    get_filename_component(PROGRAM_NAME ${PROGRAM_PATH} NAME_WE) # NAME_WE = name with extension

    set(PROGRAM_FINAL_NAME "${prefix}_${PROGRAM_NAME}" )
    add_executable( ${PROGRAM_FINAL_NAME} ${PROGRAM_SOURCES})
    target_link_libraries( ${PROGRAM_FINAL_NAME} ${COMPILER})
    install(TARGETS ${PROGRAM_FINAL_NAME} DESTINATION ${EXECUTABLE_OUTPUT_PATH})

  endforeach()
endfunction()

# project name
set(COMPILER compiler)
project(${COMPILER})

# ----------------------------------------------------------------------------
# CPP TARGETS
# ----------------------------------------------------------------------------
# We need c++14 because it defines the auto type.

# set(CMAKE_CXX_FLAGS_ALL "-std=c++17 -Wall") # this macro was invented by me

if(ENABLE_LOGS)
  set(CMAKE_CXX_FLAGS_ALL "${CMAKE_CXX_FLAGS_ALL} -Wall")
endif(ENABLE_LOGS)

if(UNIX)
  set(CMAKE_CXX_FLAGS_ALL "${CMAKE_CXX_FLAGS_ALL} -Werror=return-type")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CMAKE_CXX_FLAGS_ALL}")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_CXX_FLAGS_ALL} -O3 -mavx")
endif(UNIX)

# ----------------------------------------------------------------------------
# OUTPUT DIRECTORIES
# ----------------------------------------------------------------------------

set(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/compiledLibs")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/binaries")

# ----------------------------------------------------------------------------

message("Compiler: ${CMAKE_CXX_COMPILER}")
message("Release: ${CMAKE_CXX_FLAGS_RELEASE}")
message("Debug: ${CMAKE_CXX_FLAGS_DEBUG}")

message("Selected Build target: ${CMAKE_BUILD_TYPE}")

# ----------------------------------------------------------------------------
# ENGINE PACKAGES
# ----------------------------------------------------------------------------

file(GLOB PACKAGES_PATHS "code/*")
set(PACKAGES "")

foreach(PACKAGE_PATH ${PACKAGES_PATHS})
  get_filename_component(PACKAGE_NAME ${PACKAGE_PATH} NAME_WE) # NAME_WE = name with extension
  list(APPEND PACKAGES ${PACKAGE_NAME})
endforeach()

# ----------------------------------------------------------------------------
# PACKAGES INCLUDE DIRECTORIES
# ----------------------------------------------------------------------------

include_directories("code")

# ----------------------------------------------------------------------------
# LIBRARY OBJECTS
# ----------------------------------------------------------------------------

# object libraries
# Alternative: CMakeLists.txt with this code into source directory.
# add_subdirectory(source)
foreach(PACKAGE ${PACKAGES})
  file(GLOB_RECURSE PACKAGE_SOURCE "code/${PACKAGE}/*")
  add_library(${PACKAGE} OBJECT ${PACKAGE_SOURCE})
  # Tell CMake to use the C++ compiler/linker rules, for header-only packages
  set_target_properties(${PACKAGE} PROPERTIES LINKER_LANGUAGE CXX)
endforeach()

# list of objects
foreach(PACKAGE ${PACKAGES})
  list(APPEND OBJS $<TARGET_OBJECTS:${PACKAGE}>)
endforeach()

# list(APPEND OBJS $<TARGET_OBJECTS:Graphics_GEN>)

# ----------------------------------------------------------------------------
# COMPILER LIBRARY
# ----------------------------------------------------------------------------

# single library
add_library(${COMPILER} ${OBJS})

# ----------------------------------------------------------------------------
# LINK LIBRARIES
# ----------------------------------------------------------------------------

# link libraries with compiler library
# target_link_libraries(${COMPILER} ${OPENGL_gl_LIBRARY})
# target_link_libraries(${COMPILER} ${OPENGL_glu_LIBRARY})
# target_link_libraries(${COMPILER} glfw)
# target_link_libraries(${COMPILER} nlohmann_json)
# target_link_libraries(${COMPILER} libpng.so)

# SOIL
#target_link_libraries( ${COMPILER} SOIL )

# ----------------------------------------------------------------------------
# TESTS
# ----------------------------------------------------------------------------

# if(BUILD_UNIT_TESTS)
#   build_programs("test/unit/*" "test_unit")
# endif(BUILD_UNIT_TESTS)

if(BUILD_INTEGRATION_TESTS)
  build_programs("test/integration/*" "test_integration")
endif(BUILD_INTEGRATION_TESTS)

# ----------------------------------------------------------------------------
# TOOLS
# ----------------------------------------------------------------------------

if(BUILD_TOOLS)
  build_programs("tools/*" "tool")
endif(BUILD_TOOLS)

# ----------------------------------------------------------------------------
# GAMES
# ----------------------------------------------------------------------------

# build_programs("games/*" "games")