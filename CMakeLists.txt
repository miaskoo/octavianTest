cmake_minimum_required(VERSION 3.22.1)

project(LestaTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# includes
include_directories("./" "include" "src" "src/actions" "src/components" "src/entity" "src/entity/2d" "src/entity/3d" "src/factory" "src/system")
link_directories("./")

# test
file(GLOB_RECURSE MAIN "src/main.cpp" "src/struct.h" "src/constructorWindow.h" "src/constructorWindow.cpp")
file(GLOB_RECURSE ACTION src/actions/*.h src/actions/*.cpp)
file(GLOB_RECURSE COMPONENTS src/components/*.h src/components/*.cpp)
file(GLOB_RECURSE ENTITY src/entity/*.h src/entity/*.cpp)
file(GLOB_RECURSE FACTORY src/factory/*.h src/factory/*.cpp)
file(GLOB_RECURSE SYSTEM src/system/*.h src/system/*.cpp)
# SOIL
file(GLOB_RECURSE HEADERS_SOIL include/SOIL/*.h)

#add filters
source_group("test" FILES ${MAIN})
source_group("test/actions" FILES ${ACTION})
source_group("test/components" FILES ${COMPONENTS})
source_group("test/entity" FILES ${ENTITY})
source_group("test/factory" FILES ${FACTORY})
source_group("test/system" FILES ${SYSTEM})

source_group("SOIL" FILES ${HEADERS_SOIL})

#find frameworks
find_library(COCOA_LIBRARY Cocoa)
find_library(GLUT_LIBRARY GLUT)
find_library(OPENGL_LIBARY OpenGL)

add_executable(${PROJECT_NAME} ${ACTION} ${MAIN} ${COMPONENTS} ${ENTITY} ${FACTORY} ${SYSTEM} ${HEADERS_SOIL})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBARY} ${COCOA_LIBRARY} ${GLUT_LIBRARY} libsoil.a)

add_definitions(-DGL_SILENCE_DEPRECATION -DMAC)
add_compile_definitions($<$<CONFIG:DEBUG>:DEBUG> $<$<CONFIG:RELEASE>:NDEBUG>)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
