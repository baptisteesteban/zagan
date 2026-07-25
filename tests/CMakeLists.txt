find_package(GTest REQUIRED)

function(zagan_add_test name)
    add_executable(test_${name} "${name}.cpp")
    target_link_libraries(test_${name} PRIVATE GTest::gtest_main zagan::core)

    if(APPLE)
        set_target_properties(test_${name} PROPERTIES
            BUILD_RPATH "$<TARGET_FILE_DIR:GTest::gtest_main>"
        )
    endif()

    add_test(NAME test_${name} COMMAND test_${name})
endfunction()

add_subdirectory(colors)
add_subdirectory(core)
add_subdirectory(io)
add_subdirectory(cpr)
