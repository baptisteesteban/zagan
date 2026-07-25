zagan_add_test(stb_io)
zagan_add_test(nifti_io)
target_compile_definitions(test_nifti_io PRIVATE
    ZAGAN_TEST_DATA_DIR="${CMAKE_SOURCE_DIR}/tests/io/fixtures"
)
