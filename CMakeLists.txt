cmake_minimum_required(VERSION 3.0.0)
project(ovdb_converter)
file(GLOB SOURCE ${PROJECT_SOURCE_DIR}/src/*.h ${PROJECT_SOURCE_DIR}/src/*.cpp )
#list(APPEND SAMPLE_INCLUDE_DIR /usr/local/include)
#find_package(OpenVDB REQUIRED)
# tbb
find_library(TBB_LIBRARY tbb HINTS /usr/local/lib)

# openexr
find_library(HALF_LIB half HINTS /usr/local/lib)


list(APPEND SOURCE /usr/local/include)
find_library(OpenVDB_LIBRARY openvdb HINTS /usr/local/lib)

# add_subdirectory(${PROJECT_SOURCE_DIR}/nanovdb)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
include_directories(
    ${BOOST_INCLUDE_DIRS}
    ${OPENVDB_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/src
    )

add_executable( ${PROJECT_NAME} ${SOURCE} )
target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    ${OpenVDB_LIBRARY}
    ${TBB_LIBRARY}
)