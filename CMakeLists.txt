cmake_minimum_required(VERSION 3.5)
project(glsl2spirv)

add_definitions(-DAMD_EXTENSIONS)
add_definitions(-DNV_EXTENSIONS)

if(WIN32)
    set(CMAKE_DEBUG_POSTFIX "d")
    include(ChooseMSVCCRT.cmake)
    add_definitions(-DGLSLANG_OSINCLUDE_WIN32)
elseif(UNIX)
    add_definitions(-fPIC)
    add_definitions(-DGLSLANG_OSINCLUDE_UNIX)
else(WIN32)
    message("unknown platform")
endif(WIN32)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-Wall -Wmaybe-uninitialized -Wuninitialized -Wunused -Wunused-local-typedefs
      -Wunused-parameter -Wunused-value  -Wunused-variable -Wunused-but-set-parameter -Wunused-but-set-variable)
    add_definitions(-Wno-reorder)  # disable this from -Wall, since it happens all over.
elseif(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    add_definitions(-Wall -Wuninitialized -Wunused -Wunused-local-typedefs
      -Wunused-parameter -Wunused-value  -Wunused-variable)
    add_definitions(-Wno-reorder)  # disable this from -Wall, since it happens all over.
endif()

set(CMAKE_CXX_STANDARD 11)
set(SOURCES
  glsl2spirv.cpp
  ResourceLimits.cpp

  SPIRV/doc.cpp
  SPIRV/GlslangToSpv.cpp
  SPIRV/InReadableOrder.cpp
  SPIRV/Logger.cpp
  SPIRV/SpvBuilder.cpp

  glslang/MachineIndependent/ShaderLang.cpp
  glslang/MachineIndependent/PoolAlloc.cpp
  glslang/MachineIndependent/Intermediate.cpp
  glslang/MachineIndependent/ParseHelper.cpp
  glslang/MachineIndependent/linkValidate.cpp
  glslang/MachineIndependent/InfoSink.cpp
  glslang/MachineIndependent/Constant.cpp
  glslang/MachineIndependent/glslang_tab.cpp
  glslang/MachineIndependent/Initialize.cpp
  glslang/MachineIndependent/intermOut.cpp
  glslang/MachineIndependent/IntermTraverse.cpp
  glslang/MachineIndependent/iomapper.cpp
  glslang/MachineIndependent/limits.cpp
  glslang/MachineIndependent/linkValidate.cpp
  glslang/MachineIndependent/parseConst.cpp
  glslang/MachineIndependent/ParseContextBase.cpp
  glslang/MachineIndependent/propagateNoContraction.cpp
  glslang/MachineIndependent/reflection.cpp
  glslang/MachineIndependent/RemoveTree.cpp
  glslang/MachineIndependent/Scan.cpp
  glslang/MachineIndependent/SymbolTable.cpp
  glslang/MachineIndependent/Versions.cpp
  glslang/MachineIndependent/preprocessor/Pp.cpp
  glslang/MachineIndependent/preprocessor/PpAtom.cpp
  glslang/MachineIndependent/preprocessor/PpContext.cpp
  glslang/MachineIndependent/preprocessor/PpMemory.cpp
  glslang/MachineIndependent/preprocessor/PpScanner.cpp
  glslang/MachineIndependent/preprocessor/PpSymbols.cpp
  glslang/MachineIndependent/preprocessor/PpTokens.cpp
  glslang/GenericCodeGen/CodeGen.cpp
  glslang/GenericCodeGen/Link.cpp

  OGLCompilersDLL/InitializeDll.cpp)

if(WIN32)
  set(SOURCES ${SOURCES}
    glslang/OSDependent/Windows/ossource.cpp
    glslang/OSDependent/Windows/main.cpp)

  # MinGW GCC complains about function pointer casts to void*.
  # Turn that off with -fpermissive.
  if(${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
    target_compile_options(OSDependent PRIVATE -fpermissive)
  endif()
elseif(UNIX)
  set(SOURCES ${SOURCES} glslang/OSDependent/Unix/ossource.cpp)
else(WIN32)
  message("unknown platform")
endif(WIN32)

include_directories("${CMAKE_SOURCE_DIR}/OGLCompilersDLL")

add_library(glsl2spirv STATIC ${SOURCES})

set_target_properties(glsl2spirv PROPERTIES PUBLIC_HEADER "glsl2spirv.hpp")

install(TARGETS glsl2spirv
  ARCHIVE DESTINATION lib
  PUBLIC_HEADER DESTINATION include)
