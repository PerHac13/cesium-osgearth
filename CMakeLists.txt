cmake_minimum_required(VERSION 3.20)
project(cesium-osgearth)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use vcpkg toolchain if VCPKG_ROOT is set
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
endif()

# Configure cesium-native build options
set(CESIUM_TESTS_ENABLED OFF CACHE BOOL "" FORCE)
set(CMAKE_POSITION_INDEPENDENT_CODE ON CACHE BOOL "" FORCE)
set(CESIUM_TRACING_ENABLED OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# Add cesium-native subdirectory
add_subdirectory(extern/cesium-native)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(OpenSceneGraph REQUIRED COMPONENTS 
    osgViewer 
    osgDB 
    osgGA 
    osgUtil
    osgText
)
find_package(osgEarth CONFIG REQUIRED)

# Create data directory in build
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data)

# Copy earth file to build directory
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/data/default.earth
    ${CMAKE_CURRENT_BINARY_DIR}/data/default.earth
    COPYONLY
)

# Add executable
add_executable(${PROJECT_NAME}
    src/main.cpp
)

# Include directories
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${OPENSCENEGRAPH_INCLUDE_DIRS}
    ${OSGEARTH_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    ${OPENSCENEGRAPH_LIBRARIES}
    osgEarth::osgEarth
    Cesium3DTiles
    CesiumAsync
    CesiumGeometry
    CesiumGeospatial
    CesiumGltf
    CesiumJsonReader
    CesiumUtility
    OpenGL::GL
)

# Install targets
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/data/default.earth
    DESTINATION share/${PROJECT_NAME}/data
)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)