find_package(GTest REQUIRED)

function(zagan_add_test name)
    add_executable(test_${name} "${name}.cpp")
    target_link_libraries(test_${name} PRIVATE GTest::gtest_main zagan::core)
    add_test(NAME test_${name} COMMAND test_${name})
endfunction()

zagan_add_test(image2d)