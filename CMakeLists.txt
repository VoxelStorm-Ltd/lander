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
  set(LANDER_PLATFORM Win)
elseif(APPLE)
  set(LANDER_PLATFORM Mac)
else()
  set(LANDER_PLATFORM Linux)
endif()

string(TOLOWER "${LANDER_PLATFORM}${LANDER_ARCH_BITS}" LANDER_PLATFORM_DIR)
set(LANDER_LIB_DIR "${CMAKE_SOURCE_DIR}/lib/${LANDER_PLATFORM_DIR}")

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

# Boost headers are supplied externally in the CodeBlocks setup.  No imported
# Boost libraries are used here because the project links its bundled archives.
if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()
find_package(Boost REQUIRED)

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
target_include_directories(Lander SYSTEM PRIVATE
  "${CMAKE_SOURCE_DIR}/include"
  "${Boost_INCLUDE_DIRS}"
)

target_compile_definitions(Lander PRIVATE
  BOOST_THREAD_VERSION=3
  BOOST_THREAD_USE_LIB
  BOOST_SYSTEM_NO_DEPRECATED
  GLEW_STATIC
  CURL_STATICLIB
  $<$<CONFIG:Release>:NDEBUG>
  $<$<CONFIG:Release>:BOOST_DISABLE_ASSERTS>
)

if(WIN32 AND BUILD_32BIT)
  target_compile_definitions(Lander PRIVATE _WIN32_WINNT=0x0501)
elseif(UNIX AND NOT APPLE)
  target_compile_definitions(Lander PRIVATE _GLIBCXX_USE_CXX11_ABI=0)
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
  target_link_options(Lander PRIVATE
    -static-libgcc
    -static-libstdc++
    -no-pie
  )
  # Linux32 is static in both configurations; Linux64 is static in Release.
  if(BUILD_32BIT)
    target_link_options(Lander PRIVATE -static)
  else()
    target_link_options(Lander PRIVATE "$<$<CONFIG:Release>:-static>")
  endif()
endif()

target_link_options(Lander PRIVATE
  $<$<CONFIG:Release>:-s>
  $<$<CONFIG:Release>:-fprofile-arcs>
  $<$<CONFIG:Release>:-fuse-linker-plugin>
  $<$<CONFIG:Release>:-flto>
  $<$<CONFIG:Release>:-funsafe-math-optimizations>
)

# Libraries are deliberately spelled exactly as in lander.cbp, including the
# versioned Boost archive names and the differing Debug/Release link sets.
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
elseif(UNIX AND NOT APPLE AND BUILD_32BIT)
  target_link_libraries(Lander PRIVATE
    $<IF:$<CONFIG:Debug>,${LANDER_LIB_DIR}/libboost_system-gcc48-mt-sd-1_53.a,${LANDER_LIB_DIR}/libboost_system-gcc48-mt-s-1_53.a>
    $<IF:$<CONFIG:Debug>,${LANDER_LIB_DIR}/libboost_chrono-gcc48-mt-sd-1_53.a,${LANDER_LIB_DIR}/libboost_chrono-gcc48-mt-s-1_53.a>
    ${LANDER_LIB_DIR}/libGLEW.a
    ${LANDER_LIB_DIR}/libglfw3.a
    Xrandr
    ${LANDER_LIB_DIR}/libftgl.a
    ${LANDER_LIB_DIR}/libGLU.a
    GL
    ${LANDER_LIB_DIR}/libfreetype.a
    Xt
    Xm
    pthread
    udev
    Xinerama
  )
elseif(UNIX AND NOT APPLE)
  set(LANDER_LINUX64_DEBUG_LIBRARIES
    ${LANDER_LIB_DIR}/libboost_chrono-gcc48-mt-sd-1_54.a
    ${LANDER_LIB_DIR}/libboost_system-gcc48-mt-sd-1_54.a
    ${LANDER_LIB_DIR}/libboost_filesystem-gcc48-mt-sd-1_54.a
    ${LANDER_LIB_DIR}/libGLEW.a
    ${LANDER_LIB_DIR}/libglfw3.a
    Xrandr
    ${LANDER_LIB_DIR}/libftgl.a
    ${LANDER_LIB_DIR}/libGLU.a
    GL
    ${LANDER_LIB_DIR}/libfreetype.a
    Xt
    Xi
    X11
    Xxf86vm
    pthread
    udev
    Xinerama
    ${LANDER_LIB_DIR}/libz.a
    dl
    png
    Xcursor
  )
  set(LANDER_LINUX64_RELEASE_LIBRARIES
    ${LANDER_LIB_DIR}/libboost_system-gcc48-mt-s-1_53.a
    ${LANDER_LIB_DIR}/libboost_chrono-gcc48-mt-s-1_53.a
    ${LANDER_LIB_DIR}/libboost_filesystem-gcc48-mt-s-1_54.a
    ${LANDER_LIB_DIR}/libGLEW.a
    ${LANDER_LIB_DIR}/libglfw3.a
    Xrandr
    ${LANDER_LIB_DIR}/libftgl.a
    ${LANDER_LIB_DIR}/libGLU.a
    GL
    ${LANDER_LIB_DIR}/libfreetype.a
    Xt
    Xm
    pthread
    udev
    Xinerama
    ${LANDER_LIB_DIR}/libz.a
  )
  foreach(_library IN LISTS LANDER_LINUX64_DEBUG_LIBRARIES)
    target_link_libraries(Lander PRIVATE "$<$<CONFIG:Debug>:${_library}>")
  endforeach()
  foreach(_library IN LISTS LANDER_LINUX64_RELEASE_LIBRARIES)
    target_link_libraries(Lander PRIVATE "$<$<CONFIG:Release>:${_library}>")
  endforeach()
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
