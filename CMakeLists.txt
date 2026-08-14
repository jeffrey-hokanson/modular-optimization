cmake_minimum_required(VERSION 3.25)
project(moe LANGUAGES CXX)

# Require the GCC 16+ reflection-capable toolchain.
if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_VERSION VERSION_LESS 16)
    message(WARNING "This project expects GCC 16+ for C++26 reflection support "
                     "(-freflection). Detected: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
endif()

# glaze is provided by the flake's dev shell / package output (installed
# via its own CMake config), so find_package picks it up from CMAKE_PREFIX_PATH.
find_package(glaze CONFIG REQUIRED)

# Eigen's CMake package is still named "Eigen3" for compatibility (this
# resolves correctly against Eigen 5.x installs, including nixpkgs' `eigen`).
find_package(Eigen3 CONFIG REQUIRED)

add_library(moe
    src/linesearch/Backtracking.cpp
    src/linesearch/Backtracking.hpp
    src/linesearch/Base.cpp
    src/linesearch/Children.cpp
    src/linesearch/Children.hpp
    src/linesearch/Base.hpp
    src/firstorder/Base.hpp
    src/firstorder/GradientDescent.hpp
    src/firstorder/GradientDescent.cpp
)

target_include_directories(moe
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(moe PUBLIC cxx_std_26)

# -std=c++26 is also set explicitly (on top of cxx_std_26) since -freflection
# needs to ride alongside it, and CMake's cxx_std_26 feature alone won't add
# the reflection flag.
target_compile_options(moe PUBLIC
    -std=c++26
    -freflection
)

target_link_libraries(moe PUBLIC glaze::glaze Eigen3::Eigen)

add_executable(main "src/main.cpp")
target_link_libraries(main PUBLIC moe)
target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


install(TARGETS moe
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
install(DIRECTORY include/ DESTINATION include)
