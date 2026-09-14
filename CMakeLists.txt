cmake_minimum_required(VERSION 3.20)

project(Lander LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Match the CodeBlocks default target.
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type: Debug or Release" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release)
endif()

# A CodeBlocks target exists for each architecture.  CMake represents that
# choice as a configure-time option instead of creating duplicate executables.
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
  set(_build_32bit_default ON)
else()
  set(_build_32bit_default OFF)
endif()
option(BUILD_32BIT "Build the 32-bit target (-m32)" ${_build_32bit_default})

if(BUILD_32BIT)
  set(LANDER_ARCH_FLAG -m32)
  set(LANDER_ARCH_BITS 32)
else()
  set(LANDER_ARCH_FLAG -m64)
  set(LANDER_ARCH_BITS 64)
endif()

if(WIN32)
  set(LANDER_LIB_DIR "${CMAKE_SOURCE_DIR}/lib/win${LANDER_ARCH_BITS}")
endif()

# Source files without a target restriction in lander.cbp.
add_executable(Lander
  astronaut.cpp
  astronomical_entities/blackhole.cpp
  astronomical_entities/dustcloud.cpp
  astronomical_entities/geo/chunk.cpp
  astronomical_entities/geo/corner.cpp
  astronomical_entities/geo/region.cpp
  astronomical_entities/moon.cpp
  astronomical_entities/neutronstar.cpp
  astronomical_entities/orbitalbody.cpp
  astronomical_entities/planet.cpp
  astronomical_entities/pulsar.cpp
  astronomical_entities/star.cpp
  astronomical_entities/starsystem.cpp
  astronomical_entities/universe.cpp
  astronomical_entities/whitedwarf.cpp
  body.cpp

  devices/attitude_cmg.cpp
  devices/computers/computer_mission.cpp
  devices/device.cpp
  devices/display_converter_analogue_digital.cpp
  devices/display_converter_digital_analogue.cpp
  devices/external/camera.cpp
  devices/external/device_external.cpp
  devices/external/sensor_pressure.cpp
  devices/instruments/altimeter.cpp
  devices/instruments/button_momentary.cpp
  devices/instruments/control.cpp
  devices/instruments/display.cpp
  devices/instruments/display_digital.cpp
  devices/instruments/display_number.cpp
  devices/instruments/display_small.cpp
  devices/instruments/instrument.cpp
  devices/instruments/led_green.cpp
  devices/instruments/switch_onoff.cpp
  devices/instruments/telltale.cpp
  devices/mapper_system.cpp
  devices/memory.cpp
  devices/operators/operator_add.cpp
  devices/operators/operator_and.cpp
  devices/operators/operator_base.cpp
  devices/operators/operator_div.cpp
  devices/operators/operator_mul.cpp
  devices/operators/operator_not.cpp
  devices/operators/operator_or.cpp
  devices/operators/operator_sub.cpp
  devices/terminal.cpp
  devices/thrusters/thruster.cpp

  font_load.cpp
  init_graphics.cpp
  init_universe.cpp
  input/callback_key.cpp
  input/callback_mouse.cpp
  input/callback_mousebutton.cpp
  input/callback_scroll.cpp
  input/callback_textinput.cpp
  input/callback_windowclose.cpp
  input/callback_windowresize.cpp
  input/controls.cpp
  instrumentpanel.cpp
  main.cpp
  mathematical/integrator.cpp
  mathematical/integrator_euler.cpp
  mathematical/integrator_rungekutta4.cpp
  mathematical/integrator_rungekuttafehlberg54.cpp
  menu.cpp
  physicsbody.cpp
  random_engine.cpp
  spacecraft.cpp
  vectorstorm/vectorstorm.cpp
)

if(UNIX)
  set(Boost_USE_STATIC_LIBS ON)
  find_package(Boost REQUIRED CONFIG COMPONENTS chrono)
  find_package(Freetype REQUIRED)
  find_package(OpenGL REQUIRED)
  find_package(PkgConfig REQUIRED)
  find_package(glfw3 REQUIRED CONFIG)
  pkg_check_modules(FTGL REQUIRED IMPORTED_TARGET ftgl)
  if(APPLE)
    find_package(GLEW REQUIRED)
    set(LANDER_GLEW_LIBRARY GLEW::GLEW)
  else()
    find_path(LANDER_GLEW_INCLUDE_DIR NAMES GL/glew.h REQUIRED)
    find_library(LANDER_GLEW_STATIC_LIBRARY NAMES libGLEW.a REQUIRED)
    set(LANDER_GLEW_LIBRARY "${LANDER_GLEW_STATIC_LIBRARY}")
  endif()
else()
  # Windows retains the original bundled-library setup.  Boost headers
  # themselves are supplied externally, as they were in CodeBlocks.
  find_package(Boost REQUIRED CONFIG)
endif()

target_include_directories(Lander PRIVATE
  "${CMAKE_SOURCE_DIR}"
  "${CMAKE_SOURCE_DIR}/astronomical_entities"
  "${CMAKE_SOURCE_DIR}/astronomical_entities/geo"
  "${CMAKE_SOURCE_DIR}/mathematical"
  "${CMAKE_SOURCE_DIR}/devices"
  "${CMAKE_SOURCE_DIR}/devices/instruments"
  "${CMAKE_SOURCE_DIR}/devices/external"
  "${CMAKE_SOURCE_DIR}/devices/thrusters"
  "${CMAKE_SOURCE_DIR}/devices/operators"
  "${CMAKE_SOURCE_DIR}/devices/computers"
)
if(WIN32)
  target_include_directories(Lander SYSTEM PRIVATE
    "${CMAKE_SOURCE_DIR}/include"
    "${Boost_INCLUDE_DIRS}"
  )
elseif(NOT APPLE)
  target_include_directories(Lander SYSTEM PRIVATE
    "${LANDER_GLEW_INCLUDE_DIR}"
  )
endif()

target_compile_definitions(Lander PRIVATE
  $<$<CONFIG:Release>:BOOST_DISABLE_ASSERTS>
)

if(NOT APPLE)
  target_compile_definitions(Lander PRIVATE GLEW_STATIC)
endif()

if(WIN32)
  # Compile Chrono with the current Boost headers and toolchain instead of
  # mixing them with the bundled Boost 1.54 archives built by GCC 4.8.
  target_compile_definitions(Lander PRIVATE
    BOOST_CHRONO_HEADER_ONLY
    BOOST_ERROR_CODE_HEADER_ONLY
  )
endif()

if(WIN32 AND BUILD_32BIT)
  target_compile_definitions(Lander PRIVATE _WIN32_WINNT=0x0501)
endif()

# Options shared by all CodeBlocks targets.
target_compile_options(Lander PRIVATE
  ${LANDER_ARCH_FLAG}
  -Wall
  -Wextra
  -Wno-unused-local-typedefs
  -mmmx
  -msse
  -msse2
  $<$<CXX_COMPILER_ID:GNU>:-m3dnow>
)

if(WIN32)
  target_compile_options(Lander PRIVATE -mthreads)
elseif(UNIX AND NOT APPLE)
  target_compile_options(Lander PRIVATE -fno-pie)
endif()

target_compile_options(Lander PRIVATE
  $<$<CONFIG:Debug>:-Og>
  $<$<CONFIG:Release>:-Wno-array-bounds>
)
if(WIN32 AND BUILD_32BIT)
  target_compile_options(Lander PRIVATE
    $<$<CONFIG:Debug>:-Warray-bounds>
    $<$<CONFIG:Release>:-fomit-frame-pointer>
  )
endif()

# Release optimisation options are copied from the CodeBlocks release targets.
set(LANDER_RELEASE_COMPILE_OPTIONS
  -Ofast
  -fexpensive-optimizations
  -maccumulate-outgoing-args
  -fgcse-sm
  -fgcse-las
  -fgcse-after-reload
  -fsched-spec-load
  -fsched-spec-load-dangerous
  -fmerge-all-constants
  -fmodulo-sched
  -fmodulo-sched-allow-regmoves
  -funsafe-loop-optimizations
  -Wunsafe-loop-optimizations
  -fsched-pressure
  -fsched-stalled-insns=0
  -fsched-stalled-insns-dep=32
  -fsched2-use-superblocks
  -fipa-pta
  -ftracer
  -fno-var-tracking-assignments
  -fpeel-loops
  -funroll-loops
  -funswitch-loops
  -ftree-loop-if-convert
  -ftree-loop-distribution
  -ftree-loop-im
  -ftree-loop-ivcanon
  -fivopts
  -fvariable-expansion-in-unroller
  -freorder-blocks-and-partition
  -fbranch-target-load-optimize
  -fuse-linker-plugin
  -flto
  -fno-math-errno
  -funsafe-math-optimizations
  -fassociative-math
  -freciprocal-math
  -fno-signed-zeros
  -fno-trapping-math
)
target_compile_options(Lander PRIVATE
  "$<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:GNU>>:${LANDER_RELEASE_COMPILE_OPTIONS}>"
)

# Link options common to the platform targets.  Supplying the architecture to
# the compiler driver here is necessary for CMake's separate link step.
target_link_options(Lander PRIVATE ${LANDER_ARCH_FLAG})

if(WIN32)
  target_link_options(Lander PRIVATE
    -static
    -static-libgcc
    -static-libstdc++
  )
elseif(NOT APPLE)
  target_link_options(Lander PRIVATE
    -no-pie
    -static-libgcc
    -static-libstdc++
  )
endif()

# macOS uses AppleClang and the system's dynamic C++ runtime.  In particular,
# -static asks the Darwin linker for crt0.o, which the SDK does not provide.
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_link_options(Lander PRIVATE
    $<$<CONFIG:Release>:-s>
    $<$<CONFIG:Release>:-fuse-linker-plugin>
    $<$<CONFIG:Release>:-flto>
    $<$<CONFIG:Release>:-funsafe-math-optimizations>
  )
endif()

# Windows retains the non-Boost bundled archives from lander.cbp.
if(WIN32 AND BUILD_32BIT)
  target_link_libraries(Lander PRIVATE
    ${LANDER_LIB_DIR}/libglew32.a
    ${LANDER_LIB_DIR}/libglfw3.a
    ${LANDER_LIB_DIR}/libftgl_static_D.a
    ${LANDER_LIB_DIR}/libglu32.a
    ${LANDER_LIB_DIR}/libopengl32.a
    ${LANDER_LIB_DIR}/libfreetype.a
    ${LANDER_LIB_DIR}/libws2_32.a
    ${LANDER_LIB_DIR}/libgdi32.a
    ${LANDER_LIB_DIR}/libwinmm.a
    ${LANDER_LIB_DIR}/libsetupapi.a
    ${LANDER_LIB_DIR}/libz.a
  )
elseif(UNIX)
  target_link_libraries(Lander PRIVATE
    Boost::chrono
    Freetype::Freetype
    "${LANDER_GLEW_LIBRARY}"
    OpenGL::GL
    OpenGL::GLU
    PkgConfig::FTGL
    glfw
  )
endif()
