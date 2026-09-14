# Match the project's GCC/libstdc++ toolchain, including C++ dependencies.
if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  message(FATAL_ERROR "macOS builds require Homebrew GCC; set CXX to its versioned g++ executable.")
endif()

# These dependencies expose C APIs and can use native Homebrew binaries.
find_package(PkgConfig REQUIRED)
pkg_check_modules(MAC_DEPS REQUIRED IMPORTED_TARGET glew glfw3 freetype2)
find_package(OpenGL REQUIRED)

# Use Boost headers only, avoiding Homebrew's libc++-built Chrono library.
find_package(Boost REQUIRED CONFIG)
target_link_libraries(Lander PRIVATE Boost::headers)
target_compile_definitions(Lander PRIVATE
  BOOST_CHRONO_HEADER_ONLY
  BOOST_ERROR_CODE_HEADER_ONLY
)

# FTGL exposes C++ APIs: compile its unmodified sources with the game's GCC.
include(FetchContent)
FetchContent_Declare(mac_ftgl
  URL https://github.com/frankheckenbach/ftgl/archive/refs/tags/v2.4.0.tar.gz
  URL_HASH SHA256=aa97da1c3442a8fd3941037655df18016d70b5266381c81d81e8b5335f196ea8
  SOURCE_SUBDIR unused)
FetchContent_MakeAvailable(mac_ftgl)

# The pinned release contains only library sources in these directories.
file(GLOB mac_ftgl_sources
  "${mac_ftgl_SOURCE_DIR}/src/*.cpp"
  "${mac_ftgl_SOURCE_DIR}/src/FTFont/*.cpp"
  "${mac_ftgl_SOURCE_DIR}/src/FTGlyph/*.cpp"
  "${mac_ftgl_SOURCE_DIR}/src/FTLayout/*.cpp")
add_library(mac_ftgl STATIC ${mac_ftgl_sources})
target_include_directories(mac_ftgl SYSTEM PUBLIC "${mac_ftgl_SOURCE_DIR}/src")
target_include_directories(mac_ftgl PRIVATE
  "${mac_ftgl_SOURCE_DIR}/cmake"
  "${mac_ftgl_SOURCE_DIR}/src/FTGL"
  "${mac_ftgl_SOURCE_DIR}/src/FTFont"
  "${mac_ftgl_SOURCE_DIR}/src/FTGlyph"
  "${mac_ftgl_SOURCE_DIR}/src/FTLayout")
target_compile_definitions(mac_ftgl PUBLIC FTGL_LIBRARY_STATIC
  PRIVATE PACKAGE_VERSION="2.4.0")
target_link_libraries(mac_ftgl PRIVATE PkgConfig::MAC_DEPS OpenGL::GL OpenGL::GLU)

target_link_libraries(Lander PRIVATE mac_ftgl PkgConfig::MAC_DEPS OpenGL::GL OpenGL::GLU)
target_compile_options(Lander PRIVATE
  ${LANDER_ARCH_FLAG}
  -Wall -Wextra -Wno-unused-local-typedefs
  $<$<CONFIG:Debug>:-Og;-g;-fno-omit-frame-pointer>
  $<$<CONFIG:Release>:-O3>)
# Darwin requires dynamic system libraries; do not request a static executable.
target_link_options(Lander PRIVATE ${LANDER_ARCH_FLAG})
