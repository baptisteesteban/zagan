add_library(zagan-core INTERFACE)
add_library(zagan::core ALIAS zagan-core)

target_include_directories(zagan-core INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(zagan-core INTERFACE cxx_std_23)

add_library(zagan-cpr INTERFACE)
add_library(zagan::cpr ALIAS zagan-cpr)
target_link_libraries(zagan-cpr INTERFACE zagan::core)

target_include_directories(zagan-cpr INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

install(TARGETS zagan-core zagan-cpr EXPORT zaganTargets)

install(DIRECTORY include/zagan DESTINATION include)

install(EXPORT zaganTargets
    FILE zaganConfig.cmake
    NAMESPACE zagan::
    DESTINATION lib/cmake/zagan
)