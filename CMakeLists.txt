cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 11)
set(SOURCES StandAlone.cpp ResourceLimits.cpp)

link_directories("/home/calx/dev/glslang/build/install/lib")

add_executable(glslangValidator ${SOURCES})
set_property(TARGET glslangValidator PROPERTY FOLDER tools)

set(LIBRARIES glslang OGLCompiler OSDependent SPIRV)
set(LIBRARIES ${LIBRARIES} HLSL)

if(WIN32)
  set(LIBRARIES ${LIBRARIES} psapi)
elseif(UNIX)
  if(NOT ANDROID)
    set(LIBRARIES ${LIBRARIES} pthread)
  endif()
endif(WIN32)

target_include_directories(glslangValidator PUBLIC "/home/calx/dev/glslang/")
target_link_libraries(glslangValidator ${LIBRARIES})

install(TARGETS glslangValidator RUNTIME DESTINATION bin)
