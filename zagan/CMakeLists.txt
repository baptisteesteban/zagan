find_package(PkgConfig REQUIRED)
pkg_check_modules(STB REQUIRED stb)

add_library(zagan-core)
add_library(zagan::core ALIAS zagan-core)
target_sources(zagan-core PRIVATE src/io/imread.cpp src/io/imsave.cpp)

target_include_directories(zagan-core PRIVATE ${STB_INCLUDE_DIRS})
target_include_directories(zagan-core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(zagan-core PUBLIC cxx_std_23)

install(TARGETS zagan-core EXPORT zaganTargets)

install(DIRECTORY include/zagan DESTINATION include)

install(EXPORT zaganTargets
    FILE zaganConfig.cmake
    NAMESPACE zagan::
    DESTINATION lib/cmake/zagan
)