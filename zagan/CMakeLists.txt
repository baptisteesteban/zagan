add_library(zagan-core INTERFACE)
add_library(zagan::core ALIAS zagan-core)

target_include_directories(zagan-core INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(zagan-core INTERFACE cxx_std_23)

install(TARGETS zagan-core EXPORT zaganTargets)

install(DIRECTORY include/zagan DESTINATION include)

install(EXPORT zaganTargets
    FILE zaganConfig.cmake
    NAMESPACE zagan::
    DESTINATION lib/cmake/zagan
)