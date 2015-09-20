cmake_minimum_required(VERSION 3.1)
project(Reconstructor)

# Debug
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -Wall -g -ggdb -DEBUG_APP -DGL_GLEXT_PROTOTYPES")

# Relase
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3 -Wall -DGL_GLEXT_PROTOTYPES")

set(SOURCE_FILES src/main.cpp)
aux_source_directory(src SOURCE_FILES)

# Add OpenGL
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

# Add external include directory
IF(APPLE)  # for Mac OS X
include_directories(include
                    /opt/local/include/
                    ${OPENGL_INCLUDE_DIRS}
                    ${GLUT_INCLUDE_DIRS})
ELSEIF(UNIX) # for linux
include_directories(include
                    /usr/include/suitesparse
                   /home/sway/myLibs/Eigen3.2.5
                    ${OPENGL_INCLUDE_DIRS}
                    ${GLUT_INCLUDE_DIRS})
ENDIF()

# Add external library directories
IF(APPLE)
link_directories(/opt/local/lib/
                 /usr/lib)
ELSEIF(UNIX)
link_directories(/home/sway/myLibs/graclus1.2
                 /usr/lib)
ENDIF()

# Set libraraies
set(SUITESPARSE_LIB spqr cholmod metis colamd ccolamd camd amd umfpack)

IF(APPLE)
set(BLAS_LIB lapack cblas)
ELSEIF(UNIX)
set(BLAS_LIB lapack blas gfortran)
ENDIF()

add_executable(Reconstructor ${SOURCE_FILES})
target_link_libraries(Reconstructor ${SUITESPARSE_LIB} ${BLAS_LIB} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
