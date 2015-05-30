cmake_minimum_required(VERSION 3.2)
project(Reconstructor)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DGL_GLEXT_PROTOTYPES")

set(SOURCE_FILES src/main.cpp)
aux_source_directory(src SOURCE_FILES)

# Add OpenGL
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

# Add external include directory
include_directories(include                    /usr/include/suitesparse
                    ${OPENGL_INCLUDE_DIRS}
                    ${GLUT_INCLUDE_DIRS})


# Add external library directories
link_directories(/home/sway/Work/SFM/cmvs/lib/graclus1.2/
                 /usr/lib/lapack/)


set(SUITESPARSE_LIB spqr cholmod metis colamd ccolamd camd amd m umfpack)
set(BLAS_LIB lapack blas gfortran)

add_executable(Reconstructor ${SOURCE_FILES})
target_link_libraries(Reconstructor ${SUITESPARSE_LIB} ${BLAS_LIB} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
