cmake_minimum_required(VERSION 3.15)

project(Lander LANGUAGES CXX)

# The CodeBlocks project builds as GNU C++14 (gnu++1y).
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Match the CodeBlocks default target.
get_property(_is_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(NOT _is_multi_config AND NOT CMAKE_BUILD_TYPE)
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
elseif(APPLE)
  set(LANDER_LIB_DIR "${CMAKE_SOURCE_DIR}/lib/mac${LANDER_ARCH_BITS}")
endif()

# Source files without a target restriction in lander.cbp.
set(LANDER_SOURCES
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

add_executable(Lander ${LANDER_SOURCES})

if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

if(UNIX AND NOT APPLE)
  # Linux uses distribution-provided shared libraries and their usage metadata.
  set(Boost_USE_STATIC_LIBS OFF)
  set(GLEW_USE_STATIC_LIBS OFF)
  find_package(Boost REQUIRED CONFIG COMPONENTS chrono)
  find_package(Freetype REQUIRED)
  find_package(GLEW REQUIRED)
  find_package(OpenGL REQUIRED)
  find_package(PkgConfig REQUIRED)
  find_package(Threads REQUIRED)
  pkg_check_modules(FTGL REQUIRED IMPORTED_TARGET ftgl)
  pkg_check_modules(GLFW REQUIRED IMPORTED_TARGET glfw3)
else()
  # Windows and macOS retain the original bundled-library setup.  Boost headers
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
if(WIN32 OR APPLE)
  target_include_directories(Lander SYSTEM PRIVATE
    "${CMAKE_SOURCE_DIR}/include"
    "${Boost_INCLUDE_DIRS}"
  )
endif()

target_compile_definitions(Lander PRIVATE
  BOOST_THREAD_VERSION=3
  BOOST_THREAD_USE_LIB
  BOOST_SYSTEM_NO_DEPRECATED
  $<$<CONFIG:Release>:NDEBUG>
  $<$<CONFIG:Release>:BOOST_DISABLE_ASSERTS>
)

if(WIN32 OR APPLE)
  target_compile_definitions(Lander PRIVATE
    GLEW_STATIC
    CURL_STATICLIB
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
  -m3dnow
)

if(WIN32)
  target_compile_options(Lander PRIVATE -mthreads)
elseif(UNIX AND NOT APPLE)
  target_compile_options(Lander PRIVATE -fno-pie)
endif()

target_compile_options(Lander PRIVATE
  $<$<CONFIG:Debug>:-Og>
  $<$<CONFIG:Debug>:-g>
  $<$<CONFIG:Debug>:-fexceptions>
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
  -fprofile-generate
  -fbranch-probabilities
  -fprofile-arcs
  -fprofile-use
  -fprofile-values
  -fvpt
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
  --param=max-gcse-memory=1000000000
  --param=max-inline-insns-single=10000
  --param=max-inline-insns-auto=10000
  --param=inline-min-speedup=20
  --param=large-function-insns=10000
  --param=large-function-growth=2000
  --param=large-unit-insns=20000
  --param=inline-unit-growth=200
)
foreach(_option IN LISTS LANDER_RELEASE_COMPILE_OPTIONS)
  target_compile_options(Lander PRIVATE "$<$<CONFIG:Release>:${_option}>")
endforeach()

# Link options common to the platform targets.  Supplying the architecture to
# the compiler driver here is necessary for CMake's separate link step.
target_link_options(Lander PRIVATE ${LANDER_ARCH_FLAG})

if(WIN32)
  target_link_options(Lander PRIVATE
    -static
    -static-libgcc
    -static-libstdc++
  )
elseif(APPLE)
  target_link_options(Lander PRIVATE
    -static
    -static-libgcc
    -static-libstdc++
  )
else()
  target_link_options(Lander PRIVATE -no-pie)
endif()

target_link_options(Lander PRIVATE
  $<$<CONFIG:Release>:-s>
  $<$<CONFIG:Release>:-fprofile-arcs>
  $<$<CONFIG:Release>:-fuse-linker-plugin>
  $<$<CONFIG:Release>:-flto>
  $<$<CONFIG:Release>:-funsafe-math-optimizations>
)

# Windows retains the versioned, bundled archives from lander.cbp.
if(WIN32 AND BUILD_32BIT)
  target_link_libraries(Lander PRIVATE
    $<IF:$<CONFIG:Debug>,${LANDER_LIB_DIR}/libboost_chrono-mgw48-mt-sd-1_54.a,${LANDER_LIB_DIR}/libboost_chrono-mgw48-mt-s-1_54.a>
    $<IF:$<CONFIG:Debug>,${LANDER_LIB_DIR}/libboost_system-mgw48-mt-sd-1_54.a,${LANDER_LIB_DIR}/libboost_system-mgw48-mt-s-1_54.a>
    $<IF:$<CONFIG:Debug>,${LANDER_LIB_DIR}/libboost_filesystem-mgw48-mt-sd-1_54.a,${LANDER_LIB_DIR}/libboost_filesystem-mgw48-mt-s-1_54.a>
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
elseif(UNIX AND NOT APPLE)
  target_link_libraries(Lander PRIVATE
    Boost::chrono
    Freetype::Freetype
    GLEW::GLEW
    OpenGL::GL
    OpenGL::GLU
    PkgConfig::FTGL
    PkgConfig::GLFW
    Threads::Threads
    ${CMAKE_DL_LIBS}
  )
endif()

# -lgcov is a release linker option in every CodeBlocks target.
target_link_libraries(Lander PRIVATE "$<$<CONFIG:Release>:gcov>")

# Every CodeBlocks Release target runs strip after linking.
if(CMAKE_STRIP)
  file(GENERATE
    OUTPUT "${CMAKE_BINARY_DIR}/strip_if_release_$<CONFIG>.cmake"
    CONTENT
      "$<$<CONFIG:Release>:execute_process(COMMAND \"${CMAKE_STRIP}\" -spv \"$<TARGET_FILE:Lander>\")>\n"
  )
  add_custom_command(TARGET Lander POST_BUILD
    COMMAND ${CMAKE_COMMAND} -P
            "${CMAKE_BINARY_DIR}/strip_if_release_$<CONFIG>.cmake"
    COMMENT "Stripping release binary"
    VERBATIM
  )
endif()
